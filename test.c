#include <stdio.h>

#include "eme.h"

typedef struct _test_case {
	char *expr;
	double exp;
} test_case;

test_case tc[] = {
	{"5+5*5", 30}, {"-(5)", -5},
	{"(-(3+5))", -8}, {"(-(5+(6+5*7)))", -46},
	{"-(5+(6+5*7))", -46}, {"(10.2^(1/2)*((-10.2*3)/-10.2^2))^4", 0.778547},
	{"(5^2+7^2+11^2+(8-2)^2)^(1/2)", 15.198684}, {"5.43 + 5 * 5 + (2 ^ 4 + 1.5)", 47.93},
	{"-(-10+6)", 4}, {"+(-(5+1))", -6},
	{"6*-7+2", -40}, {"5*(-3-4)", -35},
	{"5*(-3-4)^2", 245}, {"E", 2.71828183},
	{"PI", 3.14159265}, {"T", 1.61803399},
	{"E^(3*PI)", 12391.647741}, {"E*PI^2+2*25", 76.828366},
	{"sin(cos(2^PI))", -0.734852}, {"sin(PI)", 0},
	{"cos(PI)", -1}, {"2+3", 5},
	{"5-2", 3}, {"4*6", 24},
	{"8/2", 4}, {"2^3", 8},
	{"10%3", 1}, {"sqrt(16)", 4},
	{"abs(-5.4)", 5.4}, {"sin(0)", 0},
	{"cos(0)", 1}, {"tan(0)", 0},
	{"ln(1)", 0}, {"exp(1)", 2.718282},
	{"ceil(3.5)", 4}, {"floor(3.5)", 3},
	{"2*(3+4)-5", 9}, {"(2+3)*4", 20},
	{"2+3*4", 14}, {"-5+3", -2},
	{"2+3*4-6/2", 11}, {"2^3+5", 13},
	{"sqrt(4)^3", 8}, {"2^3+4*5-6/2", 25},
	{"PI*2^2", 12.566371}, {"sin(0)+cos(0)-tan(0)", 1},
	{"ln(1)+exp(1)", 2.718282}, {"ceil(3.5) + floor(3.5)", 7},
	{"sqrt(16)+abs(-5)-sin(0)", 9}, {"cos(0)*tan(0)/exp(1)+10%3", 1},
};

int main(void){
	int passed, passed_ges = 0, failed_ges = 0;
	eme_ret res;

	printf("\n");
	for (int i = 0; i < (int)(sizeof(tc) / sizeof(test_case)); i++){
		res = eme_eval(tc[i].expr);
		printf("TEST: %d/%d | EXPR: %s | EXP: %f | GOT: %f | ", i + 1, (int)(sizeof(tc) / sizeof(test_case)), tc[i].expr, tc[i].exp, res.value);
		if (res.type == EME_RETURN_TYPE_ERR) passed = 0;
		else if (fabs(res.value-tc[i].exp) < 0.000001) passed = 1;
		else passed = 0;
#if defined(_WIN32)
		if (passed) printf("PASSED\n");
		else printf("FAILED\n");
#else
		if (passed) printf("\x1b[32mPASSED\x1b[0m\n");
		else printf("\x1b[31mFAILED\x1b[0m\n");
#endif
		passed_ges += passed;
		failed_ges += !passed;
	}
#if defined(_WIN32)
	printf("\n%d TESTS | %d PASSED | %d FAILED\n\n", (int)(sizeof(tc) / sizeof(test_case)), passed_ges, failed_ges);
#else
	printf("\n%d \x1b[36mTESTS\x1b[0m | %d \x1b[32mPASSED\x1b[0m | %d \x1b[31mFAILED\x1b[0m\n\n", (int)(sizeof(tc) / sizeof(test_case)), passed_ges, failed_ges);
#endif

	return 0;
}
