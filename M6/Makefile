CC = gcc
CFLAGS = -Wall -Wextra -O2
BINARY = setuid_demo
SOURCE = setuid_demo.c

all: $(BINARY)
	@echo "Setting setuid-root permissions..."
	sudo chown root:root $(BINARY)
	sudo chmod u+s $(BINARY)
	@echo "Permissions set. Binary created and permissions updated."

$(BINARY): $(SOURCE)
	$(CC) $(CFLAGS) -o $(BINARY) $(SOURCE)

test: $(BINARY)
	ls -l $(BINARY)
	./$(BINARY)

clean:
	rm -f $(BINARY)
