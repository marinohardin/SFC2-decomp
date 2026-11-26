"""The owner's released-function list: a listed address may leave the base gate; nothing else may."""
import json
from types import SimpleNamespace

import pytest

from conftest import FakeIndex
from harness.lib import funcs, matched, paths, source, verify

HEADER = 'address\tdate\treason\n'
ROW = '0x00427366\t2026-11-24\tdefined twice\n'


def entry(score, exact):
    return {'name': 'f', 'score': score, 'exact': exact}


def sets(base, now):
    return ({'version': 1, 'functions': base}, {'version': 1, 'functions': now})


def write(tmp_path, text):
    p = tmp_path / 'released.tsv'
    p.write_text(text)
    return str(p)


def test_a_missing_file_releases_nothing(tmp_path):
    assert matched.load_released(str(tmp_path / 'none.tsv')) == {}


def test_a_listed_address_that_dropped_is_released_and_reported(tmp_path):
    released = matched.load_released(write(tmp_path, HEADER + ROW))
    assert released == {'0x00427366': ('2026-11-24', 'defined twice')}
    base, data = sets({'0x00427366': entry(100.0, True), '0x00401000': entry(50.0, False)},
                      {'0x00401000': entry(50.0, False)})
    out, used, stale = matched.release_released(base, data, released)
    assert used == [('0x00427366', '2026-11-24', 'defined twice')]
    assert stale == []
    assert list(out['functions']) == ['0x00401000']
    assert not any(matched.worse(data['functions'].get(k), v) for k, v in out['functions'].items())


def test_an_unlisted_address_that_dropped_still_fails(tmp_path):
    released = matched.load_released(write(tmp_path, HEADER + ROW))
    base, data = sets({'0x00401000': entry(100.0, True)}, {})
    out, used, stale = matched.release_released(base, data, released)
    assert used == [] and out is base
    assert matched.worse(data['functions'].get('0x00401000'), out['functions']['0x00401000'])


def test_a_listed_address_that_is_still_exact_is_stale(tmp_path):
    released = matched.load_released(write(tmp_path, HEADER + ROW))
    base, data = sets({'0x00427366': entry(100.0, True)}, {'0x00427366': entry(100.0, True)})
    out, used, stale = matched.release_released(base, data, released)
    assert used == [] and stale == ['0x00427366'] and '0x00427366' in out['functions']


def test_a_listed_address_with_a_lowered_score_is_released_too(tmp_path):
    released = matched.load_released(write(tmp_path, HEADER + ROW))
    base, data = sets({'0x00427366': entry(100.0, True)}, {'0x00427366': entry(90.0, False)})
    out, used, _ = matched.release_released(base, data, released)
    assert [u[0] for u in used] == ['0x00427366'] and out['functions'] == {}


@pytest.mark.parametrize('text,message', [
    ('address\tdate\n' + ROW, ':1: the header must be'),
    (HEADER + '0x00427366\t2026-11-24\n', ':2: 2 cells, expected 3'),
    (HEADER + 'oops\t2026-11-24\tr\n', ":2: address 'oops'"),
    (HEADER + '0x00427366\tyesterday\tr\n', ":2: date 'yesterday'"),
    (HEADER + '0x00427366\t2026-11-24\t \n', ':2: the reason is empty'),
    (HEADER + ROW + ROW, ':3: address 0x00427366 is listed twice'),
])
def test_a_malformed_list_is_an_error(tmp_path, text, message):
    with pytest.raises(ValueError, match=message):
        matched.load_released(write(tmp_path, text))


# --- the landing step's in-tree check (verify.py --land) -------------------------------------

LISTED, UNLISTED = 0x00427366, 0x00401000


class _Index(FakeIndex):
    """FakeIndex that also resolves the two invented functions of these tests."""

    def __init__(self):
        super().__init__()
        self.by_address = {a: SimpleNamespace(address=a, name=n, size=16)
                           for a, n in ((LISTED, 'g'), (UNLISTED, 'f'))}

    def resolve(self, ident):
        return self.by_address[int(ident, 16)]


def _recorded(score, exact=False):
    return dict(name='x', symbol='?x@@YAXXZ', unit='u.cpp', size=16, score=score, exact=exact,
                recorded='2026-11-01')


def json_copy(d):
    return json.loads(json.dumps(d))


def _saved(path):
    return json.loads(path.read_text())['functions']


@pytest.fixture
def landing(tmp_path, monkeypatch):
    """One unit claiming LISTED and UNLISTED, both recorded, LISTED on the released list. The
    returned `scores` sets what each function rebuilds to; run() lands the tree against a base
    equal to the recorded matched set."""
    game = tmp_path / 'game'
    game.mkdir()
    (game / 'u.cpp').write_text(f'// FUNCTION: STARFLEETOP 0x{LISTED:08x}\nvoid g()\n{{\n}}\n'
                                f'// FUNCTION: STARFLEETOP 0x{UNLISTED:08x}\nvoid f()\n{{\n}}\n')
    (game / 'UNITS.tsv').write_text('unit\tfirst\tlast\n')
    monkeypatch.setattr(paths, 'SRC_ROOT', str(game))
    monkeypatch.setattr(paths, 'UNITS_TSV', str(game / 'UNITS.tsv'))
    monkeypatch.setattr(paths, 'MATCHED_SET', str(tmp_path / 'matched_set.json'))
    monkeypatch.setattr(paths, 'BOUNDARIES', str(tmp_path / 'boundary_errors.tsv'))
    monkeypatch.setattr(paths, 'RELEASED', write(tmp_path, HEADER + ROW))
    monkeypatch.setattr(funcs, 'FunctionIndex', lambda *a: _Index())
    monkeypatch.setattr(verify, 'check_exe', lambda rep: None)   # no retail file is read
    monkeypatch.setattr(source, 'unit_profile', lambda cfg, root, path: ('u.cpp', {'name': 't'}))
    monkeypatch.setattr(source, 'compile_unit', lambda *a, **k: dict(
        ok=True, obj=str(tmp_path / 'u.obj'), secs=0.0, log=''))
    for name in ('report_line_notes', 'report_shared_symbols', 'report_comdat_kinds',
                 'report_object_clashes', 'report_claim_symbols', 'report_table_breaks',
                 'report_claimed_credits', 'report_renamed_exact'):
        monkeypatch.setattr(verify, name, lambda *a, **k: None)
    scores = {LISTED: (37.41, False), UNLISTED: (50.0, False)}

    def score_function(fn, unit, build, exe, index, recorded=None, table_symbol=None):
        score, exact = scores[fn.address]
        return dict(address=f'0x{fn.address:08x}', name=fn.name, unit='u.cpp', score=score,
                    exact=exact, symbol=f'?{fn.name}@@YAXXZ', size=16, rebuilt_size=16,
                    error=None, relocs=[], diff_rows=[], diff_count=0, side_by_side='-')
    monkeypatch.setattr(verify, 'score_function', score_function)
    state = SimpleNamespace(scores=scores, path=tmp_path / 'matched_set.json')

    def recorded_as(listed):
        """Records LISTED as `listed` (UNLISTED at 50%) in the tree and the base alike."""
        state.base = {'version': 1, 'functions': {'0x00427366': listed,
                                                  '0x00401000': _recorded(50.0)}}
        matched.save(paths.MATCHED_SET, json_copy(state.base))

    def run():
        return verify.run(['0x00401000', '0x00427366'], check_all=True, record=True,
                          now='2026-11-26', base=(json_copy(state.base), 'develop'), land=True)
    recorded_as(_recorded(37.41))
    state.recorded_as, state.run = recorded_as, run
    return state


def test_landing_records_a_listed_function_at_its_lower_score(landing):
    landing.scores[LISTED] = (36.38, False)
    rep = landing.run()
    assert rep.regressions == [] and not rep.failed
    assert ('INFO  0x00427366 g 36.38% partial (16 bytes original, 16 rebuilt; 0 relocations, '
            '0 unverified) released (2026-11-24): defined twice; matched_set.json has 37.41%'
            in rep.lines)
    assert not any('REGRESSION' in ln for ln in rep.lines)
    saved = _saved(landing.path)
    assert saved['0x00427366']['score'] == 36.38
    assert saved['0x00427366']['recorded'] == '2026-11-26'
    assert saved['0x00401000']['score'] == 50.0          # the unlisted entry is untouched


def test_landing_records_a_listed_function_that_is_no_longer_exact(landing):
    landing.recorded_as(_recorded(100.0, True))
    landing.scores[LISTED] = (100.0, False)
    rep = landing.run()
    assert rep.regressions == []
    assert any(ln.startswith('INFO  0x00427366 g 100.00% partial') and
               'released (2026-11-24): defined twice' in ln for ln in rep.lines)
    saved = _saved(landing.path)['0x00427366']
    assert (saved['score'], saved['exact']) == (100.0, False)


def test_landing_still_refuses_an_unlisted_function_that_lowers(landing):
    before = landing.path.read_text()
    landing.scores[UNLISTED] = (40.0, False)
    rep = landing.run()
    assert rep.regressions == [UNLISTED]
    assert any(ln.startswith('ERROR 0x00401000 f 40.00% partial') and
               ln.endswith('REGRESSION: matched_set.json has 50.00%') for ln in rep.lines)
    assert 'INFO  matched_set.json not written: fix the errors above first' in rep.lines
    assert landing.path.read_text() == before


def test_a_listed_function_that_improves_is_recorded_as_usual(landing):
    landing.scores[LISTED] = (60.0, False)
    rep = landing.run()
    assert rep.regressions == []
    assert 'OK    0x00427366 recorded in matched_set.json at 60.00%' in rep.lines
    assert not any('released (' in ln for ln in rep.lines)
    assert _saved(landing.path)['0x00427366']['score'] == 60.0


def test_record_released_stores_only_a_listed_worse_entry():
    released = {'0x00427366': ('2026-11-24', 'defined twice')}
    data = {'version': 1, 'functions': {'0x00427366': _recorded(37.41),
                                        '0x00401000': _recorded(50.0)}}
    assert not matched.record_released(data, UNLISTED, entry(40.0, False), '2026-11-26', released)
    assert not matched.record_released(data, LISTED, entry(37.41, False), '2026-11-26', released)
    assert data['functions']['0x00427366']['score'] == 37.41
    assert matched.record_released(data, LISTED, entry(36.38, False), '2026-11-26', released)
    assert data['functions']['0x00427366'] == dict(name='f', score=36.38, exact=False,
                                                   recorded='2026-11-26')
    assert data['functions']['0x00401000']['score'] == 50.0


def test_a_listed_function_still_exact_keeps_the_stale_row_warning_on_landing(landing):
    landing.recorded_as(_recorded(100.0, True))
    landing.scores[LISTED] = (100.0, True)
    rep = landing.run()
    assert rep.regressions == []
    assert ('WARN  0x00427366 is on analysis/results/released.tsv but is still exact: remove '
            'the row' in rep.lines)
    assert not any('released (' in ln for ln in rep.lines)
    assert _saved(landing.path)['0x00427366']['recorded'] == '2026-11-01'
