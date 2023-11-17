# eme
> A tiny evaluation engine for math expressions

--- 

`eme` is a minimal, linear, single-header parser and evaluation engine for simple math expressions. It supports standard mathematical operations such as addition, subtraction, multiplication and division as well as operators precedence. It is especially designed to evaluate math expressions at runtime.

## Features
- Written in pure C99
- Single-header
- Supports standard math operators, constants and functions (All built-ins [here](#supported-built-ins))
- Implements standard precedence of operators
- Checks for invalid expressions
- Easy to add custom operators, constants and functions at compiletime

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
	eme_err err;

	// Evaluate the result of the expression 5.43 + 5 * 5 + (2 ^ 4 + 1.5)
	double res = eme_eval("5.43 + 5 * 5 + (2 ^ 4 + 1.5)", &err);

	// Check if expression is invalid
	if (err.status < 0) return 1;

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
### Functions
```c
double eme_eval(char *expr, eme_err *err);	// Evaluate expression and return the result
```

### Structures
> Error
```c
typedef struct _eme_err {
	int status;	// Return status of evaluation (0 - Successful, -1 - Error)
	char *msg;	// Error message (empty if status is 0)
} eme_err;
```

<br>

## REPL
To test `eme` in your terminal, you can open a simple repl (after cloning the repo) by running:

```sh
make repl
```

This lets you input math expressions like a calculator.

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

## Add custom operators, constants and functions
Custom operators, constants and functions can be added by including them in the corresponding arrays (`bi_operators`, `bi_constants`, `bi_functions`) directly in [eme.h](https://github.com/Flederossi/eme/blob/main/eme.h). The `max_prio` variable needs to be adjusted, if a new operator with a higher priority then the other ones is added.
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
	char *desc;			// The string used as descriptor for the constant
	double val;			// The value used for the constant
} eme_con;
```
> Structure of a function:
```c
typedef struct _eme_fun {
	char *desc;			// The string used as descriptor for the function
	double (*fun)(double);		// The corresponding function pointer
} eme_fun;
```
