"""tools/install_scan.py lists empty files and BMPs shorter than their header says."""
import os
import struct
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCRIPT = os.path.join(ROOT, 'tools', 'install_scan.py')


def bmp(declared, actual):
    """A BMP file header giving `declared` bytes, padded or cut to `actual` bytes."""
    head = b'BM' + struct.pack('<IHHI', declared, 0, 0, 54)
    return (head + b'\0' * max(0, actual - len(head)))[:actual]


def make_install(base):
    files = {
        'Assets/Models/space/whole.bmp': bmp(1000, 1000),
        'Assets/Models/space/cut.bmp': bmp(17462, 5517),
        'Assets/Models/space/EMPTY.BMP': b'',
        'Assets/Models/space/space24.mod': b'',
        'Assets/Models/space/space21.mod': b'MDLS....',
        'Assets/Models/ship/odd.bmp': b'not a bitmap at all',
        'Assets/Strings/strings.txt': b'hello\n',
    }
    for rel, data in files.items():
        path = os.path.join(base, rel)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, 'wb') as f:
            f.write(data)


def run(*args):
    return subprocess.run([sys.executable, SCRIPT, *args], capture_output=True, text=True)


def test_lists_each_damaged_file(tmp_path):
    make_install(tmp_path)
    r = run(str(tmp_path))
    assert r.returncode == 1
    assert r.stdout.splitlines() == [
        'Assets/Models/ship/odd.bmp\tnot a BMP\t19\t-',
        'Assets/Models/space/EMPTY.BMP\tempty\t0\t-',
        'Assets/Models/space/cut.bmp\ttruncated\t5517\t17462',
        'Assets/Models/space/space24.mod\tempty\t0\t-',
        'install_scan: 4 damaged (2 empty, 2 BMP truncated or not a BMP) of 3 BMP files read',
    ]


def test_under_limits_the_scan(tmp_path):
    make_install(tmp_path)
    r = run(str(tmp_path), '--under', 'Assets/Strings')
    assert r.returncode == 0
    assert r.stdout.splitlines() == ['install_scan: 0 damaged (0 empty, 0 BMP truncated or not a BMP) of 0 BMP files read']


def test_bad_usage(tmp_path):
    assert run().returncode == 2
    assert run(str(tmp_path / 'missing')).returncode == 2
