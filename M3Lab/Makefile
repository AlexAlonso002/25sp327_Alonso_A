CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS_STATIC = -static
LDFLAGS_DYNAMIC = 

# Targets
all: compare_static compare_dynamic

compare_static: compare.o
	$(CC) $(LDFLAGS_STATIC) -o compare_static compare.o

compare_dynamic: compare.o
	$(CC) $(LDFLAGS_DYNAMIC) -o compare_dynamic compare.o

compare.o: compare.c
	$(CC) $(CFLAGS) -c compare.c -o compare.o

.PHONY: clean compare

clean:
	rm -fv *.o compare_static compare_dynamic

compare:
	ls -lh compare_static compare_dynamic
	./compare_static
	./compare_dynamic
