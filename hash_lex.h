/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
/*structure and function for hash table*/
#ifndef HASH_H
#define HASH_H
typedef struct linked_list_node_lex {
	char *lexem;
	char *token_id;
	struct linked_list_node_lex *next;
}linked_list_node_lex;

typedef linked_list_node_lex* linked_list_lex;

typedef struct hash_table_lex {
	int size;
	linked_list_lex *table;
}hash_table_lex;

extern hash_table_lex* create_hash_table_lex(int size);
extern void free_hash_table_lex(hash_table_lex *ht);
extern int hash_function_lex(char* key, int size);
extern void display_hash_table_lex(hash_table_lex *ht);
extern int insert_into_hash_table_lex(hash_table_lex *ht, char* key, char* token_id);
extern linked_list_lex lookup_hash_table_lex(hash_table_lex *ht, char* key);
#endif