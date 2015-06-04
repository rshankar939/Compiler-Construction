/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include "stack.h"
#include "hash_par.h"
#include "symbol_table.h"

#include <stdio.h>
#ifndef PARSER_HELPER_H
#define PARSER_HELPER_H

#define scan_error 143
#define pop_error 142
#define grammar_count 141

#define max_grammar_rule_len 200
#define terminal_count 55
#define non_terminal_count 69

#define grammar_file "grammar.txt"
#define non_terminal_file "non_terminal_list.txt"
#define terminal_file "terminal_list.txt"
#define parse_table_file "parse_table.txt"

#define DISPLAY_SCOPE_GRAPH 1

typedef struct lexem {
	char token[16];
	int lexem_len;
	int lexem_line;
	char *lexem_str;
}lexem;

extern lexem* get_next_lexem(FILE *fp, lexem *lex);
extern int display_lexems(FILE *fp);
extern void free_rule_list(char **rule_list);
extern void display_grammar(char **grammar);
extern char** get_rule_list(char *filename);
extern hash_table_par* get_list(char *file);
extern void clean_list(hash_table_par *ht);
extern int** get_parse_table(char *filename);
extern int free_parse_table(int ** parse_table);
extern int display_parse_table(int **parse_table);
extern int remove_next_terminal(stack *st, lexem *lex, int **parse_table, char **grammar, int *flag, hash_table_par *ht_non_terminal, hash_table_par *ht_terminal);

extern int parser(char *filename, FILE *fp_pt, FILE *fp_ast, t_parse_tree **p_t, symbol_table **st);
#endif
