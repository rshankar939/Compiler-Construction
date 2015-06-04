/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/

#include "parse_tree.h"
#include <stdio.h>
#ifndef H_SYMBOL_TABLE
#define H_SYMBOL_TABLE

/*scope dependency graph*/
typedef struct scope_graph_node{
	int scope;
	int child_count;
	int max_child_scope_count;
	struct scope_graph_node **child_scopes;
	struct scope_graph_node *parent_scope;
}scope_graph_node;

typedef struct scope_graph {
	scope_graph_node *root;
	scope_graph_node *curr;
}scope_graph;

/*returns a scope graph with only one value defaulted to zero*/
extern scope_graph* create_scope_graph();
/*starts a new scope as a child to the current scope*/
extern int start_new_scope(scope_graph *sg, int scope_id);
/*ends a scope and move the current scope to the parent scope*/
extern int end_scope(scope_graph *sg);
/*deletes the scope graph*/
extern int free_scope_graph(scope_graph *sg);
/*get a default scope_graph_node*/
extern scope_graph_node* get_scope_graph_node();
/*displays scope graph*/
extern int display_scope_graph(scope_graph_node* sg, int level, FILE *fp);

/*symbol table declarations and types*/
#define T_FUNCTION_CALL 10
#define T_FUNCTION_DECLARATION 11
#define START_SIZE 100

typedef struct t_tuple_info {
	int num_element;
	int *type_list;
	//works as offset
	int *start_address_list;
}t_tuple_info;

typedef struct t_array_info {
	int type;
	int dimension_count;
	int *dimension_max_limit;
}t_array_info;

typedef struct t_function_declration_info {
	int parameter_count;
	int *parameter_type_list;
	// char **parameter_identifier;
	int return_count;
	int *return_type_list;
}t_function_declration_info;

typedef struct t_function_call_info {
	int parameter_count;
	int *parameter_type_list;
	//char **parameter_identifier;
}t_function_call_info;

typedef union t_info {
	t_array_info array_info;
	t_tuple_info tuple_info;
	t_function_call_info function_call_info;
	t_function_declration_info function_declration_info;
	t_value rest_info;
}t_info;

typedef struct symbol {
	int type;
	int scope;
	char *identifier;
	int line_declared;
	int *line_referred;
	int referred_count;
	int start_address;
	int size;
	t_info *info;
	struct symbol *next;
}symbol;

typedef symbol* symbol_list;

typedef struct symbol_table {
	int size;
	int num_element;
	scope_graph *sg;
	symbol_list *table;
}symbol_table;

/*free symbol data structure*/
extern void free_symbol_structure(symbol *list);
/*returns an empty symbol table*/
extern symbol_table* create_symbol_table(int size);
/*free the symbol table*/
extern void free_symbol_table(symbol_table* st);
/*returns the index of the node*/
extern int hash_function_symbol_table(char *identifier, int scope, symbol_table *st);
/*display symbol table*/
extern void display_symbol_table(symbol_table *st, FILE *fp);
/*insert the node into the symbol table*/
extern int insert_into_symbol_table(symbol_table *st, char *identifier, int scope, int line_no);
/*returns a pointer to the value queried*/
/*
*	WARNING:-
*		it returns a direct pointer to the instace stored in the symbol table
*		Careful while manupulating and free this node
*/
extern symbol_list lookup_symbol_table(symbol_table *st, char *identifier, int scope);
/*
*	function used to rehash the entire hash table 
*	if the frequency of clash increases beyond a fixed point
*/
extern symbol_table* rehash_symbol_table(symbol_table *st);

extern symbol_list exist_in_scope(char *id, int scope, symbol_table *st);
#endif