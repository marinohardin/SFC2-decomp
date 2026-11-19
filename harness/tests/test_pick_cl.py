"""The harness compiles natively only when the native compiler is fully installed on macOS."""
import os
import subprocess

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


def test_cl_native_exits_3_without_the_compiler(tmp_path):
    env = dict(os.environ, SFCOP_TOOLS=str(tmp_path / 'none'), SFCOP_WORK=str(tmp_path / 'work'))
    p = subprocess.run(['sh', paths.CL_NATIVE, '/nologo'], env=env, capture_output=True, text=True)
    assert p.returncode == 3
    assert 'fetch-native.sh' in p.stderr


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
    unit = tmp_path / 'u.cpp'
    unit.write_text('int f() { return 0; }\n')
    b = source.compile_unit(str(unit), {}, str(tmp_path), build_root=str(tmp_path / 'build'))
    assert (b['ok'], b['rc']) == (False, -1)
    assert b['secs'] < 10
    pid = int(pidfile.read_text())
    with pytest.raises(ProcessLookupError):
        os.kill(pid, 0)
    assert os.listdir(tmp_path / 'work' / 'toolchain' / 'tmp') == []
