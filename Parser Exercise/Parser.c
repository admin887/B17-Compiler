#include "Parser.h"
#include "Table.h"
#include<stdio.h>
extern int yylineno;

#define PARSE_LENGH 5
#define STOP_ON_ERROR 0
#define SHOW_TREE 0

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
	int tabsCounter = 0;
	int i = 0;
	tNode * Item = (tNode *)NULL;

	if (SHOW_TREE == 0)
		return;

	tNodeHead = &tNodeAnchor;
	tNodeTail = &tNodeAnchor;

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
	if (SHOW_TREE == 0)
		return;
	printf("%s function end parsing\n", name + PARSE_LENGH);
}
void parsePROGRAM()
{
	char* signalTaskName;
	int i, j,k;
	char* subbuff;
	int found = 0;
	errorCounter = 0;
	startParsing(__FUNCTION__);
	create_table();
	create_SignalTable(1);
	parseTASK_DEFINITIONS();
	//Match(SEP_SIGN_SEMICOLON);
	Match(KEYWORD_PARBEGIN);
	parseTASK_LIST();
	Match(KEYWORD_PAREND);
	Match(ENDOFILE);
	

	endParsing(__FUNCTION__);

	printf("Validating task defenition in Signals.....\n", errorCounter);
	
	
	for (j = 0; j < HASHSIZE && found == 0; j++)
	{
		if (create_SignalTable(0)->symbolTable[j] == NULL)
		{
			continue;
		}

		for (i = 0; i < HASHSIZE; i++)
		{
			if (getCurrentTable()->symbolTable[i] == NULL)
				continue;

			if (getCurrentTable()->symbolTable[i]->IDType == 2)
			{
				signalTaskName = (create_SignalTable(0)->symbolTable[j]->IDType);
				k = 0;
				while (signalTaskName[k] != '.')
				{
					k++;
				}

				subbuff = (char *)malloc(sizeof(char)*k);

				memcpy(subbuff, &signalTaskName[0], k);
				subbuff[k] = '\0';


				if (!strcmp(subbuff, getCurrentTable()->symbolTable[i]->IDName))
				{
					found = 1;
					break;
				}
			}

		}

		if (found == 0)
		{
			printf("task name %s did not found\n", subbuff);
			errorCounter++;
		}
		found = 0;
	}
	
		
	


	//for (i = 0; i < HASHSIZE; i++)
	//{
	//	if (getCurrentTable()->symbolTable[i] == NULL)
	//		continue;
	//
	//	if (getCurrentTable()->symbolTable[i]->IDType == 2)
	//	{
	//
	//		for (j = 0; j < HASHSIZE && found ==0; j++)
	//		{
	//			if (create_SignalTable()->symbolTable[j] == NULL)
	//			{
	//				continue;
	//			}
	//
	//			signalTaskName = (create_SignalTable()->symbolTable[j]->IDType);
	//
	//			k = 0;
	//			while (signalTaskName[k] != '.')
	//			{
	//				k++;
	//			}
	//
	//			subbuff = (char *)malloc(sizeof(char)*k);
	//
	//			memcpy(subbuff, &signalTaskName[0], k);
	//			subbuff[k] = '\0';
	//
	//			
	//			if (!strcmp(subbuff, getCurrentTable()->symbolTable[i]->IDName))
	//			{
	//				found = 1;
	//				break;
	//			}
	//		}
	//		if (found == 0)
	//		{
	//			printf("task Number did not found");
	//			
	//		}
	//		found = 0;
	//	}
	//
	//	
	//}

	pop_CurrentTable();
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

	int arrayOfFollowsTokens[] = { INDETIFIER };

	startParsing(__FUNCTION__);
	switch (tokenPointer.tokenType)
	{
	case SEP_SIGN_SEMICOLON:
		parseCOMMAND();
		parseCOMMANDS_TAG();
		break;
	case INDETIFIER:
	{
		PrintError("", tokenPointer, "exptected to get ';'");
		Back_Token();
		parseCOMMAND();
		parseCOMMANDS_TAG();
		break;
	}
	default:
	{
		Back_Token();
	}

	}
	endParsing(__FUNCTION__);
}
void parseCOMMAND()
{
	table_entry entityTask;
	char* lexemaTaskType;
	char* lexemaTaskid;
	char* lexemaSignalid;
	table_entry idEntiry;
	table_entry signalEntity;
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
			if (idEntiry->IDType == 2)
			{
				PrintError("", tokenPointer, "Task id cannot be set in left side of assignments");
			}
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
		lexemaTaskid = Match(INDETIFIER);		//task_id
		Match(POINT_SIGN);		//.
		lexemaSignalid = Match(INDETIFIER);		//signal_id  //gTODO add signal name for checking accept
		
		if (lexemaTaskid != NULL && lexemaSignalid != NULL)
		{
			signalEntity = addSignal(lexemaSignalid);
			set_type(signalEntity, lexemaTaskid);
		}
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
	case KEYWORD_UNTIL:
		Back_Token();
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
				if (entry->IDType == 2) //Task type
				{
					PrintError("", tokenPointer, "Task cannot be set in expression");
				}
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
	printf("%s Error: In line: %d; Lexema: %s; %s\n", errorHeader, ptrToken.lineNumber, ptrToken.lexema, message);
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