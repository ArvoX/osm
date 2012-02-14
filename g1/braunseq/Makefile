CC=gcc
CFLAGS=-g -pedantic -std=c99 -Wall -Wextra
EXECUTABLE=testbraunseq

$(EXECUTABLE): testbraunseq.c braunseq.o
	$(CC) $(CFLAGS) testbraunseq.c braunseq.o -o $(EXECUTABLE)

braunseq.o: braunseq.c braunseq.h
	$(CC) $(CFLAGS) -c braunseq.c

clean:
	rm -f *.o
	rm -f $(EXECUTABLE)

all: $(EXECUTABLE)
