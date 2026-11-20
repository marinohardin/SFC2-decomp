"""The harness compiles natively only when the native compiler is fully installed on macOS."""
import os
import subprocess
import time

import pytest

from harness.lib import paths


def install(tools, wibo=True, versions=('6.4', '6.5')):
    if wibo:
        w = os.path.join(tools, 'wibo', '1.2.0', 'wibo-macos')
        os.makedirs(os.path.dirname(w))
        with open(w, 'w') as f:
            f.write('#!/bin/sh\n')
        os.chmod(w, 0o755)
    for v in versions:
        d = os.path.join(tools, 'msvc6', f'msvc{v}', 'Bin')
        os.makedirs(d)
        open(os.path.join(d, 'CL.EXE'), 'wb').close()
    return str(tools)


def test_auto_takes_native_when_installed_on_macos(tmp_path):
    assert paths.pick_cl({}, install(tmp_path), 'Darwin') == paths.CL_NATIVE


@pytest.mark.parametrize('kw', [dict(wibo=False), dict(versions=('6.4',)), dict(versions=())])
def test_auto_takes_docker_when_anything_is_missing(tmp_path, kw):
    assert paths.pick_cl({}, install(tmp_path, **kw), 'Darwin') == paths.CL_DOCKER


def test_auto_takes_docker_off_macos(tmp_path):
    assert paths.pick_cl({}, install(tmp_path), 'Linux') == paths.CL_DOCKER


def test_sfcop_tools_overrides_the_default_folder(tmp_path):
    tools = install(tmp_path / 'elsewhere')
    os.makedirs(tmp_path / 'empty')
    assert paths.pick_cl({'SFCOP_TOOLS': tools}, str(tmp_path / 'empty'), 'Darwin') == paths.CL_NATIVE


def test_harness_compiler_forces_either(tmp_path):
    tools = install(tmp_path)
    assert paths.pick_cl({'HARNESS_COMPILER': 'docker'}, tools, 'Darwin') == paths.CL_DOCKER
    assert paths.pick_cl({'HARNESS_COMPILER': 'native'}, str(tmp_path / 'none'), 'Linux') == paths.CL_NATIVE


def test_harness_compiler_rejects_other_values(tmp_path):
    with pytest.raises(ValueError):
        paths.pick_cl({'HARNESS_COMPILER': 'wine'}, str(tmp_path), 'Darwin')


def test_the_compiler_of_record_stays_the_image():
    """docs/INTERFACE.md: a consumer's compile_unit call gets the image, whatever is installed;
    only the harness's scoring passes paths.HARNESS_CL."""
    import inspect
    from harness.lib import source
    assert paths.CL == paths.CL_DOCKER
    assert inspect.signature(source.compile_unit).parameters['cl'].default is None


def test_cl_native_exits_3_without_the_compiler(tmp_path):
    env = dict(os.environ, SFCOP_TOOLS=str(tmp_path / 'none'), SFCOP_WORK=str(tmp_path / 'work'))
    p = subprocess.run(['sh', paths.CL_NATIVE, '/nologo'], env=env, capture_output=True, text=True)
    assert p.returncode == 3
    assert 'fetch-native.sh' in p.stderr


def _alive(pid):
    """True while `pid` runs; a zombie (killed, not yet reaped) counts as stopped."""
    for _ in range(50):
        p = subprocess.run(['ps', '-o', 'stat=', '-p', str(pid)], capture_output=True, text=True)
        stat = p.stdout.strip()
        if not stat or stat.startswith('Z'):
            return False
        time.sleep(0.1)
    return True


def test_an_interrupted_compile_is_stopped_with_its_compiler(tmp_path, monkeypatch):
    """Ctrl-C (KeyboardInterrupt in the harness) stops the compile's whole group too."""
    from harness.lib import source
    tools = tmp_path / 'tools'
    install(tools)
    pidfile = tmp_path / 'compiler.pid'
    (tools / 'wibo' / '1.2.0' / 'wibo-macos').write_text(
        f'#!/bin/sh\necho $$ > {pidfile}\nexec sleep 30\n')
    monkeypatch.setenv('SFCOP_TOOLS', str(tools))
    monkeypatch.setenv('SFCOP_WORK', str(tmp_path / 'work'))
    monkeypatch.setattr(paths, 'CL', paths.CL_NATIVE)
    real = subprocess.Popen.communicate

    def interrupted(self, input=None, timeout=None):
        if timeout == source.COMPILE_TIMEOUT:
            for _ in range(100):
                if pidfile.exists() and pidfile.read_text().strip():
                    break
                time.sleep(0.05)
            raise KeyboardInterrupt
        return real(self, input, timeout)
    monkeypatch.setattr(subprocess.Popen, 'communicate', interrupted)
    unit = tmp_path / 'u.cpp'
    unit.write_text('int f() { return 0; }\n')
    with pytest.raises(KeyboardInterrupt):
        source.compile_unit(str(unit), {}, str(tmp_path), build_root=str(tmp_path / 'build'))
    assert not _alive(int(pidfile.read_text()))
    assert os.listdir(tmp_path / 'work' / 'toolchain' / 'tmp') == []


def test_a_compile_that_does_not_finish_is_stopped_with_its_compiler(tmp_path, monkeypatch):
    """The timeout stops the wrapper and the compiler it started, and cl-native's temporary
    folder is removed."""
    from harness.lib import source
    tools = tmp_path / 'tools'
    install(tools)
    pidfile = tmp_path / 'compiler.pid'
    (tools / 'wibo' / '1.2.0' / 'wibo-macos').write_text(
        f'#!/bin/sh\necho $$ > {pidfile}\nexec sleep 30\n')
    monkeypatch.setenv('SFCOP_TOOLS', str(tools))
    monkeypatch.setenv('SFCOP_WORK', str(tmp_path / 'work'))
    monkeypatch.setattr(paths, 'CL', paths.CL_NATIVE)
    monkeypatch.setattr(source, 'COMPILE_TIMEOUT', 1)
    # No docker on PATH: the native compiler needs none, and stopping it must not either.
    monkeypatch.setenv('PATH', '/usr/bin:/bin')
    unit = tmp_path / 'u.cpp'
    unit.write_text('int f() { return 0; }\n')
    b = source.compile_unit(str(unit), {}, str(tmp_path), build_root=str(tmp_path / 'build'))
    assert (b['ok'], b['rc']) == (False, -1)
    assert b['secs'] < 10
    pid = int(pidfile.read_text())
    assert not _alive(pid)
    assert os.listdir(tmp_path / 'work' / 'toolchain' / 'tmp') == []


def test_an_interrupt_stops_the_compiles_of_every_worker(tmp_path, monkeypatch):
    """Ctrl-C reaches only the main thread; the compiles that two worker threads started are
    stopped too, and no worker starts another."""
    import threading
    from harness.lib import source, verify
    tools = tmp_path / 'tools'
    install(tools)
    pids = tmp_path / 'pids'
    pids.mkdir()
    (tools / 'wibo' / '1.2.0' / 'wibo-macos').write_text(
        f'#!/bin/sh\necho $$ > {pids}/$$\nexec sleep 30\n')
    monkeypatch.setenv('SFCOP_TOOLS', str(tools))
    monkeypatch.setenv('SFCOP_WORK', str(tmp_path / 'work'))
    monkeypatch.setattr(paths, 'CL', paths.CL_NATIVE)
    units = []
    for n in range(4):
        u = tmp_path / f'u{n}.cpp'
        u.write_text('int f() { return 0; }\n')
        units.append(str(u))
    real_join = threading.Thread.join

    def join(self, timeout=None):
        if threading.current_thread() is threading.main_thread():
            for _ in range(200):
                if len(os.listdir(pids)) >= 2:
                    break
                time.sleep(0.05)
            raise KeyboardInterrupt
        return real_join(self, timeout)
    monkeypatch.setattr(threading.Thread, 'join', join)
    with pytest.raises(KeyboardInterrupt):
        verify._in_threads(lambda u: source.compile_unit(u, {}, str(tmp_path),
                                                         build_root=str(tmp_path / 'build')),
                           units, 2)
    monkeypatch.setattr(threading.Thread, 'join', real_join)
    started = [int(x) for x in os.listdir(pids)]
    assert len(started) == 2
    time.sleep(1)
    assert len(os.listdir(pids)) == 2
    assert not any(_alive(pid) for pid in started)


def test_no_compile_starts_once_the_running_ones_are_stopped(tmp_path, monkeypatch):
    """A worker that reaches compile_unit after stop_all took its list of running compiles does
    not start a compile the list would have missed; allow_compiles lets the next run start."""
    from harness.lib import source
    started = []
    monkeypatch.setattr(subprocess, 'Popen', lambda *a, **k: started.append(a))
    unit = tmp_path / 'u.cpp'
    unit.write_text('int f() { return 0; }\n')
    source.stop_all(grace=0)
    try:
        with pytest.raises(KeyboardInterrupt):
            source.compile_unit(str(unit), {}, str(tmp_path), build_root=str(tmp_path / 'build'))
        assert started == []
    finally:
        source.allow_compiles()
    assert source._stopping is False


def test_an_interrupted_run_leaves_compiles_allowed(tmp_path, monkeypatch):
    import threading
    from harness.lib import source, verify
    real_join = threading.Thread.join

    def join(self, timeout=None):
        if threading.current_thread() is threading.main_thread():
            raise KeyboardInterrupt
        return real_join(self, timeout)
    monkeypatch.setattr(threading.Thread, 'join', join)
    with pytest.raises(KeyboardInterrupt):
        verify._in_threads(lambda x: time.sleep(0.2), range(4), 2)
    assert source._stopping is False


def test_ci_checks_natively_and_the_audit_uses_the_image():
    """The check job's runner is a Mac that has the native compiler; the per-pull-request check
    uses it, and the scheduled audit rebuilds with the image, the compiler of record."""
    workflows = os.path.join(paths.REPO, '.github', 'workflows')
    with open(os.path.join(workflows, 'pull-request.yml')) as f:
        text = f.read()
    check = text[text.index('\n  check:'):]
    assert 'HARNESS_COMPILER: native' in check
    with open(os.path.join(workflows, 'docker-audit.yml')) as f:
        assert 'HARNESS_COMPILER: docker' in f.read()


def test_a_new_run_allows_compiles_again():
    from harness.lib import source, verify
    source.stop_all(grace=0)
    assert verify._in_threads(lambda x: x * 2, [1, 2], 2) == [2, 4]
    assert source._stopping is False
