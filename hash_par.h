/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
/*structure and function for hash table*/
#ifndef HASH_PAR_H
#define HASH_PAR_H
typedef struct linked_list_node {
	char *token_id;
	int value;
	struct linked_list_node *next;
}linked_list_node;

typedef linked_list_node* linked_list;

typedef struct hash_table_par {
	int size;
	linked_list *table;
}hash_table_par;

extern hash_table_par* create_hash_table(int size);
extern void free_hash_table(hash_table_par *ht);
extern void free_linked_list(linked_list list);
extern int hash_function(char *token_id,int size);
extern void display_hash_table(hash_table_par *ht);
extern int insert_into_hash_table(hash_table_par *ht, int value, char* token_id);
extern linked_list lookup_hash_table(hash_table_par *ht, char* token_id);
#endif