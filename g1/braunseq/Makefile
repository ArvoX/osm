CC=gcc
CFLAGS=-g -pedantic -std=c99 -Wall -Wextra
ifeq "$(DEBUG)" "true"
	CFLAGS += -DDEBUG
endif
EXECUTABLE=testbraunseq

$(EXECUTABLE): testbraunseq.c braunseq.o drawtree.o queue.o
	$(CC) $(CFLAGS) testbraunseq.c queue.o drawtree.o braunseq.o -o $(EXECUTABLE)

debug:
	make DEBUG=true

braunseq.o: braunseq.c braunseq.h
	$(CC) $(CFLAGS) -c braunseq.c

drawtree.o: drawtree.c drawtree.h queue.h
	$(CC) $(CFLAGS) -c drawtree.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

clean:
	rm -f *.o
	rm -f $(EXECUTABLE)

all: $(EXECUTABLE)
