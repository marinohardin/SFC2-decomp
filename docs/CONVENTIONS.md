# SFC2-decomp — Conventions

The standard every change is held to. `DESIGN.md` says what the system does; this says what the code looks like. The automated review reads this and `AGENTS.md`.

---

## 1. Toolchain

| Tool | Convention |
| --- | --- |
| Languages | `op/`: C++ that compiles with VC6 SP4 (§4.1); tooling: Python 3.9 or later, standard library only |
| Compiler | VC6 SP4 from `toolchain/`, run under wibo: in the `sfcop-msvc6` container, or natively on macOS (`toolchain/bin/cl-native`, the same compiler files, which score every claimed function as the container does; the harness's scoring uses it when installed; a consumer's build and the scheduled Docker audit use the container; the per-pull-request CI check uses the native compiler). Versions in `toolchain/VERSIONS.md` |
| Tests | `python3 -m pytest tests harness/tests` for tooling; the harness's byte check and the gates through `sh scripts/check.sh` |
| Format | `op/` keeps the original's style and is never reformatted |
| CI | GitHub Actions: the hosted `tests` job, and `check` on a runner that has the retail install; the same `scripts/check.sh` as locally |

Versions are pinned in a file in the repository, never "latest".

## 2. Repository layout

```
op/                 the decompiled original, one file per original translation unit at its original path
  Source/...          SFC2_5\Source: the game proper
  Meta/...            the Dynaverse client
  Common/...          Taldren's utility layer (SharedR)
  Switch/...          nSwitch
  dmp3d/, Tiger/      the inherited engine and platform libraries, shared with Empires at War (DESIGN §7.5)
  unsorted/           units whose original file is not known yet (harness/SRC_LAYOUT.md)
  port/               types.h and the few headers a consumer's compile mode needs; nothing else
eaw/                  Empires at War, once it starts (DESIGN §7.5)
harness/, toolchain/, analysis/, ghidra/   the decompilation's judges and evidence (hotspots where marked)
tools/                the manifest and unit-map generators, progress charts, the batch check
scripts/              check.sh, progress.py, the small scripts the documents name
tests/                Python tests for the tooling
docs/                 DESIGN.md, CONVENTIONS.md, INTERFACE.md
```

`op/` includes only `op/`, by paths from the `op/` root (§4.1). Generated files (objects, Ghidra projects, build directories) live under `work/` outside the clone, never in git.

## 3. Naming

Files

| Kind | Convention | Example |
| --- | --- | --- |
| Decompiled unit | The original path and file name, drive and project root dropped | `op/Common/DynamicAllocator.cpp` |
| Unknown-origin unit | `op/unsorted/<Class>.cpp`, or `fn_<page>.cpp` for free functions by 4 KB page | `op/unsorted/tShipYard.cpp` |
| Tests | `tests/test_<topic>.py` | `tests/test_manifest.py` |
| Documents and data | `UPPER.md` for the documents the workflow names, lowercase for data | `docs/INTERFACE.md`, `analysis/results/matched_set.json` |

Identifiers

| Kind | Convention | Example |
| --- | --- | --- |
| Original classes, functions, members | The original identifier, exactly, once evidence gives it (a symbol, RTTI, an assertion string, an SDK match); `fn_<address>` until then | `tDynamicAllocator::Instance`, `fn_006FC690` |
| Names that exist only in a leaked or unlicensed source | Never used; a descriptive name of this project's own instead | |
| Macros in `op/port/` | `SFCOP_` prefix | `SFCOP_GUEST` |

Branches follow §7.1. Commit scopes are the top-level directory or the component: `feat(op/Common): ...`, `fix(harness): ...`, `docs(design): ...`.

## 4. Code style

### 4.1 Decompiled code (`op/`)

- The goal is the original's source, so the style is the original's: its spacing, its idioms, its member order. The matched SDK sources (`Common/`, the scripting library) show that style; follow it.
- Every function carries its `// FUNCTION: STARFLEETOP 0x...` or `// STUB:` line (DESIGN §6.3). Functions appear in address order inside a unit.
- A function VC6 generates from a global object's definition, with no source line of its own, is claimed by annotations stacked directly above that definition (`tINIMgr g_tINIMgr;`), never by a stand-in function: counted upwards from the definition, the dynamic initializer (whose annotation is the one directly above the line), the atexit registration, the atexit destructor (`tools/manifest.py`).
- Includes are paths from the `op/` root: `#include "Common/DynamicAllocator.h"`, with `op/` and `op/port/` the only include directories (owner's decision, 2026-11-04). The original wrote bare file names with a long list of include directories; its include lines are rewritten to this form when a unit is imported, and that rewrite is the one standing exception to "the original's source", since include lines change no code bytes.
- Only what VC6 SP4 compiles: no C++11 or later, no `long long`, no anonymous namespaces where the original had none. A construct a consumer's compiler rejects is fixed with a `PORT:` line, never by forking the file.
- A `PORT:` line is one line, tagged on the line itself: `// PORT: <reason>`. The reasons allowed: 64-bit or pointer-size hygiene, undefined behavior the original relied on, a platform difference a consumer cannot absorb outside `op/`.
- A fix to game code never goes in `op/` (no `#if SFCOP_FIX(name)`, no changed line); it belongs to a consumer such as the port.
- Pointer-typed members and `long` keep the original's VC6 types. A consumer links the VC6 object of a unit (DESIGN §7.2, §12.4), so `op/` is never compiled in a guest mode.
- A `MATCH-ONLY:` comment marks code in `op/` whose form exists only to make VC6 emit the original bytes: an `inline` the original's compiler chose by itself, a compiler-generated member written out, a load order, or a helper with no original address that is never called and exists only so the compiler emits an original function out of line. The comment names the function it serves and why the plain form does not match. A helper of that last kind carries no `// FUNCTION:` line, is not counted or claimed, and stays in the unit of the function it serves; when a later match makes it unnecessary it is deleted. `MATCH-ONLY:` never changes behavior; a change in behavior is a `PORT:` line or a fix.
- No comment restates the code. A comment records evidence (`// matches SharedR.lib DynamicAllocator.obj`), a judgement (`// the original reads one past the end here; kept`) or a `PORT:` reason.

## 5. Dependencies

- Python standard library only (pytest for the tests); the Docker image is pinned in `toolchain/VERSIONS.md`. Adding a dependency is the integrator's decision.

## 6. Testing

- Tests sit in `tests/`, and the harness's own in `harness/tests/`.
- There is no line-coverage threshold. Progress is measured by `scripts/progress.py` as percent of original code bytes that are byte-exact, plus percent of functions named; no pull request may lower any function's score, compared by original address.
- Every new feature and every bug fix comes with tests. A bug fix comes with a test that fails without the fix.
- Test names say what is guaranteed, not what function is called.
- No network, no real external services, no real clock in unit tests. Pass time in or fake it.
- No real personal or confidential data in fixtures. Fixtures are plainly invented.
- Expected values are written out, not computed by calling the code under test.
- Decompiled code is tested by the harness (byte exact), not by unit tests.
- A test that needs the retail install is marked `pytest.mark.retail` and is skipped, not failed, where the install is absent.
- Fixtures are invented: addresses in the game's range, names like `TestCaptain`, addresses from the RFC 5737 ranges, keys in the game's format but made up.

## 7. Git and pull requests

### 7.1 Branches

| Branch | Purpose |
| --- | --- |
| `main` | What is released |
| `develop` | Integrated work. The default branch and the base for all new work |
| `feature/<slug>` | Longer-running work. Receives small pull requests; merged into `develop` when the whole thing is done |
| `feat/<slug>`, `fix/<slug>` | One pull request's worth of work |
| `wp/<id>-<slug>` | A work package |
| `grind/<batch>-<lane>` | A grind lane (`harness/loop.sh`) |

- **Never commit to `main` or `develop` directly.** Both accept changes only through pull requests. The hooks in `.githooks/`, switched on once per clone with `sh .githooks/install.sh`, refuse such a commit or push.
- **Never work in the main checkout.** Every change is done in its own git worktree, on its own branch cut from `develop`. Worktrees are created by Claude Code under `.claude/worktrees/` and the branch is then renamed.
- **Releases** are a pull request from `develop` into `main`, merged with a merge commit, never squashed.

### 7.2 Commits

- Small and atomic. One logical change per commit.
- Conventional Commits with the module as scope. Imperative subject, 72 characters or fewer. The body says why when the diff does not.
- The tests for a change are in the same commit as the change.
- Pull requests are squash-merged: what lands on `develop` is one commit per pull request, named after it.

### 7.3 Pull requests

- Atomic: one concern each. Small enough to review in one sitting, or split, or sent through a `feature/` branch in pieces.
- `sh scripts/check.sh` passes before pushing.
- Opened as a draft; marked ready when the checks pass, which starts the automated review. Never left in draft when handed to the owner. Nobody comments `@claude` to ask for a review. A grind landing (a `grind/` or `integrate/` branch) gets no automated review: `check` judges its functions.
- The checks are `tests`, `check` and `hotspots`. A pull request is merged only when they are green; from a fork, where `check` does not run, the description carries the last lines of a local `sh scripts/check.sh`. The automated review is advisory: read it, fix what is right, say why when it is not.
- Title: a Conventional Commits subject. Description: what changed, why, how it was tested.
- A pull request that changes behavior described in DESIGN.md or a rule in this document updates that document in the same pull request.
- Never commit generated output, secrets, the owner's local configuration or anything under `.agent/`.

## 8. Checklists

**Decompiling a function**
1. Find it in `ghidra/exports/functions.csv`; take the unit path from its evidence, or `op/unsorted/`.
2. Write it at its address position in the unit with its `// FUNCTION:` line and the original identifiers.
3. `harness/build-and-verify.sh 0x<address>` until exact, or until the harness's stop rule ends the run; the harness records it in `difficult_functions.tsv` or `revisit_functions.tsv` (harness/README.md, "Model tiers and the stop rule").
4. `harness/update-matched-set.sh 0x<address>`; `python3 scripts/progress.py --gate`.
5. Commit: `feat(op/<dir>): decompile <Class>::<Method>`; the matched set change in the same commit.

**Closing a grind batch**
1. Once the batch's pull requests are merged, on a branch cut from `develop`: `python3 scripts/progress.py --markdown --matched-set analysis/results/matched_set.json`, then `python3 tools/progress_chart.py`. Nothing in the output is edited by hand, and a warning from the second script is fixed, not ignored.
2. Commit the progress files, the README badges and the treemap together: `chore(progress): regenerate after <batch>`, with the before and after figures in the body.
3. The progress files are regenerated here and nowhere else: no CI job writes them, and another pull request does so only when it changes how they are computed. Batch pull requests then never conflict over them.
