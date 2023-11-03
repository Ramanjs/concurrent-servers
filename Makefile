CC=gcc
BINS=fork-server thread-server

all: $(BINS)

fork-server: fork-server.c utils.c
	$(CC) fork-server.c utils.c -o fork-server

thread-server: thread-server.c utils.c
	$(CC) thread-server.c utils.c -o thread-server

clean:
	rm -rf $(BINS)
