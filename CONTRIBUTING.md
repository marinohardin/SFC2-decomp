# Contributing to SFC2-decomp

This file is for outside contributors: a person, or a coding agent working for one, who wants to
decompile part of Orion Pirates and send it as a pull request. It is meant to be enough on its
own. Where it and `AGENTS.md` differ for you, this file wins.

## Who you are here

- You are an outside contributor. You are not the integrator, no one gives you a brief, no lane
  is assigned to you, and you do not run grind batches. Those are the owner's (see the glossary).
- You work as one lane would: on units you have claimed, committing source only, judged by the
  harness. The owner then rebuilds your work on the owner's machine and merges it.
- AI-written work is welcome. It is held to the same byte-exact check and the same review as any
  other work.

## What you need

On every platform: git, Python 3.9 or later with pytest, Docker (running), `gh` (the GitHub
command line, for the claim list), and your own Orion Pirates install patched to 2.5.5.2, with the
SDK that comes with it. `scripts/setup.sh` checks every file the tools read against
`analysis/results/retail_hashes.sha256`. For the automated loop you also need Claude Code, logged
in with a subscription (the loop refuses an API key).

| Platform | State | Notes |
| --- | --- | --- |
| macOS on Apple silicon | Tested | The harness uses the native compiler, which `setup.sh` fetches |
| Linux x86-64 | **Untested** | The compiler runs in the Docker image. Before running the harness, `export OBJDIFF=<tools>/objdiff/objdiff-cli-linux-x86_64` (the harness's default is the Mac build). `shasum` must be installed (it comes with Perl). A native Linux compiler is planned |
| Windows | **Untested** | Use WSL2 with a Linux distribution and Docker, then follow the Linux steps. Keep the clone, `work/` and `tools/` inside the WSL file system (for example `~/sfc2`), not under `/mnt/c` |

Linux and Windows have not been run end to end. If you try one, say what happened in an issue.

Disk and time, as far as the repository records them (`toolchain/README.md`, `harness/README.md`):
the compiler image is about 100 MB, and building it leaves about 1.5 GB of Docker build cache
(`docker builder prune` removes it). The image takes about 18 minutes to build on an Apple silicon
Mac, most of it emulation; on x86-64 it has not been measured. A full rebuild of every matched
function leaves about 115 MB of output per checkout and takes about 4 minutes cold with the native
compiler; it is slower in the image.

## Setup

Clone the main repository into a folder of its own; `work/` and `tools/` are made beside it.
Add your fork as a second remote, so that `origin/develop` is always the main repository's
`develop`, which every check compares with.

```sh
mkdir sfc2 && cd sfc2
git clone https://github.com/marinohardin/SFC2-decomp.git && cd SFC2-decomp
git remote add fork https://github.com/<you>/SFC2-decomp.git
sh scripts/setup.sh --install "/path/to/Starfleet Command Orion Pirates"
sh .githooks/install.sh
```

`setup.sh` links your install (it is read in place and never written), checks its hashes, builds
the compiler image, fetches the native compiler on a Mac and objdiff, and calibrates. Generated
data goes to `../work/` and downloaded tools to `../tools/`; `SFCOP_WORK` and `SFCOP_TOOLS` move
them.

## Find and claim work

The unit of work is a translation unit: one row of `op/UNITS.tsv`, identified by its address
range, because the `unsorted/` page units can be split again later.

1. `git fetch origin && python3 tools/claims.py free` lists units with functions not yet exact
   that nobody holds, smallest first, with each unit's `range:` line. Units labelled
   `good first unit` in the issues are a good start.
2. Open a claim with the "Claim units" issue form: the units, their `range:` lines and, once you
   have it, your branch.
3. Do not claim inside an open `reserved` issue. The owner posts one for each grind batch, listing
   the ranges the batch's lanes hold, and closes it when the batch is closed.

Rules:

- Keep one or two claims open at a time.
- Stay inside your claim. The guard fails a pull request that adds, changes or removes an
  annotation outside the claimed ranges.
- A claim with no activity for 14 days is closed; a reminder comes at 10 days. A comment on the
  claim issue counts as activity. Close your claim yourself if you stop.

## Work as one lane

1. Branch from `develop` as `contrib/<claim number>-<slug>`, for example `contrib/42-tshipyard`.
2. Make your lane file list from your claim's units, so the harness lets you edit only them and
   the headers only they use:

   ```sh
   mkdir -p ../work/contrib-42
   printf '%s\n' op/unsorted/tShipYard.cpp > ../work/contrib-42/units.txt
   python3 tools/lane_plan.py --units ../work/contrib-42/units.txt --write-files ../work/contrib-42
   export HARNESS_LANE_FILES="$PWD/../work/contrib-42/files-lane.txt"
   export SFCOP_LANE=1
   ```

   `lane_plan.py` also lists the functions a shared header blocks. Leave those. `SFCOP_LANE=1`
   tells `scripts/check.sh` that this branch commits source only.
3. Decompile, with the loop or by hand (`harness/README.md`, "The loop", and
   `harness/SRC_LAYOUT.md`):
   - The loop: `harness/loop.sh --unit unsorted/tShipYard.cpp --no-lean`. It runs one fresh
     session per function, judges each with the harness and commits each improvement.
   - By hand: write the function with its `// FUNCTION: STARFLEETOP 0x<address>` line, then
     `sh harness/build-and-verify.sh <unit or address>` until it is exact or you stop.
4. Follow the stop rule (`harness/README.md`, "Model tiers and the stop rule"): take the easy
   exact functions first. An attempt that does not raise a function's best score ends its run,
   and so do two attempts that leave it at 90% or below. Move on; a near miss is still worth
   committing.
5. Commit source only, one function per commit. The loop writes the message. By hand, write it
   the same way, with the trailer as the last paragraph:

   ```sh
   git commit -m "feat(op): match tShipYard::Reset (0x00401234)" \
              -m "build-and-verify: 0.00% -> 100.00% (exact)." \
              -m "Harness-Score: STARFLEETOP 0x00401234 exact"
   ```

   For an improvement short of exact, the trailer gives the percent: `... 0x00401234 97.93`.
6. If a header you need is outside your file list, do not change it. Open an issue saying which
   header, what it needs and for which address (the loop writes these as `HEADER REQUEST:` lines).

## What never goes in

- Generated files: `analysis/results/matched_set.json`, `op/manifest.tsv` and the symbol table
  `analysis/symbols/STARFLEETOP.tsv`. The owner's landing step writes them after merging. The loop
  commits the symbol table with your source; before the pull request, put it back as it was:
  `git checkout $(git merge-base HEAD origin/develop) -- analysis/symbols/STARFLEETOP.tsv` and
  commit that.
- Anything under `harness/`, `tools/`, `scripts/`, `docs/`, `.github/`, `analysis/`, `toolchain/`,
  `op/port/`, and `op/UNITS.tsv`. If one of them is wrong, open an issue.
- Any byte copied from the game, and any file from the install.
- Names without evidence. `docs/CONVENTIONS.md` §3 and §4 say how source is written and where a
  name may come from: the executable, the SDK, or `fn_<address>` until one of them gives it.

## Before the pull request

Run `sh scripts/check.sh`, with `SFCOP_LANE=1` still set. With the compiler and the game it
rebuilds every matched function, yours included, and fails if any score dropped. Without them
its last line says `NOT VERIFIED`: nothing was compared with the original, and a pass then means
only that the files are well formed. If the symbol table step after the rebuild says the build
compiled another symbol than the table has, run `python3 tools/symbols.py claim`, check again, and
still leave the table out of the pull request as above.

Save the harness's output for your units and make the evidence block:

```sh
sh harness/build-and-verify.sh unsorted/tShipYard.cpp > ../work/contrib-42/result.txt
python3 tools/evidence.py --log ../work/contrib-42/result.txt
```

`evidence.py` masks your home folder and user name. Read what you paste anyway.

## The pull request

Push your branch to your fork and open a draft pull request to `develop`, with a Conventional
Commits title. Fill in the template's "From a fork" section: `Claim: #<number>` on its own line,
the `RESULT` line, and the evidence block. Mark it ready when it is done.

What runs on a fork's pull request, on hosted runners, once the owner approves the run: the
tests, the guard (`tools/contrib_guard.py`, which reads your diff) and the hosted check
(`scripts/check.sh` without the rebuild). What does not run: the build on the owner's machine and
the automated review.

The owner then starts a build review. It takes the head commit the owner read, copies only your
`op/` files into a fresh copy of `develop`, rebuilds them with `develop`'s harness and posts the
result on the pull request, with a `build-review` status. Your local numbers are your evidence;
the build review decides. A pull request is squash-merged, and the owner records the scores in
the landing step afterwards.

## Licence

**Placeholder, not decided.** The licence has not been chosen yet, and there are no contribution
terms. Until there are, the repository has no licence file, and the terms under which a
contribution is accepted are not settled. If that matters to you, wait, or ask in an issue before
you send work.

## Glossary

| Term | Meaning |
| --- | --- |
| Batch | One round of the owner's grind: several lanes started together and closed together |
| Claim | An open issue, from the claim form, holding the units you work on as `range:` lines |
| Exact | A function the harness scores 100% with the strict byte check equal |
| Grind | The owner's automated decompilation, run as batches of lanes |
| Hotspot | A file only the integrator changes (`.github/hotspots.txt`) |
| Integrator | The owner, or the owner's lead agent, who plans batches and merges their work |
| Landing | `harness/landing.sh`, run by the owner after a merge: records the scores, the symbol table and the manifest |
| Lane | One branch of a batch, working on its own units and committing source only |
| Near miss | A claimed function that is not exact yet |
| Stand-in | A placeholder class or callee symbol that is not the original's; the harness refuses new ones |
| Stop rule | When the harness stops working on a function (`harness/README.md`) |
| Unit | One translation unit of the original: a row of `op/UNITS.tsv` and its source file under `op/` |
