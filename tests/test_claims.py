"""tools/claims.py: free units, and the expiry of quiet claims. Fixtures in place of gh; no
network, no real clock. Expected values are written out by hand."""
import datetime
import importlib.util
import json
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[1]
_spec = importlib.util.spec_from_file_location('claims', ROOT / 'tools/claims.py')
claims = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(claims)

UNITS = [
    {'unit': 'unsorted/a.cpp', 'first': '0x00401000', 'last': '0x00401040'},
    {'unit': 'unsorted/b.cpp', 'first': '0x00402000', 'last': '0x00402020'},
    {'unit': 'Source/c.cpp', 'first': '0x00403000', 'last': '0x00403100'},
    {'unit': 'unsorted/d.cpp', 'first': '0x00404000', 'last': '0x00404010'},
    {'unit': 'unsorted/e.cpp', 'first': '0x00405000', 'last': '0x00405010'},
]


def _fn(address, size, kind='function', library='0'):
    return {'address': address, 'size': str(size), 'kind': kind, 'is_library': library}


FUNCTIONS = [
    _fn('00401000', 40), _fn('00401020', 30), _fn('00401040', 10, kind='eh-funclet'),
    _fn('00402000', 8), _fn('00402010', 6, library='1'),
    _fn('00403000', 100), _fn('00403080', 20),
    _fn('00404000', 5),
    _fn('00405000', 7),
]
MATCHED = {'0x00401000': {'exact': True}, '0x00401020': {'exact': False, 'score': 80.0},
           '0x00404000': {'exact': True}}
ISSUES = [
    {'number': 3, 'state': 'OPEN', 'labels': [{'name': 'claim'}],
     'body': 'range: 0x00403050-0x00403060\n'},
    {'number': 4, 'state': 'OPEN', 'labels': [{'name': 'reserved'}],
     'body': 'Batch 7, lane a\nrange: 0x00405000-0x00405000\n'},
    {'number': 5, 'state': 'CLOSED', 'labels': [{'name': 'claim'}],
     'body': 'range: 0x00402000-0x00402020\n'},
]


def test_free_units_skip_exact_library_and_claimed_work():
    taken = claims.taken_ranges(ISSUES)
    assert taken == [(0x403050, 0x403060), (0x405000, 0x405000)]
    assert claims.free_units(UNITS, FUNCTIONS, MATCHED, {0x00402000}, taken) == [
        ('unsorted/a.cpp', 0x401000, 0x401040, 1, 30),
    ]
    assert claims.free_units(UNITS, FUNCTIONS, MATCHED, set(), taken) == [
        ('unsorted/b.cpp', 0x402000, 0x402020, 1, 8),
        ('unsorted/a.cpp', 0x401000, 0x401040, 1, 30),
    ]


def _issue(number, created, comments=(), labels=('claim',)):
    return {'number': number, 'state': 'OPEN', 'labels': [{'name': n} for n in labels],
            'createdAt': created,
            'comments': [{'createdAt': t, 'body': b} for t, b in comments]}


NOW = datetime.datetime(2026, 11, 25, 3, 0, tzinfo=datetime.timezone.utc)
REMINDER = claims.MARKER + '\nThis claim has had no activity'


def test_expiry_reminds_at_10_days_and_closes_at_14():
    issues = [
        _issue(1, '2026-11-20T10:00:00Z'),                                   # 4 days: nothing
        _issue(2, '2026-11-14T09:00:00Z'),                                   # 10 days 18 hours: remind
        _issue(3, '2026-11-01T00:00:00Z', [('2026-11-14T08:00:00Z', 'still on it')]),
        _issue(4, '2026-11-01T00:00:00Z', [('2026-11-12T00:00:00Z', 'progress'),
                                           ('2026-11-22T00:00:00Z', REMINDER)]),  # reminded
        _issue(5, '2026-11-01T00:00:00Z', [('2026-11-11T00:00:00Z', REMINDER)]),  # 24: close
        _issue(6, '2026-10-01T00:00:00Z', labels=('claim', 'reserved')),     # never
        _issue(7, '2026-10-01T00:00:00Z', labels=('question',)),             # not a claim
    ]
    assert claims.expiry_plan(issues, NOW) == [
        ('remind', 2, 10), ('remind', 3, 10), ('close', 5, 24),
    ]


def test_expire_prints_without_apply_and_calls_gh_with_it(tmp_path, capsys):
    f = tmp_path / 'issues.json'
    f.write_text(json.dumps([_issue(2, '2026-11-14T09:00:00Z'),
                             _issue(5, '2026-11-01T00:00:00Z')]))
    calls = []

    class Done:
        returncode = 0

    def run(cmd):
        calls.append(cmd)
        return Done()
    argv = ['expire', '--now', '2026-11-25T03:00:00Z', '--issues', str(f)]
    assert claims.main(argv, run=run) == 0
    assert calls == []
    assert capsys.readouterr().out == ('remind #2 (10 days without activity)\n'
                                       'close #5 (24 days without activity)\n')
    assert claims.main(argv + ['--apply'], run=run) == 0
    assert [c[:4] for c in calls] == [['gh', 'issue', 'comment', '2'],
                                      ['gh', 'issue', 'close', '5']]
    assert calls[0][5].startswith(claims.MARKER + '\nThis claim has had no activity for 10 days')
