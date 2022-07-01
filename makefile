all: main

main: main.o ClientSocket.o md5.o
	g++ main.o ClientSocket.o md5.o -o main

main.o: main.cpp
	g++ -c main.cpp

ClientSocket.o: ClientSocket.cpp
	g++ -c ClientSocket.cpp

md5.o: md5.cpp
	g++ -c md5.cpp

clean:
	rm main.o ClientSocket.o