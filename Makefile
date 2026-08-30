CC=gcc
CFLAGS=--std=c11 -Wpedantic -Wall -Werror -O0 -g
OBJECTS := wordle.o layout.o

wordle: main.c $(OBJECTS) wordle.h common_defs.h words_alpha.txt
	$(CC) $(CFLAGS) $< -o $@ $(OBJECTS)

wordle.o: wordle.c wordle.h layout.h common_defs.h dynamic_array.h helper.h
	$(CC) $(CFLAGS) -c $< -o $@

layout.o: layout.c layout.h common_defs.h helper.h
	$(CC) $(CFLAGS) -c $< -o $@

utest.h:
	wget -q https://raw.githubusercontent.com/sheredom/utest.h/main/utest.h

layout_test: layout_test.c utest.h layout.o
	$(CC) $(CFLAGS) $< -o $@ layout.o && \
	./$@

dynamic_array_test: dynamic_array_test.c dynamic_array.h utest.h
	$(CC) $(CFLAGS) $< -o $@ && \
	./$@

words_alpha.txt:
	wget -q https://raw.githubusercontent.com/dwyl/english-words/master/words_alpha.txt

dependencies:
	sudo locale-gen tr_TR.UTF-8

clean:
	rm -rf wordle layout_test dynamic_array_test utest.h *.o words_alpha.txt *.dSYM/