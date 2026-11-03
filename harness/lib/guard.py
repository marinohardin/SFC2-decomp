"""Decisions for the PreToolUse hook of a function session (harness/hooks/guard.py).

The agent may edit rebuilt source and nothing else, and may run the build-and-verify script and
read-only commands. Everything that judges its work (the harness, the compiler and its flags, the
original exe and its hashes, the analysis exports, generated objects and listings, the queue and
the matched set) is out of reach, as are the hook settings themselves.
"""
import os
import re
import shlex

from . import paths

# Commands a function session may run, by first word. Each segment of a pipeline or a command
# list is checked on its own, and the arguments of the commands that can write a file (sort -o,
# uniq OUT, xxd -r or OUT, find -fprint, sed w/e, git --output, file -C, rg --pre) are checked
# below. awk, less and more are left out: each can run commands or write files.
READ_ONLY = {'ls', 'cat', 'head', 'tail', 'grep', 'egrep', 'rg', 'wc', 'find', 'sort', 'uniq',
             'cut', 'diff', 'cmp', 'xxd', 'od', 'file', 'stat', 'pwd', 'echo', 'printf', 'true',
             'sed', 'tr', 'column', 'nl', 'basename', 'dirname', 'realpath', 'c++filt', 'which',
             'test', '['}
GIT_READ_ONLY = {'diff', 'status', 'log', 'show', 'blame', 'grep', 'ls-files', 'rev-parse'}
VERIFY_REL = ('harness/build-and-verify.sh', './harness/build-and-verify.sh')
SEGMENT_SPLIT = re.compile(r'\|\||&&|\|&|;|\||&|\n')
SAFE_REDIRECTS = re.compile(r'\d?>&\d|\d?>\s*/dev/null')
FIND_ACTIONS = {'-delete', '-exec', '-execdir', '-ok', '-okdir', '-fprint', '-fprint0',
                '-fprintf', '-fls'}
# sed may only print line ranges or matching lines: 10,20p  $p  /re/p  /a/,/b/p
SED_PRINT = re.compile(r'^((\d+|\$)(,(\d+|\$))?|/[^/;]*/(,/[^/;]*/)?)p$')
XXD_VALUE_FLAGS = {'-c', '-cols', '-l', '-len', '-s', '-seek', '-g', '-groupsize', '-o',
                   '-offset', '-n', '-name'}


def protected_roots():
    r = paths.REPO
    return [os.path.join(r, p) for p in
            ('harness', 'toolchain', 'analysis', 'ghidra', 'scripts', 'oracle', '.claude',
             '.githooks', '.github', 'third_party', 'docs', 'AGENTS.md', 'CLAUDE.md')] + [
        paths.STATE, paths.SHARED, paths.DIFFICULT, paths.REFERENCE, paths.TOOLS, paths.MATCHED_SET, paths.UNITS, paths.EXE,
        paths.FUNCTIONS_CSV, paths.HASHES]


def _inside(path, root):
    path, root = os.path.realpath(path), os.path.realpath(root)
    return path == root or path.startswith(root.rstrip(os.sep) + os.sep)


def check_edit(file_path, cwd=None):
    """None to allow, or the reason to deny an Edit/Write of file_path."""
    if not file_path:
        return 'no file path given'
    p = file_path if os.path.isabs(file_path) else os.path.join(cwd or paths.REPO, file_path)
    for root in protected_roots():
        if _inside(p, root):
            return (f'{file_path} is part of what judges your work (harness, toolchain, original '
                    f'binary, analysis exports, matched set, queue or settings); it cannot be '
                    f'edited from a function session')
    if p.lower().endswith(('.asm', '.obj', '.lib', '.exe', '.dll', '.pdb')):
        return f'{file_path} is generated or binary output; change the source instead'
    if not _inside(p, paths.SRC_ROOT):
        return (f'{file_path} is outside the rebuilt source tree '
                f'({os.path.relpath(paths.SRC_ROOT, paths.REPO)}/); edit only source files there')
    return None


def check_bash(command):
    """None to allow, or the reason to deny a Bash command."""
    if 'ANTHROPIC_API_KEY' in command or 'ANTHROPIC_AUTH_TOKEN' in command:
        return 'API credentials are not used in function sessions'
    if '`' in command or '$(' in command or '<(' in command or '>(' in command:
        return 'command and process substitution are not allowed; run the commands one at a time'
    stripped = SAFE_REDIRECTS.sub('', command)
    if '>' in stripped:
        return 'redirecting output to a file is not allowed; use the Edit or Write tool on source'
    for seg in SEGMENT_SPLIT.split(stripped):
        seg = seg.strip()
        if not seg:
            continue
        try:
            words = shlex.split(seg)
        except ValueError:
            return f'cannot parse {seg!r}'
        if not words:
            continue
        if re.match(r'^[A-Za-z_][A-Za-z0-9_]*=', words[0]):
            return 'setting environment variables is not allowed'
        reason = _check_words(words)
        if reason:
            return reason
    return None


def _is_verify(word):
    """The real build-and-verify.sh, not a look-alike elsewhere (the session runs in the repo)."""
    return word in VERIFY_REL or word == os.path.join(paths.HARNESS, 'build-and-verify.sh')


def _positional(args, value_flags=()):
    out, skip = [], False
    for a in args:
        if skip:
            skip = False
        elif a in value_flags:
            skip = True
        elif not a.startswith('-') or a == '-':
            out.append(a)
    return out


def _check_words(words):
    cmd, args = words[0], words[1:]
    if cmd in ('sh', 'bash') and args and _is_verify(args[0]):
        cmd, args = args[0], args[1:]
    if _is_verify(cmd):
        if any(a == '--record' for a in args):
            return 'only the harness records results'
        return None
    if any(a.startswith(('--output', '--pre', '--hostname-bin')) for a in args):
        return f'{cmd}: options that write files or run programs are not allowed'
    if cmd == 'git':
        if any(a == '-c' or a.startswith(('-O', '--open-files-in-pager', '--ext-diff', '--exec'))
               for a in args):
            return 'git options that run programs or change configuration are not allowed'
        sub = next((a for a in args if not a.startswith('-')), '')
        if sub in GIT_READ_ONLY:
            return None
        return f'git {sub} is not allowed; the loop commits and resets for you'
    if cmd not in READ_ONLY:
        return (f'{cmd} is not allowed in a function session: run harness/build-and-verify.sh '
                f'<function> to build and score, and read-only commands to look around')
    if cmd == 'find' and any(a in FIND_ACTIONS for a in args):
        return 'find may only list files here'
    if cmd == 'sort' and any(a.startswith('-') and not a.startswith('--') and 'o' in a
                             for a in args):
        return 'sort -o writes a file; it is not allowed'
    if cmd == 'uniq' and len(_positional(args, ('-f', '-s', '-w'))) > 1:
        return 'uniq with an output file is not allowed'
    if cmd == 'xxd':
        if any(a.startswith('-r') for a in args):
            return 'xxd -r writes binary; it is not allowed'
        if len(_positional(args, XXD_VALUE_FLAGS)) > 1:
            return 'xxd with an output file is not allowed'
    if cmd == 'file' and any(a in ('-C', '--compile') for a in args):
        return 'file -C writes a file; it is not allowed'
    if cmd == 'sed':
        return _check_sed(args)
    return None


def _check_sed(args):
    """sed only as a pager: -n with one print-only script (use the Read tool for anything else)."""
    deny = 'sed may only print lines here (sed -n 10,20p FILE); use the Read tool or Edit instead'
    if '-n' not in args or any(a.startswith(('-i', '--in-place', '-f', '--file', '-s', '-E',
                                             '-r', '-z', '-u', '-l'))
                               for a in args if a != '-n'):
        return deny
    scripts = [args[i + 1] for i, a in enumerate(args[:-1]) if a == '-e']
    if not scripts:
        rest = [a for a in args if a != '-n']
        scripts = rest[:1]
    if not scripts or any(not SED_PRINT.match(sc.replace(' ', '')) for sc in scripts):
        return deny
    return None


def decide(event):
    """PreToolUse event dict -> None (no objection) or a deny reason."""
    tool = event.get('tool_name', '')
    ti = event.get('tool_input') or {}
    if tool in ('Edit', 'Write', 'MultiEdit'):
        return check_edit(ti.get('file_path'), event.get('cwd'))
    if tool == 'NotebookEdit':
        return 'notebooks are not part of the rebuilt source'
    if tool == 'Bash':
        return check_bash(ti.get('command', ''))
    return None
