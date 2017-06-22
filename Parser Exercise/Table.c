#include "Table.h"

table_ptr* current_table = NULL;

void create_table()
{
	int i = 0;
	table_ptr* new_table = (table_ptr*)malloc(1 * sizeof(table_ptr));
	new_table->fatherTable = current_table;

	while (i < HASHSIZE)
	{
		new_table->symbolTable[i] = NULL;
		i++;
	}
	current_table = new_table;
}

table_ptr* pop_table(table_ptr* cur_tab)
{
	table_ptr* fatherTable = cur_tab->fatherTable;
	return fatherTable;
}

table_entry add(char* id_name)
{
	table_entry cur_entry = lookfor(current_table, id_name);

	if (cur_entry == NULL)
	{
		cur_entry = (Table_entry*)malloc(sizeof(Table_entry));
		cur_entry->IDName = m_strdup(id_name);
		if (cur_entry->IDName == NULL)
			return NULL;

		cur_entry->next = NULL;
		setEntry(current_table, cur_entry, id_name);

		return cur_entry;
	}
	else
		return NULL;
}

table_entry lookfor(table_ptr* cur_tab, char* id_name)
{
	unsigned int cryptoID = crypto(id_name);
	table_entry cur_entry = cur_tab->symbolTable[cryptoID];
	while (cur_entry != NULL)
	{
		if (!strcmp(cur_entry->IDName, id_name))
			return cur_entry;
		cur_entry = cur_entry->next;
	}

	return NULL;
}

table_entry find(char* id_name)
{
	table_entry id_entry;
	table_ptr* tab = current_table;

	while (tab != NULL)
	{
		id_entry = lookfor(tab, id_name);
		if (id_entry != NULL)
		{
			return id_entry;
		}
		else
		{
			tab = pop_table(tab);
		}
	}

	printf("ERROR: undeclared identifier %s \n", id_name);
	return NULL;
}

void set_type(table_entry id_entry, int id_type)
{
	id_entry->IDType = id_type;
}

int get_type(table_entry id_entry)
{
	return id_entry->IDType;
}

//duplicates string
char* m_strdup(char *o)
{
	int l = strlen(o) + 1;
	char *ns = (char*)malloc(l * sizeof(char));
	strcpy(ns, o);

	return ns;
}

void setEntry(table_ptr* cur_tab, table_entry i_Entry, char* id_name)
{
	unsigned int cryptoID = crypto(id_name);

	table_entry cur_entry = cur_tab->symbolTable[cryptoID];

	if (cur_entry == NULL)
	{
		cur_tab->symbolTable[cryptoID] = i_Entry;
	}
	else
	{
		while (cur_entry->next != NULL)
		{
			cur_entry = cur_entry->next;
		}

		cur_entry->next = i_Entry;
	}
}

int crypto(char *ptr)
{
	int c=0,i=0;

	while (ptr[i] != '\0')
	{
		c = ptr[i] + (c * 87);
		i++;
	}
	return c%HASHSIZE;
}
