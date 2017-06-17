#ifndef GARAGE
#define GARAGE

#include <stdio.h>
#include <string.h>
#include "Token.h"

void create_and_store_token(int, int, char*);
token Next_Token();
token Back_Token();

#endif