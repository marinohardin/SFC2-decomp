"""Where the harness finds its inputs and keeps its state.

Every location can be overridden by an environment variable, which is how the tests and the proof
run against their own matched set, unit registry and source root without touching the committed
files.
"""
import hashlib
import os
import subprocess

HARNESS = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REPO = os.path.dirname(HARNESS)


def _main_checkout():
    """The main checkout: reference/ lives there and is not copied into worktrees."""
    try:
        common = subprocess.run(['git', '-C', REPO, 'rev-parse', '--path-format=absolute',
                                 '--git-common-dir'], capture_output=True, text=True,
                                check=True).stdout.strip()
        return os.path.dirname(common)
    except (OSError, subprocess.CalledProcessError):
        return REPO


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
CL = os.path.join(REPO, 'toolchain', 'bin', 'cl')
OBJDIFF = env_path('OBJDIFF', os.path.join(TOOLS, 'objdiff', 'objdiff-cli-macos-arm64'))

# Committed results, written only by update-matched-set.sh and queue.py. They live in
# analysis/results/, not harness/, because harness/ is a hotspot and every grind pull request
# updates them.
MATCHED_SET = env_path('HARNESS_MATCHED_SET',
                       os.path.join(REPO, 'analysis', 'results', 'matched_set.json'))
DIFFICULT = env_path('HARNESS_DIFFICULT',
                     os.path.join(REPO, 'analysis', 'results', 'difficult_functions.tsv'))
UNITS = env_path('HARNESS_UNITS', os.path.join(HARNESS, 'units.json'))
# Rebuilt game source (see harness/SRC_LAYOUT.md).
SRC_ROOT = env_path('HARNESS_SRC_ROOT', os.path.join(REPO, 'game'))
# Generated, uncommitted state. Per checkout, so loops in several worktrees do not overwrite
# each other's claim, queue and objects: work/harness/wt-<checkout>-<hash of its path>/.
_wt = f'wt-{os.path.basename(REPO)}-{hashlib.sha1(REPO.encode()).hexdigest()[:8]}'
STATE = env_path('HARNESS_STATE', os.path.join(WORK, 'harness', _wt))
# Shared by all loops on this machine: the attempt log (attempt caps count every loop's tries)
# and one lock file per claimed function. A run with its own HARNESS_STATE (the proofs, tests)
# keeps these inside it unless HARNESS_SHARED says otherwise.
SHARED = env_path('HARNESS_SHARED', STATE if os.environ.get('HARNESS_STATE')
                  else os.path.join(WORK, 'harness', 'shared'))
BUILD = os.path.join(STATE, 'build')
QUEUE = os.path.join(STATE, 'queue.json')
CLAIM = os.path.join(STATE, 'claim.json')
ATTEMPTS = os.path.join(SHARED, 'attempts.tsv')
CLAIMS = os.path.join(SHARED, 'claims')
