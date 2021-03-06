#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define MAXTOKEN 100

enum {NAME, PARENS, BRACKETS};

void dcl(void);
void dirdcl(void);

int gettoken(void);
int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];
int named = 0;

int main(){
	while (gettoken() != EOF){
		strcpy(datatype, token);
		out[0] = '\0';
		dcl();
		/* Allow for multiple declaration in one line separated by commas or semicolons*/
		if (tokentype == '\n' || tokentype == ';' || tokentype == ',')
			printf("%s: %s %s\n", name, out, datatype);
		else
			printf("syntax error\n");	
	}
	return 0;
}

int gettoken(void){
	int c, getch(void);
	void ungetch(int);
	char *p = token;

	while ((c = getch()) == ' ' || c == '\t')
		;
	if (c == '(' && named == 0){
		/* keep running in spite of spurrious blanks inside function parens*/
		while ((c = getch()) == ' ' || c == '\t')
			;	
		if (c == ')'){
			strcpy(token, "()");
			return tokentype = PARENS;
		} else {			ungetch(c);
			return tokentype = '(';
		}
	} else if (c == '(' && named == 1) {
		for (*p++ = c; (*p++ = getch()) != ')'; )
			;
		*p = '\0';
		return tokentype = PARENS;
	} else if (c == '['){
		for (*p++ = c; (*p++ = getch()) != ']'; )
			;
		*p = '\0';
		return tokentype = BRACKETS;
	} else if (isalpha(c)){
		for (*p++ = c; isalnum(c = getch()); )
			*p++ = c;
		*p = '\0';
		ungetch(c);
		named = 1;
		return tokentype = NAME;
	} else {
		return tokentype = c;
	}
}

void dcl(void){
	int ns;

	for (ns = 0; gettoken() == '*'; )
		ns++;
	dirdcl();
	while (ns-- > 0)
		strcat(out, " pointer to");
}

void dirdcl(void){
	int type;

	if (tokentype == '('){
		dcl();
		if (tokentype != ')')
			printf("error: missing )\n");
	} else if (tokentype == NAME)
		strcpy(name, token);
	else
		printf("error: expected name or (dcl)\n");

	while ((type = gettoken()) == PARENS || type == BRACKETS)
		if (type == PARENS){
			strcat(out, " function");
			strcat(out, token);
			strcat(out, " returning");
		}
		else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
}

#define BUFFSIZE 100

char buf[BUFFSIZE];
int bufp = 0;

int getch(void){
	return (bufp > 0) ? buf[--bufp]: getchar();
}

void ungetch(int c){  
	if (bufp >= BUFFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}