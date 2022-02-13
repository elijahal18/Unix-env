all: env

env: env.c
	gcc -std=c99 -Wall -o env env.c

clean:
	rm -f env