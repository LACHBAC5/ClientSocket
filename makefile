all: main

main: main.o ClientSocket.o HTTPClientSocket.o HTTPAuthorization.o Camera.o md5.o
	g++ main.o ClientSocket.o HTTPClientSocket.o HTTPAuthorization.o Camera.o md5.o -o main.out

%.o: %.cpp
	g++ -std=c++17 -c $<

clean:
	rm main.o ClientSocket.o