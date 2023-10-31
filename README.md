# eme
> A tiny evaluation engine for math expressions

--- 

`eme` is a minimal, linear, single-header parser and evaluation engine for simple math expressions. It supports standard mathematical operations such as addition, subtraction, multiplication and division as well as the priority rules.

<br>

## Building
Just add [eme.h](https://github.com/Flederossi/eme/blob/main/src/include/eme.h) to your project and you are ready to go!

<br>

## Basic Example
```c
#include <stdio.h>

#include "eme.h"

int main(void){
	int err;
	double res = eme_calc("5.43 + 5 * 5 + (2 ^ 4 + 1.5)", &err);

	if (err < 0) return 1;
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
The function `double eme_calc` takes two arguments:
- the expression (`char *`)
- the error (`int *`)

The error is set to -1 and the output to 0 if an invalid expression is detected. Otherwise the error is 0 and the output is the calculated result.
