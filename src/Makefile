CC= gcc -std=gnu99
CFLAGS=-g -O2 -Wall
LDFLAGS=
	
all: pdbsel

pdbsel: pdbsel.c pdb.o util.o
	$(CC) $(CFLAGS) $(LDFLAGS) pdb.o util.o pdbsel.c -o pdbsel