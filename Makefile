CC		= gcc
CFLAGS	= -Wall

all: clean tp2virtual

tp2virtual.o: tp2virtual.h tp2virtual.c
	$(CC) $(CFLAGS) -c tp2virtual.c

tp2virtual: tp2virtual.o 
	$(CC) $(CFLAGS) -o tp2virtual tp2virtual.o

clean:
	rm -f tp2virtual.o tp2virtual