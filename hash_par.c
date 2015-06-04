/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include "hash_par.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void free_linked_list(linked_list list)
{
	linked_list temp;
	while(list != NULL)
	{
		temp = list->next;
		free(list->token_id);
		free(list);
		list = temp;
	}
	return;
}

void free_hash_table(hash_table_par *ht)
{
	int i;
	linked_list list,temp;
	for(i=0;i<ht->size;i++)
	{
		list = ht->table[i];
		free_linked_list(list);
	}
	free(ht->table);
	free(ht);
	return;
}

int insert_into_hash_table(hash_table_par *ht, int value, char* token_id)
{
	int index = hash_function(token_id, ht->size);
	linked_list node;
	//check if the element is already in the hash table
	node = lookup_hash_table(ht, token_id);
	if(node!=NULL)
	{
			printf("The key value already exists in the hash table.\n");
			return 2;
	}
	//get a new node for the data
	node = (linked_list)malloc(sizeof(linked_list_node));
	if(node == NULL)
	{
		printf("Unable to insert node due to insufficent memory.\n");
		return 1;
	}
	node->token_id = strdup(token_id);
	node->value = value;
	//insert into the hashtable
	node->next = ht->table[index];
	ht->table[index] = node;
//	display(ht);
	return 0;
}

linked_list lookup_hash_table(hash_table_par *ht, char* token_id)
{
	int index = hash_function(token_id, ht->size);
	linked_list list=ht->table[index], ret=NULL;
	while(list!=NULL)
	{
		if(strcmp(list->token_id, token_id) == 0)
		{
			ret = (linked_list)malloc(sizeof(linked_list_node));
			if(ret == NULL)
			{
				printf("insufficent memory for lookup\n");
				break;
			}
			ret->token_id = strdup(list->token_id);
			ret->value = list->value;
			ret->next = NULL;
			break;
		}
		list = list->next;
	}
	return ret;
}

int hash_function(char *str, int size) {
	unsigned int val=0;
	for(; *str != '\0'; str++)
		val = (int)*str + (val<<5) - val;
	return (int)(val%(size));
}

hash_table_par* create_hash_table(int size)
{
	int i;
	hash_table_par *ht;
	if((ht = (hash_table_par*)malloc(sizeof(hash_table_par))) == NULL)
	{
		printf("Unable to allocate space for the hash table.\n");
		return NULL;
	}
	//allocate space for buckets in the hash table
	if((ht->table = (linked_list*)malloc(size*sizeof(linked_list))) == NULL)
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

void display_hash_table(hash_table_par *ht)
{
	int i,f;
	linked_list list, temp;
	printf("size of the stored linked list is %d.\n",ht->size);
	for(i=0;i<ht->size;i++)
	{
		f=0;
		list = ht->table[i];
		while(list!=NULL)
		{
			f=1;
			temp = list->next;
			printf("%d->%s ",list->value,list->token_id);
			list = temp;
		}
		if(f)
			printf("\n");
	}
	return;
}
