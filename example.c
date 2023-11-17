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
