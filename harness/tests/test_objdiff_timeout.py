"""An objdiff run that never finishes is reported as an error, not waited on."""
import os
import stat

from harness.lib import paths, verify


def test_an_objdiff_that_does_not_finish_is_an_error(tmp_path, monkeypatch):
    hang = tmp_path / 'objdiff'
    hang.write_text('#!/bin/sh\nexec sleep 30\n')
    hang.chmod(hang.stat().st_mode | stat.S_IXUSR)
    monkeypatch.setattr(paths, 'OBJDIFF', str(hang))
    left, right, err = verify.objdiff('a.obj', 'b.obj', '?f@@YAXXZ', str(tmp_path), timeout=1)
    assert (left, right) == (None, None)
    assert err == 'objdiff did not finish in 1 s'
    assert not os.path.exists(tmp_path / 'objdiff.json')
