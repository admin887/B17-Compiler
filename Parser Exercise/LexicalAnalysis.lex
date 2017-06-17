/*
		Gil Raev 303973465
*/

%{
#include <stdio.h>
#include "Garage.h"
#include "Token.h"
#include "Parser.h"
%}

%option yylineno

DIGIT		 [0-9]
LETTER		[a-z]
PLUS		"+"
MINUS		"-"
MUL		"*"
DIV		"/"
EQ		"=="
NOTEQ		"!="
BIGGER		">"
SMALLER		"<"
SMALLEROEQ	"<="
BIGGEROEQ	">="
EQ_SIGN		"="

PARBEGIN	"parbegin"
PAREND		"parend"
TASK		"task"
BEGIN		"begin"
END		"end"
INTEGER		"integer"
REAL		"real"
DO		"do"
UNTIL		"until"
OD		"od"
SEND		"send"
ACCEPT		"accept"
COMMA		","
SEMICOLON	";"
BRACKETS_OPEN_S "{"
BRACKETS_CLOSE_S "}"
BRACKETS_OPEN_R "("
BRACKETS_CLOSE_R ")"
TAB		\t
NEWLINE		\n
SPACE		" "
BLANK		\s*
DOUBLE_LINES	"||"
POINT		"."

id		{LETTER}+({LETTER}|{DIGIT})*
task_id		{LETTER}+({LETTER}|{DIGIT})*
signal_id	{LETTER}+({LETTER}|{DIGIT})*
binary_ar_op	{PLUS}|{MINUS}|{MUL}|{DIV}
rel_op		{EQ}|{NOTEQ}|{BIGGER}|{SMALLER}|{SMALLEROEQ}|{BIGGEROEQ}
int_number	([1-9]{DIGIT}*)|([0]+)|{MINUS}([1-9]{DIGIT}*)|([0]+)
real_number	({int_number})(\.?)({DIGIT}*)|{MINUS}({int_number})(\.?)({DIGIT}*)
number		({int_number}|{real_number})
sep_sign	{COMMA}|{SEMICOLON}|{BRACKETS}

special		{SPACE}|{TAB}|{NEWLINE}|{BLANK}

keyword		{PARBEGIN}|{PAREND}|{TASK}|{BEGIN}|{END}|{INTEGER}|{REAL}|{DO}|{UNTIL}|{OD}|{SEND}|{ACCEPT}




%%
{binary_ar_op}		{
				printf("Token Type: %s:Token Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(ARITHMETIC_OPERATION),yytext, yylineno);
				create_and_store_token(ARITHMETIC_OPERATION, yylineno, yytext);
				return 1;
			}

{rel_op}		{
				printf("Token Type: %s: Token Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(RELATION_OPERATION),yytext, yylineno);
				create_and_store_token(RELATION_OPERATION, yylineno, yytext);
				return 1;
			}
{PARBEGIN}		{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(KEYWORD_PARBEGIN),yytext, yylineno);
				create_and_store_token(KEYWORD_PARBEGIN, yylineno, yytext);
				return 1;
			}
{PAREND}		{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(KEYWORD_PAREND),yytext, yylineno);
				create_and_store_token(KEYWORD_PAREND, yylineno, yytext);
				return 1;
			}
{TASK}			{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(KEYWORD_TASK),yytext, yylineno);
				create_and_store_token(KEYWORD_TASK, yylineno, yytext);
				return 1;
			}
{BEGIN}			{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(KEYWORD_BEGIN),yytext, yylineno);
				create_and_store_token(KEYWORD_BEGIN, yylineno, yytext);
				return 1;
			}
{END}			{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(KEYWORD_END),yytext, yylineno);
				create_and_store_token(KEYWORD_END, yylineno, yytext);
				return 1;
			}
{INTEGER}		{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(KEYWORD_INTEGER),yytext, yylineno);
				create_and_store_token(KEYWORD_INTEGER, yylineno, yytext);
				return 1;
			}
{REAL}			{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(KEYWORD_REAL),yytext, yylineno);
				create_and_store_token(KEYWORD_REAL, yylineno, yytext);
				return 1;
			}
{DO}			{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(KEYWORD_DO),yytext, yylineno);
				create_and_store_token(KEYWORD_DO, yylineno, yytext);
				return 1;
			}
{UNTIL}			{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(KEYWORD_UNTIL),yytext, yylineno);
				create_and_store_token(KEYWORD_UNTIL, yylineno, yytext);
				return 1;
			}
{OD}			{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(KEYWORD_OD),yytext, yylineno);
				create_and_store_token(KEYWORD_OD, yylineno, yytext);
				return 1;
			}
{SEND}			{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(KEYWORD_SEND),yytext, yylineno);
				create_and_store_token(KEYWORD_SEND, yylineno, yytext);
				return 1;
			}
{ACCEPT}		{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(KEYWORD_ACCEPT),yytext, yylineno);
				create_and_store_token(KEYWORD_ACCEPT, yylineno, yytext);
				return 1;
			}
{id}			{
				printf("Token Type: %s: \tToken Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(INDETIFIER),yytext, yylineno);
				create_and_store_token(INDETIFIER, yylineno, yytext);
				return 1;
			}

{task_id}		{
				printf("Token Type: %s: \tToken Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(INDETIFIER),yytext, yylineno);
				create_and_store_token(INDETIFIER, yylineno, yytext);
				return 1;
			}

{signal_id}		{
				create_and_store_token(INDETIFIER, yylineno, yytext);
				printf("Token Type: %s: \tToken Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(INDETIFIER),yytext, yylineno);
				return 1;
			}
{SEMICOLON}		{
				create_and_store_token(SEP_SIGN_SEMICOLON, yylineno, yytext);
				printf("Token Type: %s: \tToken Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(SEP_SIGN_SEMICOLON),yytext, yylineno);
				return 1;

			}
{COMMA}			{
				create_and_store_token(SEP_SIGN_COMMA, yylineno, yytext);
				printf("Token Type: %s: \tToken Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(SEP_SIGN_COMMA),yytext, yylineno);
				return 1;
			}
{DOUBLE_LINES}		{
				create_and_store_token(SEP_SIGN_DOUBLE_LINES, yylineno, yytext);
				printf("Token Type: %s: \tToken Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(SEP_SIGN_DOUBLE_LINES),yytext, yylineno);
				return 1;
			}
{EQ_SIGN}		{
				create_and_store_token(EQ_SIGN, yylineno, yytext);
				printf("Token Type: %s: \tToken Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(EQ_SIGN),yytext, yylineno);
				return 1;
			}
{POINT}			{
				create_and_store_token(POINT_SIGN, yylineno, yytext);
				printf("Token Type: %s: \tToken Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(POINT_SIGN),yytext, yylineno);
				return 1;
			}
{BRACKETS_OPEN_S}	{
				create_and_store_token(BRACKETS_OPEN_S, yylineno, yytext);
				printf("Token Type: %s: \tToken Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(BRACKETS_OPEN_S),yytext, yylineno);
				return 1;
			}
{BRACKETS_CLOSE_S}	{
				create_and_store_token(BRACKETS_CLOSE_S, yylineno, yytext);
				printf("Token Type: %s: \tToken Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(BRACKETS_CLOSE_S),yytext, yylineno);
				return 1;
			}
{BRACKETS_OPEN_R}		{
				create_and_store_token(BRACKETS_OPEN_R, yylineno, yytext);
				printf("Token Type: %s: \tToken Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(BRACKETS_OPEN_R),yytext, yylineno);
				return 1;
			}

{BRACKETS_CLOSE_R}	{
				create_and_store_token(BRACKETS_CLOSE_R, yylineno, yytext);
				printf("Token Type: %s: \tToken Lexeme: \"%s\" Token Line Number: %d\n",getTokenName(BRACKETS_CLOSE_R),yytext, yylineno);
				return 1;
			}
	

{int_number}		{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(NUMBER_INT),yytext, yylineno);
				create_and_store_token(NUMBER_INT, yylineno, yytext);
				return 1;
			}
{real_number}		{
				printf("Token Type: %s: \t\tToken Lexeme: \"%s\" Token Line Number: %d \n",getTokenName(NUMBER_REAL),yytext, yylineno);
				create_and_store_token(NUMBER_REAL, yylineno, yytext);
				return 1;
			}


{special}		{
				create_and_store_token(SPECIAL, yylineno, yytext);
				return 1;
			}


.			{
				int a;
				printf("BAD Token!!!!!!! \t\t\tToken: \"%s\" Number line: %d\n",yytext, yylineno);
				scanf("%d",&a);
			}
											
<<EOF>>			{
				printf("END OF FILE\n\n");
				create_and_store_token(EOF, yylineno, yytext);
				return 1;
			}

%%


int yywrap(void){return 1;}

void main(int argc, char* argv[])
{	
	int a;
	token currentToken;
	printf("\t\tAnalyzing input text for defined tokens: \n\n");

	yyin=fopen(argv[1],"r");
	
	parsePROGRAM();
	
	printf("Type any number to exit:");
	scanf("%d",&a);
}