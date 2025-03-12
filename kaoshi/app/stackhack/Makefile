CC = gcc
CFLAGS = -O0

all: target attacker

target: target.c
	$(CC) $(CFLAGS) -o target target.c

attacker: attacker.c
	$(CC) $(CFLAGS) -o attacker attacker.c

clean:
	rm -f target attacker