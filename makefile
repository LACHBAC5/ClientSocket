all: main

main: main.o ClientSocket.o Camera.o md5.o
	g++ main.o ClientSocket.o Camera.o md5.o -o main.out

%.o: %.cpp
	g++ -c $<

clean:
	rm main.o ClientSocket.o Camera.o