all: env

env: env.c
	gcc -std=c99 -Wall -o environ env.c

clean:
	rm -f env