"""scripts/port_footprint.py lists every PORT: line and SFCOP_FIX( site under a tree."""
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCRIPT = os.path.join(ROOT, 'scripts', 'port_footprint.py')


def write(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w') as f:
        f.write(text)


def run(root, cwd, *flags):
    return subprocess.run([sys.executable, SCRIPT, *flags, root], cwd=cwd, capture_output=True,
                          text=True)


def make_tree(base):
    write(os.path.join(base, 'game', 'Common', 'Allocator.cpp'),
          '// FUNCTION: STARFLEETOP 0x006fc690\n'
          'void *tAllocator::Get(int n) {\n'
          '    long size = n; // PORT: 64-bit, the original stores a pointer in a long\n'
          '#if SFCOP_FIX(fix_shield_overflow)\n'
          '    if (n < 0) return 0;\n'
          '#else\n'
          '#endif\n'
          '    return Pool(size);\n'
          '}\n')
    write(os.path.join(base, 'game', 'Source', 'Ship.h'),
          'struct tShip {\n'
          '    int mShields; /* PORT: undefined behaviour the original relied on */\n'
          '};\n'
          '#if SFCOP_FIX(fix_a) || SFCOP_FIX(fix_b)\n'
          '#endif\n')
    # Not source files, and words that only look like the tags: none of these count.
    write(os.path.join(base, 'game', 'README.md'), '// PORT: a note in a document\nSFCOP_FIX(x)\n')
    write(os.path.join(base, 'game', 'Source', 'Ship.cpp'),
          'int PORT = 1; // the word PORT without the colon tag\n'
          'int MY_SFCOP_FIX(int);\n')


def test_lists_each_site_with_file_and_line_and_the_totals(tmp_path):
    make_tree(str(tmp_path))
    p = run('game', str(tmp_path))
    assert p.returncode == 0, p.stderr
    assert p.stdout.splitlines() == [
        'PORT game/Common/Allocator.cpp:3: long size = n; '
        '// PORT: 64-bit, the original stores a pointer in a long',
        'PORT game/Source/Ship.h:2: int mShields; '
        '/* PORT: undefined behaviour the original relied on */',
        'SFCOP_FIX game/Common/Allocator.cpp:4: #if SFCOP_FIX(fix_shield_overflow)',
        'SFCOP_FIX game/Source/Ship.h:4: #if SFCOP_FIX(fix_a) || SFCOP_FIX(fix_b)',
        'SFCOP_FIX game/Source/Ship.h:4: #if SFCOP_FIX(fix_a) || SFCOP_FIX(fix_b)',
        'PORT lines: 2',
        'SFCOP_FIX sites: 3',
    ]


def test_an_empty_tree_counts_zero(tmp_path):
    os.makedirs(os.path.join(str(tmp_path), 'game'))
    p = run('game', str(tmp_path))
    assert p.returncode == 0
    assert p.stdout.splitlines() == ['PORT lines: 0', 'SFCOP_FIX sites: 0']


def test_a_missing_tree_is_an_error(tmp_path):
    p = run('game', str(tmp_path))
    assert p.returncode == 2
    assert 'no directory game' in p.stderr


def test_the_check_lists_port_lines_in_the_source_tree_that_exists():
    # check.sh skips the step when the folder it names is missing, so a stale folder name turns
    # the step off without failing anything.
    with open(os.path.join(ROOT, 'scripts', 'check.sh')) as f:
        check = f.read()
    tested = re.findall(r'^if \[ -d (\S+) \]; then\n\s+python3 scripts/port_footprint\.py '
                        r'--no-fix-sites (\S+)$', check, re.M)
    assert tested == [('op', 'op')], (
        'check.sh no longer has the PORT: step as `if [ -d op ]; then` followed by '
        '`python3 scripts/port_footprint.py --no-fix-sites op`; update this test with it')
    assert os.path.isdir(os.path.join(ROOT, 'op'))
