CFLAGS ?= -Wall -Wextra -std=c99

example:
	$(CC) *.c -o example -lm $(CFLAGS)

run: example
	./example

clean:
	rm example
