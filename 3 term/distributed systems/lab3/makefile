CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
TARGETS = server client

all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) -o server server.c

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f $(TARGETS) /tmp/my_fifo

run_server: server
	./server

run_client: client
	./client

.PHONY: all clean run_server run_client