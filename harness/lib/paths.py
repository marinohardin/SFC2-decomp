"""Where the harness finds its inputs and keeps its state.

Every location can be overridden by an environment variable, which is how the tests and the proof
run against their own matched set, unit registry and source root without touching the committed
files.
"""
import hashlib
import os
import platform
import subprocess

HARNESS = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REPO = os.path.dirname(HARNESS)


def _git(repo, *args):
    return subprocess.run(['git', '-C', repo, *args], capture_output=True, text=True,
                          check=True).stdout.strip()


def _main_checkout(repo=REPO):
    """The main checkout: reference/ lives there and is not copied into worktrees.

    When this repository is a submodule of another (the port pins it as decomp/), the main
    checkout is the containing repository's: that is where the clone's reference/ link and
    the folder beside it, holding work/ and tools/, are.
    """
    try:
        top = _git(repo, 'rev-parse', '--show-superproject-working-tree') or repo
        return os.path.dirname(_git(top, 'rev-parse', '--path-format=absolute',
                                    '--git-common-dir'))
    except (OSError, subprocess.CalledProcessError):
        return repo


def env_path(var, default):
    v = os.environ.get(var)
    return os.path.abspath(v) if v else default


MAIN = _main_checkout()
# The SFCOP root holds the repository, work/ and tools/ side by side.
SFCOP_ROOT = env_path('SFCOP_ROOT', os.path.dirname(MAIN))
WORK = env_path('SFCOP_WORK', os.path.join(SFCOP_ROOT, 'work'))
TOOLS = os.path.join(SFCOP_ROOT, 'tools')
REFERENCE = env_path('SFCOP_REFERENCE', os.path.join(MAIN, 'reference'))
EXE_REL = 'installed game/Starfleet Command Orion Pirates/StarFleetOP.exe'
EXE = env_path('HARNESS_EXE', os.path.join(REFERENCE, EXE_REL))
HASHES = os.path.join(REPO, 'analysis', 'results', 'retail_hashes.sha256')
FUNCTIONS_CSV = env_path('HARNESS_FUNCTIONS', os.path.join(REPO, 'ghidra', 'exports', 'functions.csv'))
# Extra name -> address evidence for relocation checks (SDK library matches).
SYMBOL_TSVS = [os.path.join(REPO, 'analysis', 'results', n) for n in
               ('StarFleetOP_2552_sdklib_fuzzy.tsv', 'StarFleetOP_2552_sdklib_fuzzy_data.tsv')]
CL_DOCKER = os.path.join(REPO, 'toolchain', 'bin', 'cl')
CL_NATIVE = os.path.join(REPO, 'toolchain', 'bin', 'cl-native')


def wibo(tools):
    """The wibo binary toolchain/bin/cl-native runs, under the tools folder."""
    return os.path.join(tools, 'wibo', '1.2.0', 'wibo-macos')


def pick_cl(env, tools, system):
    """The compiler wrapper the harness scores with. HARNESS_COMPILER=docker or native forces one;
    otherwise (auto) the native one when toolchain/fetch-native.sh has installed it on macOS,
    else the Docker image. Both score every claimed function the same (toolchain/README.md)."""
    choice = env.get('HARNESS_COMPILER', 'auto')
    if choice in ('docker', 'native'):
        return CL_DOCKER if choice == 'docker' else CL_NATIVE
    if choice != 'auto':
        raise ValueError(f'HARNESS_COMPILER must be auto, docker or native, not {choice!r}')
    tools = env.get('SFCOP_TOOLS') or tools
    native = (system == 'Darwin'
              and os.access(wibo(tools), os.X_OK)
              and all(os.path.isfile(os.path.join(tools, 'msvc6', f'msvc{v}', 'Bin', 'CL.EXE'))
                      for v in ('6.4', '6.5')))
    return CL_NATIVE if native else CL_DOCKER


# The compiler of record: the image (docs/INTERFACE.md). A consumer's build and anything that
# links or compares whole objects uses it.
CL = CL_DOCKER
# What the harness's own scoring compiles with (verify.py): the native compiler on macOS when it
# is installed. Its objects score the same, but differ from the image's in the order of some
# COMDAT sections (toolchain/README.md), so it is never the default of compile_unit.
HARNESS_CL = pick_cl(os.environ, TOOLS, platform.system())
OBJDIFF = env_path('OBJDIFF', os.path.join(TOOLS, 'objdiff', 'objdiff-cli-macos-arm64'))

# The committed matched set: develop's record of the best verified score per function. A grind
# lane never writes it (harness/README.md, "Lanes commit source only"); the landing step does.
COMMITTED_MATCHED_SET = os.path.join(REPO, 'analysis', 'results', 'matched_set.json')
# What the harness reads and update-matched-set.sh writes. By default the committed file, which
# is what a person running build-and-verify.sh by hand is held to. loop.sh exports its lane's own
# copy in the lane's state folder, so the ratchet, the queue and the hooks see the lane's scores
# without a committed file changing.
MATCHED_SET = env_path('HARNESS_MATCHED_SET', COMMITTED_MATCHED_SET)
# Analysis boundaries known to be wrong: an address the analysis records as a function that is
# only a piece of one (an exception prolog split from its body, a static initializer's fragment,
# a compiler stub). queue.py never takes them, and a matched-set entry for one may be dropped if
# it was never exact (harness/README.md, "Boundary errors").
BOUNDARIES = env_path('HARNESS_BOUNDARIES',
                      os.path.join(REPO, 'analysis', 'results', 'boundary_errors.tsv'))
UNITS = env_path('HARNESS_UNITS', os.path.join(HARNESS, 'units.json'))
# The unit map (tools/units.py): one row per translation unit with its address range.
SYMBOL_TABLE = env_path('HARNESS_SYMBOL_TABLE', os.path.join(REPO, 'analysis', 'symbols', 'STARFLEETOP.tsv'))
UNITS_TSV = env_path('HARNESS_UNITS_TSV', os.path.join(REPO, 'op', 'UNITS.tsv'))
# Rebuilt game source (see harness/SRC_LAYOUT.md).
SRC_ROOT = env_path('HARNESS_SRC_ROOT', os.path.join(REPO, 'op'))
# Generated, uncommitted state. Per checkout, so loops in several worktrees do not overwrite
# each other's claim, queue and objects: work/harness/wt-<checkout>-<hash of its path>/.
_wt = f'wt-{os.path.basename(REPO)}-{hashlib.sha1(REPO.encode()).hexdigest()[:8]}'
STATE = env_path('HARNESS_STATE', os.path.join(WORK, 'harness', _wt))
# Shared by all loops on this machine: the attempt log (the stop rule counts every loop's tries)
# and one lock file per claimed function. A run with its own HARNESS_STATE (the proofs, tests)
# keeps these inside it unless HARNESS_SHARED says otherwise.
SHARED = env_path('HARNESS_SHARED', STATE if os.environ.get('HARNESS_STATE')
                  else os.path.join(WORK, 'harness', 'shared'))
BUILD = os.path.join(STATE, 'build')
QUEUE = os.path.join(STATE, 'queue.json')
CLAIM = os.path.join(STATE, 'claim.json')
# What loop.sh --unit/--address resolved to when the loop started (queue.py select).
SELECTION = os.path.join(STATE, 'selection.json')
ATTEMPTS = os.path.join(SHARED, 'attempts.tsv')
# Handoff notes: what each unfinished attempt left for the next one, one file per function.
NOTES = os.path.join(SHARED, 'notes')
CLAIMS = os.path.join(SHARED, 'claims')

# The revisit and difficult lists (harness/README.md, "Model tiers") live outside the repository:
# one sub-folder per lane under HARNESS_LISTS, holding that lane's two files. A loop writes only
# its own lane's files and reads the union of every lane's, so memory carries across lanes with no
# write races and nothing to merge. The lane is the state folder's name (HARNESS_LANE overrides).
# A run with its own HARNESS_STATE (the proofs, tests) keeps the lists inside it.
LISTS = env_path('HARNESS_LISTS', os.path.join(STATE, 'lists') if os.environ.get('HARNESS_STATE')
                 else os.path.join(WORK, 'harness', 'lists'))
LANE = os.environ.get('HARNESS_LANE') or os.path.basename(STATE)
DIFFICULT_NAME = 'difficult_functions.tsv'
REVISIT_NAME = 'revisit_functions.tsv'
# Functions that had every attempt the stop rule gives without becoming exact, for a human.
DIFFICULT = env_path('HARNESS_DIFFICULT', os.path.join(LISTS, LANE, DIFFICULT_NAME))
# Functions the stop rule ended short of 100% before their full chance (harness/README.md,
# "Model tiers"); queue.py skips them unless HARNESS_TAKE_REVISIT=1 (loop.sh --revisit).
REVISIT = env_path('HARNESS_REVISIT_LIST', os.path.join(LISTS, LANE, REVISIT_NAME))
# The near-miss label of each non-exact function of this lane (harness/lib/nearmiss.py); only
# this lane's file is read.
NEARMISS = env_path('HARNESS_NEARMISS_LABELS', os.path.join(LISTS, LANE, 'nearmiss_labels.tsv'))


def _all_lists(own, name, overridden):
    """This lane's file first, then the same file of every other lane. An explicit path
    (HARNESS_DIFFICULT, HARNESS_REVISIT_LIST) is the only file read."""
    if overridden:
        return [own]
    import glob
    others = sorted(p for p in glob.glob(os.path.join(LISTS, '*', name)) if p != own)
    return [own] + others


def difficult_files():
    return _all_lists(DIFFICULT, DIFFICULT_NAME, 'HARNESS_DIFFICULT' in os.environ)


def revisit_files():
    return _all_lists(REVISIT, REVISIT_NAME, 'HARNESS_REVISIT_LIST' in os.environ)
