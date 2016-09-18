#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define NUMBER		256
#define PLUS		257
#define STAR		258
#define LPAREN		259
#define RPAREN		260
#define END			261
#define EXPRESSION	0
#define TERM		1
#define FACTOR		2
#define ACC			999

int action[12][6] = {
	{5, 0, 0, 4, 0, 0},
	{0, 6, 0, 0, 0, ACC},
	{0, -2, 7, 0, -2, -2},
	{0, -4, -4, 0, -4, -4},
	{5, 0, 0, 4, 0, 0},
	{0, -6, -6, 0, -6, -6},
	{5, 0, 0, 4, 0, 0},
	{5, 6, 0, 4, 0, 0},
	{0, 6, 0, 0, 11, 0},
	{0, -1, 7, 0, -1, -1},
	{0, -3, -3, 0, -3, -3},
	{0, -5, -5, 0, -5, -5}
};

int gotoTable[12][3] = {
	{1, 2, 3}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {8, 2, 3}, {0, 0, 0},
	{0, 9, 3}, {0, 0, 10}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}
};

int prod_left[7] = {0, EXPRESSION, EXPRESSION, TERM, TERM, FACTOR, FACTOR};
int prod_len[7] = {0, 3, 1, 3, 1, 3, 1};
int stack[2][1000];
int top = -1;
int sym;
int num=0;

void push(int);
void shift(int);
void reduce(int);
int getToken();
void lrerror();
void lrparse();

int main(void){
	lrparse();
	exit(0);
}

void lrparse(){
	int i;
	stack[0][++top] = 0;
	sym = getToken();
	do{
		i = action[stack[0][top]][sym-256];

		if(i==ACC)
			printf("%d\n", stack[1][top]);
		else if(i>0)		//shift
			shift(i);
		else if(i<0)		//reduce
			reduce(-i);
		else			//error
			lrerror("Parse Error.");
	}while(i != ACC);
}

void push(int i){
	stack[0][++top] = i;
}

void shift(int i){
	push(i);
	if(sym == NUMBER){
		stack[1][top] = num;
		num = 0;
	}
	sym = getToken();
}


void reduce(int i){
	top -= prod_len[i];
	push(gotoTable[stack[0][top]][prod_left[i]]);
	if(i == 1)
		stack[1][top] += stack[1][top+2];
	else if(i == 3)
		stack[1][top] *= stack[1][top+2];
	else if(i == 5)
		stack[1][top] = stack[1][top+1]; 
}

void lrerror(char *errmsg){
	printf("%s\n", errmsg);
	exit(1);
}

int getToken(){
	static char ch = ' ';
	int i=0;
	while(ch == ' ' || ch == '\t' || ch == '\n')
		ch = getchar();
	if(isdigit(ch)){
		do{
			num = num * 10 + (ch - '0');
			ch = getchar();
		}while(isdigit(ch));
		return NUMBER;
	}
	else if(ch == '+'){
		ch = getchar();
		return PLUS;
	}
	else if(ch == '*'){
		ch = getchar();
		return STAR;
	}
	else if(ch == '('){
		ch = getchar();
		return LPAREN;
	}
	else if(ch == ')'){
		ch = getchar();
		return RPAREN;
	}
	else if(ch == EOF){
		return END;
	}else{
		lrerror("Wrong Input");
	}
}

