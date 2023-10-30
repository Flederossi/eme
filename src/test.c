#include <stdio.h>

#include "eme.h"

int main(void){
	char in[1024];
	int err;
	double res;

	printf("\n");
	for (;;){
		printf("> ");
		scanf("%s", in);
		res = eme_calc(in, &err);
		if (err == -1) printf("= INVALID");
		else printf("= %f", res);
		printf("\n\n");
	}
	return 0;
}
