#!/usr/bin/env python3
"""Exercise real terminal input with synthetic credentials; never system PAM."""
import os
from pathlib import Path
import pty
import select
import signal
import sys
import termios
import time

build = Path(sys.argv[1]).resolve()
CASES = ('tty-password', 'tty-match', 'tty-retry-password', 'tty-match-password',
         'tty-match-wrong-password', 'tty-match-erased', 'tty-match-escape', 'tty-startup-password',
         'tty-noninteractive', 'tty-cancel', 'tty-blocked-cancel', 'tty-overflow', 'tty-match-unicode-erased')
for mode in CASES:
    child, terminal = pty.fork()
    if child == 0:
        os.environ['LC_ALL'] = 'C.UTF-8'
        os.environ['TERM'] = 'xterm-256color'
        os.environ.pop('NO_COLOR', None)
        os.execv(str(build / 'pam-conversation-test'), [str(build / 'pam-conversation-test'),
                 str(build / 'pam_fpstudio_test.so'), mode, str(build / 'pam_test_token.so')])
    output = b''
    partial = typed = False
    started = time.monotonic()
    try:
        while time.monotonic() - started < 4:
            if select.select([terminal], [], [], .01)[0]:
                try: chunk = os.read(terminal, 8192)
                except OSError: break
                if not chunk: break
                output += chunk
            prompt = b'Password or fingerprint:' in output
            if mode in ('tty-cancel', 'tty-blocked-cancel', 'tty-overflow') and not typed and prompt:
                os.write(terminal, b'synthetic-' * 200 + b'\n' if mode == 'tty-overflow' else b'\x03')
                typed = True
            if mode == 'tty-startup-password' and not typed and time.monotonic() - started > .05:
                assert not termios.tcgetattr(terminal)[3] & termios.ECHO, 'startup echo is on'
                os.write(terminal, b'synthetic-test-password\n')
                typed = True
            if mode == 'tty-password' and not typed and prompt:
                assert not termios.tcgetattr(terminal)[3] & termios.ECHO
                os.write(terminal, b'synthetic-test-password\n')
                typed = True
            if mode in ('tty-match-password', 'tty-match-wrong-password', 'tty-retry-password') and not partial and prompt:
                os.write(terminal, b'synthetic-')
                partial = True
            if mode in ('tty-match-erased', 'tty-match-escape', 'tty-match-unicode-erased') and not partial and prompt:
                key = ('가'.encode() + b'\x7f' if mode == 'tty-match-unicode-erased' else
                       b'x\x7f' if mode == 'tty-match-erased' else b'\x1b[D')
                os.write(terminal, key)
                partial = True
            match = b'Matched.' in output
            retry = b'(1/20)' in output
            if partial and not typed and (match or (mode == 'tty-retry-password' and retry)):
                time.sleep(.1)
                assert not termios.tcgetattr(terminal)[3] & termios.ECHO, 'early echo restore after keypress'
                if mode in ('tty-match-erased', 'tty-match-escape', 'tty-match-unicode-erased'): tail = b'\n'
                elif mode == 'tty-match-wrong-password': tail = b'wrong-password\n'
                else: tail = b'test-password\n'
                os.write(terminal, tail)
                typed = True
        done, status = os.waitpid(child, os.WNOHANG)
        deadline = time.monotonic() + .5
        while done == 0 and time.monotonic() < deadline:
            time.sleep(.01)
            done, status = os.waitpid(child, os.WNOHANG)
        expected = -signal.SIGINT if mode in ('tty-cancel', 'tty-blocked-cancel') else 0
        assert done == child and os.waitstatus_to_exitcode(status) == expected, (mode, status, output)
        assert b'synthetic-' not in output and b'wrong-password' not in output, 'secret echoed'
        assert b'FPSTUDIO_INPUT_READY' not in output, 'private conversation output leaked'
        assert '가'.encode() not in output, 'non-ASCII input echoed'
        assert output.count(b'\x1b[31m[Admin Auth]\x1b[0m') == (0 if mode == 'tty-noninteractive' else 1), (mode, output)
        if mode not in ('tty-match', 'tty-match-escape', 'tty-match-erased', 'tty-match-unicode-erased', 'tty-noninteractive', 'tty-cancel', 'tty-blocked-cancel'):
            assert b'*' in output, ('missing masked input', mode, output)
        assert termios.tcgetattr(terminal)[3] & termios.ECHO, 'echo not restored'
        print(mode, 'passed: red header, masked input, sticky keyboard mode, echo restored')
    finally:
        try: os.kill(child, signal.SIGKILL); os.waitpid(child, 0)
        except (ProcessLookupError, ChildProcessError): pass
        os.close(terminal)
