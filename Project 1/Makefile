CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g
WALL=-Wall

make: myBashProj.c
	$(CC) $(WALL) $(CFLAGS) -o myBashProj myBashProj.c

extra: extraBashProj.c
	$(CC) $(WALL) $(CFLAGS) -o extraBashProj extraBashProj.c

debug: myBashProj.c
	$(CC) $(WALL) $(CFLAGS) $(DBFLAGS) -o myBashProj myBashProj.c

clean:
	rm myBashProj
	rm extraBashProj
