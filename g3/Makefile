CC=gcc
CFLAGS=-pthread 
TARGET=PThread_rwlock CREW_test1
OBJS=$(shell ls *.c)
all: $(TARGET)


$(TARGET): 
# $(OBJS)
	$(CC) -o $@ $(CFLAGS) $@.c

#%.o: %.c
#	$(CC) -o $@  $(CFLAGS) -c $<


ctags:
	find . -type f -name '*.[ch]' | xargs ctags

etags:
	find . -type f -name '*.[ch]' | xargs etags

tags: ctags etags

clean:
	rm -f $(TARGET)
	rm -f *.o

clean-tags: clean
	rm -f tags TAGS


