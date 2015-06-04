/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include "./parse_tree.h"
#ifndef DECO_AST_H
#define DECO_AST_H

extern int delete_node(t_parse_tree_node* parse_tree);
extern int move_node_up(t_parse_tree_node* parse_tree);

extern int delete_all_declaration_stmt(t_parse_tree_node *parse_tree);
extern int delete_all_stmts(t_parse_tree_node *parse_tree);

extern t_parse_tree* get_decorated_ast(t_parse_tree *parse_tree);
extern int process_node_ast(t_parse_tree_node *parse_tree);
extern int handle_declaration_stmt(t_parse_tree_node *parse_tree);
extern int handle_assignment_stmt(t_parse_tree_node *parse_tree);

#endif