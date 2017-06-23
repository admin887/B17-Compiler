#include "Garage.h"
#include "DoublyLinkedList.h"
char* Match(int);
void startParsing(char * name);
void endParsing(char * name);
//void parserErrorPrint(token *ptToken,char* expectedToken);
//void parserErrorRecover(token *ptToken,int typesCount);

void parsePROGRAM();
void parseTASK_DEFINITIONS();
void parseTASK_DEFINITIONS_TAG();
void parseTASK_DEFINITION();
void parseTASK_LIST();
void parseTASK_LIST_TAG();
void parseDECLARATIONS();
void parseDECLARATIONS_TAG();
void parseDECLARATION();
void parseCOMMANDS();
void parseCOMMANDS_TAG();
void parseCOMMAND();
void parseEXPRESSION(int idType);
void parseEXPRESSION_TAG(int idType);
void parseCONDITION();
void parsePARAM_LIST();
void parsePARAM_LIST_TAG();

void errorPrint(token, int);
void errorScope(token);

void errorRecover(int *, int);
char * getTokenName(int tokenType);
int typeOfTokensContains(int *, int, int);
int ValidateTypes(int, int);