#ifndef EME_H
#define EME_H

#include <stdlib.h>
#include <strings.h>
#include <math.h>

enum EME_TOKEN_TYPE {
	EME_TOKEN_TYPE_NUM,
	EME_TOKEN_TYPE_OPR,
	EME_TOKEN_TYPE_BRA,
	EME_TOKEN_TYPE_NUL
};

typedef struct _eme_tok {
	int type, prio;
	double value;
} eme_tok;

int eme_tok_type(char c);
double eme_calc(char *expr, int *err);

int eme_tok_type(char c){
	if (c >= '0' && c <= '9') return EME_TOKEN_TYPE_NUM;
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') return EME_TOKEN_TYPE_OPR;
	if (c == '(' || c == ')') return EME_TOKEN_TYPE_BRA;
	return EME_TOKEN_TYPE_NUL;
}

double eme_calc(char *expr, int *err){

	/* --- PARSER --- */
	
	char c;
	eme_tok *tokens = malloc(0);
	int tok_num = 0, o_bra = 0;
	
	for (int i = 0; i < (int)strlen(expr); i++){
		c = expr[i];
		eme_tok t = (eme_tok){.type = EME_TOKEN_TYPE_NUL};
		if (eme_tok_type(c) == EME_TOKEN_TYPE_NUM){
			double n = c - '0';
			int dou = 0, dou_n = 10;
			for (int j = i + 1; j < (int)strlen(expr); j++){
				c = expr[j];
				if (eme_tok_type(c) == EME_TOKEN_TYPE_NUM){
					n = dou == 0 ? n * 10 + (c - '0') : n + (double)(c - '0') / dou_n;
					dou_n = dou == 1 ? dou_n * 10 : dou_n;
				} else if (c == '.' || c == ',') dou = 1;
				else{ i = j - 1; break; }
				if (j == (int)strlen(expr) - 1) i = j;
			}
			t.type = EME_TOKEN_TYPE_NUM; t.value = n;
		}
		else if (eme_tok_type(c) == EME_TOKEN_TYPE_OPR){
			t.type = EME_TOKEN_TYPE_OPR; t.value = c;
			t.prio = o_bra * 4 + (c == '+' || c == '-' ? 1 : (c == '*' || c == '/' ? 2 : 3));
		}
		else if (eme_tok_type(c) == EME_TOKEN_TYPE_BRA){
			t.type = EME_TOKEN_TYPE_BRA; t.value = c;
			o_bra = c == '(' ? o_bra + 1 : o_bra - 1;
		}
		if (t.type != EME_TOKEN_TYPE_NUL){
			tokens = realloc(tokens, (tok_num + 1) * sizeof(eme_tok));
			tokens[tok_num] = t;
			tok_num++;
		}
	}
	
	/* --- VALIDATOR --- */

	int valid = 1;
	if (o_bra != 0) valid = 0;
	if (tok_num > 0){
		if (tokens[0].type == EME_TOKEN_TYPE_OPR || (tokens[0].type == EME_TOKEN_TYPE_BRA && tokens[0].value == ')')) valid = 0;
		if (tokens[tok_num - 1].type == EME_TOKEN_TYPE_OPR || (tokens[tok_num - 1].type == EME_TOKEN_TYPE_BRA && tokens[tok_num - 1].value == '(')) valid = 0;
	}
	for (int i = 0; i < tok_num - 1; i++){
		int t1 = tokens[i].type, t2 = tokens[i + 1].type;
		if (t1 == EME_TOKEN_TYPE_NUM && (t2 == EME_TOKEN_TYPE_NUM || (t2 == EME_TOKEN_TYPE_BRA && tokens[i + 1].value == '('))) valid = 0;
		if (t1 == EME_TOKEN_TYPE_OPR && (t2 == EME_TOKEN_TYPE_OPR || (t2 == EME_TOKEN_TYPE_BRA && tokens[i + 1].value == ')'))) valid = 0;
		if (t1 == EME_TOKEN_TYPE_BRA && ((t2 == EME_TOKEN_TYPE_OPR && tokens[i].value == '(') || (t2 == EME_TOKEN_TYPE_NUM && tokens[i].value == ')'))) valid = 0;
		if (t1 == EME_TOKEN_TYPE_BRA && (t2 == EME_TOKEN_TYPE_BRA && tokens[i].value != tokens[i + 1].value)) valid = 0;
	}
	if (!valid){ *err = -1; return 0; }

	/* --- LEXER --- */

	while (tok_num > 1){
		eme_tok *new_tokens = malloc(0);
		int new_tok_num = 0, max_prio = 0, op = 0;
		for (int i = 0; i < tok_num; i++) max_prio = tokens[i].prio > max_prio ? tokens[i].prio : max_prio;
		for (int i = 0; i < tok_num; i++){
			eme_tok new_tok = (eme_tok){};
			if (tokens[i].type == EME_TOKEN_TYPE_OPR && i > 0 && i < tok_num - 1 && tokens[i - 1].type == EME_TOKEN_TYPE_NUM && tokens[i + 1].type == EME_TOKEN_TYPE_NUM && tokens[i].prio == max_prio && op == 0){
				new_tok.type = EME_TOKEN_TYPE_NUM;
				if ((int)tokens[i].value == '+') new_tok.value = tokens[i - 1].value + tokens[i + 1].value;
				if ((int)tokens[i].value == '-') new_tok.value = tokens[i - 1].value - tokens[i + 1].value;
				if ((int)tokens[i].value == '*') new_tok.value = tokens[i - 1].value * tokens[i + 1].value;
				if ((int)tokens[i].value == '/') new_tok.value = tokens[i - 1].value / tokens[i + 1].value;
				if ((int)tokens[i].value == '^') new_tok.value = pow(new_tok.value = tokens[i - 1].value, tokens[i + 1].value);
				new_tokens[new_tok_num - 1] = new_tok;
				i++; op = 1;
			}else if (tokens[i].type == EME_TOKEN_TYPE_NUM && i > 0 && i < tok_num - 1 && tokens[i - 1].type == EME_TOKEN_TYPE_BRA && tokens[i + 1].type == EME_TOKEN_TYPE_BRA){
				new_tok = tokens[i];
				new_tokens[new_tok_num - 1] = new_tok;
				i++;
			}else{
				new_tok = tokens[i];
				new_tokens = realloc(new_tokens, (new_tok_num + 1) * sizeof(eme_tok));
				new_tokens[new_tok_num] = new_tok;
				new_tok_num++;
			}
		}
		memcpy(tokens, new_tokens, sizeof(eme_tok) * new_tok_num);
		tok_num = new_tok_num;
		free(new_tokens);
	}
	*err = 0;
	return tokens[0].value;
}

#endif