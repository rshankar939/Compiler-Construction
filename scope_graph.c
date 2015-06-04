/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>

/*starts a new scope as a child to the current scope*/
int start_new_scope(scope_graph *sg, int scope_id)
{
	int i;
	if(sg->curr->child_count == sg->curr->max_child_scope_count)
	{
		sg->curr->max_child_scope_count += 5;
		sg->curr->child_scopes = (scope_graph_node **)realloc(sg->curr->child_scopes, sg->curr->max_child_scope_count*sizeof(scope_graph_node *));
		for(i=0;i<5;i++)
		{
			sg->curr->child_scopes[sg->curr->child_count+i] = NULL;
		}
	}
	scope_graph_node *node = get_scope_graph_node();
	node->scope = scope_id;
	node->parent_scope = sg->curr;
	sg->curr->child_scopes[sg->curr->child_count] = node;
	(sg->curr->child_count)++;
	sg->curr = sg->curr->child_scopes[sg->curr->child_count-1];
	return 0;
}

/*ends a scope and move the current scope to the parent scope*/
int end_scope(scope_graph *sg)
{
	if(sg->curr == sg->root)
	{
		return 1;
	}
	sg->curr = sg->curr->parent_scope;
	// display_scope_graph(sg->root, 0 , stdout);
	// printf("\n\n");
	return 0;
}


/*helper function for free scope graph*/
int helper_free_scope_graph(scope_graph_node *sg)
{
	int i;
	for(i=0;i<sg->child_count;i++)
	{
		if(sg->child_scopes[i] != NULL)
			helper_free_scope_graph(sg->child_scopes[i]);
	}
	// free(sg);
	return 0;
}
/*deletes the scope graph*/
int free_scope_graph(scope_graph *sg)
{
	helper_free_scope_graph(sg->root);
	return 0;
}

/*returns a scope graph with only one value defaulted to zero*/
scope_graph* create_scope_graph()
{
	scope_graph* sg = (scope_graph*)malloc(sizeof(scope_graph));
	if(sg == NULL)
	{
		printf("insufficent memory of scope dependency graph\n");
		return NULL;
	}
	sg->root=get_scope_graph_node();
	if(sg->root == NULL)
	{
		printf("insufficent memory of scope dependency graph\n");
		free(sg);
		return NULL;	
	}
	sg->curr = sg->root;
	return sg;
}

/*get a default scope_graph_node*/
scope_graph_node* get_scope_graph_node()
{
	scope_graph_node *node = (scope_graph_node*)malloc(sizeof(scope_graph_node));
	if(node == NULL)
		return NULL;
	node->scope = 0;
	node->child_count = 0;
	node->max_child_scope_count = 2;
	node->parent_scope = NULL;
	node->child_scopes = (scope_graph_node **)malloc(node->max_child_scope_count*sizeof(scope_graph_node *));
	node->child_scopes[0] = NULL;
	node->child_scopes[1] = NULL;
	return node;
}

/*displays scope graph*/
int display_scope_graph(scope_graph_node* root, int level, FILE *fp)
{
	int i=0;
	for(i=0;i<level;i++)
		fprintf(fp,"\t");
	fprintf(fp,"%d\n", root->scope);
	for(i=0;i<root->child_count;i++)
	{
		if(root->child_scopes[i] != NULL)
		display_scope_graph(root->child_scopes[i], level+1, fp);
	}
	return 0;
}
