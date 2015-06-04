/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void free_token_linked_list(token_linked_list list)
{
	token_linked_list temp;
	while(list != NULL)
	{
		temp = list->next;
		free(list->token);
		free(list);
		list = temp;
	}
	return;
}

token_linked_list top(stack *st)
{
	if(st->size == 0 || st->list == NULL)
	{
		return NULL;
	}
	fflush(stdout);	
	token_linked_list node;
	node = (token_linked_list)malloc(sizeof(token_linked_list_node));
	if(node == NULL)
	{
		printf("insufficient memory.\n");
		return NULL;
	}
	node->token = strdup(st->list->token);
	node->next = NULL;
	// printf("\n%s ",node->token);
	return node;
}

void display_stack(stack *st)
{
	token_linked_list node = st->list;
	printf("size of the stack is %d\n", st->size);
	while(node!=NULL)
	{
		printf("%s\n",node->token);
		node = node->next;
	}
	return;
}


/*push the rule in reverse order*/
int push_rule(stack *st, char *rule_rhs)
{
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
	while(list != NULL)
	{
		push(st, list->token);
		node = list->next;
		free(list->token);
		free(list);
		list = node;
	}
	return 0;
}

/*intializes it with $ and <module>*/
stack* create_stack()
{
	stack *st = (stack *)malloc(sizeof(stack));
	if(st == NULL)
	{
		printf("Unable to allocate memory for the stack.\n");
		return NULL;
	}
	st->size = 0;
	st->list = NULL;
	if(push(st, (char *)"$"))
	{
		free(st);
		return NULL;
	}
	if(push(st, (char *)"<module>"))
	{
		free(st);
		return NULL;
	}
	return st;
}

int push(stack *st, char *str)
{
	token_linked_list node;
	node = (token_linked_list)malloc(sizeof(token_linked_list_node));
	if(node == NULL)
	{
		printf("Insufficient memory.\n");
		return 1;
	}
	node->token = strdup(str);
	node->next = st->list;
	st->list = node;
	(st->size)++;
	return 0;
}

int pop(stack *st)
{
	/*no element to remove*/
	if(st == NULL)
		return 1;
	token_linked_list node = st->list;
	st->list = st->list->next;
	free(node);
	(st->size)--;
	return 0;
}

void free_stack(stack *st)
{
	token_linked_list temp;
	while(st->list != NULL)
	{
		temp = st->list->next;
		free(st->list->token);
		free(st->list);
		st->list = temp;
	}
	free(st);
	return;
}
