CC=gcc
CFLAGS=-g -pedantic -std=c99 -Wall -Wextra
EXECUTABLE=testbraunseq

$(EXECUTABLE): testbraunseq.c braunseq.o drawtree.o
	$(CC) $(CFLAGS) testbraunseq.c drawtree.o braunseq.o -o $(EXECUTABLE)

braunseq.o: braunseq.c braunseq.h
	$(CC) $(CFLAGS) -c braunseq.c

drawtree.o: drawtree.c drawtree.h
	$(CC) $(CFLAGS) -c drawtree.c

clean:
	rm -f *.o
	rm -f $(EXECUTABLE)

all: $(EXECUTABLE)
