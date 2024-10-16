# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings

all: mirror_client
sendreceive: sendreceive

sendreceive: sendreceive.o 
	$(CC)  sendreceive.o -lm -o sendreceive

mirror_client: mirror_client.o 
	$(CC)  mirror_client.o -lm -o mirror_client


mirror_client.o: mirror_client.c
	$(CC) -c mirror_client.c

sendreceive.o: sendreceive.c
	$(CC) -c sendreceive.c

