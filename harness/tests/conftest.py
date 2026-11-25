import os
import struct
import sys

import pytest

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))



@pytest.fixture(autouse=True)
def _no_harness_cache(monkeypatch):
    """Tests never read or write the real build and score caches; test_cache.py turns them on
    in its own folders."""
    monkeypatch.setenv('HARNESS_NO_CACHE', '1')


@pytest.fixture(autouse=True)
def _no_symbol_table(monkeypatch, tmp_path):
    """Tests never read the checkout's symbol table (harness/lib/symtab.py); a test that wants
    one writes it to symtab.path()."""
    from harness.lib import symtab
    monkeypatch.setattr(symtab, 'path', lambda: str(tmp_path / 'symbols.tsv'))


IMAGE_BASE = 0x400000
TEXT_RVA = 0x1000


def make_pe(text, image_base=IMAGE_BASE, text_rva=TEXT_RVA, tls_dir_rva=None):
    """A minimal PE image with one .text section holding `text` at image_base + text_rva.
    tls_dir_rva, if given, is the RVA (inside `text`) of an IMAGE_TLS_DIRECTORY it declares."""
    raw = bytearray(0x200)
    raw[0:2] = b'MZ'
    struct.pack_into('<I', raw, 0x3C, 0x40)
    raw[0x40:0x44] = b'PE\0\0'
    struct.pack_into('<HHIIIHH', raw, 0x44, 0x14C, 1, 0, 0, 0, 0xE0, 0x102)
    opt = 0x58
    struct.pack_into('<H', raw, opt, 0x10B)
    struct.pack_into('<I', raw, opt + 28, image_base)
    if tls_dir_rva is not None:
        struct.pack_into('<I', raw, opt + 92, 16)
        struct.pack_into('<II', raw, opt + 96 + 9 * 8, tls_dir_rva, 24)
    struct.pack_into('<8sIIII', raw, opt + 0xE0, b'.text', len(text), text_rva, len(text), 0x200)
    return bytes(raw) + text


class FakeIndex:
    """Stands in for funcs.FunctionIndex in target tests."""

    def __init__(self, known=None, starts=()):
        self.known = {k: set(v) for k, v in (known or {}).items()}
        self.names_at = {}
        for name, addrs in self.known.items():
            for a in addrs:
                self.names_at.setdefault(a, set()).add(name)
        self.starts = sorted(starts)

    def next_start(self, address):
        later = [s for s in self.starts if s > address]
        return later[0] if later else None
