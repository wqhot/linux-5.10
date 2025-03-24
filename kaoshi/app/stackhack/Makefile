CC = gcc
CXX = g++
CFLAGS = -O0

all: target attacker normal

target: target.c
	$(CC) $(CFLAGS) -o target.o target.c

attacker: attacker.c
	$(CC) $(CFLAGS) -o attacker.o attacker.c

normal: normal.cpp
	$(CXX) $(CFLAGS) -lpthread -o normal.o normal.cpp

clean:
	rm -f target.o attacker.o normal.o