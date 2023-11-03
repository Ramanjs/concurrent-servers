CC=gcc
BINS=fork-server thread-server select-server

all: $(BINS)

fork-server: fork-server.c utils.c
	$(CC) fork-server.c utils.c -o fork-server

thread-server: thread-server.c utils.c
	$(CC) thread-server.c utils.c -o thread-server

select-server: select-server.c utils.c
	$(CC) select-server.c utils.c -o select-server

clean:
	rm -rf $(BINS)
