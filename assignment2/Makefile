CC = g++

CompileParms = -c -Wall -std=c++11 -O2

OBJS = main.o parser.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o LambdaCalc

clean:
	rm -f *.o LamdaCalc

main.o: main.cc
	$(CC) $(CompileParms)  main.cc

parser.o: parser.cc
	$(CC) $(CompileParms)  parser.cc