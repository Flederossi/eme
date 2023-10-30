CFLAGS ?= -Wall -Wextra -std=c99

test:
	$(CC) src/*.c -o bin/test -I src/include -lm $(CFLAGS)

run: test
	./bin/test

clean:
	rm bin/test
