CFLAGS ?= -Wall -Wextra -std=c99

example: eme.h
	$(CC) example.c -o example -lm $(CFLAGS)

run: example
	./example

clean:
	rm example
