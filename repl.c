#include <stdio.h>

#include "eme.h"

int main(void){
	eme_ret res;
	char in[1024];

	printf("\n[eme-repl started]\n\n");
	for (;;){
		printf("> ");
		fgets(in, sizeof(in) * sizeof(char), stdin);
		res = eme_eval(in);
		if (res.type == EME_RETURN_TYPE_ERR) printf("= %s", res.err.msg);
		else if (res.type == EME_RETURN_TYPE_NUM) printf("= %f", res.value);
		else if (res.type == EME_RETURN_TYPE_BOOL) printf(res.value ? "= True" : "= False");
		printf("\n\n");
	}

	return 0;
}
