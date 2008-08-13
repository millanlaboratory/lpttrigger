CC = gcc
CFLAGS = -std=gnu99 -Wall -pedantic

all: testport

testport: testport.o lpttrigger.o lptwriter-linux.o
	$(CC) -std=gnu99 $(LFLAGS) $^ -o $@ -lrt

clean:
	$(RM) testport
	$(RM) *.o
