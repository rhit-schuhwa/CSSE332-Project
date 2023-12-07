
b.bin: b.o c.o a.o
	gcc -o b.bin a.o b.o c.o

b.o: b.c a.h
	gcc -c b.c

c.o: c.c
	gcc -c c.c

a.o: a.c a.h
	gcc -c a.c
