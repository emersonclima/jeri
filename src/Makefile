# Makefile for building Lua
# See ../doc/readme.html for installation and customization instructions.

CC= gcc -std=gnu99
CFLAGS= -O2 -Wall
LDFLAGS=
	
# == END OF USER SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE =======

# Targets start here.

all: jeri test

clean: 
	-rm *.o jeri_test jeri

test: clean jeri_test clean
	@echo -n "Running tests..."
	@./jeri_test	
	@echo "Ok"

jeri: main.o
	$(CC) -g $(CFLAGS) $(LDFLAGS) jeri.o main.o -o $@

jeri_test: test.o
	$(CC) -g $(CFLAGS) $(LDFLAGS) jeri.o test.o -o $@

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

main.o: jeri.o
test.o: jeri.o

# (end of Makefile)