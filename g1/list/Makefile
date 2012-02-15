CC=gcc
CFLAGS=-g -pedantic -std=c99 -Wall -Wextra
EXECUTABLE=testlist

$(EXECUTABLE): testlist.c list.o
	$(CC) $(CFLAGS) testlist.c list.o -o $(EXECUTABLE)

calc: calc.o stack.o
	$(CC) $(CFLAGS) calc.o stack.o -o calc

calc.o: calc.c stack.h
	$(CC) $(CFLAGS) -c calc.c

stack.o: stack.c stack.h list.h
	$(CC) $(CFLAGS) -c stack.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c



clean:
	rm -f *.o
	rm -f $(EXECUTABLE)

all: $(EXECUTABLE)
