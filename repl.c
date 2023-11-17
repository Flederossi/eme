#include <stdio.h>

#include "eme.h"

int main(void){
	eme_err err;
	double res;
	char in[1024];

	printf("\n[eme-repl started]\n\n");
	for (;;){
		printf("> ");
		fgets(in, sizeof(in) * sizeof(char), stdin);
		res = eme_eval(in, &err);
		if (err.status < 0) printf("= %s", err.msg);
		else printf("= %f", res);
		printf("\n\n");
	}

	return 0;
}
