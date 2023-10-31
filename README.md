# eme
> A tiny evaluation engine for math expressions

--- 

`eme` is a minimal, linear, single-header parser and evaluation engine for simple math expressions. It supports standard mathematical operations such as addition, subtraction, multiplication and division as well as operators precedence. It is especially designed to evaluate math expressions at runtime.

## Features
- Written in pure C99
- Single-header
- Supports the standard math operators (`+`, `-`, `*`, `/`, `^`, `%`)
- Implements standard precedence of operators
- Checks for invalid expressions

<br>

## Building
Just add [eme.h](https://github.com/Flederossi/eme/blob/main/eme.h) to your project and you are ready to go!

<br>

## Basic Example

> This example can also be found [here](https://github.com/Flederossi/eme/blob/main/example.c).

```c
#include <stdio.h>

#include "eme.h"

int main(void){
	int err;

	// Evaluate the result of the expression 5.43 + 5 * 5 + (2 ^ 4 + 1.5)
	double res = eme_eval("5.43 + 5 * 5 + (2 ^ 4 + 1.5)", &err);

	// Check if expression is invalid
	if (err < 0) return 1;

	// Output the result
	printf("%f\n", res);

	return 0;
}
```

### Output

```
47.930000
```

<br>

## API
The function `double eme_eval` takes two arguments:
- the expression (`char *`)
- the error (`int *`)

The error is set to -1 and the output to 0, if an invalid expression is detected. Otherwise the error is set to 0 and the output is the calculated result.

<br>

## REPL
To test `eme` in your terminal, you can open a simple repl (after cloning the repo) by running:

```sh
make repl
```

This lets you input math expressions like a calculator.
