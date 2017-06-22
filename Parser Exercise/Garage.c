#include "Garage.h"
#include "DoublyLinkedList.h"

#define SIZE 250

extern int yylex();

static int arrayIndex = 0;
static int flag = 0;
static int LineNumber=0;
static token *arrayOfTokens;
static int NotFirstRun;

char* getTokenName(int type)
{
	char* str;
	switch(type)
	{
		case ARITHMETIC_OPERATION:
		str = "ARITHMETIC OPERATION";
		break;
		case RELATION_OPERATION:
		str = "RELATION OPERATION";
		break;
		case KEYWORD_PARBEGIN:
		str = "KEYWORD_PARBEGIN";
		break;
		case KEYWORD_PAREND:
			str = "KEYWORD_PAREND";
			break;
		case KEYWORD_TASK:
			str = "KEYWORD_TASK";
			break;
		case KEYWORD_BEGIN:
			str = "KEYWORD_BEGIN";
			break;
		case KEYWORD_END:
			str = "KEYWORD_END";
			break;
		case KEYWORD_INTEGER:
			str = "KEYWORD_INTEGER";
			break;
		case KEYWORD_REAL:
			str = "KEYWORD_REAL";
			break;
		case KEYWORD_DO:
			str = "KEYWORD_DO";
			break;
		case KEYWORD_UNTIL:
			str = "KEYWORD_UNTIL";
			break;
		case KEYWORD_OD:
			str = "KEYWORD_OD";
			break;
		case KEYWORD_SEND:
			str = "KEYWORD_SEND";
			break;
		case KEYWORD_ACCEPT:
			str = "KEYWORD_ACCEPT";
			break;
		case INDETIFIER:
		str = "INDETIFIER";
		break;

		case SEP_SIGN_COMMA:
		str = "SEP_SIGN_COMMA";
		break;
		case SEP_SIGN_SEMICOLON:
			str = "SEP_SIGN_SEMICOLON";
			break;
		case BRACKETS_OPEN_S:
			str = "BRACKETS_OPEN_S";
			break;
		case BRACKETS_CLOSE_S:
			str = "BRACKETS_CLOSE_S";
			break;
		case BRACKETS_OPEN_R:
			str = "BRACKETS_OPEN_R";
			break;
		case BRACKETS_CLOSE_R:
			str = "BRACKETS_CLOSE_R";
			break;
		case NUMBER_INT:
		str = "NUMBER_INT";
		break;
		case NUMBER_REAL:
			str = "NUMBER_REAL";
			break;
		case POINT_SIGN:
			str = "POINT_SIGN";
			break;
		case SPECIAL:
		str = "SPECIAL";
		break;
		case EQ_SIGN:
			str = "EQ_SIGN";
			break;
		case EOF:
		str="EOF";
		break;
		default:
		str="BAD";
	}
	return str;
}

void create_and_store_token(int typeOfToken, int numberOfLine, char * tokenLexeme)
{
		if(numberOfLine!=LineNumber)
		{
			if(NotFirstRun==1)
			{
				InsertAtTail(arrayOfTokens);
			}
			
			LineNumber++;
			arrayIndex=0;
			arrayOfTokens= (token*)malloc(sizeof(token)*SIZE);

			NotFirstRun=1;
		}

			arrayOfTokens[arrayIndex].tokenType = typeOfToken;
			arrayOfTokens[arrayIndex].lineNumber = numberOfLine;
			//arrayOfTokens[arrayIndex].lexema = getTokenName(typeOfToken);
			arrayOfTokens[arrayIndex].lexema = tokenLexeme;

		
}

token Next_Token()
{
	token next;

	if(!flag)
	{
		yylex();
		
	}

	flag = 0;
	next = arrayOfTokens[arrayIndex++];
	
	while (next.tokenType == SPECIAL)
	{
		next = Next_Token();
	}
	return next;
}

token Back_Token()
{
	flag = 1;
	arrayIndex--;

	return arrayOfTokens[arrayIndex];
}



