all: main

main: main.o ClientSocket.o md5.o Camera.o
	g++ main.o ClientSocket.o md5.o Camera.o -o main.out

%.o: %.cpp
	g++ -std=c++17 -c $<

clean:
	rm main.o ClientSocket.o