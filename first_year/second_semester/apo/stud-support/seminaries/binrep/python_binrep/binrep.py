#!/usr/bin/python3

import struct

a = 0x5f

print("{0:08b}".format(a))

s = ''

binord = 7

while binord >= 0:
    if a & (1 << binord):
        s += '1'
    else:
        s += '0'
    binord -= 1
print(s)


f = 12.3456
g = 1024.0

buf = struct.pack('<ff', g, f)

print(buf)

print (["{0:02x}".format(b) for b in buf])

(u32_f, u32_g) = struct.unpack('<II', buf)

print(" f = {0:08x}".format(u32_f))
print(" g = {0:08x}".format(u32_g))
