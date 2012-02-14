CC=gcc
CFLAGS=-g -pedantic -std=c99 -Wall -Wextra
EXECUTABLE=testlist

$(EXECUTABLE): testlist.c list.o
	$(CC) $(CFLAGS) testlist.c list.o -o $(EXECUTABLE)

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

clean:
	rm -f *.o
	rm -f $(EXECUTABLE)

all: $(EXECUTABLE)
