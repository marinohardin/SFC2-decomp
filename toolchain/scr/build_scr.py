#!/usr/bin/env python3
"""Build an OP API mission script (.scr DLL) with the VC6 SP4 container.

    build_scr.py <srcdir> <out.scr> [--jobs N] [--keep]

<srcdir> holds the script's .cpp/.h files. If it has a .dsp, its Release compile and link lines
and source list are used; otherwise every .cpp is compiled with the flags of the SDK's
Single-mission template (_ScriptTemplates/Single/ProjectFile.dsp.template). The sources are
staged with a copy of the SDK Scripts tree under $SFCOP_WORK/toolchain/scr/<name>/ at
Scripts/Missions/<name>/, the depth the template's relative /I paths expect, and linked against
the SDK Libraries/ plus the VC6 Lib/ set chosen by MSVC_LIB_SET (see toolchain/README.md).
Two workarounds are built in: the SDK's truncated dmp3D_MSVCR.lib is replaced by a library of its
intact members plus dmp3d_shim.cpp, and the DLL export file is made by a separate "lib /def" call
because link.exe cannot spawn its own under wibo + QEMU.
"""
import argparse, concurrent.futures, glob, os, shlex, shutil, struct, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(HERE))
SFCOP = os.path.dirname(REPO)
WORK = os.path.join(os.environ.get('SFCOP_WORK', os.path.join(SFCOP, 'work')), 'toolchain', 'scr')
SDK = os.path.join(REPO, 'reference/installed game/Starfleet Command Orion Pirates/OP API - R2.1')
TEMPLATE_DSP = os.path.join(SDK, 'Scripts/_ScriptTemplates/Single/ProjectFile.dsp.template')
CL = os.path.join(REPO, 'toolchain/bin/cl')
LINK = os.path.join(REPO, 'toolchain/bin/link')
sys.dont_write_bytecode = True
sys.path.insert(0, HERE)
import pe_exports  # noqa: E402


def release_lines(dsp_text):
    """The Release '# ADD CPP' and '# ADD LINK32' arguments and the .cpp source list of a .dsp."""
    cpp = link = None
    sources, in_release = [], False
    for ln in dsp_text.replace('\r', '').split('\n'):
        if ln.startswith('!IF') or ln.startswith('!ELSEIF'):
            in_release = 'Release' in ln
        if in_release and ln.startswith('# ADD CPP') and cpp is None:
            cpp = shlex.split(ln[len('# ADD CPP'):])
        if in_release and ln.startswith('# ADD LINK32') and link is None:
            link = shlex.split(ln[len('# ADD LINK32'):])
        if ln.startswith('SOURCE=') and ln.lower().endswith('.cpp'):
            sources.append(ln[len('SOURCE='):].strip('"').replace('\\', '/'))
    return cpp, link, sources


def link_args(link):
    """Drop output/library paths (we set our own); the templates' 'machine:I386' lacks its '/',
    which link.exe would read as an input file name."""
    out = []
    for a in link:
        low = a.lower()
        if low.startswith('/out:') or low.startswith('/libpath:'):
            continue
        out.append('/machine:I386' if low == 'machine:i386' else a)
    return out


def stage(name, srcdir):
    root = os.path.join(WORK, name)
    if os.path.isdir(root):
        shutil.rmtree(root)
    for sub in ('ScriptInterface', 'shared'):
        shutil.copytree(os.path.join(SDK, 'Scripts', sub), os.path.join(root, 'Scripts', sub))
    shutil.copytree(os.path.join(SDK, 'STL'), os.path.join(root, 'STL'),
                    ignore=shutil.ignore_patterns('*.Lib', '*.lib'))
    proj = os.path.join(root, 'Scripts', 'Missions', name)
    shutil.copytree(srcdir, proj)
    for d, _, files in os.walk(root):
        for f in files:
            os.chmod(os.path.join(d, f), 0o644)
    return root, proj


def salvage_dmp3d():
    """The SDK's dmp3D_MSVCR.lib is truncated as shipped (848,082 of ~4.2 MB; only 9 of the
    56 object members are whole), so link.exe stops with LNK1127. Rebuild a library from the intact
    members; the directory returned goes first on /libpath. Symbols from the lost members stay
    unresolved and link names them."""
    libdir = os.path.join(WORK, '_salvaged')
    lib = os.path.join(libdir, 'dmp3D_MSVCR.lib')
    if os.path.exists(lib):
        return libdir
    sys.path.insert(0, os.path.join(REPO, 'toolchain', 'calibration'))
    from calibrate import archive_members
    objdir = os.path.join(libdir, 'obj')
    os.makedirs(objdir, exist_ok=True)
    data_len = os.path.getsize(os.path.join(SDK, 'Scripts/Libraries/dmp3D_MSVCR.lib'))
    objs = []
    for i, (name, body) in enumerate(archive_members(os.path.join(SDK, 'Scripts/Libraries/dmp3D_MSVCR.lib'))):
        name = name.rstrip('/')
        if not name.lower().endswith('.obj') or len(body) < 20:
            continue
        nsym_end = int.from_bytes(body[8:12], 'little') + 18 * int.from_bytes(body[12:16], 'little')
        if nsym_end > len(body):  # the truncated last member
            continue
        p = os.path.join(objdir, f'{i:02d}_' + name.replace('\\', '_').replace('/', '').lstrip('._'))
        open(p, 'wb').write(body)
        objs.append('Z:' + p)
    rc, log = run([os.path.join(REPO, 'toolchain/bin/lib'), '/nologo', f'/out:Z:{lib}'] + objs, libdir)
    if rc:
        sys.exit('could not rebuild dmp3D_MSVCR.lib: ' + log)
    print(f'salvaged {len(objs)} intact members of dmp3D_MSVCR.lib ({data_len} bytes as shipped)')
    return libdir


def export_members(libnames, outdir):
    """The SDK library members whose .drectve section carries '-export:' linker directives, written
    to outdir. ScriptInterfaceR.lib(ScriptInterface.obj) has -export:_ScriptEntry and
    -export:_MissionSpecEntry, which is how every script DLL gets its two exports. The separate
    "lib /def" step only sees the files it is given, so these members are added to its inputs."""
    sys.path.insert(0, os.path.join(REPO, 'toolchain', 'calibration'))
    from calibrate import archive_members
    libdir = os.path.join(SDK, 'Scripts/Libraries')
    by_lower = {f.lower(): f for f in os.listdir(libdir)}
    paths, exports = [], []
    for lib in dict.fromkeys(l.lower() for l in libnames):
        if lib not in by_lower:
            continue
        for i, (mname, body) in enumerate(archive_members(os.path.join(libdir, by_lower[lib]))):
            if len(body) < 20 or body[:2] != b'\x4c\x01':  # i386 COFF objects only
                continue
            nsec, optsz = struct.unpack_from('<H', body, 2)[0], struct.unpack_from('<H', body, 16)[0]
            found = []
            for k in range(nsec):
                sh = 20 + optsz + 40 * k
                if body[sh:sh + 8].rstrip(b'\0') == b'.drectve':
                    size, ptr = struct.unpack_from('<II', body, sh + 16)
                    found += [t[8:] for t in body[ptr:ptr + size].decode('latin1').split()
                              if t.lower().startswith('-export:')]
            if found:
                base = mname.rstrip('/').replace('\\', '/').split('/')[-1]
                p = os.path.join(outdir, f'{os.path.splitext(lib)[0]}_{i}_{base}')
                open(p, 'wb').write(body)
                paths.append(p)
                exports += found
    return paths, exports


def run(cmd, cwd):
    p = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True, errors='replace')
    return p.returncode, p.stdout + p.stderr


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('srcdir')
    ap.add_argument('out')
    ap.add_argument('--jobs', type=int, default=int(os.environ.get('JOBS', '4')))
    args = ap.parse_args()
    srcdir, out = os.path.abspath(args.srcdir), os.path.abspath(args.out)
    name = os.path.splitext(os.path.basename(out))[0]
    dsps = glob.glob(os.path.join(srcdir, '*.dsp'))
    if dsps:
        cpp, link, sources = release_lines(open(dsps[0], 'rb').read().decode('latin1'))
        sources = [s[2:] if s.startswith('./') else s for s in sources]
    else:
        text = open(TEMPLATE_DSP, 'rb').read().decode('latin1').replace('##ProjName##', name)
        cpp, link, _ = release_lines(text)
        sources = sorted(f for f in os.listdir(srcdir) if f.lower().endswith('.cpp'))
    root, proj = stage(name, srcdir)
    objdir = os.path.join(root, 'obj')
    os.makedirs(objdir)
    stl = os.path.join(root, 'STL', 'Include')

    # The truncated dmp3D_MSVCR.lib lacks tErrInfo::tErrInfo and tMatrix::mIdentity, which every
    # script references; dmp3d_shim.cpp supplies them (byte-identical to the shipped copies).
    shutil.copyfile(os.path.join(HERE, 'dmp3d_shim.cpp'), os.path.join(proj, '_dmp3d_shim.cpp'))
    sources = sources + ['_dmp3d_shim.cpp']

    def compile_one(src):
        base = os.path.splitext(os.path.basename(src))[0]
        flags = [f for f in cpp if f not in ('/Fr', '/FR')]
        cmd = [CL, '/I', stl] + flags + [f'/Fo{objdir}/{base}.obj', f'/Fd{objdir}/{base}/',
                                         f'/Fp{objdir}/{base}.pch', src]
        os.makedirs(os.path.join(objdir, base), exist_ok=True)
        rc, log = run(cmd, proj)
        # /YX writes a ~9 MB .pch per file and /Fd a .idb; nothing later needs either.
        shutil.rmtree(os.path.join(objdir, base), ignore_errors=True)
        if os.path.exists(os.path.join(objdir, base + '.pch')):
            os.remove(os.path.join(objdir, base + '.pch'))
        return src, rc, log, os.path.join(objdir, base + '.obj')

    with concurrent.futures.ThreadPoolExecutor(args.jobs) as ex:
        results = list(ex.map(compile_one, sources))
    failed = [r for r in results if r[1] != 0 or not os.path.exists(r[3])]
    for src, rc, log, _ in results:
        print(f'cl {src}: rc={rc}')
        if rc:
            print(log.strip()[-3000:])
    if failed:
        sys.exit(f'{len(failed)} of {len(sources)} sources failed to compile')
    tmp_out = os.path.join(root, name + '.scr')
    z = lambda p: 'Z:' + p  # link.exe reads a bare /Volumes/... argument as an option
    # link.exe builds a DLL's export file by spawning "link -lib -def", and wibo cannot spawn
    # processes under QEMU (clone: Invalid argument, LNK1141). Build the .exp first; link.exe then
    # takes it as an input and does not spawn anything.
    # /name: gives the export directory the .scr's own file name, as link.exe /out: would.
    members, exports = export_members([a for a in link if a.lower().endswith('.lib')], objdir)
    exp_lib = os.path.join(root, name + '.lib')
    rc, log = run([os.path.join(REPO, 'toolchain/bin/lib'), '/nologo', '/def', '/machine:I386',
                   f'/name:{os.path.basename(out)}', f'/out:{z(exp_lib)}']
                  + [z(r[3]) for r in results] + [z(m) for m in members], proj)
    print(f'lib /def (export file, {" ".join(exports)}): rc={rc}')
    if rc:
        sys.exit(log)
    cmd = [LINK] + link_args(link) + [f'/out:{z(tmp_out)}', f'/libpath:{z(salvage_dmp3d())}',
                                      f'/libpath:{z(os.path.join(SDK, "Scripts/Libraries"))}',
                                      f'/map:{z(os.path.join(root, name + ".map"))}',
                                      z(exp_lib[:-4] + '.exp')] + [z(r[3]) for r in results]
    rc, log = run(cmd, proj)
    open(os.path.join(root, 'link.log'), 'w').write(' '.join(shlex.quote(c) for c in cmd) + '\n' + log)
    print(f'link: rc={rc}')
    if log.strip():
        print(log.strip()[-3000:])
    if rc or not os.path.exists(tmp_out):
        sys.exit('link failed; see ' + os.path.join(root, 'link.log'))
    os.makedirs(os.path.dirname(out), exist_ok=True)
    shutil.copyfile(tmp_out, out)
    dll, names = pe_exports.exports(out)
    print(f'{out}: {os.path.getsize(out)} bytes, export name {dll}, exports {", ".join(names)}')


if __name__ == '__main__':
    main()
