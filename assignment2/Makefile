# Makefile for simple C++ program
CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -Wall -Wextra -std=c++11
LDFLAGS=-g 
LDLIBS=
SRCS=main.cc parser.cc reduction.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: tool
tool: $(OBJS)
	$(CXX) $(LDFLAGS) -o tool $(OBJS) $(LDLIBS)
main.o: main.cc parser.h reduction.h
parser.o: parser.cc parser.h
reduction.o: reduction.cc reduction.h

clean:
	$(RM) $(OBJS)
distclean: clean
	$(RM) tool
build: tool
	echo finished
run: build
	./tool positives.txt
# alternatively:
# ./tool < positives.txt
# if your program reads from the standard input (stdin)