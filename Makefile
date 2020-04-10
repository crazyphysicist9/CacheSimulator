#
# Makefile for Cache Lab
# Note: requires a 64-bit x86-64 system 
#
CC = gcc
CFLAGS = -g -Wall -std=c99 -m64

csim: csim.c cachelab.c cachelab.h list.o
	$(CC) $(CFLAGS) -o csim csim.c cachelab.c list.o -lm 

list.o: list.c
	$(CC) $(CFLAGS) -c list.c -o list.o


#
# Clean the src dirctory
#
clean:
	rm -rf *.o
	rm -f *.tar
	rm -f csim
