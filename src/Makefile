
# Making: make builds calc, make clean cleans all.

CC = gcc
CFLAGS = -g -Wall -Wextra -I $(INCLUDES)
INCLUDES = $(shell pwd)

TARGETS = calculate.o parse.o vector.o
DEPS = calculate.h parse.h vector.h

TESTDIR = tests

calc : calc.o $(TARGETS)
	$(CC) -o $@ $^ $(CFLAGS)


%.o : %.c $(DEPS)
	$(CC) -c $^ $(CFLAGS)

vector_test : $(TESTDIR)/vector_test.c $(TARGETS) $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY : clean

clean :
	rm -f calc *_test *.o *.gch
