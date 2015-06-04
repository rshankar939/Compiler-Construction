/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
// #include "parser_helper.h"
// #include "symbol_table.h"
#include <stdio.h>
#ifndef PARSE_TREE_H
#define PARSE_TREE_H

/*types of variables*/
#define T_INT 1
#define T_REAL 2
#define T_CHAR 3
#define T_BOOLEAN 4
#define T_TUPLE 5
#define T_STRING 6
#define T_ARRAY 7
#define T_IDENTIFIER 8
#define T_FUNC_IDENTIFIER 9
#define T_NON_TERMINAL 10
#define T_TERMINAL 11			//terminals other than identifiers
#define T_EPSILON 12

/*for boollean*/
#define T_TRUE 1
#define T_FALSE 0
typedef char *t_string;

typedef struct {
	int integer;
	float real;
	int boolean;
	t_string str;
	char character;
}t_value;

typedef struct t_parse_tree_data {
	t_string token;
	int line_no;
	char *lexem;
	int type;
	int scope;
	t_value val;
	void *symbol_table_pointer;
}t_parse_tree_data;

typedef struct t_parse_tree_node {
	t_parse_tree_data *data;
	struct t_parse_tree_node *parent;
	struct t_parse_tree_node **child_nodes;
	int current_child_ptr;
	int child_nodes_count;
}t_parse_tree_node;

typedef struct {
	t_parse_tree_node *root;
	t_parse_tree_node *curr;
}t_parse_tree;

/*return an AST form the parse tree*/
extern t_parse_tree* get_AST(t_parse_tree *parse_tree);
/*in-order traversal of the parse_tree*/
extern int get_AST_helper(t_parse_tree_node *parse_tree, int level);
/*reduces the node*/
extern int reduce_node(t_parse_tree_node *parse_tree);
/*removes EPSILON from the child node*/
extern int remove_unwanted(t_parse_tree_node *node);

/*return an empty node with all values set to zero*/
extern t_parse_tree_node* get_node();
/*return a parse tree with <module> as at its root node*/
extern t_parse_tree* create_parse_tree();
/*moves the pointer to the parent of current node*/
extern int move_to_parent(t_parse_tree *parse_tree);
/*is this the last child of the parent node*/
extern int is_last_child(t_parse_tree *parse_tree);
/*moves the pointer to next child node in the node*/
extern int move_next_child(t_parse_tree *parse_tree);
/*insert rule into the parse tree and moves the current pointer to the first element in the rule*/
extern int insert_rule_parse_table(t_parse_tree *parse_tree, t_string rule_rhs);
/*insert a token into the node*/
// extern int insert_token_into_node(t_parse_tree_node *parse_tree, );
/*function to delete parse tree*/
extern int free_parse_tree(t_parse_tree *parse_tree);
/*helper function to free_parse_tree*/
extern int helper_free_parse_tree(t_parse_tree_node *parse_tree);
/*function to display the parse tree*/
extern int display_parse_tree(t_parse_tree_node *parse_tree, int level, FILE *fp);
/*functions to update current node*/


/*function to parse the input from the given file(lexer output)*/
// extern int parser(char *filename, FILE *fp_pt, FILE *fp_ast, t_parse_tree **p_t, symbol_table **st);
#endif