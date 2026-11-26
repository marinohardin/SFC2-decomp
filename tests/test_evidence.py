"""tools/evidence.py: the masked evidence block a contributor pastes. No network, no install;
expected values are written out by hand."""
import importlib.util
import pathlib
import struct

ROOT = pathlib.Path(__file__).resolve().parents[1]
_spec = importlib.util.spec_from_file_location('evidence', ROOT / 'tools/evidence.py')
evidence = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(evidence)


def test_mask_hides_the_home_directory_and_the_user_name():
    text = 'objdiff at /home/ann/sfc2/tools/objdiff, user ann, annex /srv/ann/x, planned'
    assert evidence.mask(text, home='/home/ann', user='ann') == \
        'objdiff at ~/sfc2/tools/objdiff, user <user>, annex /srv/<user>/x, planned'


def test_mask_leaves_a_very_short_user_name_alone():
    assert evidence.mask('/Users/al/x al', home='/Users/al', user='al') == '~/x al'


LOG = '''INFO  exe ok
OK    0x00401017 tApp3dBase::tApp3dBase 100.00% exact (28 bytes original, 28 rebuilt; 0 relocations, 0 unverified); matched_set.json has 100.00%
ERROR 0x00401033 tApp3dBase::vfunction13 97.00% partial (5 bytes original, 5 rebuilt; 0 relocations, 0 unverified) REGRESSION: matched_set.json has 100.00%
ERROR   relocation +0x11 ??3@YAXPAX@Z: exe uses 0x00401000, known at 0x00402000
WARN  3 relocations of exact functions name another symbol than the claim (class stand-in); see x
WARN  stand-in: 0x00401000 reaches 0x00402000 as ?f@@YAXXZ
WARN  something else
ERROR build Source/a.cpp: BUILD HAS FAILED. Treat this as a build failure
RESULT ERROR regression
'''


def test_summarize_log_counts_error_and_warn_lines_by_class():
    result, counts = evidence.summarize_log(LOG)
    assert result == 'RESULT ERROR regression'
    assert counts == {('ERROR', 'regression'): 1, ('ERROR', 'relocation'): 1,
                      ('ERROR', 'build-failed'): 1, ('WARN', 'stand-in'): 2,
                      ('WARN', 'other'): 1}


def test_pe_version_reads_the_fixed_file_info(tmp_path):
    exe = tmp_path / 'CL.EXE'
    exe.write_bytes(b'MZ' + b'\0' * 40 + b'\xbd\x04\xef\xfe'
                    + struct.pack('<III', 0x10000, (12 << 16) | 0, (8804 << 16) | 0) + b'\0' * 8)
    assert evidence.pe_version(str(exe)) == '12.0.8804.0'
    (tmp_path / 'none').write_bytes(b'MZ')
    assert evidence.pe_version(str(tmp_path / 'none')) is None
    assert evidence.pe_version(str(tmp_path / 'missing')) is None


def test_judge_hash_covers_the_name_and_content_of_each_module(tmp_path):
    lib = tmp_path / 'harness' / 'lib'
    lib.mkdir(parents=True)
    (lib / 'a.py').write_text('x = 1\n')
    (lib / 'notes.txt').write_text('ignored\n')
    first = evidence.judge_hash(str(tmp_path))
    (lib / 'notes.txt').write_text('changed\n')
    assert evidence.judge_hash(str(tmp_path)) == first
    (lib / 'a.py').write_text('x = 2\n')
    assert evidence.judge_hash(str(tmp_path)) != first
    assert len(first) == 64
