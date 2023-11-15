#include <stdio.h>

#include "eme.h"

typedef struct _test_case {
	char *expr;
	double exp;
} test_case;

test_case tc[] = {
	{"5+5*5", 30},
	{"-(5)", -5},
	{"(-(3+5))", -8},
	{"(-(5+(6+5*7)))", -46},
	{"-(5+(6+5*7))", -46},
	{"(10.2^(1/2)*((-10.2*3)/-10.2^2))^4", 0.778547},
	{"(5^2+7^2+11^2+(8-2)^2)^(1/2)", 15.198684},
	{"5.43 + 5 * 5 + (2 ^ 4 + 1.5)", 47.93},
	{"+(-(5+1))", -6},
	{"6*-7+2", -40},
	{"5*(-3-4)", -35},
	{"5*(-3-4)^2", 245},
};

int main(void){
	int err, passed, passed_ges = 0, failed_ges = 0;
	double res;

	printf("\n");
	for (int i = 0; i < (int)(sizeof(tc) / sizeof(test_case)); i++){
		res = eme_eval(tc[i].expr, &err);
		printf("TEST: %d/%d | EXPR: %s | EXP: %f | GOT: %f | ", i + 1, (int)(sizeof(tc) / sizeof(test_case)), tc[i].expr, tc[i].exp, res);
		if (err < 0) passed = 0;
		else if (fabs(res-tc[i].exp) < 0.000001) passed = 1;
		else passed = 0;
		if (passed) printf("\x1b[32mPASSED\x1b[0m\n");
		else printf("\x1b[31mFAILED\x1b[0m\n");
		passed_ges += passed;
		failed_ges += !passed;
	}
	printf("\n%d \x1b[36mTESTS\x1b[0m | %d \x1b[32mPASSED\x1b[0m | %d \x1b[31mFAILED\x1b[0m\n\n", (int)(sizeof(tc) / sizeof(test_case)), passed_ges, failed_ges);

	return 0;
}
