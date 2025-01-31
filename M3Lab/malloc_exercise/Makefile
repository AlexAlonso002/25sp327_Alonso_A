CC=gcc
CFLAGS=-Wall -Wextra
ALL=test_mymalloc

all:${ALL}

test_mymalloc: mymalloc.o test_mymalloc.o
	${CC} ${CFLAGS} -o test_mymalloc mymalloc.o test_mymalloc.o

test_mymalloc.o: test_mymalloc.c mymalloc.h
	${CC} ${CFLAGS} -c test_mymalloc.c -o test_mymalloc.o

mymalloc.o: mymalloc.c mymalloc.h
	${CC} ${CFLAGS} -c mymalloc.c -o mymalloc.o

clean:
	rm -vf *.o ${ALL}