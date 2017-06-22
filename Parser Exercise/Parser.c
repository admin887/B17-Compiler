#include "Parser.h"
#include "Table.h"
#include<stdio.h>
extern int yylineno;

#define PARSE_LENGH 5

typedef struct _node
{
	char * FunctionString;
	struct _node * Next;
	struct _node * Prev;
} tNode;

tNode tNodeAnchor = { 0 };
tNode * tNodeHead = &tNodeAnchor;
tNode * tNodeTail = &tNodeAnchor;

char* Match(int tokenType)
{
	token curToken = Next_Token();

	while (curToken.tokenType == SPECIAL)
	{
		curToken = Next_Token();
	}

	if (curToken.tokenType == tokenType)
	{
		return curToken.lexema;
	}
	errorPrint(curToken, getTokenName(tokenType));
}
void startParsing(char * FuncName)
{
	int i = 0;
	int tabsCounter = 0;
	tNode * Item = (tNode *)NULL;

	tNodeTail->Next = (tNode *)malloc(sizeof(tNode));
	tNodeTail->Next->Prev = tNodeTail;
	tNodeTail->Next->Next = (tNode *)NULL;
	tNodeTail->Next->FunctionString = FuncName;
	tNodeTail = tNodeTail->Next;

	Item = tNodeHead->Next;
	printf("%d: Start parsing %s:\n", yylineno, FuncName + PARSE_LENGH);


	while (Item != NULL)
	{
		printf("\t\t %d. ", tabsCounter);
		printf("%s\t", Item->FunctionString + PARSE_LENGH); //displaying the function name by removing the 'parse' string. 'parse' string is 5 length
		Item = Item->Next;
		tabsCounter++;
		printf("\n");
	}

}
void endParsing(char * name)
{
	printf("%s function end parsing\n", name + PARSE_LENGH);
}
void parsePROGRAM()
{
	startParsing(__FUNCTION__);
	parseTASK_DEFINITIONS();
	Match(SEP_SIGN_SEMICOLON);
	Match(KEYWORD_PARBEGIN);
	parseTASK_LIST();
	Match(KEYWORD_PAREND);
	Match(ENDOFILE);
	endParsing(__FUNCTION__);
}
void parseTASK_DEFINITIONS()
{
	startParsing(__FUNCTION__);
	parseTASK_DEFINITION();
	parseTASK_DEFINITIONS_TAG();
	endParsing(__FUNCTION__);
}
void parseTASK_DEFINITIONS_TAG()
{
	token tokenPointer = Next_Token();

	startParsing(__FUNCTION__);
	switch (tokenPointer.tokenType)
	{
	case SEP_SIGN_SEMICOLON:
		tokenPointer = Next_Token();
		if (tokenPointer.tokenType == KEYWORD_PARBEGIN)
		{
			return;
		}
		else
		{
			Back_Token();
		}
		parseTASK_DEFINITION();
		parseTASK_DEFINITIONS_TAG();
		break;
	default:
		Back_Token();
	}
	endParsing(__FUNCTION__);
}
void parseTASK_DEFINITION()
{
	startParsing(__FUNCTION__);
	Match(KEYWORD_TASK);
	Match(INDETIFIER);
	Match(KEYWORD_BEGIN);
	parseDECLARATIONS();
	create_table();
	Match(BRACKETS_OPEN_S);
	parseCOMMANDS();
	Match(BRACKETS_CLOSE_S);
	pop_CurrentTable();
	Match(KEYWORD_END);
	endParsing(__FUNCTION__);

}
void parseDECLARATIONS()
{
	parseDECLARATION();
	parseDECLARATIONS_TAG();
}
void parseDECLARATIONS_TAG()
{
	token tokenPointer = Next_Token();
	startParsing(__FUNCTION__);
	switch (tokenPointer.tokenType)
	{
	case SEP_SIGN_SEMICOLON:
		parseDECLARATIONS();
		break;
	default:
		Back_Token();
	}
	endParsing(__FUNCTION__);
}
void parseDECLARATION()
{
	int SIZE_OF_ARRAY = 3;
	char* IDlexema;
	int IDType;
	table_entry tableRow;

	int arrayOfFollowsTokens[] = { SEP_SIGN_SEMICOLON, BRACKETS_OPEN_S, BRACKETS_CLOSE_R };

	token tokenPointer = Next_Token();
	startParsing(__FUNCTION__);

	
	switch (tokenPointer.tokenType)
	{
		case KEYWORD_INTEGER:
		{
			IDType = 0;		//synthesized
			break;
		}
		case KEYWORD_REAL:
		{
			IDType = 1;		//synthesized
			break;
		}
		default:
			errorRecover(arrayOfFollowsTokens, SIZE_OF_ARRAY);
	}


	IDlexema = Match(INDETIFIER);
	tableRow = add(IDlexema);

	if(tableRow == NULL)
	{
		errorScope(tokenPointer);
	}
	else
	{
		set_type(tableRow, IDType);		//inherited
	}

	endParsing(__FUNCTION__);
}
void parseTASK_LIST()
{
	startParsing(__FUNCTION__);
	Match(INDETIFIER);
	parseTASK_LIST_TAG();
	endParsing(__FUNCTION__);

}
void parseTASK_LIST_TAG()
{
	token tokenPointer = Next_Token();
	startParsing(__FUNCTION__);
	switch (tokenPointer.tokenType)
	{
	case SEP_SIGN_DOUBLE_LINES:
		Match(INDETIFIER);
		parseTASK_LIST_TAG();
		break;
	default:
		Back_Token();

	}
	endParsing(__FUNCTION__);
}
void parseCOMMANDS()
{
	startParsing(__FUNCTION__);
	parseCOMMAND();
	parseCOMMANDS_TAG();
	endParsing(__FUNCTION__);
}
void parseCOMMANDS_TAG()
{
	token tokenPointer = Next_Token();

	startParsing(__FUNCTION__);
	switch (tokenPointer.tokenType)
	{
	case SEP_SIGN_SEMICOLON:
		parseCOMMAND();
		parseCOMMANDS_TAG();
		break;
	default:
		Back_Token();

	}
	endParsing(__FUNCTION__);
}
void parseCOMMAND()
{
	token tokenPointer = Next_Token();
	int SIZE_OF_ARRAY = 3;
	int arrayOfFollowsTokens[] = { SEP_SIGN_SEMICOLON, BRACKETS_CLOSE_S, KEYWORD_UNTIL };

	startParsing(__FUNCTION__);
	switch (tokenPointer.tokenType)
	{
	case INDETIFIER:
		Match(EQ_SIGN);
		parseEXPRESSION();
		break;
	case KEYWORD_DO:
		parseCOMMANDS();
		Match(KEYWORD_UNTIL);
		parseCONDITION();
		Match(KEYWORD_OD);
		break;
	case KEYWORD_SEND:
		Match(INDETIFIER);		//task_id
		Match(POINT_SIGN);		//.
		Match(INDETIFIER);		//signal_id
		Match(BRACKETS_OPEN_R);
		parsePARAM_LIST();
		Match(BRACKETS_CLOSE_R);
		break;
	case KEYWORD_ACCEPT:
		Match(INDETIFIER);		//signal_id
		Match(BRACKETS_OPEN_R);
		parseDECLARATIONS();
		Match(BRACKETS_CLOSE_R);
		break;
	case KEYWORD_BEGIN:
		parseDECLARATIONS();
		Match(BRACKETS_OPEN_S);
		parseCOMMANDS();
		Match(BRACKETS_CLOSE_S);
		Match(KEYWORD_END);
		break;
	default:
		errorRecover(arrayOfFollowsTokens, SIZE_OF_ARRAY);
	}
	endParsing(__FUNCTION__);
}
void parsePARAM_LIST()
{
	startParsing(__FUNCTION__);
	parseEXPRESSION();
	parseEXPRESSION_TAG();
	endParsing(__FUNCTION__);
}
void parsePARAM_LIST_TAG()
{
	token tokenPointer = Next_Token();

	startParsing(__FUNCTION__);
	switch (tokenPointer.tokenType)
	{
	case SEP_SIGN_COMMA:
		parsePARAM_LIST();
		break;
	default:
		Back_Token();

	}
	endParsing(__FUNCTION__);
}
void parseEXPRESSION()
{
	token tokenPointer = Next_Token();
	int SIZE_OF_ARRAY = 5;
	int arrayOfFollowsTokens[] = { SEP_SIGN_SEMICOLON, BRACKETS_CLOSE_S,SEP_SIGN_COMMA,BRACKETS_CLOSE_R, KEYWORD_UNTIL };

	startParsing(__FUNCTION__);

	switch (tokenPointer.tokenType)
	{
	case NUMBER_INT:
		break;
	case NUMBER_REAL:
		break;
	case INDETIFIER:
		parseEXPRESSION_TAG();
		break;
	default:
		errorRecover(arrayOfFollowsTokens, SIZE_OF_ARRAY);

	}
	endParsing(__FUNCTION__);
}
void parseEXPRESSION_TAG()
{
	token tokenPointer = Next_Token();

	startParsing(__FUNCTION__);
	switch (tokenPointer.tokenType)
	{
	case ARITHMETIC_OPERATION:
		parseEXPRESSION();
		break;
	default:
		Back_Token();

	}
	endParsing(__FUNCTION__);

}
void parseCONDITION()
{
	startParsing(__FUNCTION__);
	Match(BRACKETS_OPEN_R);
	Match(INDETIFIER);
	Match(RELATION_OPERATION);
	Match(INDETIFIER);
	Match(BRACKETS_CLOSE_R);
	endParsing(__FUNCTION__);
}

void errorPrint(token ptrToken, char* expectedTokenName)
{
	printf("SyntaxError: Unexpected token on line %d, illegal %s expected token %s\n", ptrToken.lineNumber, ptrToken.lexema, expectedTokenName);

}
void errorScope(token ptrToken)
{
	printf("ScopeError: In line: %d, %s is already defined", ptrToken.lineNumber, ptrToken.lexema);

}
void errorRecover(int * arrayOfFollowsTokens, int typesArrayCount)
{
	token ptrToken;
	int i = 0;
	Back_Token();
	do
	{
		ptrToken = Next_Token();
		for (i = 0; i < typesArrayCount; i++)
		{
			if (ptrToken.tokenType == arrayOfFollowsTokens[i])
			{
				Back_Token();
				break;
			}
		}
	} while (ptrToken.tokenType != arrayOfFollowsTokens[i] && ptrToken.tokenType != EOF);
	if (typeOfTokensContains(arrayOfFollowsTokens, typesArrayCount, EOF))
	{
		printf("Reached end of file, exisiting");
		exit(0);
	}
}
int typeOfTokensContains(int * arrayOfFollowsTokens, int typesArrayCount, int matchToken)
{
	int i;
	for (i = 0; i < typesArrayCount; i++)
	{
		if (arrayOfFollowsTokens[i] == matchToken)
			return 1;
	}
	return 0;
}