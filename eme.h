#ifndef EME_H
#define EME_H

#include <stdlib.h>
#include <strings.h>
#include <math.h>


/* --- DEFINITION --- */

#define EME_PI 3.14159265

enum EME_TOKEN_TYPE {
	EME_TOKEN_TYPE_NUM,
	EME_TOKEN_TYPE_OPR,
	EME_TOKEN_TYPE_BRA,
	EME_TOKEN_TYPE_CHR,
	EME_TOKEN_TYPE_FUN,
	EME_TOKEN_TYPE_NUL
};

enum EME_RETURN_TYPE {
	EME_RETURN_TYPE_NUM,
	EME_RETURN_TYPE_BOOL,
	EME_RETURN_TYPE_ERR
};

typedef struct _eme_tok {
	int type, prio;
	double value;
} eme_tok;

typedef struct _eme_opr {
	char desc;
	int prio, ret_type;
	double (*fun)(double, double);
} eme_opr;

typedef struct _eme_con {
	char *desc;
	double val;
} eme_con;

typedef struct _eme_fun {
	char *desc;
	double (*fun)(double);
} eme_fun;

typedef struct _eme_err {
	int status;
	char *msg;
} eme_err;

typedef struct _eme_ret {
	double value;
	int type;
	eme_err err;
} eme_ret;

double eme_add(double a, double b);
double eme_sub(double a, double b);
double eme_mul(double a, double b);
double eme_div(double a, double b);
double eme_mod(double a, double b);
double eme_equ(double a, double b);
double eme_lb(double a);
double eme_lg(double a);
double eme_sgn(double a);
double eme_deg(double a);
double eme_rad(double a);
double eme_frac(double a);

int eme_tok_type(char c);
int eme_max_str();
int eme_max_prio();
eme_ret eme_eval(char *expr);


/* --- IMPLEMENTATION --- */

/* -- BUILT-INS -- */

double eme_add(double a, double b) { return a + b; }
double eme_sub(double a, double b) { return a - b; }
double eme_mul(double a, double b) { return a * b; }
double eme_div(double a, double b) { return a / b; }
double eme_mod(double a, double b) { return a - (int)(a / b) * b; }
double eme_equ(double a, double b) { return a == b; }
double eme_lb(double a) { return log(a) / log(2); }
double eme_lg(double a) { return log(a) / log(10); }
double eme_sgn(double a) { return fabs(a) / a; }
double eme_deg(double a) { return a * (180 / EME_PI); }
double eme_rad(double a) { return a * (EME_PI / 180); }
double eme_frac(double a) { return a - floor(a); }

const eme_opr bi_operators[] = {
	{'=', 1, EME_RETURN_TYPE_BOOL, &eme_equ}, {'+', 2, EME_RETURN_TYPE_NUM, &eme_add},
	{'-', 2, EME_RETURN_TYPE_NUM, &eme_sub}, {'*', 3, EME_RETURN_TYPE_NUM, &eme_mul},
	{'/', 3, EME_RETURN_TYPE_NUM, &eme_div}, {'%', 3, EME_RETURN_TYPE_NUM, &eme_mod},
	{'^', 4, EME_RETURN_TYPE_NUM, &pow},
};

const eme_con bi_constants[] = {
	{"E", 2.71828183}, {"PI", EME_PI},
	{"T", 1.61803399}, {"True", 1},
	{"False", 0},
};

const eme_fun bi_functions[] = {
	{"sqrt", &sqrt}, {"abs", &fabs},
	{"sin", &sin}, {"cos", &cos},
	{"tan", &tan}, {"asin", &asin},
	{"acos", &acos}, {"atan", &atan},
	{"sinh", &sinh}, {"cosh", &cosh},
	{"tanh", &tanh}, {"asinh", &asinh},
	{"acosh", &acosh}, {"atanh", &atanh},
	{"ln", &log}, {"exp", &exp},
	{"ceil", &ceil}, {"floor", &floor},
	{"lb", &eme_lb}, {"lg", &eme_lg},
	{"sgn", &eme_sgn}, {"round", &round},
	{"deg", &eme_deg}, {"rad", &eme_rad},
	{"frac", &eme_frac}, {"trunc", &floor},
	{"cbrt", &cbrt}
};

/* -- EME -- */

int eme_tok_type(char c){
	if (c >= '0' && c <= '9') return EME_TOKEN_TYPE_NUM;
	if (c == '(' || c == ')') return EME_TOKEN_TYPE_BRA;
	for (int i = 0; i < (int)(sizeof(bi_operators) / sizeof(eme_opr)); i++) { if (c == bi_operators[i].desc) return EME_TOKEN_TYPE_OPR; }
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_') return EME_TOKEN_TYPE_CHR;
	return EME_TOKEN_TYPE_NUL;
}

int eme_max_str(){
	int max = 0;
	for (int i = 0; i < (int)(sizeof(bi_functions) / sizeof(eme_fun)); i++) if ((int)strlen(bi_functions[i].desc) > max) max = strlen(bi_functions[i].desc);
	for (int i = 0; i < (int)(sizeof(bi_constants) / sizeof(eme_con)); i++) if ((int)strlen(bi_constants[i].desc) > max) max = strlen(bi_constants[i].desc);
	return max;
}

int eme_max_prio(){
	int max = 0;
	for (int i = 0; i < (int)(sizeof(bi_operators) / sizeof(eme_opr)); i++) if (bi_operators[i].prio > max) max = bi_operators[i].prio;
	return max;
}

eme_ret eme_eval(char *expr){

	/* --- PARSER --- */
	
	const int max_str_len = eme_max_str();
	const int max_opr_prio = eme_max_prio();
	const int sign_prio = max_opr_prio + 1;
	const int fun_prio = max_opr_prio + 2;
	const int bra_prio = max_opr_prio + 3;
	
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
				tokens[tok_num + 1] = (eme_tok){.type = EME_TOKEN_TYPE_OPR, .prio = o_bra * bra_prio + sign_prio};
				for (int j = 0; j < (int)(sizeof(bi_operators) / sizeof(eme_opr)); j++) if ('*' == bi_operators[j].desc) tokens[tok_num + 1].value = j;
				tok_num += 2;
			}else{
				// OPERATOR
				t.type = EME_TOKEN_TYPE_OPR;
				for (int j = 0; j < (int)(sizeof(bi_operators) / sizeof(eme_opr)); j++)
					if (c == bi_operators[j].desc){ t.prio = o_bra * bra_prio + bi_operators[j].prio; t.value = j; break; };
			}
		}else if (eme_tok_type(c) == EME_TOKEN_TYPE_BRA){
			// BRACKET
			t.type = EME_TOKEN_TYPE_BRA; t.value = c;
			o_bra = c == '(' ? o_bra + 1 : o_bra - 1;
		}else if (eme_tok_type(c) == EME_TOKEN_TYPE_CHR){
			char curr_str[max_str_len + 1];
			int curr_str_pos = 0;
			memset(curr_str, 0, max_str_len + 1);
			curr_str[curr_str_pos] = c;
			curr_str_pos++;
			for (int j = i + 1; j < (int)strlen(expr); j++){
				if (curr_str_pos == max_str_len) break;
				c = expr[j];
				if (eme_tok_type(c) == EME_TOKEN_TYPE_CHR) curr_str[curr_str_pos] = c;
				else{ i = j - 1; break; }
				curr_str_pos++;
			}
			curr_str[curr_str_pos] = '\0';
			int found = 0;
			// CONSTANT
			for (int j = 0; j < (int)(sizeof(bi_constants) / sizeof(eme_con)); j++)
				if (strcmp(bi_constants[j].desc, curr_str) == 0){ t.type = EME_TOKEN_TYPE_NUM; t.value = bi_constants[j].val; found = 1; break; }
			// FUNCTION
			if (!found)
				for (int j = 0; j < (int)(sizeof(bi_functions) / sizeof(eme_fun)); j++)
					if (strcmp(bi_functions[j].desc, curr_str) == 0){ t.type = EME_TOKEN_TYPE_FUN; t.prio = o_bra * bra_prio + fun_prio; t.value = j; found = 1; break; }
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
	if (!valid){ return (eme_ret){.value = 0, .type = EME_RETURN_TYPE_ERR, .err = (eme_err){.status = -1, .msg = "INVALID"}}; }

	/* --- LEXER --- */

	int ret_type = EME_RETURN_TYPE_NUM;
	while (tok_num > 1){
		eme_tok *new_tokens = malloc(0);
		int new_tok_num = 0, max_prio = 0, op = 0;
		for (int i = 0; i < tok_num; i++) max_prio = tokens[i].prio > max_prio ? tokens[i].prio : max_prio;
		for (int i = 0; i < tok_num; i++){
			eme_tok new_tok = (eme_tok){};
			if (tokens[i].type == EME_TOKEN_TYPE_OPR && i > 0 && i < tok_num - 1 && tokens[i - 1].type == EME_TOKEN_TYPE_NUM && tokens[i + 1].type == EME_TOKEN_TYPE_NUM && tokens[i].prio == max_prio && op == 0){
				// RESOLVE OPERATOR
				new_tok.type = EME_TOKEN_TYPE_NUM;
				new_tok.value = bi_operators[(int)tokens[i].value].fun(tokens[i - 1].value, tokens[i + 1].value);
				new_tokens[new_tok_num - 1] = new_tok;
				ret_type = bi_operators[(int)tokens[i].value].ret_type;
				i++; op = 1;
			}else if (tokens[i].type == EME_TOKEN_TYPE_NUM && i > 0 && i < tok_num - 1 && tokens[i - 1].type == EME_TOKEN_TYPE_BRA && tokens[i + 1].type == EME_TOKEN_TYPE_BRA){
				// RESOLVE BRACKETS
				new_tok = tokens[i];
				new_tokens[new_tok_num - 1] = new_tok;
				i++;
			}else if (tokens[i].type == EME_TOKEN_TYPE_NUM && i > 0 && tokens[i - 1].type == EME_TOKEN_TYPE_FUN && tokens[i - 1].prio == max_prio && op == 0){
				// RESOLVE FUNCTION
				new_tok.type = EME_TOKEN_TYPE_NUM;
				new_tok.value = bi_functions[(int)tokens[i - 1].value].fun(tokens[i].value);
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
	return (eme_ret){.value = tokens[0].value, .type = ret_type, .err = (eme_err){.status = 0, .msg = ""}};
}

#endif
