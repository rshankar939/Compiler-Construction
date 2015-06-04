/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include "lexer_helper.h"
#include <stdio.h>
#include <stdlib.h>

int free_pointer(void* ptr)
{
	free(ptr);
	ptr = NULL;
	return 0;
}

hash_table_lex* get_lexem_list(char *filename)
{
	int ret;
	char lexem[32], token[32];
	FILE *inp_file = fopen(filename, "r");
	hash_table_lex *ht = create_hash_table_lex(200);
	while(fscanf(inp_file, "%s %s", lexem, token)!=EOF)
	{
		if((ret = insert_into_hash_table_lex(ht, lexem, token)))
		{
			printf("%s was not inserted into the hashtable and return value was %d.\n",lexem,ret);
		}
	}
	return ht;
}

void clean_lexem_list(hash_table_lex *ht)
{
	free_hash_table_lex(ht);
	return;
}
