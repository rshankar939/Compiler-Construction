/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./deco_ast.h"
#include "./parse_tree.h"
#define debug(A) printf("%s\n",A);fflush(stdout)
#define debug2(A) printf("%d\n",A);fflush(stdout)
/*
*	deletes node and make corresponding changes in the parent
*
*/
int delete_node(t_parse_tree_node* parse_tree)
{
	int count = 0, i, j, temp;
	t_parse_tree_node *parent = parse_tree->parent;
	int child_count = parent->child_nodes_count+parse_tree->child_nodes_count-1;
	parent->child_nodes = (t_parse_tree_node **)realloc(parent->child_nodes, child_count*sizeof(t_parse_tree_node *));
	for(i=0;i<parent->child_nodes_count;i++)
	{
		if(parent->child_nodes[i] == parse_tree)
		{
			temp = i;
			i++;
			for(j=child_count-i+1; i<parent->child_nodes_count;i++,j++)
			{
				parent->child_nodes[j] = parent->child_nodes[i];
			}
			for(j=0,i=temp;j<parse_tree->child_nodes_count;j++,i++)
			{
				parent->child_nodes[i] = parse_tree->child_nodes[j];
			}
			parent->child_nodes_count = child_count;
			break;
		}
	}
	// parse_tree->child_nodes_count = 0;
	// parse_tree->parent = NULL;
	//free(parent->child_nodes);
	//parent->child_nodes = NULL;
	// helper_free_parse_tree(parse_tree);
	parse_tree = NULL;
	return 0;
}
/*
*	the current node replaces the parent node
*	also it frees the parent node
*/
int move_node_up(t_parse_tree_node* parse_tree)
{
	int count = 0, i, j, k;
	t_parse_tree_node *parent = parse_tree->parent;
	t_parse_tree_node **child_nodes = (t_parse_tree_node **)malloc((parent->child_nodes_count+parse_tree->child_nodes_count-1)*sizeof(t_parse_tree_node *));
	/*move all the children nodes from parent to the node*/
	for(i=0,j=0;j<parent->child_nodes_count;j++)
	{
		if(parent->child_nodes[j] == parse_tree)
		{
			for(k=0;k<parse_tree->child_nodes_count;k++,i++)
			{
				child_nodes[i] = parse_tree->child_nodes[k];
			}
			continue;
		}
		child_nodes[i] = parent->child_nodes[j];
		i++;
	}
	/*move information form parent to node*/
	parse_tree->parent = parent->parent;
	parse_tree->child_nodes_count += parent->child_nodes_count;
	(parse_tree->child_nodes_count)--;
	free(parse_tree->child_nodes);
	parse_tree->child_nodes = child_nodes;
	/*free parent*/
	parent->child_nodes_count = 0;
	parent->parent = NULL;
	free(parent->child_nodes);
	// helper_free_parse_tree(parent);
	// debug("here7");
	return 0;
}

/*
*	uses in-order traversal or reduction
*	return values
*		0 => no error
*		1=> node deleted
*		else some error occurred
*/
int get_decorated_AST_helper(t_parse_tree_node *parse_tree)
{
	int i=0,ret,count=0,j;
	t_parse_tree_node *parent = parse_tree->parent;
	ret = process_node_ast(parse_tree);
	if(ret == 1)
		return 1;
	for(i=0;i<parse_tree->child_nodes_count;)
	{
		ret = get_decorated_AST_helper(parse_tree->child_nodes[i]);
	}
	return 0;
}

/*
*	return a pointer to DECORATED AST
*		it reduces the given parse tree into an abstract syntax tree
*		NULL is returned in case of any errors in conversion
*/
t_parse_tree* get_decorated_ast(t_parse_tree *parse_tree)
{
	t_parse_tree_node *root = parse_tree->root;
	delete_all_declaration_stmt(parse_tree->root);
	// delete_all_stmts(parse_tree->root);
	return parse_tree;

	if(get_decorated_AST_helper(root))
	{
		return NULL;
	}
	parse_tree->curr = parse_tree->root;
	return parse_tree;
}

/*
*	return value
*		0=> processing complete
*		1=> processing complete, node deleted
*/
int process_node_ast(t_parse_tree_node *parse_tree)
{
	int flag = 0;
	// if(strcmp(parse_tree->data->token, "<stmts>") == 0)
	// {
	// 	handle_stmts(parse_tree);
	// 	flag = 1;
	// }
	// else if(strcmp(parse_tree->data->token, "<assignmentStmt>") == 0)
	// {
	// 	handle_assignment_stmt(parse_tree);
	// }
	return flag;
}

int handle_stmts(t_parse_tree_node *parse_tree)
{
	// move_node_up(parse_tree->child_nodes[0]);
	delete_node(parse_tree);
	return 0;
}

int handle_assignment_single(t_parse_tree_node *parse_tree)
{
	return 0;
}

int handle_assignment_multiple(t_parse_tree_node *parse_tree)
{
	return 0;
}

int handle_assignment_stmt(t_parse_tree_node *parse_tree)
{
	if(strcmp(parse_tree->child_nodes[0]->data->token, "<assignStmtList>") == 0)
	{
		handle_assignment_single(parse_tree);
	}
	else if(strcmp(parse_tree->child_nodes[0]->data->token, "<assignStmtSingle>") == 0)
	{
		handle_assignment_multiple(parse_tree);
	}
	return 0;
}

int handle_declaration_stmt(t_parse_tree_node *parse_tree)
{
	// delete_node(parse_tree);
	helper_free_parse_tree(parse_tree);
	parse_tree = NULL;

	return 0;
}

int delete_all_declaration_stmt(t_parse_tree_node *parse_tree)
{
	int ret,count=0,i;
	if(strcmp(parse_tree->data->token, "<globalDeclarationStmt>")==0 || strcmp(parse_tree->data->token, "<declarationStmt>")==0)
	{
		// debug(parse_tree->data->token);
		handle_declaration_stmt(parse_tree);
		return 1;
	}
	t_parse_tree_node *parent = parse_tree->parent;

	for(i=0;i<parse_tree->child_nodes_count; i++)
	{
		ret = delete_all_declaration_stmt(parse_tree->child_nodes[i]);
		if(ret == 1)
		{
			parse_tree->child_nodes[i] = NULL;
			count++;
		}
	}
	if(count > 0)
	{
		int fc = 0;
		for(i=0;i<parse_tree->child_nodes_count;i++)
		{
			if(parse_tree->child_nodes[i] == NULL)
			{
				fc++;
				continue;
			}
			parse_tree->child_nodes[i-fc] = parse_tree->child_nodes[i];
		}
		if(fc)
		{
			parse_tree->child_nodes_count = parse_tree->child_nodes_count -fc;
			parse_tree->child_nodes = (t_parse_tree_node **)realloc(parse_tree->child_nodes,parse_tree->child_nodes_count*sizeof(t_parse_tree_node *));
		}
	}
	return 0;
}

// int delete_all_stmts(t_parse_tree_node *parse_tree)
// {
// 	int ret,count=0,i;
// 	if(strcmp(parse_tree->data->token, "<stmts>")==0)
// 	{
// 		// debug(parse_tree->data->token);
// 		handle_declaration_stmt(parse_tree);
// 		return 1;
// 	}	
// }