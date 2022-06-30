all: main

main: main.o ClientSocket.o
	g++ main.o ClientSocket.o -o main

main.o: main.cpp
	g++ -c main.cpp

ClientSocket.o: ClientSocket.cpp
	g++ -c ClientSocket.cpp

clean:
	rm main.o ClientSocket.o