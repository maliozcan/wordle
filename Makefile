CC=gcc
CFLAGS=--std=c11 -Wpedantic -Wall -Werror -O0 -g
OBJECTS := wordle.o layout.o

wordle: main.c $(OBJECTS) words_alpha.txt
	$(CC) $(CFLAGS) $< -o $@ $(OBJECTS)

wordle.o: wordle.c wordle.h dynamic_array.h helper.h
	$(CC) $(CFLAGS) -c $< -o $@

layout.o: layout.c layout.h
	$(CC) $(CFLAGS) -c $< -o $@

utest.h:
	wget -q https://raw.githubusercontent.com/sheredom/utest.h/main/utest.h

test: test.c utest.h $(OBJECTS)
	$(CC) $(CFLAGS) $< -o $@ $(OBJECTS) && \
	./$@

dynamic_array_test: dynamic_array_test.c dynamic_array.h utest.h
	$(CC) $(CFLAGS) $< -o $@ && \
	./$@

words_alpha.txt:
	wget -q https://raw.githubusercontent.com/dwyl/english-words/master/words_alpha.txt

clean:
	rm -f wordle test dynamic_array_test utest.h *.o words_alpha.txt