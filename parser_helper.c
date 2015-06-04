/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include "parser_helper.h"
#include "stack.h"
#include "parse_tree.h"
#include "hash_par.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* \return values:-
*	0 => parsing completed
*	1 => parsing errors occured
*	8 => unable to read parse table
*	2 => unable to read terminal list
*	3 => unable to read non terminal list
*	4 => unable to intialize stack
*	5 => unable to open lexer output
*	6 => unexpected end of parsing phase
*	7 => unexpected end of input lexem stream
*	8 => unable to get parse tree
*	9 => unable to get scope graph
*	10=> unable to get symbol table
*/
int parser(char *filename, FILE *fp_pt, FILE *fp_ast, t_parse_tree **p_t, symbol_table **sym_t)
{
	int flag = 0, acc = 0, curr_scope = 0;
	/*get hash tables*/
	hash_table_par *ht_terminal = get_list((char *)terminal_file);
	if(ht_terminal == NULL)
	{
		return 2;
	}
	hash_table_par *ht_non_terminal = get_list((char *)non_terminal_file);
	if(ht_terminal == NULL)
	{
		clean_list(ht_terminal);
		return 3;
	}
	/*get parse table*/
	int **parse_table = get_parse_table((char *)parse_table_file);
	if(parse_table == NULL)
	{
		clean_list(ht_terminal);
		clean_list(ht_non_terminal);
		printf("Unable to generate parse table.\n");
		return 8;
	}
	/*get stack*/
	stack *st = create_stack();
	if(st == NULL)
	{
		clean_list(ht_terminal);
		clean_list(ht_non_terminal);
		free_parse_table(parse_table);
		return 4;
	}
	/*read grammar rules*/
	char **grammar = get_rule_list((char *)grammar_file);
	if(grammar == NULL)
	{
		clean_list(ht_terminal);
		clean_list(ht_non_terminal);
		free_parse_table(parse_table);
		free_stack(st);
	}
	/*get file pointer to lexer output*/
	FILE *fp = fopen(filename, "r");
	if(fp == NULL)
	{
		clean_list(ht_terminal);
		clean_list(ht_non_terminal);
		free_parse_table(parse_table);
		free_stack(st);
		free_rule_list(grammar);
		return 5;
	}
	/*get parse tree with <module> at the root of it*/
	// t_parse_tree *parse_tree = create_parse_tree();
	t_parse_tree *parse_tree = *p_t;
	// parse_tree = create_parse_tree();
	// if(parse_tree == NULL)
	// {
	// 	clean_list(ht_terminal);
	// 	clean_list(ht_non_terminal);
	// 	free_parse_table(parse_table);
	// 	free_stack(st);
	// 	free_rule_list(grammar);
	// 	fclose(fp);
	// 	return 8;
	// }
	scope_graph* sg = create_scope_graph();
	if(sg == NULL)
	{
		free_parse_tree(parse_tree);
		clean_list(ht_terminal);
		clean_list(ht_non_terminal);
		free_parse_table(parse_table);
		free_stack(st);
		free_rule_list(grammar);
		fclose(fp);
		return 9;
	}
	symbol_table *table = *sym_t;
	// symbol_table *table = create_symbol_table(START_SIZE);
	// if(table == NULL)
	// {
	// 	free_scope_graph(sg);
	// 	free_parse_tree(parse_tree);
	// 	clean_list(ht_terminal);
	// 	clean_list(ht_non_terminal);
	// 	free_parse_table(parse_table);
	// 	free_stack(st);
	// 	free_rule_list(grammar);
	// 	fclose(fp);
	// 	return 10;
	// }
	char token[200],top_stack[200];
	token_linked_list tok;
	linked_list n1,n2;
	lexem *lex = (lexem *)malloc(sizeof(lexem));
	lex->lexem_str = NULL;
	lex = get_next_lexem(fp, lex);
	while(lex->lexem_str != NULL)
	{
		// printf("%s - %s\n",lex->token, lex->lexem_str);
		/*parser logic*/
		tok = top(st);
		/*if lex and top are equal to $*/
		if(strcmp(tok->token, "$") == 0 && strcmp(lex->lexem_str,"$")==0)
		{
			acc = 1;
			break;
		}
		/*if $ comes before the end of the input stream*/
		else if(strcmp(tok->token, "$") == 0 || strcmp(tok->token, "$") == 0)
		{
			printf("Unexpected end of parser at line number %d near %s\n",lex->lexem_line,lex->lexem_str);
			free(tok->token);
			free(tok);
			free(lex->lexem_str);
			free(lex);
			fclose(fp);
			return 6;
		}
		/*update the stack and remove the lexem from top*/
		while(1)
		{
			/*if top of stack is EPSILON*/
			if(strcmp(tok->token, "EPSILON") == 0)
			{
				pop(st);
				if(!flag)
				{
					move_to_parent(parse_tree);
					while(is_last_child(parse_tree)&&(parse_tree->root!=parse_tree->curr))
						move_to_parent(parse_tree);
					move_next_child(parse_tree);
				}
				free(tok->token);
				free(tok);
				tok = top(st);
				continue;
			}
			/*if top of stack is a terminal*/
			n1 = lookup_hash_table(ht_terminal, tok->token);
			if(n1 != NULL)
			{
				/*if top of stack is not same as lexem
				remove from stack untill a matching one is at the top*/
				if(strcmp(tok->token, lex->token))
				{
					printf("PARSER ERROR:-\n\t");
					printf("unexpected %s at line number %d\n", lex->lexem_str, lex->lexem_line);
					//TODO enter expected line will need a terminal lookup hashtable
					/*pop from stack*/
					flag = 1;
					pop(st);
					free(n1->token_id);
					free(n1);
				}
				/*top of stack is same as lexem - break*/
				/*if the token is "TK_ID" insert into the symbol table*/
				else
				{
					// printf("SAME %s %s\n",tok->token, lex->token);
					// fflush(stdout);
					//TODO AST and SYMBOL TABLE insterions
					parse_tree->curr->data->scope = curr_scope;
					if(!flag)
					{
						if(strcmp(tok->token, "TK_LC") == 0)
						{
							curr_scope++;
							start_new_scope(sg, curr_scope);
						}
						else if(strcmp(tok->token, "TK_RC") == 0)
						{
							end_scope(sg);
						}
					}
					parse_tree->curr->data->line_no = lex->lexem_line;
					parse_tree->curr->data->lexem = strdup(lex->lexem_str);
					parse_tree->curr->data->scope = curr_scope;
					if(strcmp(tok->token, "TK_NUM") == 0)
					{
						parse_tree->curr->data->val.integer = atoi(lex->lexem_str);
					}
					else if(strcmp(tok->token, "TK_RNUM") == 0)
					{
						parse_tree->curr->data->val.real = atof(lex->lexem_str);
					}
					else if(strcmp(tok->token, "TK_CCONST") == 0)
					{
						parse_tree->curr->data->val.character = lex->lexem_str[1];
					}
					else if(strcmp(tok->token, "TK_BCONST") == 0)
					{
						if(strcmp(lex->lexem_str, "true") == 0)
							parse_tree->curr->data->val.boolean = 1;
						else if(strcmp(lex->lexem_str, "false") == 0)
							parse_tree->curr->data->val.boolean = 0;
					}
					else if(strcmp(tok->token, "TK_STR") == 0)
					{
						parse_tree->curr->data->val.str = strdup(lex->lexem_str);
					}
					// if(!flag)
					// {
					// 	if(strcmp(tok->token, "TK_ID") == 0)
					// 	{
					// 		// t_parse_tree_node *temp;
					// 		// temp = parse_tree->curr->paremt;
					// 		// while
					// 		if(strcmp(parse_tree->curr->parent->data->token, "<globalDeclarationStmt>") == 0)
					// 			printf("here1\n");
					// 		else if(strcmp(parse_tree->curr->parent->data->token, "<declarationStmt>") == 0)
					// 			printf("here2\n");
					// 		printf("%s here3\n", parse_tree->curr->parent->data->token);
					// 		fflush(stdout);
					// 		insert_into_symbol_table(table, lex->lexem_str, curr_scope, lex->lexem_line);
					// 	}
					// 	else if(strcmp(tok->token, "TK_FUNCID") == 0)
					// 	{
					// 		insert_into_symbol_table(table, lex->lexem_str, curr_scope, lex->lexem_line);
					// 	}
					// }
					if(!flag)
					{
						while(is_last_child(parse_tree)&&(parse_tree->root != parse_tree->curr))
						{
							move_to_parent(parse_tree);
						}
						move_next_child(parse_tree);
					}
					pop(st);
					free(n1->token_id);
					free(n1);
					break;
				}
			}
			/*top of the stack is a non terminal*/
			else
			{
				n1 = lookup_hash_table(ht_non_terminal, tok->token);
				n2 = lookup_hash_table(ht_terminal, lex->token);
				/*if entry in parse table is scan error
				skip untill a  matching input*/
				if(parse_table[n1->value][n2->value] == scan_error)
				{
					flag = 1;
					printf("PARSER ERROR:-\n\t");
					printf("unexpected %s at line number %d\n", lex->lexem_str, lex->lexem_line);
					printf("\t%s %d %d\n", tok->token, n1->value, n2->value);
					//TODO expected error
					/*free n1 and n2*/
					free(n1->token_id);
					free(n1);
					free(n2->token_id);
					free(n2);
					break;
				}
				/*if entry in parse table is pop error
				ie the terminal is in the follow set but not in first set
				skip this terminal*/
				else if(parse_table[n1->value][n2->value] == pop_error)
				{
					flag = 1;
					// exit(0);
					printf("PARSER ERROR:-\n\t");
					printf("pop_error\n");

					if(!flag)
					{
						/*move to next terminal or a non-terminal if its not the last child*/
						while(1)
						{
							if(is_last_child(parse_tree))
							{
								move_to_parent(parse_tree);
								if(!is_last_child(parse_tree) && parse_tree->curr->data->type == T_NON_TERMINAL)
									break;
							}
							else
							{
								move_next_child(parse_tree);
								break;
							}	
						}	
					}
					pop(st);
				}
				/*if entry in parse table is valid entry*/
				else
				{
					pop(st);
					// printf("current node is %s\n",parse_tree->curr->data->token);
					// printf("pushing %d rule %s->%s\n", parse_table[n1->value][n2->value]-1,n1->token_id, grammar[parse_table[n1->value][n2->value]-1]);
					fflush(stdout);
					push_rule(st, grammar[parse_table[n1->value][n2->value]-1]);
					insert_rule_parse_table(parse_tree, grammar[parse_table[n1->value][n2->value]-1]);
					parse_tree->curr = parse_tree->curr->child_nodes[0];

					//TODO AST
				}
				/*free n1 and n2*/
				free(n1->token_id);
				free(n1);
				free(n2->token_id);
				free(n2);
			}
			// display_parse_tree(parse_tree->root, 0);
			/*update tok*/
			free(tok->token);
			free(tok);
			tok = top(st);
		}
		/*update lex*/
		// free(lex->lexem_str);
		// lex->lexem_str = NULL;
		// free(lex);
		// lex=NULL;
		// printf("%s\n", lex->lexem_str);
		// display_stack(st);
		lex = get_next_lexem(fp,lex);
		// printf("%s\n", lex->lexem_str);
		free(tok->token);
		free(tok);
	}
	fflush(stdout);
	clean_list(ht_terminal);
	clean_list(ht_non_terminal);
	free_parse_table(parse_table);
	free_stack(st);
	free_rule_list(grammar);
	free(lex);
	fclose(fp);
	if(!flag)
	{
		table->sg = sg;
	}
	else
	{
		printf("parse_error_occured\n");
		free_parse_tree(parse_tree);
		free_scope_graph(sg);
		// free_symbol_table(table);	
	}
	return flag;
}

lexem* get_next_lexem(FILE *fp, lexem *lex)
{
	int ret;
	free(lex->lexem_str);
	lex->lexem_str = NULL;
	if(fscanf(fp,"%s",lex->token)!=EOF)
	{
		fscanf(fp,"%d",&(lex->lexem_len));
		lex->lexem_str = (char*)malloc((lex->lexem_len+2)*sizeof(char));
		fgets(lex->lexem_str, lex->lexem_len+2, fp);
		// fscanf(fp,"%s %d",lex->lexem_str,&(lex->lexem_line));
		fscanf(fp, "%d", &(lex->lexem_line));
		// printf("aaaaaaaaaaaaaaaaaaaaaa\n");
		// fprintf(stdout,"%s %d %s %d\n", lex->token, lex->lexem_len, lex->lexem_str, lex->lexem_line);
		// printf("aaaaaaaaaaaaaaaaaaaaa\n");
		// fflush(stdout);
	}
	return lex;
}

int display_lexems(FILE *fp)
{
	lexem *lex = (lexem*)malloc(sizeof(lexem));
	if(lex == NULL)
	{
		printf("insufficient memory\n");
		return 1;
	}
	lex->lexem_str = NULL;
	lex = get_next_lexem(fp, lex);
	while(lex->lexem_str!=NULL)
	{
		printf("%s %s %d\n", lex->token, lex->lexem_str, lex->lexem_line);
		lex = get_next_lexem(fp, lex);
	}
	free(lex->lexem_str);
	free(lex);
	return 0;
}


int free_parse_table(int **parse_table)
{
	int i;
	for(i=0;i<non_terminal_count;i++)
	{
		free(parse_table[i]);
	}
	free(parse_table);
	return 0;
}

int** get_parse_table(char *filename)
{
	int **arr,i,j;
	arr = (int**)malloc(non_terminal_count*sizeof(int*));
	if(arr == NULL)
	{
		printf("insufficient memory for parse_table.\n");
		return NULL;
	}
	FILE *fp = fopen(filename,"r");
	for(i=0;i<non_terminal_count;i++)
	{
		arr[i] = (int*)malloc(terminal_count*sizeof(int));
		for(j=0;j<terminal_count;j++)
		{
			fscanf(fp,"%d",&arr[i][j]);
		}
	}
	fclose(fp);
	return arr;
}

int display_parse_table(int **parse_table)
{
	int i,j;
	printf("[\n");
	for(i=0;i<non_terminal_count;i++)
	{
		printf("[%d,",parse_table[i][0]);
		for(j=1;j<terminal_count;j++)
		{
			printf(",%d",parse_table[i][j]);
		}
		printf("]\n");
	}
	printf("]\n");
	return 0;
}

void free_rule_list(char **rule_list)
{
	int i;
	for(i=0;i<grammar_count;i++)
	{
		free(rule_list[i]);
	}
	free(rule_list);
	return;
}

void display_grammar(char **grammar)
{
	int ret;
	for(ret = 0; ret < grammar_count; ret++)
	{
		printf("%d %s\n",ret+1,grammar[ret]);
	}
	return;
}

char** get_rule_list(char *filename)
{
	int ret,c=0;
	char **arr;
	arr = (char **)malloc(grammar_count*sizeof(char*));
	if(arr == NULL)
	{
		printf("insufficient memory for grammar.\n");
		return NULL;
	}
	char grammar[max_grammar_rule_len],*temp,*t;
	FILE *inp_file = fopen(filename, "r");
	while(fgets(grammar, max_grammar_rule_len-1, inp_file)!=NULL)
	{
		grammar[strlen(grammar)-1]='\0';
		t = strdup(grammar);
		temp = strtok(t, "-> ");
		temp = strtok(NULL, " ");
		temp = strtok(NULL, "");
		arr[c] = strdup(temp);
		c++;
	}
	fclose(inp_file);
	return arr;
}

hash_table_par* get_list(char *filename)
{
	int ret;
	char token[32];
	int value;
	FILE *inp_file = fopen(filename, "r");
	hash_table_par *ht = create_hash_table(200);
	while(fscanf(inp_file, "%d %s", &value, token)!=EOF)
	{
		if((ret = insert_into_hash_table(ht, value, token)))
		{
			printf("%s was not inserted into the hashtable and return value was %d.\n",token,ret);
			free_hash_table(ht);
			return NULL;
		}
	}
	fclose(inp_file);
	return ht;
}

void clean_list(hash_table_par *ht)
{
	free_hash_table(ht);
	return;
}
