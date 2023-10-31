CFLAGS ?= -Wall -Wextra -std=c99

example: eme.h
	$(CC) example.c -o example -lm $(CFLAGS)
	./example
	rm example

repl: eme.h
	$(CC) repl.c -o repl -lm $(CFLAGS)
	./repl
	rm repl
