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


def _comment(t, body, who='ann', association='NONE'):
    return {'createdAt': t, 'body': body, 'author': {'login': who},
            'authorAssociation': association}


def _issue(number, created, comments=(), labels=('claim',)):
    """An issue opened by ann; a comment is (time, body) by ann, or a full _comment()."""
    return {'number': number, 'state': 'OPEN', 'labels': [{'name': n} for n in labels],
            'createdAt': created, 'author': {'login': 'ann'},
            'comments': [c if isinstance(c, dict) else _comment(*c) for c in comments]}


NOW = datetime.datetime(2026, 11, 25, 3, 0, tzinfo=datetime.timezone.utc)
REMINDER = claims.MARKER + '\nThis claim has had no activity'
BOT = 'github-actions'


def test_expiry_reminds_at_10_days_and_closes_at_14():
    issues = [
        _issue(1, '2026-11-20T10:00:00Z'),                                   # 4 days: nothing
        _issue(2, '2026-11-14T09:00:00Z'),                                   # 10 days 18 hours: remind
        _issue(3, '2026-11-01T00:00:00Z', [('2026-11-14T08:00:00Z', 'still on it')]),
        _issue(4, '2026-11-01T00:00:00Z', [('2026-11-12T00:00:00Z', 'progress'),
                                           _comment('2026-11-22T00:00:00Z', REMINDER, BOT)]),  # reminded
        _issue(5, '2026-11-01T00:00:00Z', [_comment('2026-11-11T00:00:00Z', REMINDER, BOT)]),  # close
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


def test_only_the_author_and_maintainers_count_as_activity_and_only_the_bot_reminds():
    issues = [
        # A stranger's comment and a stranger's copy of the marker change nothing: close.
        _issue(1, '2026-11-01T00:00:00Z', [
            _comment('2026-11-20T00:00:00Z', 'me too', 'eve'),
            _comment('2026-11-21T00:00:00Z', REMINDER, 'eve')]),
        # A maintainer's comment is activity (5 days ago): nothing.
        _issue(2, '2026-11-01T00:00:00Z', [
            _comment('2026-11-20T00:00:00Z', 'keep it', 'own', 'OWNER')]),
        # A collaborator's comment 11 days ago, with no reminder since: remind.
        _issue(3, '2026-11-01T00:00:00Z', [
            _comment('2026-11-14T00:00:00Z', 'ok', 'col', 'COLLABORATOR')]),
        # The author's own fake marker counts as activity, not as a reminder: remind.
        _issue(4, '2026-11-01T00:00:00Z', [('2026-11-14T00:00:00Z', REMINDER)]),
        # The bot's reminder after the author's last comment: nothing yet.
        _issue(5, '2026-11-01T00:00:00Z', [
            ('2026-11-14T00:00:00Z', 'progress'),
            _comment('2026-11-24T00:00:00Z', REMINDER, 'github-actions[bot]')]),
    ]
    assert claims.expiry_plan(issues, NOW) == [
        ('close', 1, 24), ('remind', 3, 11), ('remind', 4, 11),
    ]
