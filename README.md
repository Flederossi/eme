# eme
> A tiny evaluation engine for math expressions

--- 

`eme` is a minimal, iterative, single-header parser and evaluation engine for simple math expressions. It supports standard mathematical operations such as addition, subtraction, multiplication and division as well as operators precedence. It is especially designed to evaluate math expressions at runtime.

## Overview
- [Features](#features)
- [Building](#building)
- [Example](#basic-example)
- [API](#api)
- [REPL](#repl)
- [Built-Ins](#supported-built-ins)
- [Custom Objects](#add-custom-operators-constants-and-functions)

<br>

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

	// Evaluate the result of the expression 5.43 + 5 * 5 + (2 ^ 4 + 1.5)
	eme_ret res = eme_eval("5.43 + 5 * 5 + (2 ^ 4 + 1.5)");

	// Check if expression is invalid
	if (res.type == EME_RETURN_TYPE_ERR){
		printf("%s\n", res.err.msg);
		return 1;
	}

	// Output the result
	if (res.type == EME_RETURN_TYPE_NUM)
		printf("%f\n", res.value);

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
eme_ret eme_eval(char *expr);	// Evaluate expression and return the result
```

### Structures
> Return
```c
typedef struct _eme_ret {
	double value;	// Result of evaluation (1 or 0 if return type is bool)
	int type;	// Type of result
	eme_err err;	// Error struct (only relevant if type == EME_RETURN_TYPE_ERR)
} eme_ret;
```

> Error
```c
typedef struct _eme_err {
	int status;	// Return status of evaluation (0 - Successful, -1 - Invalid expression)
	char *msg;	// Error message (empty if status is 0)
} eme_err;
```

### States/Types
> Return types
```c
EME_RETURN_TYPE_NUM	// Normal double
EME_RETURN_TYPE_BOOL	// Bool (0 or 1)
EME_RETURN_TYPE_ERR	// Error
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
| Operator | Description | Operator | Description |
| - | - | - | - |
| `+` | Addition | `-` | Subtraction |
| `*` | Multiplication | `/` | Division |
| `^` | Power | `%` | Modulo |
| `=` | Equal (Returns type bool) | `<` | Less than (Returns type bool) |
| `>` | Greater than (Returns type bool) | | |

### Constants
| Constant | Description |
| - | - |
| `PI` | PI |
| `E` | Euler's number |
| `T` | Golden ratio |
| `True` | = 1 |
| `False` | = 0 |

### Functions
| Function | Description | Function | Description |
| - | - | - | - |
| `sqrt()` | Square root | `abs()` | Absolute value |
| `sin()`| Sine | `asin()`| Arcsine |
| `cos()` | Cosine | `acos()` | Arccosine |
| `tan()` | Tangent | `atan()` | Arctangent |
| `sinh()` | Hyperbolic sine | `cosh()` | Hyperbolic cosine |
| `tanh()` | Hyperbolic tangent | `asinh()` | Hyperbolic arcsine |
| `acosh()` | Hyperbolic arccosine | `atanh()` | Hyperbolic arctangent |
| `lb()` | Logarithm to base 2 | `lg()` | Logarithm to base 10 |
| `ln()` | Natural logarithm | `exp()` | e to the power off |
| `ceil()` | Round up | `floor()` | Round down |
| `sgn()` | Sign (-1 or 1) | `round()` | Round |
| `deg()` | Radians to degrees | `rad()` | Degrees to radians |
| `frac()` | Fraction component | `trunc()` | Truncate after decimal point |
| `cbrt()` | Cube root | `fact()` | Factorial |

<br>

## Add custom operators, constants and functions
Custom operators, constants and functions can be added by including them in the corresponding arrays (`bi_operators`, `bi_constants`, `bi_functions`) directly in [eme.h](https://github.com/Flederossi/eme/blob/main/eme.h).
> Structure of an operator:
```c
typedef struct _eme_opr {
	char desc;			// The character used as the operator
	int prio, ret_type;		// The priority used for precedence of operators and the return type
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
