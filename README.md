# eme
> A tiny evaluation engine for math expressions

--- 

`eme` is a minimal, linear, single-header parser and evaluation engine for simple math expressions. It supports standard mathematical operations such as addition, subtraction, multiplication and division as well as operators precedence. It is especially designed to evaluate math expressions at runtime.

## Features
- Written in pure C99
- Single-header
- Supports standard math operators, constants and functions
- Implements standard precedence of operators
- Checks for invalid expressions
- Easy to add custom operators, constants and functions at compile- and runtime

<br>

## Supported Built-Ins
### Operators
| Operator | Description |
| - | - |
| `+` | Addition |
| `-` | Subtraction |
| `*` | Multiplication |
| `/` | Division |
| `^` | Power |
| `%` | Modulo |

### Constants
| Constant | Description |
| - | - |
| `PI` | PI |
| `E` | Euler's number |
| `T` | Golden ratio |

### Functions
| Function | Description |
| - | - |
| `sin()`| Sine |
| `cos()` | Cosine |
| `tan()` | Tangent |

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

## Add custom operators and constants
Custom operators and constants can be added by including them in the corresponding arrays (`operators`, `constants`) directly in [eme.h](https://github.com/Flederossi/eme/blob/main/eme.h).
> Structure of an operator:
```c
typedef struct _eme_opr {
	char desc;			// The character used as the operator
	int prio;			// The priority used for precedence of operators
	double (*fun)(double, double);	// The corresponding function pointer to execute the operation
} eme_opr;
```
> Structure of a constant:
```c
typedef struct _eme_con {
	char desc;			// The character used for the constant
	double val;			// The value used for the constant
} eme_con;
```

<br>

## REPL
To test `eme` in your terminal, you can open a simple repl (after cloning the repo) by running:

```sh
make repl
```

This lets you input math expressions like a calculator.
