#!/usr/bin/python3

import struct

a = 1234.56789
b = -11.1111111111111111111111
c = a + b

buf = struct.pack('<f', c)

print ('C float LE:' + ' '.join(["{0:02x}".format(b) for b in buf]))
print ('C float LE:' + str(struct.unpack('<f', buf)[0]))

buf = struct.pack('>f', c)

print ('C float BE:' + ' '.join(["{0:02x}".format(b) for b in buf]))
print ('C float BE:' + str(struct.unpack('>f', buf)[0]))

buf = struct.pack('<d', c)

print ('C double LE:' + ' '.join(["{0:02x}".format(b) for b in buf]))
print ('C double LE:' + str(struct.unpack('<d', buf)[0]))

buf = struct.pack('>d', c)

print ('C double BE:' + ' '.join(["{0:02x}".format(b) for b in buf]))
print ('C double BE:' + str(struct.unpack('>d', buf)[0]))
