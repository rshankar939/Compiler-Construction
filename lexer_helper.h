/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include "hash_lex.h"

#ifndef LEXER_HELPER_H
#define LEXER_HELPER_H

extern hash_table_lex* get_lexem_list(char *file);
extern void clean_lexem_list(hash_table_lex *ht);
extern int free_pointer(void* ptr);

#endif
