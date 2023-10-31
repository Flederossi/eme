#include <stdio.h>

#include "eme.h"

typedef struct _test_case {
	char *expr;
	double exp;
} test_case;

test_case tc[] = {
	{"5+5*5", 30}
};

int main(void){
	int err, passed, passed_ges = 0, failed_ges = 0;
	double res;

	printf("\n");
	for (int i = 0; i < (int)(sizeof(tc) / sizeof(test_case)); i++){
		res = eme_eval(tc[i].expr, &err);
		printf("TEST: %d/%d | EXPR: %s | EXP: %f | GOT: %f | ", i + 1, (int)(sizeof(tc) / sizeof(test_case)), tc[i].expr, tc[i].exp, res);
		if (err < 0) passed = 0;
		else if (res == tc[i].exp) passed = 1;
		else passed = 0;
		if (passed) printf("\x1b[32mPASSED\x1b[0m\n");
		else printf("\x1b[31mFAILED\x1b[0m\n");
		passed_ges += passed;
		failed_ges += !passed;
	}
	printf("\n%d \x1b[36mTESTS\x1b[0m | %d \x1b[32mPASSED\x1b[0m | %d \x1b[31mFAILED\x1b[0m\n\n", (int)(sizeof(tc) / sizeof(test_case)), passed_ges, failed_ges);

	return 0;
}
