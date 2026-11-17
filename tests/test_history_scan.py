"""Tests for tools/history_scan.py.

Every name, path, address, key and "retail" byte here is invented. The repository tests build a
throwaway git repository and a throwaway retail folder under pytest's tmp_path, with no remote
and no network.
"""
import importlib.util
import io
import pathlib
import subprocess

import pytest

ROOT = pathlib.Path(__file__).resolve().parents[1]


def _load():
    spec = importlib.util.spec_from_file_location('history_scan', ROOT / 'tools/history_scan.py')
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


history_scan = _load()

# 320 bytes with a NUL every 23, as code and data have: stands in for a retail binary.
RETAIL_BYTES = bytes(0 if i % 23 == 0 else (i * 131 + (i >> 3) * 17 + 7) % 251 + 1
                     for i in range(320))
# A retail text file with CRLF line ends, as the original's sources have.
RETAIL_TEXT = b''.join(b'int gInventedTable%02d = %d * kInventedScale;\r\n' % (i, i * 37)
                       for i in range(12))


def _git(repo, *args, email='test@users.noreply.github.com'):
    subprocess.run(['git', '-c', 'user.name=Test', '-c', 'user.email=' + email,
                    '-c', 'commit.gpgsign=false', '-c', 'core.hooksPath=/dev/null',
                    '-C', str(repo), *args], check=True, capture_output=True)


def _commit(repo, message, files, **kw):
    for name, content in files.items():
        path = repo / name
        path.parent.mkdir(parents=True, exist_ok=True)
        if content is None:
            path.unlink()
        else:
            path.write_bytes(content)
    _git(repo, 'add', '-A')
    _git(repo, 'commit', '-q', '-m', message, **kw)


@pytest.fixture
def repo(tmp_path):
    repo = tmp_path / 'repo'
    repo.mkdir()
    _git(repo, 'init', '-q', '-b', 'develop')
    _commit(repo, 'chore: start', {'README.md': b'An invented project.\n'})
    return repo


def _scan(repo, terms=None, retail=(), extras=()):
    out = io.StringIO()
    found = history_scan.scan(str(repo), ['--all'], terms or history_scan.Terms(),
                              [str(r) for r in retail], [str(e) for e in extras], out=out)
    return {k: v for k, v in found.items()}


def _kinds(found):
    return sorted(found)


def test_a_clean_history_has_no_findings(repo):
    assert _scan(repo) == {}


def test_a_term_is_found_in_a_file_deleted_long_ago(repo):
    _commit(repo, 'docs: add notes', {'notes.md': b'first line\nsee the Zorblatt Archive here\n'})
    _commit(repo, 'docs: drop notes', {'notes.md': None})
    found = _scan(repo, history_scan.Terms(['zorblatt archive']))
    assert _kinds(found) == [('term', 'Zorblatt Archive')]
    finding = found['term', 'Zorblatt Archive']
    assert dict(finding.paths) == {'notes.md': 1}
    assert finding.sample == '2: see the Zorblatt Archive here'
    assert finding.at_tip is False


def test_a_term_still_in_the_tree_is_marked_at_the_tip(repo):
    _commit(repo, 'docs: add notes', {'notes.md': b'the Zorblatt Archive\n'})
    found = _scan(repo, history_scan.Terms(['zorblatt']))
    assert found['term', 'Zorblatt'].at_tip is True


def test_a_term_is_found_in_a_commit_message_a_path_and_a_branch_name(repo):
    _commit(repo, 'docs: copied from zorblatt', {'zorblatt/a.txt': b'nothing\n'})
    _git(repo, 'branch', 'feat/zorblatt-import')
    found = _scan(repo, history_scan.Terms(['zorblatt']))
    paths = set(found['term', 'zorblatt'].paths)
    assert '(a file path)' in paths
    assert '(a ref name)' in paths
    assert any(p.startswith('commit ') for p in paths)


def test_an_allow_line_drops_a_match_inside_it_and_no_other(repo):
    _commit(repo, 'docs: names', {'a.txt': b'by quillon-hub\n', 'b.txt': b'quillon was here\n'})
    found = _scan(repo, history_scan.Terms(['quillon'], allow=['quillon-hub']))
    assert dict(found['term', 'quillon'].paths) == {'b.txt': 1}


def test_a_hashed_secret_is_found_and_shown_masked(repo):
    digest = history_scan.secret_digest(b'QX7Z-11AB-9KLM-22CD')
    assert digest == history_scan.secret_digest(b'qx7z11ab9klm22cd')
    _commit(repo, 'chore: config', {'setup.ini': b'[k]\nkey=QX7Z-11AB-9KLM-22CD\n'})
    found = _scan(repo, history_scan.Terms(hashes=[digest]))
    assert _kinds(found) == [('secret', 'QX**-****-****-****')]


def test_personal_data_shapes(repo):
    _commit(repo, 'chore: paths', {'run.sh': (
        b'cd /Users/zebedee/src/project\n'
        b'cd /Volumes/InventedDisk/stuff\n'
        b'cd /Users/me/project\n'
        b'mail zebedee.quark@mailhost.zz\n'
        b'mail someone@example.com 1234+bot@users.noreply.github.com\n'
        b'serial AB12-CD34-EF56-GH78\n'
        b'range 2000-2001-2002 and ABCD-EFGH-IJKL\n')})
    assert _kinds(_scan(repo)) == [
        ('email', 'zebedee.quark@mailhost.zz'),
        ('key', 'AB**-****-****-****'),
        ('path', '/Users/zebedee'),
        ('path', '/Volumes/InventedDisk'),
    ]


def test_a_commit_identity_that_is_not_a_no_reply_address_is_listed(repo):
    _commit(repo, 'chore: other', {'x.txt': b'x\n'}, email='zebedee@mailhost.zz')
    found = _scan(repo)
    assert ('identity', 'Test <zebedee@mailhost.zz>') in found


def test_an_extra_file_is_scanned(repo, tmp_path):
    extra = tmp_path / 'pulls.txt'
    extra.write_bytes(b'PR 4: taken from the Zorblatt Archive\n')
    found = _scan(repo, history_scan.Terms(['zorblatt']), extras=[extra])
    assert dict(found['term', 'Zorblatt'].paths) == {'extra file pulls.txt': 1}


@pytest.fixture
def retail(tmp_path):
    folder = tmp_path / 'retail'
    (folder / 'sdk').mkdir(parents=True)
    (folder / 'game.bin').write_bytes(RETAIL_BYTES)
    (folder / 'sdk' / 'table.cpp').write_bytes(RETAIL_TEXT)
    return folder


def test_a_copied_retail_file_is_found_after_it_was_deleted(repo, retail):
    _commit(repo, 'chore: add blob', {'data/game.bin': RETAIL_BYTES})
    _commit(repo, 'chore: drop blob', {'data/game.bin': None})
    found = _scan(repo, retail=[retail])
    assert _kinds(found) == [('retail', 'data/game.bin')]
    assert found['retail', 'data/game.bin'].detail == 'identical to retail file game.bin (at byte 0)'


def test_a_slice_of_a_retail_file_inside_a_larger_file_is_found(repo, retail):
    _commit(repo, 'test: fixture', {'fix.dat': b'\x01' * 40 + RETAIL_BYTES[100:220] + b'\x02' * 40})
    found = _scan(repo, retail=[retail])
    assert found['retail', 'fix.dat'].detail == \
        '120 bytes at byte 40 are in retail file game.bin (at byte 100)'


def test_a_hex_dump_and_a_c_array_of_retail_bytes_are_found(repo, retail):
    dump = RETAIL_BYTES[64:192].hex().encode()
    array = b', '.join(b'0x%02x' % b for b in RETAIL_BYTES[200:300])
    _commit(repo, 'test: expected bytes', {
        'expected.txt': b'expected: ' + dump + b'\n',
        'bytes.h': b'static const unsigned char k[] = { ' + array + b', };\n'})
    found = _scan(repo, retail=[retail])
    assert found['retail', 'expected.txt'].detail == \
        'hex dump at byte 10 decodes to 128 bytes of retail file game.bin (at byte 64)'
    assert found['retail', 'bytes.h'].detail == \
        'hex dump at byte 35 decodes to 100 bytes of retail file game.bin (at byte 200)'


def test_a_text_copy_is_found_whatever_its_line_ends(repo, retail):
    _commit(repo, 'feat: table', {'op/table.cpp': RETAIL_TEXT.replace(b'\r\n', b'\n')})
    found = _scan(repo, retail=[retail])
    assert found['retail', 'op/table.cpp'].detail == \
        'identical to retail file sdk/table.cpp (at byte 0)'


def test_one_shared_line_is_not_a_finding(repo, retail):
    _commit(repo, 'feat: one', {'op/one.cpp': (
        b'// written here\nint gInventedTable03 = 111 * kInventedScale;\nint mine = 4;\n')})
    assert _scan(repo, retail=[retail]) == {}


def test_a_file_of_scattered_retail_lines_is_found(repo, tmp_path):
    folder = tmp_path / 'retail'
    folder.mkdir()
    strings = [b'Invented retail message number %03d for the player' % i for i in range(60)]
    (folder / 'game.bin').write_bytes(b'\x00\x00'.join(strings))
    _commit(repo, 'docs: strings', {'strings.txt': b'\n'.join(strings) + b'\n'})
    found = _scan(repo, retail=[folder])
    assert _kinds(found) == [('retail-lines', 'strings.txt')]
    # 60 lines of 49 bytes, in a file of 60 * 50 bytes
    assert found['retail-lines', 'strings.txt'].detail == \
        '98% of the file (2940 of 3000 bytes) is lines that occur in retail files'


def test_exit_status_and_report(repo, tmp_path, capsys):
    terms = tmp_path / 'terms.txt'
    terms.write_text('# invented\nzorblatt\nallow: zorblatt-ok\n')
    args = ['--repo', str(repo), '--terms', str(terms), '--no-retail']
    assert history_scan.main(args) == 0
    assert capsys.readouterr().out.endswith('history scan: nothing found\n')
    _commit(repo, 'docs: notes', {'notes.md': b'zorblatt\n'})
    assert history_scan.main(args) == 1
    out = capsys.readouterr().out
    assert '== PRIVATE TERMS: 1' in out
    assert 'zorblatt   [1 version, at the tip]' in out
    assert out.endswith('history scan: 1 finding\n')
    assert history_scan.main(['--repo', str(repo), '--terms', str(tmp_path / 'none')]) == 2
    terms.write_text('zor(blatt\n')
    assert history_scan.main(args) == 2
