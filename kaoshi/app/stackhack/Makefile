CC = gcc
CXX = g++
CFLAGS = -O0

all: target attacker normal

target: target.c
	$(CC) $(CFLAGS) -o target target.c

attacker: attacker.c
	$(CC) $(CFLAGS) -o attacker attacker.c

normal: normal.cpp
	$(CXX) $(CFLAGS) -o normal normal.cpp

clean:
	rm -f target attacker normal