#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define HASHSIZE 101

typedef struct tableEntry
{
	// 0 - int
	// 1 - real
	// 2 - task
	int IDType;
	char* IDName;
	struct tableEntry* next;
}Table_entry;

typedef Table_entry* table_entry;

typedef struct Table
{
	table_entry symbolTable[HASHSIZE];
	struct Table* fatherTable;
}table_ptr;

void create_table();
table_ptr* pop_table(table_ptr* cur_tab);
table_ptr* pop_CurrentTable();
table_entry add(char* id_name);
table_entry lookfor(table_ptr* cur_tab, char* id_name);
table_entry find(char* id_name);
void set_type(table_entry id_entry, int id_type);
int get_type(table_entry id_entry);
void setEntry(table_ptr* cur_tab, table_entry i_Entry, char* id_name);
char* m_strdup(char *o);
unsigned int crypto(char *s);
table_ptr* getCurrentTable();

table_ptr* create_SignalTable(int);
table_entry addSignal(char*);
void setEntrySignal(table_ptr*, table_entry, char*);
