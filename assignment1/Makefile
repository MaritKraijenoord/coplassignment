CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=$(shell root-config --libs)
SRCS=main.cc parser.cc
OBJS=$(subst .cc,.o,$(SRCS))
all: main
main: $(OBJS)
$(CXX) $(LDFLAGS) -o main $(OBJS) $(LDLIBS)
main.o: main.cc parser.h
parser.o: parser.h parser.cc
clean:
$(RM) $(OBJS)
distclean: clean
$(RM) main
build: main
echo finished
run: build
./main positives.txt
# alternatively:
# ./main < positives.txt
# if your program reads from the standard input (stdin)