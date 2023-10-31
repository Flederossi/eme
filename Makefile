CFLAGS ?= -Wall -Wextra -std=c99

example: eme.h
	$(CC) example.c -o example -lm $(CFLAGS)
	./example
	rm example

test: eme.h
	$(CC) test.c -o test -lm $(CFLAGS)
	./test
	rm test

repl: eme.h
	$(CC) repl.c -o repl -lm $(CFLAGS)
	./repl
	rm repl
