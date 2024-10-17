CC=g++
CFLAGS=-g -std=c++20 -I./simple-svg
CLIBS=

all: Main

Main: Graph.o GraphSalesman.o Main.o
	$(CC) $(CFLAGS) Main.o Graph.o GraphSalesman.o -o main $(CLIBS)

Main.o: Main.cpp 
	$(CC) $(CFLAGS) -c Main.cpp 

Graph.o: Graph.cpp Graph.hpp 
	$(CC) $(CFLAGS) -c Graph.cpp 

GraphSalesman.o: GraphSalesman.cpp GraphSalesman.hpp 
	$(CC) $(CFLAGS) -c GraphSalesman.cpp 



.PHONY: clean

clean:
	-rm -f *.o main
