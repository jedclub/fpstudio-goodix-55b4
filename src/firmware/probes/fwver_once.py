import sys, goodix, protocol
d = goodix.Device(0x55b4, protocol.USBProtocol)
d.nop()
sys.stderr.write("RESULT:" + d.firmware_version() + "\n")
