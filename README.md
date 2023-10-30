# eme
> A tiny evaluation engine for math expressions

--- 

`eme` is a minimal, linear, single-header evaluation engine for simple math expressions. It supports standard mathematical operations such as addition, subtraction, multiplication and division as well as the priority rules.

## Basic Example
```c
#include <stdio.h>

#include "eme.h"

int main(void){
	int err;
	double res = eme_calc("", &err);

	if (err < 0) return 1;
	printf("%f\n", res);

	return 0;
}
```
