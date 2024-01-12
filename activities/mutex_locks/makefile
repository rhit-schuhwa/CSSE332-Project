all: simple_sum.bin array_sum.bin

simple_sum.bin: simple_sum.c
	gcc -ggdb -pthread simple_sum.c -o simple_sum.bin

array_sum.bin: array_sum.c
	gcc -ggdb -pthread array_sum.c -o array_sum.bin

clean:
	rm -rf *.bin *.o *.dSYM

