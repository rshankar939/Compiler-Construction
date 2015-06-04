/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_tree.h"
#include "parser_helper.h"
#include "stack.h"
#define debug(i) printf("%d\n",i);fflush(stdout)
/*
*	return a pointer to AST
*		it reduces the given parse tree into an abstract syntax tree
*		NULL is returned in case of any errors in conversion
*/
t_parse_tree* get_AST(t_parse_tree *parse_tree)
{
	t_parse_tree_node *root = parse_tree->root;
	if(get_AST_helper(root, 0))
	{
		return NULL;
	}
	parse_tree->curr = parse_tree->root;
	return parse_tree;
}

/*
*	uses in-order traversal or reduction
*	return values
*		0 => no error
*		else some error occurred
*/
int get_AST_helper(t_parse_tree_node *parse_tree, int level)
{
	int i;
	for(i=0;i<parse_tree->child_nodes_count;i++)
	{
		get_AST_helper(parse_tree->child_nodes[i], level+1);
	}
	// printf("------------------------------------\n");
	// printf("parse_tree %s %d\n", parse_tree->data->token, level);
	// display_parse_tree(parse_tree, level);
	remove_unwanted(parse_tree);
	// printf("--------------------------\n");
	// printf("parse_tree %s %d\n", parse_tree->data->token, level);
	// display_parse_tree(parse_tree, level);
	fflush(stdout);
	reduce_node(parse_tree);
	// printf("----------------\n");
	// printf("parse_tree %s %d\n", parse_tree->data->token, level);
	// display_parse_tree(parse_tree, level);
	fflush(stdout);
	return 0;
}

/*
*	reduces the node,
*	if it is a terminal node skip
*	if it is a non-terminal node
*		if it has only one child then move it up
*		if there is no child node then modifies it to a EPSILON node
*		else uses the grammar to reduce the node (HOW?? TODO)
*	return values
*		0 => non-terminal with non-zero child reduces
*		1 => terminal hence skipped
*		2 => no child node, reduced to epsilon
*		else some error occurred
*/
int reduce_node(t_parse_tree_node *parse_tree)
{
	if(parse_tree->data->token[0] == 'T')
	{
		return 1;
	}
	if(parse_tree->child_nodes_count == 0 && strcmp(parse_tree->data->token, "EPSILON"))
	{
		free(parse_tree->data->token);
		parse_tree->data->token = strdup("EPSILON");
		return 2;
	}
	// else if(parse_tree->child_nodes_count == 1 && )
	// {

	// }
	//TODO other non terminal reduction
	return 0;
}

/*
*	removes all the child node with EPSILON as token
*	return values
*		0 => all the epsilons are removed
*		else some error occurred
*/
int remove_unwanted(t_parse_tree_node *node)
{
	if(node->child_nodes_count == 0)
		return 0;
	int i,j,counter=0;
	t_parse_tree_node *temp;
	for(i=0;i<node->child_nodes_count;i++)
	{
		//remove all epsilon from the child nodes
		if((strcmp(node->child_nodes[i]->data->token, "EPSILON") == 0)
				|| (strcmp(node->child_nodes[i]->data->token, "TK_LS") == 0)
				|| (strcmp(node->child_nodes[i]->data->token, "TK_RS") == 0)
				|| (strcmp(node->child_nodes[i]->data->token, "TK_LP") == 0)
				|| (strcmp(node->child_nodes[i]->data->token, "TK_RP") == 0)
				|| (strcmp(node->child_nodes[i]->data->token, "TK_LC") == 0)
				|| (strcmp(node->child_nodes[i]->data->token, "TK_RC") == 0)
				|| (strcmp(node->child_nodes[i]->data->token, "TK_COMMA") == 0)
				|| (strcmp(node->child_nodes[i]->data->token, "TK_COLON") == 0)
				|| (strcmp(node->child_nodes[i]->data->token, "TK_SEMICOLON") == 0)
				|| (strcmp(node->child_nodes[i]->data->token, "TK_OPRET") == 0)
			)
		{
			//TODO free the nodes
			free(node->child_nodes[i]);
			node->child_nodes[i] = NULL;
			counter++;
		}
	}
	for(i=0;i<counter;i++)
	{
		for(j=0;j<node->child_nodes_count-1;j++)
		{
			if(node->child_nodes[j] == NULL)
			{
				temp = node->child_nodes[j];
				node->child_nodes[j] = node->child_nodes[j+1];
				node->child_nodes[j+1] = temp;		
			}
		}
	}
	node->child_nodes_count -= counter;
	//todo need to call realloc on node->child_nodes
	node->child_nodes = (t_parse_tree_node **)realloc(node->child_nodes,node->child_nodes_count*sizeof(t_parse_tree_node *));
	return 0;
}

/*
*	return value
*		by default its 0
*/
int display_parse_tree(t_parse_tree_node *root, int level, FILE *fp)
{
	// debug(1);
	int i=0;
	// debug(2);
	t_parse_tree_node *parse_tree = root;
	/*print the current node*/
	for(i=0;i<level;i++)
		fprintf(fp,"\t");
	fprintf(fp, "%d. %s %d line %d scope %d childs (%d)",level, parse_tree->data->token, parse_tree->data->type, parse_tree->data->line_no, parse_tree->data->scope,parse_tree->child_nodes_count);
	if(parse_tree->data->type == T_INT)
	{
		fprintf(fp, "%d\n",parse_tree->data->val.integer);
	}
	else if(parse_tree->data->type == T_REAL)
	{
		fprintf(fp, "%f\n", parse_tree->data->val.real);
	}
	else if(parse_tree->data->type == T_BOOLEAN)
	{
		fprintf(fp, "%d\n", parse_tree->data->val.boolean);
	}
	else if(parse_tree->data->type == T_CHAR)
	{
		fprintf(fp, "'%c'\n", parse_tree->data->val.character);
	}
	else if(parse_tree->data->type == T_STRING)
	{
		fprintf(fp, "%s\n", parse_tree->data->val.str);
	}
	else if(parse_tree->data->type == T_ARRAY)
	{
		fprintf(fp, "ARRAY\n");
	}
	else if(parse_tree->data->type == T_TUPLE)
	{
		fprintf(fp, "TUPLE\n");
	}
	else if(parse_tree->data->type == T_NON_TERMINAL)
	{
		fprintf(fp, "NON_TERMINAL\n");
	}
	else
		fprintf(fp, "\n");
	for(i=0;i<parse_tree->child_nodes_count;i++)
	{
		display_parse_tree(parse_tree->child_nodes[i], level+1, fp);
	}
	return 0;
}

/*
*	return values:-
*		0 => successfully free parse tree
*		1 => failure in freeing parse tree
*/
int free_parse_tree(t_parse_tree *parse_tree)
{
	helper_free_parse_tree(parse_tree->root);
	free(parse_tree);
	return 0;	
}

/*
*	return values:-
*		0 => successfully free parse tree
*		1 => failure in freeing parse tree
*/
int helper_free_parse_tree(t_parse_tree_node *parse_tree)
{
	int i=0;
	for(i=0;i<parse_tree->child_nodes_count;i++)
	{
		helper_free_parse_tree(parse_tree->child_nodes[i]);
	}
	free(parse_tree->data);
	free(parse_tree->child_nodes);
	free(parse_tree);	
	return 0;
}

/*
*	reuturn values:-
*		0 => successfully inserted into the parse_tree
*
*/
int insert_rule_parse_table(t_parse_tree *parse_tree, t_string rule_rhs)
{
	// printf("inside insert rule into parse tree\n");
	int s=0;
	char *tok,*val=strdup(rule_rhs);
	token_linked_list list=NULL,node;
	tok = strtok(val, " ");
	while(tok != NULL)
	{
		node = (token_linked_list)malloc(sizeof(token_linked_list_node));
		if(node == NULL)
		{
			printf("Insufficient memory to push the rule.\n");
			free_token_linked_list(list);
			return 1;
		}
		node->token = strdup(tok);
		node->next = list;
		list = node;
		s++;
		tok = strtok(NULL, " ");
	}
	parse_tree->curr->child_nodes = (t_parse_tree_node **)malloc(s*sizeof(t_parse_tree_node *));
	parse_tree->curr->child_nodes_count = s;
	parse_tree->curr->current_child_ptr = 0;
	for(s = parse_tree->curr->child_nodes_count-1; s>=0; s--)
	{
		parse_tree->curr->child_nodes[s] = get_node();
		parse_tree->curr->child_nodes[s]->data->token = strdup(node->token);
		parse_tree->curr->child_nodes[s]->parent = parse_tree->curr;
		if(node->token[0] == '<')
		{
			parse_tree->curr->child_nodes[s]->data->type = T_NON_TERMINAL;
		}
		else if(strcmp(node->token, "TK_ID") == 0)
		{
			parse_tree->curr->child_nodes[s]->data->type = T_IDENTIFIER;
		}
		else if(strcmp(node->token, "TK_NUM") == 0)
		{
			parse_tree->curr->child_nodes[s]->data->type = T_INT;
		}
		else if(strcmp(node->token, "TK_RNUM") == 0)
		{
			parse_tree->curr->child_nodes[s]->data->type = T_REAL;
		}
		else if(strcmp(node->token, "TK_CCONST") == 0)
		{
			parse_tree->curr->child_nodes[s]->data->type = T_CHAR;
		}
		else if(strcmp(node->token, "TK_BCONST") == 0)
		{
			parse_tree->curr->child_nodes[s]->data->type = T_BOOLEAN;
		}
		else if(strcmp(node->token, "TK_FUNCID") == 0)
		{
			parse_tree->curr->child_nodes[s]->data->type = T_FUNC_IDENTIFIER;
		}
		else if(strcmp(node->token, "TK_STR") == 0)
		{
			parse_tree->curr->child_nodes[s]->data->type = T_STRING;
		}
		else if(strcmp(node->token, "EPSILON") == 0)
		{
			parse_tree->curr->child_nodes[s]->data->type = T_EPSILON;
		}
		else
		{
			parse_tree->curr->child_nodes[s]->data->type = T_TERMINAL;	
		}
		node = list->next;
		free(list->token);
		free(list);
		list = node;
	}
	// display_parse_tree(parse_tree->root, 0);
	return 0;
}

/*
*	return values:-
*		0 => successfully moved to next child of the current node
*		1 => it is the last node of the current node
*/
int move_next_child(t_parse_tree *parse_tree)
{
	if(is_last_child(parse_tree))
	{
		return 1;
	}
	(parse_tree->curr->parent->current_child_ptr)++;
	parse_tree->curr = parse_tree->curr->parent->child_nodes[parse_tree->curr->parent->current_child_ptr];
	return 0;
}

/*
*	reutrn values:-
*		0 => its not the last child of the node
*		1 => it is the last child of the node
*/
int is_last_child(t_parse_tree *parse_tree)
{
	if(parse_tree->root == parse_tree->curr)
	{
		return 1;
	}
	if(parse_tree->curr->parent->child_nodes_count == parse_tree->curr->parent->current_child_ptr + 1)
	{
		return 1;
	}
	return 0;
}

/*
*	return values:-
*		0 => moved to parent
*		1 => no parent node i.e. it is the root
*/
int move_to_parent(t_parse_tree *parse_tree)
{
	if(parse_tree->curr == parse_tree->root)
	{
		return 1;
	}
	parse_tree->curr = parse_tree->curr->parent;
	return 0;
}

/*
*	return value:-
*		pointer to the parse tree
*		NULL => something went wrong
*/
t_parse_tree* create_parse_tree()
{
	t_parse_tree *parse_tree = (t_parse_tree *)malloc(sizeof(t_parse_tree));
	if(parse_tree == NULL)
	{
		/*insufficent memory error*/
		return NULL;
	}
	parse_tree->root = get_node();
	if(parse_tree->root == NULL)
	{
		free(parse_tree);
		return NULL;
	}
	parse_tree->curr = parse_tree->root;
	parse_tree->curr->data->token = strdup("<module>");
	parse_tree->curr->data->type = T_NON_TERMINAL;
	return parse_tree;
}

/*
*	return value:-
*		NULL => something went wrong
*		else returns a pointer to a node of parse tree
*/
t_parse_tree_node* get_node()
{
	t_parse_tree_node *node = (t_parse_tree_node *)malloc(sizeof(t_parse_tree_node));
	if(node == NULL)
	{
		/*insufficent memory error*/
		return NULL;
	}
	node->data = (t_parse_tree_data *)malloc(sizeof(t_parse_tree_data));
	if(node->data == NULL)
	{
		/*insufficent memory error*/
		return NULL;
	}
	node->data->token = NULL;
	node->data->type = 0;
	node->data->val.integer = 0;
	node->data->val.real = 0;
	node->data->val.character = (char)0;
	node->data->val.boolean = T_TRUE;
	node->data->val.str = NULL;

	node->parent = NULL;
	node->child_nodes = NULL;
	node->current_child_ptr = 0;
	node->child_nodes_count = 0;

	node->data->line_no = 0;
	node->data->lexem = NULL;
	node->data->symbol_table_pointer = NULL;
	return node;
}