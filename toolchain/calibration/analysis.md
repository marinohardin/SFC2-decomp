## Analysis (hand-written, 2026-11-02)

### Verdict

The oracle reproduces Taldren's code exactly. With the `.dsp` Release flags and VC6 SP4, all 23
objects compile, every one of the 1230 functions is byte-exact in objdiff, and the independent
strict check finds every code section (1425) and data section (762) identical, relocations
included. 8 of the 23 objects are byte-identical to the retail members apart from the COFF
timestamp. The other 15 differ only in compiler-internal label numbers and, in 3 objects, in the
order of RTTI records (below), neither of which changes any code or data bytes.

### What had to change from the `.dsp`

No compiler flag was changed. The command line is the Release `# ADD CPP` line verbatim
(`/nologo /MT /W3 /GR /GX /O2 /I ... /D WIN32 /D NDEBUG /D _WINDOWS /Fr /YX /FD /c` for
ScriptInterface; `_Shared.dsp` has `/D _MBCS /D _LIB /FR` instead) with these additions:

| Addition | Why |
|---|---|
| `/I <SDK>/STL/Include` before the `.dsp`'s own `/I` list | The SDK readme tells users to copy these patched Dinkumware headers over VC6's own. An `/I` dir is searched before `INCLUDE`, which has the same effect without touching the compiler tree. |
| `/Fo<dir>/ /Fd<dir>/ /Fp<dir>/<unit>.pch`, and `/Fr` / `/FR` given a directory | Output locations only. The IDE adds `/Fo /Fd /Fp` itself; a per-file `.pch` lets files compile in parallel. |
| Sources passed as `C:\Projects\Taldren\Projects\SFC2_5\SCRIPTS\<Project>\<path>` | The retail members' `.file` symbols and `__FILE__` strings hold this path (the IDE passes absolute paths). The staged SDK `Scripts/` is mounted at `/Projects/Taldren/Projects/SFC2_5/SCRIPTS` and wibo maps `C:\` to `/`. Without it, code is still identical, but `NetMsgClass.cpp`'s `__FILE__` string and every `.file` record differ. |

The `.dsp` include paths are relative to the project directory, so sources are compiled with that
directory as the working directory, as in the IDE. Many listed paths do not exist in the SDK
(`../../QSIInc/Tiger`, `..\..\Source\Taldren\Common_`, ...); they are harmless and were left in.
wibo resolves paths case-insensitively, so `..\Shared\...` finds the SDK's `shared/` directory.

### Variants

- `sp5` (msvc6.5, `C2.DLL` 8966): the same 100% code and data, so SP4 and SP5 generate identical
  code for this corpus. The only difference is the `@comp.id` stamp (`0x000B2306`, build 8966,
  instead of `0x000B225F`, build 8799). The retail members carry 8799, as does the bulk of
  `StarFleetOP.exe`'s Rich header, so SP4 is the right compiler. That conclusion rests on the
  stamp, not on codegen. Keep `msvc6.4` as the default and treat SP5 as an equivalent fallback.
- `ob1`, `gy`: byte-for-byte the same objects as the baseline, because VC6 `/O2` already implies
  `/Ob1` and `/Gy`. These experiments cannot tell anything apart.
- `ox`: `/Ox` drops `/Gy` (and `/Gf`), so all functions land in one or two `.text` sections and
  calls between them lose their COMDAT relocations: 85% of functions byte-exact, 99.5% fuzzy. Wrong.
- `gf`: `/GF` moves string literals to read-only, mangled `??_C@` COMDATs, so code that references
  a string gets a different relocation target: 95.9% of functions. Wrong. `/O2` already implies
  `/Gf`, the writable form, which is what Taldren's objects use.
- `dsp-nopch` (no `/YX /Fr /FD`): code and data bytes are still identical and objdiff still says
  100%, but the compiler-internal numbering changes completely (`$L`/`$T` labels shift by
  about 13,600; `$E` static-initializer numbers and emission order change), so only 1 object is
  identical apart from the timestamp, against 8 with `/YX`. The `$L` counter state is stored in the
  `/YX` precompiled header. Keep `/YX`: it is needed for object-level identity, though not for
  function matching.

### What does not match, and why

1. **Label numbering offset of −35 or −36** (15 of 23 objects). Exception-handling labels (`$L`)
   and temporaries (`$T`) in our objects are numbered 35 (ScriptInterface) or 36 (Shared) lower than
   in Taldren's, with one consistent offset per object. The other 8 objects (`DrawCanvas`, `MapObject`,
   `sfcstreams`, ...) have no offset. The counter advances while
   headers are parsed, so the preprocessed input differs slightly from Taldren's. The most likely
   source is a VC6 or Platform SDK header that differs between Taldren's install and the
   decomp.me tarball. Compiling a whole project in one `cl` call with a shared `.pch`, as the IDE
   does, gives the same offset, so it is not a `/YX` effect. These labels are local, never survive
   linking and do not change any byte of code or data.
2. **RTTI/vtable record order** (`ScriptInterface.cpp`, `Strength.cpp`, `missionspec.cpp`). The
   compiler emits the `??_R0`/`??_R1`/`??_R2`/`??_R3`/`??_R4` and `??_G`/`??_E` COMDATs for two
   classes in swapped order (tNetMsg vs tShipSystemsInfo; tBaseException vs tOutOfBounds;
   tStrength vs tTeamSpec). Each section is still identical, so only the section numbering
   differs. The emission order probably depends on the same header difference as item 1; a
   shared-`.pch` compile does not change it. Effect after linking: at most the order of a few RTTI
   records in `.rdata`, which matters only for a whole-image byte match, not for function
   matching.
3. **`texttoken.cpp`** defines no functions (only data tables); its object matches byte for byte.

Not observed, though the task asked to watch for them: template instantiation order inside code
(every COMDAT matched), string pooling (`/Gf` from `/O2` matches) and `/YX` codegen effects (none).

### Limits of this calibration

- 23 files, 1230 functions, 139,810 bytes of code and 24,407 of data, all from late 2001 / January 2002 (`.lib` dated
  2002-01-14). The game exe was linked in October 2003; SP4 is confirmed for it by the Rich
  header, not by this calibration.
- The sample is C++ with Taldren's STL and `/GX` EH. It does not exercise C files, inline asm,
  `#pragma optimize`, or other `.dsp` settings the game may have used (e.g. `/Ox`, `/G6`, `/Zp`).
  Each game project's own flags still have to be recovered.
- The 66 `Common_Release` members of the same `.lib` (Taldren's Common library) have no sources
  in the SDK and were not tested.
- objdiff's "100%" pairs compiler-generated local symbols (`$E` initializers) by content, not
  name, so it tolerates internal-numbering differences. `strictcmp.py` exists to catch what
  objdiff tolerates.

### Tooling notes

- Docker Desktop's Rosetta cannot run `wibo-x86_64` (`rosetta error: invalid gdt selector index 4`),
  so the image is linux/386 and runs under QEMU. A typical SDK file takes 3 to 16 s (the heavy
  STL-using ones about 15 s), with 3 compiles in parallel.
- `LIB.EXE` and `DUMPBIN.EXE` re-spawn `LINK.EXE`, and wibo's `clone()` fails under QEMU, so
  the wrappers call `LINK.EXE /lib` and `/dump` directly.
- A single `cl` invocation with several sources sharing one `/YX` `.pch` can hang at 0% CPU
  under wibo (seen twice, after 2 and after 8 files). Compile one source per `cl` call, as
  `calibrate.py` does. The likely link is wibo's VC6 precompiled-header support (upstream PR #146
  was still open in the research notes).
- The decomp.me tarballs have no `Lib/` directory, so linking real programs needs VC6 CRT and
  Win32 import libraries from elsewhere (see P0-6).
