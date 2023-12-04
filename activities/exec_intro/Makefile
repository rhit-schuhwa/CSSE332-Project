SRC = $(wildcard *c)
BINS = $(patsubst %.c, %.bin, $(SRC))

CFLAGS += -ggdb -Werror

.PHONY: all clean

all: ${BINS}

%.bin: %.o
	${CC} ${CFLAGS} ${LDLIBS} -o $@ $^

clean:
	rm -rf ${BINS} *.o
