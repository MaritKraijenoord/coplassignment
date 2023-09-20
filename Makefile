CC = g++

CompileParms = -c -Wall -std=c++11 -O2

OBJS = main.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o LambdaCalc

clean:
	rm -f *.o LamdaCalc

main.o: main.cc
	$(CC) $(CompileParms)  main.cc