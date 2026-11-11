"""Run a command with a wall-clock limit.

    python3 -m harness.lib.timebox <seconds> <command> [args...]

Runs the command in its own process group with this process's standard streams. When it is
still running after <seconds> (0 means no limit), the whole group gets SIGTERM, then SIGKILL
after a grace period, and the exit status is 124, as with coreutils `timeout`, which macOS lacks.
Otherwise the exit status is the command's.

The command's own process group keeps a Ctrl-C at the terminal from reaching it directly, so
this process stands in for it: SIGINT, SIGTERM and SIGHUP sent here are passed on to the whole
group, and this process waits for the command to end (SIGKILL after the grace period) before it
exits with 128 + the signal's number. A session therefore never outlives the loop that started
it, which releases its claim on exit.
"""
import os
import signal
import subprocess
import sys

GRACE = float(os.environ.get('HARNESS_TIMEBOX_GRACE', 10))
TIMED_OUT = 124


FORWARDED = (signal.SIGINT, signal.SIGTERM, signal.SIGHUP)


class _Signalled(Exception):
    def __init__(self, signum):
        self.signum = signum


def _stop(p, first, grace):
    """Send `first` to the command's group, then SIGKILL if it is still there after `grace`."""
    for sig, wait in ((first, grace), (signal.SIGKILL, None)):
        try:
            os.killpg(p.pid, sig)
        except ProcessLookupError:
            break
        try:
            p.wait(timeout=wait)
            break
        except subprocess.TimeoutExpired:
            continue
    try:  # reap it if killpg found the group gone before the wait
        p.wait(timeout=grace)
    except subprocess.TimeoutExpired:
        pass


def run(seconds, argv, grace=GRACE):
    def raise_signalled(signum, frame):
        raise _Signalled(signum)

    old = {s: signal.getsignal(s) for s in FORWARDED}
    for s in FORWARDED:
        signal.signal(s, raise_signalled)
    p = None
    try:
        p = subprocess.Popen(argv, start_new_session=True)
        return p.wait(timeout=seconds if seconds > 0 else None)
    except subprocess.TimeoutExpired:
        for s in FORWARDED:  # no second signal interrupts the shutdown
            signal.signal(s, signal.SIG_IGN)
        _stop(p, signal.SIGTERM, grace)
        return TIMED_OUT
    except _Signalled as e:
        for s in FORWARDED:
            signal.signal(s, signal.SIG_IGN)
        if p is not None:
            _stop(p, e.signum, grace)
        return 128 + e.signum
    finally:
        for s, h in old.items():
            signal.signal(s, h)


def main(argv):
    if len(argv) < 2:
        print(__doc__, file=sys.stderr)
        return 2
    return run(float(argv[0]), argv[1:])


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
