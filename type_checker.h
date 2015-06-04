/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/

#include "parse_tree.h"
#include "symbol_table.h"
#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

extern int type_checker(t_parse_tree_node *pt, symbol_table **st);
extern int process_node(t_parse_tree_node *parse_tree, symbol_table **st);
extern int handle_declaration(t_parse_tree_node *pt, symbol_table **st);
extern int handle_function_declaration(t_parse_tree_node *parse_tree, symbol_table **st);
extern int handle_function_call(t_parse_tree_node *parse_tree, symbol_table **st);
extern int handle_tk_id(t_parse_tree_node *parse_tree, symbol_table **st);
extern int handle_derived_types(t_parse_tree_node *parse_tree, symbol_table **st);
extern int handle_array_init(t_parse_tree_node *pt, symbol_table **st);
extern int handle_assignment_statement(t_parse_tree_node *pt, symbol_table **st);

#endif