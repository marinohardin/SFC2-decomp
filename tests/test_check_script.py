"""scripts/check.sh says NOT VERIFIED on its last line when it rebuilt nothing.

A run without the compiler or the install skips the rebuild and still exits 0, so its last line
must not read as a match. Read from the script's text; nothing is run.
"""
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[1]


def _lines():
    return [l.strip() for l in (ROOT / 'scripts/check.sh').read_text().splitlines()]


def test_only_the_rebuild_sets_the_rebuilt_flag():
    lines = _lines()
    sets = [i for i, l in enumerate(lines) if l == 'rebuilt=1']
    assert len(sets) == 1
    assert lines[sets[0] - 1] == 'sh harness/build-and-verify.sh --all --base "$base"'
    assert 'rebuilt=' in lines[:sets[0]]


def test_the_last_line_names_a_run_that_compared_nothing():
    lines = [l for l in _lines() if l]
    assert lines[-5:] == [
        'if [ "$rebuilt" = 1 ]; then',
        'echo "check: ok"',
        'else',
        'echo "check: ok (NOT VERIFIED: no function was rebuilt or compared with the original)"',
        'fi',
    ]
