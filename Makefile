output: main.o
	g++ main.o -o output -lcurl

main.o: main.cpp
	g++ -c main.cpp -lcurl

target: dependencies
	action

clean: 
	rm *.o output