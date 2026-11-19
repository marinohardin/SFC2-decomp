"""--all builds several units at once and still reports them in unit order."""
import threading
import time

import pytest

from conftest import FakeIndex
from harness.lib import funcs, matched, paths, source, verify

UNITS = ['a.cpp', 'b.cpp', 'c.cpp', 'd.cpp', 'e.cpp']


@pytest.fixture
def five_units(tmp_path, monkeypatch):
    """Five units with one recorded claim each. Every build fails, after a delay that is longest
    for the first unit, so a pool that reported in finishing order would put a.cpp last."""
    game = tmp_path / 'game'
    game.mkdir()
    entries = {}
    for i, name in enumerate(UNITS):
        a = 0x00401000 + 0x10 * i
        (game / name).write_text(f'// FUNCTION: STARFLEETOP 0x{a:08x}\nvoid f{i}()\n{{\n}}\n')
        entries[f'0x{a:08x}'] = dict(name=f'f{i}', symbol=f'?f{i}@@YAXXZ', unit=name, size=16,
                                     score=100.0, exact=True, recorded='2026-11-01')
    (game / 'UNITS.tsv').write_text('unit\tfirst\tlast\n')
    monkeypatch.setattr(paths, 'SRC_ROOT', str(game))
    monkeypatch.setattr(paths, 'UNITS_TSV', str(game / 'UNITS.tsv'))
    monkeypatch.setattr(paths, 'MATCHED_SET', str(tmp_path / 'matched_set.json'))
    monkeypatch.setattr(paths, 'BOUNDARIES', str(tmp_path / 'boundary_errors.tsv'))
    monkeypatch.setattr(funcs, 'FunctionIndex', lambda *a: FakeIndex())
    monkeypatch.setattr(verify, 'check_exe', lambda rep: None)
    monkeypatch.setattr(source, 'unit_profile',
                        lambda cfg, root, path: (path.rsplit('/', 1)[-1], {'name': 'test'}))
    matched.save(paths.MATCHED_SET, {'version': 1, 'functions': entries})

    state = {'running': 0, 'most': 0}
    lock = threading.Lock()

    def compile_unit(path, profile, src_root, build_root=None):
        name = path.rsplit('/', 1)[-1]
        with lock:
            state['running'] += 1
            state['most'] = max(state['most'], state['running'])
        time.sleep(0.05 * (len(UNITS) - UNITS.index(name)))
        with lock:
            state['running'] -= 1
        return dict(ok=False, rc=2, obj=None, listing=None, secs=0.0, out_dir=str(tmp_path),
                    log=f'{name}(1) : error C2065: broken in {name}')

    monkeypatch.setattr(source, 'compile_unit', compile_unit)
    return state


def build_lines(rep):
    return [ln for ln in rep.lines if 'build ' in ln or 'error C' in ln or 'no longer builds' in ln]


def test_units_are_reported_in_unit_order_whatever_the_number_of_jobs(five_units):
    serial = verify.run([], check_all=True, jobs=1)
    parallel = verify.run([], check_all=True, jobs=4)
    assert parallel.lines == serial.lines
    assert sorted(parallel.regressions) == sorted(serial.regressions) == \
        [0x00401000, 0x00401010, 0x00401020, 0x00401030, 0x00401040]
    order = [ln for ln in build_lines(parallel) if 'BUILD HAS FAILED' in ln]
    assert [ln.split('build ')[1].split(':')[0] for ln in order] == UNITS


def test_jobs_builds_that_many_units_at_once(five_units):
    verify.run([], check_all=True, jobs=1)
    assert five_units['most'] == 1
    five_units['most'] = 0
    verify.run([], check_all=True, jobs=3)
    assert five_units['most'] == 3


@pytest.mark.parametrize('value, want', [('', 4), ('2', 2), ('8', 8), ('0', 4), ('-1', 4),
                                         ('four', 4)])
def test_harness_jobs_sets_the_default(monkeypatch, value, want):
    monkeypatch.setenv('HARNESS_JOBS', value)
    assert verify.default_jobs() == want


def test_in_threads_keeps_the_order_and_raises_a_worker_error():
    assert verify._in_threads(lambda x: x * 2, [3, 1, 2], 2) == [6, 2, 4]

    def boom(x):
        if x == 2:
            raise ValueError('unit 2')
        return x

    with pytest.raises(ValueError, match='unit 2'):
        verify._in_threads(boom, [1, 2, 3], 3)


def test_the_pool_does_not_import_the_queue_module():
    # concurrent.futures imports `queue`, which harness/queue.py shadows when harness/ is on
    # sys.path (CI's test run): the pool must not depend on it.
    src = open(verify.__file__).read()
    assert 'concurrent.futures' not in src
