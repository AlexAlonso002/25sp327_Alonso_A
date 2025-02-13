CC=gcc
CFLAGS=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra
LIBS=-lcmocka

all: input_main test_input

# Compile input_main (normal build)
input_main: input_main.o
	@echo "\nLink step to create input_main:"
	$(CC) -o input_main input_main.o

input_main.o: input.c
	$(CC) -Wall -Wextra -c input.c -o input_main.o

# Compile test_input (with UNIT_TEST)
test_input: test_input.o input.o
	@echo "\nLink step to create test_input:"
	$(CC) -o test_input test_input.o input.o $(LIBS)

test_input.o: test_input.c
	$(CC) $(CFLAGS) -c test_input.c -o test_input.o

input.o: input.c
	$(CC) $(CFLAGS) -DUNIT_TEST -c input.c -o input.o

# Clean up compiled files
clean:
	@echo "Cleaning up..."
	rm -fv input_main test_input *.o
	@echo

.PHONY: all clean
