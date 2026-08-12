CC=gcc
CFLAGS=--std=c11 -Wpedantic -Wall -Werror -O0 -g

wordle: wordle.c layout.h layout.c
	$(CC) $(CFLAGS) wordle.c layout.c  -o wordle

utest.h:
	wget https://raw.githubusercontent.com/sheredom/utest.h/main/utest.h

test: utest.h test.c layout.h layout.c
	$(CC) $(CFLAGS) utest.h test.c layout.h layout.c -o test
	./test

clean:
	rm -f wordle test utest.h