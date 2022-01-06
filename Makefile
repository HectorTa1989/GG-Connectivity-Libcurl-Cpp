output: main.o
	g++ main.o -o output -lcurl

main.o: main.cpp
	g++ -c -std=c++11 -Wall main.cpp

target: dependencies
	action

clean: 
	rm -f *.o output
