#!/usr/bin/env python3
"""Exercise the production sensor worker on a private synthetic D-Bus.

Does not connect to system fprintd or touch any real fingerprint data.
"""
import os
import socket
import struct
import subprocess
import sys
import time
import unittest


def serve(address, mode):
    import dbus
    import dbus.service
    from dbus.mainloop.glib import DBusGMainLoop
    from gi.repository import GLib
    DBusGMainLoop(set_as_default=True)
    connection = dbus.bus.BusConnection(address)
    name = dbus.service.BusName('net.reactivated.Fprint', bus=connection)

    class Manager(dbus.service.Object):
        @dbus.service.method('net.reactivated.Fprint.Manager', out_signature='ao')
        def GetDevices(self):
            return ['/net/reactivated/Fprint/Device/0']

    class Device(dbus.service.Object):
        @dbus.service.method('net.reactivated.Fprint.Device', in_signature='s', out_signature='as')
        def ListEnrolledFingers(self, user):
            return [] if mode == 'empty' else ['right-index-finger']

        @dbus.service.method('net.reactivated.Fprint.Device', in_signature='s')
        def Claim(self, user):
            if mode == 'slow-claim': time.sleep(0.7)
            if mode == 'busy':
                raise dbus.exceptions.DBusException('busy', name='net.reactivated.Fprint.Error.AlreadyInUse')

        @dbus.service.signal('net.reactivated.Fprint.Device', signature='sb')
        def VerifyStatus(self, status, done):
            pass

        @dbus.service.method('net.reactivated.Fprint.Device', in_signature='s')
        def VerifyStart(self, finger):
            def report():
                if mode in ('match', 'slow-claim'):
                    self.VerifyStatus('verify-match', True)
                    self.VerifyStatus('verify-no-match', True)  # teardown must not undo success
                elif mode == 'retry':
                    self.VerifyStatus('verify-no-match', True)
                elif mode == 'retry-nonterminal':
                    for _ in range(20): self.VerifyStatus('verify-retry-scan', False)
                elif mode == 'error':
                    self.VerifyStatus('verify-disconnected', True)
                return False
            GLib.timeout_add(10, report)

        @dbus.service.method('net.reactivated.Fprint.Device')
        def VerifyStop(self):
            pass

        @dbus.service.method('net.reactivated.Fprint.Device')
        def Release(self):
            pass

    objects = [Manager(name, '/net/reactivated/Fprint/Manager'),
               Device(name, '/net/reactivated/Fprint/Device/0')]
    print('ready', flush=True)
    GLib.MainLoop().run()
    return objects


class WorkerBusTest(unittest.TestCase):
    def run_case(self, mode, expected, cancel=False):
        daemon = subprocess.Popen(['dbus-daemon', '--session', '--nofork', '--print-address=1'],
                                  stdout=subprocess.PIPE, text=True)
        address = daemon.stdout.readline().strip()
        server = subprocess.Popen([sys.executable, __file__, '--server', address, mode],
                                  stdout=subprocess.PIPE, text=True)
        parent, child = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
        worker = None
        try:
            self.assertEqual(server.stdout.readline().strip(), 'ready')
            # Test process is unprivileged. Production PAM spawns this same
            # executable with an explicit environment excluding bus overrides.
            env = dict(os.environ, DBUS_SYSTEM_BUS_ADDRESS=address)
            def setup_fd():
                os.dup2(child.fileno(), 3)
            worker = subprocess.Popen([BINARY, 'synthetic-user'], env=env,
                                      preexec_fn=setup_fd, close_fds=False)
            child.close()
            parent.settimeout(3)
            if cancel:
                parent.close()
                worker.wait(timeout=2)
                return
            events = []
            while True:
                data = parent.recv(32)
                if not data: break
                events.append(struct.unpack('ii', data))
            worker.wait(timeout=2)
            self.assertEqual(events[-1][0], expected, events)
            if mode.startswith('retry'):
                self.assertEqual(events[-1][1], 20)
            if mode == 'match':
                self.assertEqual(sum(e[0] == 3 for e in events), 1)
        finally:
            parent.close(); child.close()
            for process in (worker, server, daemon):
                if process is not None:
                    if process.poll() is None: process.terminate()
                    process.wait(timeout=3)
                    if process.stdout: process.stdout.close()

    def test_match_is_not_overwritten_by_cleanup(self): self.run_case('match', 3)
    def test_slow_device_initialization_still_matches(self): self.run_case('slow-claim', 3)
    def test_twenty_failed_contacts_stop(self): self.run_case('retry', 5)
    def test_twenty_retry_events_stop(self): self.run_case('retry-nonterminal', 5)
    def test_disconnection_is_not_a_match(self): self.run_case('error', 4)
    def test_no_enrollment_is_not_a_match(self): self.run_case('empty', 4)
    def test_busy_sensor_is_not_a_match(self): self.run_case('busy', 4)
    def test_parent_close_releases_worker(self): self.run_case('idle', None, cancel=True)


if __name__ == '__main__':
    if sys.argv[1] == '--server':
        serve(sys.argv[2], sys.argv[3])
    else:
        BINARY = sys.argv.pop(1)
        unittest.main()
