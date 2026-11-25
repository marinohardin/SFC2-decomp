"""The owner's released-function list: a listed address may leave the base gate; nothing else may."""
import pytest

from harness.lib import matched

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
