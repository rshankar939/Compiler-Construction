/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include "hash_lex.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void free_hash_table_lex(hash_table_lex *ht)
{
	int i;
	linked_list_lex list,temp;
	for(i=0;i<ht->size;i++)
	{
		list = ht->table[i];
		while(list != NULL)
		{
			temp = list->next;
			free(list->lexem);
			free(list->token_id);
			free(list);
			list = temp;
		}
	}
	free(ht->table);
	free(ht);
	return;
}

int insert_into_hash_table_lex(hash_table_lex *ht, char* key, char* token_id)
{
	int index = hash_function_lex(key, ht->size);
	linked_list_lex node;
	//check if the element is already in the hash table
	node = lookup_hash_table_lex(ht, key);
	if(node!=NULL)
	{
			printf("The key value already exists in the hash table.\n");
			return 2;
	}
	//get a new node for the data
	node = (linked_list_lex)malloc(sizeof(linked_list_node_lex));
	if(node == NULL)
	{
		printf("Unable to insert node due to insufficent memory.\n");
		return 1;
	}
	node->token_id = strdup(token_id);
	node->lexem = strdup(key);
	//insert into the hashtable
	node->next = ht->table[index];
	ht->table[index] = node;
//	display(ht);
	return 0;
}

linked_list_lex lookup_hash_table_lex(hash_table_lex *ht, char* key)
{
	int index = hash_function_lex(key, ht->size);
	linked_list_lex list=ht->table[index],ret;
	while(list!=NULL)
	{
		if(strcmp(list->lexem, key) == 0)
		{
			ret = (linked_list_lex)malloc(sizeof(linked_list_node_lex));
			ret->lexem = strdup(list->lexem);
			ret->token_id = strdup(list->token_id);
			ret->next = NULL;
			return ret;
		}
		list = list->next;
	}
	return NULL;
}

int hash_function_lex(char *str, int size) {
	unsigned int val=0;
	for(; *str != '\0'; str++)
		val = (int)*str + (val<<5) - val;
	return (int)(val%(size));
}

hash_table_lex* create_hash_table_lex(int size)
{
	int i;
	hash_table_lex *ht;
	if((ht = (hash_table_lex*)malloc(sizeof(hash_table_lex))) == NULL)
	{
		printf("Unable to allocate space for the hash table.\n");
		return NULL;
	}
	//allocate space for buckets in the hash table
	if((ht->table = (linked_list_lex*)malloc(size*sizeof(linked_list_lex))) == NULL)
	{
		printf("Unable to allocate space for buckets of hash table.\n");
		return NULL;
	}
	ht->size = size;
	//initailize the buckets of hash table
	for(i=0;i<ht->size;i++)
	{
		ht->table[i] = NULL;
	}
	return ht;
}

void display_hash_table_lex(hash_table_lex *ht)
{
	int i,f;
	linked_list_lex list, temp;
	printf("size of the stored linked list is %d.\n",ht->size);
	for(i=0;i<ht->size;i++)
	{
		f=0;
		list = ht->table[i];
		while(list!=NULL)
		{
			f=1;
			temp = list->next;
			printf("%s->%s ",list->lexem,list->token_id);
			list = temp;
		}
		if(f)
			printf("\n");
	}
	return;
}
