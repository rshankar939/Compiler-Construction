/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
/*implementing stacks as linked list*/
#ifndef STACK_H
#define STACK_H
typedef struct token_linked_list_node {
	char *token;
	struct token_linked_list_node *next;
}token_linked_list_node;

typedef token_linked_list_node *token_linked_list;

typedef struct {
	int size;
	token_linked_list list;
}stack;

/*utility functions for the stack as particular to parser needs*/
extern token_linked_list top(stack *st);
extern void display_stack(stack *st);
extern stack* create_stack();//intializes it with $ and TK_MODULE
extern int push_rule(stack *st, char *rule_rhs);
extern int push(stack *st, char *str);
extern int pop(stack *st);
extern void free_stack(stack *st);
extern void free_token_linked_list(token_linked_list list);
#endif