CC=gcc
DBFLAGS=-g
WALL=-Wall

all:
	$(CC) $(WALL) -o server server.c
	$(CC) $(WALL) -o client1 client1.c
	$(CC) $(WALL) -o client2 client2.c

server: server.c
	$(CC) $(WALL) -o server server.c

client1: client1.c
	$(CC) $(WALL) -o client1 client1.c

client2: client2.c
	$(CC) $(WALL) -o client2 client2.c


debug: myBashProj.c
	$(CC) $(WALL) $(DBFLAGS) -o myBashProj myBashProj.c

clean:
	rm server
	rm client1
	rm client2
