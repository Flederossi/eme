#ifndef EME_H
#define EME_H

#include <stdlib.h>
#include <strings.h>
#include <math.h>

/* --- DEFINITION --- */

double eme_add(double a, double b);
double eme_sub(double a, double b);
double eme_mul(double a, double b);
double eme_div(double a, double b);
double eme_mod(double a, double b);
int eme_tok_type(char c);
double eme_eval(char *expr, int *err);

enum EME_TOKEN_TYPE {
	EME_TOKEN_TYPE_NUM,
	EME_TOKEN_TYPE_OPR,
	EME_TOKEN_TYPE_BRA,
	EME_TOKEN_TYPE_CHR,
	EME_TOKEN_TYPE_FUN,
	EME_TOKEN_TYPE_NUL
};

typedef struct _eme_tok {
	int type, prio;
	double value;
} eme_tok;

typedef struct _eme_opr {
	char desc;
	int prio;
	double (*fun)(double, double);
} eme_opr;

typedef struct _eme_con {
	char desc;
	double val;
} eme_con;

typedef struct _eme_fun {
	char *desc;
	double (*fun)(double);
} eme_fun;

/* --- IMPLEMENTATION --- */

double eme_add(double a, double b) { return a + b; }
double eme_sub(double a, double b) { return a - b; }
double eme_mul(double a, double b) { return a * b; }
double eme_div(double a, double b) { return a / b; }
double eme_mod(double a, double b) { return a - (int)(a / b) * b; }

const eme_opr operators[] = {
	{'+', 1, &eme_add}, {'-', 1, &eme_sub},
	{'*', 2, &eme_mul}, {'/', 2, &eme_div},
	{'^', 3, &pow}, {'%', 2, &eme_mod},
};

const int max_prio = 3;

const eme_con constants[] = {
	{'e', 2.71828183}, {'p', 3.14159265},
	{'t', 1.61803399},
};

const eme_fun functions[] = {
	{"sin", &sin}, {"cos", &cos},
	{"tan", &tan},
};

const int sign_prio = max_prio + 1;
const int fun_prio = max_prio + 2;
const int bra_prio = max_prio + 3;

int eme_tok_type(char c){
	if (c >= '0' && c <= '9') return EME_TOKEN_TYPE_NUM;
	if (c == '(' || c == ')') return EME_TOKEN_TYPE_BRA;
	for (int i = 0; i < (int)(sizeof(operators) / sizeof(eme_opr)); i++) { if (c == operators[i].desc) return EME_TOKEN_TYPE_OPR; }
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return EME_TOKEN_TYPE_CHR;
	return EME_TOKEN_TYPE_NUL;
}

double eme_eval(char *expr, int *err){

	/* --- PARSER --- */
	
	char c;
	eme_tok *tokens = malloc(0);
	int tok_num = 0, o_bra = 0;
	
	for (int i = 0; i < (int)strlen(expr); i++){
		c = expr[i];
		eme_tok t = (eme_tok){.type = EME_TOKEN_TYPE_NUL};
		if (eme_tok_type(c) == EME_TOKEN_TYPE_NUM){
			// NUMBER
			double n = c - '0';
			int dou = 0, dou_n = 10;
			for (int j = i + 1; j < (int)strlen(expr); j++){
				c = expr[j];
				if (eme_tok_type(c) == EME_TOKEN_TYPE_NUM){
					n = dou == 0 ? n * 10 + (c - '0') : n + (double)(c - '0') / dou_n;
					dou_n = dou == 1 ? dou_n * 10 : dou_n;
				} else if (c == '.') dou = 1;
				else{ i = j - 1; break; }
				if (j == (int)strlen(expr) - 1) i = j;
			}
			t.type = EME_TOKEN_TYPE_NUM; t.value = n;
		}else if (eme_tok_type(c) == EME_TOKEN_TYPE_OPR){
			if ((c == '+' || c == '-') && (tok_num == 0 || (tokens[tok_num - 1].type != EME_TOKEN_TYPE_NUM && (tokens[tok_num - 1].type != EME_TOKEN_TYPE_BRA || tokens[tok_num - 1].value != ')')))){
				// NUMBER SIGN
				tokens = realloc(tokens, (tok_num + 2) * sizeof(eme_tok));
				tokens[tok_num] = (eme_tok){.type = EME_TOKEN_TYPE_NUM, .value = (c == '+' ? 1 : -1)};
				tokens[tok_num + 1] = (eme_tok){.type = EME_TOKEN_TYPE_OPR, .value = '*', .prio = o_bra * bra_prio + sign_prio};
				tok_num += 2;
			}else{
				// OPERATOR
				t.type = EME_TOKEN_TYPE_OPR; t.value = c;
				for (int j = 0; j < (int)(sizeof(operators) / sizeof(eme_opr)); j++)
					if (c == operators[j].desc) t.prio = o_bra * bra_prio + operators[j].prio;

			}
		}else if (eme_tok_type(c) == EME_TOKEN_TYPE_BRA){
			// BRACKET
			t.type = EME_TOKEN_TYPE_BRA; t.value = c;
			o_bra = c == '(' ? o_bra + 1 : o_bra - 1;
		}else if (eme_tok_type(c) == EME_TOKEN_TYPE_CHR){
			if (i == (int)strlen(expr) - 1 || eme_tok_type(expr[i + 1]) != EME_TOKEN_TYPE_CHR){
				// CONSTANT
				for (int j = 0; j < (int)(sizeof(constants) / sizeof(eme_con)); j++)
					if (constants[j].desc == c){ t.type = EME_TOKEN_TYPE_NUM; t.value = constants[j].val; break; }
			}else{
				// FUNCTION
				char *curr_str = malloc(sizeof(char));
				curr_str[0] = c;
				for (int j = i + 1; j < (int)strlen(expr); j++){
					c = expr[j];
					if (eme_tok_type(c) == EME_TOKEN_TYPE_CHR){
						curr_str = realloc(curr_str, (strlen(curr_str) + 1) * sizeof(char));
						curr_str[strlen(curr_str)] = c;
					}else{ i = j - 1; break; }
				}
				curr_str = realloc(curr_str, (strlen(curr_str) + 1) * sizeof(char));
				curr_str[strlen(curr_str)] = '\0';
				for (int j = 0; j < (int)(sizeof(functions) / sizeof(eme_fun)); j++)
					if (strcmp(functions[j].desc, curr_str) == 0){ t.type = EME_TOKEN_TYPE_FUN; t.prio = o_bra * bra_prio + fun_prio; t.value = j; break; }
			}
		}
		if (t.type != EME_TOKEN_TYPE_NUL){
			tokens = realloc(tokens, (tok_num + 1) * sizeof(eme_tok));
			tokens[tok_num] = t;
			tok_num++;
		}
	}

	/* --- VALIDATOR --- */

	int valid = 1;
	// OPEN BRACKETS DON'T MATCH WITH CLOSE
	if (o_bra != 0) valid = 0;
	if (tok_num > 0){
		// FIRST TOKEN IS OPERATOR OR CLOSE BRACKET
		if (tokens[0].type == EME_TOKEN_TYPE_OPR || (tokens[0].type == EME_TOKEN_TYPE_BRA && tokens[0].value == ')')) valid = 0;
		// LAST TOKEN IS OPERATOR OR OPEN BRACKET
		if (tokens[tok_num - 1].type == EME_TOKEN_TYPE_OPR || (tokens[tok_num - 1].type == EME_TOKEN_TYPE_BRA && tokens[tok_num - 1].value == '(')) valid = 0;
		// LAST TOKEN IS FUNCTION
		if (tokens[tok_num - 1].type == EME_TOKEN_TYPE_FUN) valid = 0;
	// NO TOKENS
	}else valid = 0;
	for (int i = 0; i < tok_num - 1; i++){
		int t1 = tokens[i].type, t2 = tokens[i + 1].type;
		// NUMBER BEFORE OPEN BRACKET
		if (t1 == EME_TOKEN_TYPE_NUM && (t2 == EME_TOKEN_TYPE_NUM || (t2 == EME_TOKEN_TYPE_BRA && tokens[i + 1].value == '('))) valid = 0;
		// OPERATOR BEFORE CLOSE BRACKET
		if (t1 == EME_TOKEN_TYPE_OPR && (t2 == EME_TOKEN_TYPE_OPR || (t2 == EME_TOKEN_TYPE_BRA && tokens[i + 1].value == ')'))) valid = 0;
		// OPERATOR AFTER OPEN BRACKET OR NUMBER AFTER CLOSE BRACKET
		if (t1 == EME_TOKEN_TYPE_BRA && ((t2 == EME_TOKEN_TYPE_OPR && tokens[i].value == '(') || (t2 == EME_TOKEN_TYPE_NUM && tokens[i].value == ')'))) valid = 0;
		// CLOSED BRACKET AFTER OPEN AND VICE VERSA
		if (t1 == EME_TOKEN_TYPE_BRA && (t2 == EME_TOKEN_TYPE_BRA && tokens[i].value != tokens[i + 1].value)) valid = 0;
		// NO OPEN BRACKET AFTER FUNCTION
		if (t1 == EME_TOKEN_TYPE_FUN && (t2 != EME_TOKEN_TYPE_BRA || tokens[i + 1].value == ')')) valid = 0;
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
				// RESOLVE OPERATOR
				new_tok.type = EME_TOKEN_TYPE_NUM;
				for (int j = 0; j < (int)(sizeof(operators) / sizeof(eme_opr)); j++)
					if (operators[j].desc == (int)tokens[i].value)
						new_tok.value = operators[j].fun(tokens[i - 1].value, tokens[i + 1].value);
				new_tokens[new_tok_num - 1] = new_tok;
				i++; op = 1;
			}else if (tokens[i].type == EME_TOKEN_TYPE_NUM && i > 0 && i < tok_num - 1 && tokens[i - 1].type == EME_TOKEN_TYPE_BRA && tokens[i + 1].type == EME_TOKEN_TYPE_BRA){
				// RESOLVE BRACKETS
				new_tok = tokens[i];
				new_tokens[new_tok_num - 1] = new_tok;
				i++;
			}else if (tokens[i].type == EME_TOKEN_TYPE_NUM && i > 0 && tokens[i - 1].type == EME_TOKEN_TYPE_FUN && tokens[i - 1].prio == max_prio && op == 0){
				// RESOLVE FUNCTION
				new_tok.type = EME_TOKEN_TYPE_NUM;
				new_tok.value = functions[(int)tokens[i - 1].value].fun(tokens[i].value);
				new_tokens[new_tok_num - 1] = new_tok;
				op = 1;
			}else{
				// ADD TOKEN AS IS
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
