CC=gcc
BINS=fork-server thread-server select-server poll-server epoll-server

all: $(BINS)

fork-server: fork-server.c utils.c
	$(CC) fork-server.c utils.c -o fork-server

thread-server: thread-server.c utils.c
	$(CC) thread-server.c utils.c -o thread-server

select-server: select-server.c utils.c
	$(CC) select-server.c utils.c -o select-server

poll-server: poll-server.c utils.c
	$(CC) poll-server.c utils.c -o poll-server

epoll-server: epoll-server.c utils.c
	$(CC) epoll-server.c utils.c -o epoll-server

clean:
	rm -rf $(BINS)
