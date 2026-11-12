"""tools/install_manifest.py writes path, size and SHA-256 of an install's import set."""
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCRIPT = os.path.join(ROOT, 'tools', 'install_manifest.py')

# Worked out by hand: printf 'invented exe\n' | shasum -a 256, and so on.
EXE = '6a849476470a588ba548dda079fc6e74a80420b3595b835f1679e0809b54ae49'
MODEL = 'e4abd5e0a853321dba13a23ae9cc5819f03dda33fb74a5c432b3a9b56350f15f'
INI = 'c9be7e83c13e8c1d9e04f2c528d54a662966c3fe6505e01c811021113ac09a75'


def make_install(base):
    files = {
        'StarFleetOP.exe': b'invented exe\n',
        'sfc.ini': b'[3D]\n',
        'Readme.txt': b'[3D]\n',
        'Patch.exe': b'[3D]\n',
        'SDK/Libraries/Shared.lib': b'[3D]\n',
        'Assets/Models/test.mod': b'invented model\n',
        'Assets/.DS_Store': b'[3D]\n',
        'Assets/.hidden/x.mod': b'[3D]\n',
        'metaassets/db/a.txt': b'[3D]\n',
        'EReg/ereg.ini': b'[3D]\n',
    }
    for rel, data in files.items():
        path = os.path.join(base, rel)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, 'wb') as f:
            f.write(data)


def run(*args):
    return subprocess.run([sys.executable, SCRIPT, *args], capture_output=True, text=True)


def test_the_import_set_is_listed_with_sizes_and_hashes(tmp_path):
    make_install(str(tmp_path / 'op'))
    out = tmp_path / 'install_manifest.tsv'
    result = run(str(tmp_path / 'op'), '-o', str(out))
    assert result.returncode == 0, result.stderr
    assert result.stderr == 'install_manifest: 4 files, 38 bytes checked, 0 unchecked\n'
    rows = [l for l in out.read_text().splitlines() if not l.startswith('#')]
    assert rows == [
        'Assets/Models/test.mod\t15\t' + MODEL,
        'StarFleetOP.exe\t13\t' + EXE,
        'metaassets/db/a.txt\t5\t' + INI,
        'sfc.ini\t5\t' + INI,
    ]
    assert out.read_text().startswith(
        '# Sizes and SHA-256 of the files of an installed 2.5.5.2 game')
    assert '# 4 rows checked; 0 rows unchecked' in out.read_text()


def test_listed_files_are_unchecked_rows_with_path_only(tmp_path):
    make_install(str(tmp_path / 'op'))
    listed = tmp_path / 'unchecked.txt'
    listed.write_text('# repaired\nassets/models/TEST.mod\n')
    out = tmp_path / 'install_manifest.tsv'
    result = run(str(tmp_path / 'op'), '-o', str(out), '--unchecked', str(listed))
    assert result.returncode == 0, result.stderr
    assert result.stderr == 'install_manifest: 4 files, 23 bytes checked, 1 unchecked\n'
    rows = [l for l in out.read_text().splitlines() if not l.startswith('#')]
    assert rows[0] == 'Assets/Models/test.mod\t\t'
    assert rows[1] == 'StarFleetOP.exe\t13\t' + EXE
    assert '# 3 rows checked; 1 rows unchecked' in out.read_text()


def test_an_unchecked_list_naming_the_exe_or_a_stray_file_exits_2(tmp_path):
    make_install(str(tmp_path / 'op'))
    listed = tmp_path / 'unchecked.txt'
    listed.write_text('Readme.txt\nStarFleetOP.exe\n')
    result = run(str(tmp_path / 'op'), '--unchecked', str(listed))
    assert result.returncode == 2
    assert result.stderr == ('install_manifest: the unchecked list names files outside the '
                             'import set, or the executable: readme.txt, starfleetop.exe\n')


def test_standard_output_without_o(tmp_path):
    make_install(str(tmp_path / 'op'))
    result = run(str(tmp_path / 'op'))
    assert result.returncode == 0
    assert 'StarFleetOP.exe\t13\t' + EXE + '\n' in result.stdout


def test_a_folder_without_the_game_or_bad_usage_exits_2(tmp_path):
    result = run(str(tmp_path))
    assert result.returncode == 2
    assert result.stderr == 'install_manifest: %s holds no StarFleetOP.exe\n' % tmp_path
    assert run().returncode == 2
    assert run('-o').returncode == 2
