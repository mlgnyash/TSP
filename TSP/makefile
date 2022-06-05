output: main.o source.o
	g++ main.o source.o -o output

main.o: main.cpp
	g++ -c main.cpp

source.o: source.cpp header.h
	g++ -c source.cpp

clear:
	rm *.o output
