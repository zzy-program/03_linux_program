CC=gcc
CPP=g++
AS=as
STRIP=strip

.PHONY: all
all:
	${CC} -o server server.c
	${CC} -o client client.c

.PHONY: run
run:
	./server &
	./client

.PHONY: clean
clean:
	rm server client
