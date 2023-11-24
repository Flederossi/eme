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
