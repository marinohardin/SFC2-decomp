"""The harness's two lists of unfinished functions hold each address once, and never both.

`harness/lib/workqueue.py` reads each list into a dict keyed by address, so a second row for an
address silently replaces the first, and its `listed` time decides which attempts a revisit run
counts. An address on both lists is skipped for good by the difficult list while its revisit row
says it is still waiting.
"""
import collections
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[1]
RESULTS = ROOT / "analysis" / "results"


def addresses(text):
    """Return the address column of a list's text, header excluded, in file order."""
    return [line.split("\t", 1)[0] for line in text.splitlines()[1:] if line.strip()]


def repeated(rows):
    """Return the addresses that appear more than once, sorted."""
    return sorted(a for a, n in collections.Counter(rows).items() if n > 1)


def test_repeated_finds_an_address_listed_twice():
    text = "address\tname\n0x00401000\ta\n0x00402000\tb\n0x00401000\ta\n"
    assert repeated(addresses(text)) == ["0x00401000"]


def test_repeated_is_empty_for_distinct_addresses():
    assert repeated(addresses("address\tname\n0x00401000\ta\n0x00402000\tb\n")) == []


def test_each_list_holds_an_address_once():
    for name in ("difficult_functions.tsv", "revisit_functions.tsv"):
        assert repeated(addresses((RESULTS / name).read_text())) == [], name


def test_no_address_is_on_both_lists():
    difficult = set(addresses((RESULTS / "difficult_functions.tsv").read_text()))
    revisit = set(addresses((RESULTS / "revisit_functions.tsv").read_text()))
    assert sorted(difficult & revisit) == []
