#include "Parser.h"
#include "Table.h"
#include<stdio.h>
extern int yylineno;

#define PARSE_LENGH 5
#define STOP_ON_ERROR 0

typedef struct _node
{
	char * FunctionString;
	struct _node * Next;
	struct _node * Prev;
} tNode;

tNode tNodeAnchor = { 0 };
tNode * tNodeHead = &tNodeAnchor;
tNode * tNodeTail = &tNodeAnchor;

int errorCounter = 0;

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
	create_table();
	parseTASK_DEFINITIONS();
	//Match(SEP_SIGN_SEMICOLON);
	Match(KEYWORD_PARBEGIN);
	parseTASK_LIST();
	Match(KEYWORD_PAREND);
	Match(ENDOFILE);
	pop_CurrentTable();

	endParsing(__FUNCTION__);
	printf("%d errors found\n", errorCounter);
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
			Back_Token();
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
	char* taskIDLexema;
	table_entry taskEntry;
	token t;
	startParsing(__FUNCTION__);
	Match(KEYWORD_TASK);
	taskIDLexema = Match(INDETIFIER);

	if (taskIDLexema != NULL)
	{
		taskEntry = add(taskIDLexema);
		if (taskEntry == NULL)
		{
			t = Back_Token();
			t = Next_Token();
			PrintError("", t, "Task allready defined");
		}
		else
		{
			set_type(taskEntry, 2);
		}
	}

	Match(KEYWORD_BEGIN);
	create_table();
	parseDECLARATIONS();
	create_table();
	Match(BRACKETS_OPEN_S);
	parseCOMMANDS();
	Match(BRACKETS_CLOSE_S);
	pop_CurrentTable();
	Match(KEYWORD_END);
	pop_CurrentTable();
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
		//errorScope(tokenPointer, );
		PrintError("Scope", tokenPointer, "Lexema already defined");
	}
	else
	{
		set_type(tableRow, IDType);		//inherited
	}

	endParsing(__FUNCTION__);
}
void parseTASK_LIST()
{
	char* taskNameInLine;
	table_entry taskNameFromTable;
	token bt;
	token nt;

	startParsing(__FUNCTION__);
	taskNameInLine = Match(INDETIFIER);

	taskNameFromTable = find(taskNameInLine);

	if (taskNameFromTable == NULL)
	{
		bt= Back_Token();
		nt = Next_Token();
		PrintError("", nt, "is Not found\n");
	}

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
		//Match(INDETIFIER);
		parseTASK_LIST();
		//parseTASK_LIST_TAG();
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
	table_entry idEntiry;
	token tokenPointer = Next_Token();
	int SIZE_OF_ARRAY = 3;
	int arrayOfFollowsTokens[] = { SEP_SIGN_SEMICOLON, BRACKETS_CLOSE_S, KEYWORD_UNTIL };

	startParsing(__FUNCTION__);
	switch (tokenPointer.tokenType)
	{
	case INDETIFIER:
		idEntiry = find(tokenPointer.lexema);

		if (idEntiry == NULL)
		{
		
			PrintError("Scope", tokenPointer, "undeclared identifier");
			errorRecover(arrayOfFollowsTokens, SIZE_OF_ARRAY);
			return;
		}
		else
		{
			Match(EQ_SIGN);
			parseEXPRESSION(idEntiry->IDType);
		}
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
		Match(INDETIFIER);		//signal_id  //gTODO add signal name for checking accept
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
	parseEXPRESSION(-1);
	//parseEXPRESSION_TAG(-1);
	parsePARAM_LIST_TAG();
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
void parseEXPRESSION(int idType)
{
	token tokenPointer = Next_Token();
	table_entry entry;
	int typeInLine;
	int SIZE_OF_ARRAY = 5;
	int arrayOfFollowsTokens[] = { SEP_SIGN_SEMICOLON, BRACKETS_CLOSE_S,SEP_SIGN_COMMA,BRACKETS_CLOSE_R, KEYWORD_UNTIL };

	startParsing(__FUNCTION__);

	switch (tokenPointer.tokenType)
	{
	case NUMBER_INT:
		typeInLine = 0;
		if (ValidateTypes(idType, typeInLine) == -1)
		{
			PrintError("", tokenPointer, "types do not matched");
		}
		break;
	case NUMBER_REAL:
		typeInLine = 1;
		if (ValidateTypes(idType, typeInLine) == -1)
		{
			PrintError("", tokenPointer, "types do not matched");
		}
		break;
	case INDETIFIER:
		if (idType == -1)
		{
			entry = find(tokenPointer.lexema);

			if (entry == NULL)
			{
				PrintError("Scope", tokenPointer, "undefine INDETIFIER");
			}
			else
			{
				idType = find(tokenPointer.lexema)->IDType;
			}
		}
		else
		{
			table_entry entiry = find(tokenPointer.lexema);

			if (entiry != NULL)
			{
				if (ValidateTypes(idType, entiry->IDType) == -1)
				{
					PrintError("", tokenPointer, "Types are not matched");
				}
			}
		}
		parseEXPRESSION_TAG(idType);
		break;
	default:
		errorRecover(arrayOfFollowsTokens, SIZE_OF_ARRAY);

	}
	endParsing(__FUNCTION__);
}
void parseEXPRESSION_TAG(int idType)
{
	token tokenPointer = Next_Token();

	startParsing(__FUNCTION__);
	switch (tokenPointer.tokenType)
	{
	case ARITHMETIC_OPERATION:
		parseEXPRESSION(idType);
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
	errorCounter++;
	if (STOP_ON_ERROR == 1)
	{
		
		getchar();
	}
}

void PrintError(char* errorHeader, token ptrToken, char * message)
{
	printf("%s Error: In line: %d; Lexema: %s; %s", errorHeader, ptrToken.lineNumber, ptrToken.lexema, message);
	errorCounter++;
	if (STOP_ON_ERROR == 1)
	{
		getchar();
	}
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

int ValidateTypes(int lType, int rType)
{
	if (lType == 0 && rType == 1)
	{
		// gTODO add better error message
		//printf("error");
		return -1;
	}
	if (lType != rType)
	{
		// gTODO add better error message
		//printf("error");
		return -1;
	}
	return 1; //good
}