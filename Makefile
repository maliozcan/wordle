CC=gcc
CFLAGS=--std=c11 -Wpedantic -Wall -Werror -O0 -g
OBJECTS := layout.o

wordle: main.c $(OBJECTS)
	$(CC) $(CFLAGS) $< -o $@ $(OBJECTS)

layout.o: layout.c layout.h
	$(CC) $(CFLAGS) -c $< -o $@

utest.h:
	wget -q https://raw.githubusercontent.com/sheredom/utest.h/main/utest.h

test: test.c utest.h $(OBJECTS)
	$(CC) $(CFLAGS) $< -o $@ $(OBJECTS) && \
	./$@

clean:
	rm -f wordle test utest.h *.o