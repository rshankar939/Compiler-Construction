/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define debug(A) printf("%s\n",A);fflush(stdout)

char *type_arr1[13] = {"", "T_INT", "T_REAL", "T_CHAR", "T_BOOLEAN", "T_TUPLE", "T_STRING", "T_ARRAY", "T_IDENTIFIER", "T_FUNC_IDENTIFIER", "T_NON_TERMINAL", "T_TERMINAL", "T_EPSILON"};

/*
*	function used to rehash the entire hash table 
*	if the frequency of clash increases beyond a fixed point
*/
symbol_table* rehash_symbol_table(symbol_table *st)
{
	if(st->num_element < st->size)
	{
		return st;
	}
	symbol_table *new_st = (symbol_table*)malloc(sizeof(symbol_table));
	if(new_st == NULL)
		return st;
	new_st->size = 2*st->size;
	new_st->table = (symbol_list *)malloc(new_st->size*sizeof(symbol_list));

	int i,index;
	symbol_list list,temp,t1;
	for(i=0;i<st->size;i++)
	{
		list = st->table[i];
		while(list != NULL)
		{
			temp = list->next;
			index = hash_function_symbol_table(list->identifier, list->scope, new_st);
			t1 = new_st->table[index];
			new_st->table[index] = list;
			list->next = t1;
			list = temp;
		}
	}
	free_symbol_table(st);
	return new_st;
}

/*returns a pointer to the value queried*/
/*
*	WARNING:-
*		it returns a direct pointer to the instace stored in the symbol table
*		Careful while manupulating and freeing this node
*/
symbol_list lookup_symbol_table(symbol_table *st, char *identifier, int scope)
{
	// debug("entering lookup_symbol_table");
	int index = hash_function_symbol_table(identifier, scope, st);
	symbol_list list = st->table[index];
	while(list!=NULL)
	{
		// printf("lookup id->%s scope(%d) referred_count:- %d\n", list->identifier, list->scope, list->referred_count);
		// fflush(stdout);
		if((strcmp(identifier, list->identifier) == 0) && (scope == list->scope))
		{
			// debug("exiting lookup_symbol_table");
			return list;
		}
		list = list->next;
	}
	// debug("exiting2 lookup_symbol_table");
	return NULL;
}

/*add the line referred to the node*/
int add_line_referred(symbol_list node, int line_no)
{
	node->line_referred = (int *)realloc(node->line_referred, (node->referred_count + 1)*sizeof(int));
	node->line_referred[node->referred_count] = line_no;
	(node->referred_count)++;
}

/*insert the node into the symbol table*/
int insert_into_symbol_table(symbol_table *st, char *identifier, int scope, int line_no)
{
	// debug("entering insert into symbol table");
	int index = hash_function_symbol_table(identifier, scope, st);
	symbol_list node, temp;
	/*
		check if the element is already in the hash table
		if it exist then add the line number to the reffered list
	*/
	node = lookup_symbol_table(st, identifier, scope);
	if(node != NULL)
	{
		// printf("The key value is already in the symbol table\n");
		add_line_referred(node, line_no);
		return 1;
	}
	//get a new node
	node = (symbol_list)malloc(sizeof(symbol));
	if(node == NULL)
	{
		printf("Unable to insert node due to insufficent memory.\n");
		return 2;
	}
	node->scope = scope;
	node->identifier = strdup(identifier);
	node->line_referred = NULL;
	node->referred_count = 0;
	node->next = NULL;
	add_line_referred(node, line_no);
	temp = st->table[index];
	st->table[index] = node;
	node->next = temp;
	//call the rehash function
	st = rehash_symbol_table(st);
	// debug("exting insert into symbol table");
	return 0;
}

/*returns an empty symbol table*/
symbol_table* create_symbol_table(int size)
{
	symbol_table *st = (symbol_table *)malloc(sizeof(symbol_table));
	if(st == NULL)
	{
		printf("Insuffficent space for symbol table\n");
		return NULL;
	}
	st->size = size;
	st->sg = NULL;
	st->num_element = 0;
	st->table = (symbol_list *)malloc(size*sizeof(symbol_list));
	int i;
	for(i=0;i<st->num_element;st++)
	{
		st->table[i] = NULL;
	}
	return st;
}

/*free symbol data structure*/
void free_symbol_structure(symbol *list)
{
	free(list->line_referred);
	free(list->identifier);
	if(list->type == T_FUNCTION_CALL)
	{
		//TODO free structure
	}
	else if(list->type == T_FUNCTION_DECLARATION)
	{
		//TODO free structure
	}
	free(list->info);
	return;
}

/*frees the symbol table*/
void free_symbol_table(symbol_table* st)
{
	int i;
	symbol_list temp,list;
	for(i=0;i<st->size;i++)
	{
		list = st->table[i];
		while(list != NULL)
		{
			temp = list->next;
			free_symbol_structure(list);
			list = temp;
		}
	}
	free(st->table);
	free_scope_graph(st->sg);
	free(st);
	return;
}

/*returns the index of the node*/
int hash_function_symbol_table(char *identifier, int scope, symbol_table *st)
{
	unsigned int val=0;
	char *str = identifier;
	for(; *str != '\0'; str++)
		val = (int)*str + (val<<5) - val;
	val += scope;
	return (int)(val%(st->size));
}

/*display symbol table*/
void display_symbol_table(symbol_table *st, FILE *fp)
{
	// debug("entering display_symbol_table");
	int i;
	symbol_list list, temp;
	for(i=0;i<st->size;i++)
	{
		list = st->table[i];
		while(list != NULL)
		{
			fprintf(fp,"id-%s scope(%d) referred_count:- %d type:- %s\n", list->identifier, list->scope, list->referred_count, type_arr1[list->type]);
			list = list->next;
		}
	}
	// debug("exiting display_symbol_table");
	return;
}

/*
*	0 if exists
*	1 otherwise
*/
scope_graph_node* goto_scope(int scope, scope_graph *sg)
{
	scope_graph_node* temp;
	if(sg->curr->scope == scope)
		return sg->curr;
	int i=0;
	// printf("here2 %d %d\n",sg->curr->child_count,sg->curr->scope);
	for(i=0;i<sg->curr->child_count; i++)
	{
		sg->curr = sg->curr->child_scopes[i];
		if((temp = goto_scope(scope, sg)) != NULL)
			return temp;
		sg->curr = sg->curr->parent_scope;
		// printf("here4 %d %d\n",sg->curr->child_count,sg->curr->scope);
		// fflush(stdout);
	}
	return NULL;
}

/*
*/
symbol_list exist_in_scope(char *id, int scope, symbol_table *st)
{
	/*go to current scope*/
	st->sg->curr = st->sg->root;
	scope_graph_node *temp;
	if( (temp = goto_scope(scope, st->sg)) == NULL)
	{
		return NULL;
	}
	st->sg->curr = temp;
	symbol_list sl;
	while(1)
	{
		sl = lookup_symbol_table(st, id, st->sg->curr->scope);
		if(sl!=NULL)
		{
			return sl;
		}
		if( (st->sg->curr->parent_scope)==NULL )
		{
			return NULL;
		}
		st->sg->curr = st->sg->curr->parent_scope;
	}
	return NULL;
}