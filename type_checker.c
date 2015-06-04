/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include "type_checker.h"
#include "parse_tree.h"
#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

#define MAX_DIM 100
#define MAX_ELE 1000
#define debug(A) printf("%s\n",A);fflush(stdout)
/*
*	resposible for processing an entire parse tree
*	return values:
*		0=> successfully completed
*		1=>	some error occured
*/

char *type_arr[13] = {"", "T_INT", "T_REAL", "T_CHAR", "T_BOOLEAN", "T_TUPLE", "T_STRING", "T_ARRAY", "T_IDENTIFIER", "T_FUNC_IDENTIFIER", "T_NON_TERMINAL", "T_TERMINAL", "T_EPSILON"};
static int main_count = 0;
static int main_line = 0;

int type_checker(t_parse_tree_node *pt, symbol_table **st)
{
	// t_parse_tree_node *parse_tree = pt->root;
	int i=0;
	int ret = process_node(pt, st);
	if(ret == 1)
		return 0;
	for(i=0;i<pt->child_nodes_count;i++)
	{
		type_checker(pt->child_nodes[i], st);
	}
	if(!ret)
		return ret;
	return ret;
}

/*
*	responsible for processing a node
*	return values:
*		0=> successfully completed
*		1=>	skip the rest of the tree
*		2=> some error occured
*/
int process_node(t_parse_tree_node *parse_tree, symbol_table **st)
{
	if(strcmp(parse_tree->data->token, "<globalDeclarationStmt>") == 0 ||
		strcmp(parse_tree->data->token, "<declarationStmt>") == 0)
	{
		int ret = handle_declaration(parse_tree, st);
		if(!ret)
			return 1;
	}
	else if(strcmp(parse_tree->data->token, "TK_ID") == 0)
	{
		int ret = handle_tk_id(parse_tree, st);
	}
	else if(strcmp(parse_tree->data->token, "<derivedTypes>") == 0)
	{
		int ret = handle_derived_types(parse_tree, st);
	}
	else if(strcmp(parse_tree->data->token, "<functionDef>") == 0)
	{
		int ret = handle_function_declaration(parse_tree, st);
	}
	else if(strcmp(parse_tree->data->token, "<functionCallStmt>") == 0)
	{
		int ret = handle_function_call(parse_tree, st);
		return 0;
	}
	else if(strcmp(parse_tree->data->token, "<main>") == 0)
	{
		main_count++;
		if(main_count == 1)
			main_line = parse_tree->child_nodes[0]->data->line_no;
		if(main_count > 1)
		{
			printf("Error: Multiple occurrences of main function on line %d, previous usage on line number %d\n", parse_tree->child_nodes[0]->data->line_no, main_line);
		}
	}
	else if(strcmp(parse_tree->data->token, "<assignmentStmt>") == 0)
	{
		int ret = handle_assignment_statement(parse_tree, st);
	}
	//else if()assignStmtSingle
	return 0;
}

/*
*	return values
*		0=> no error
*		else type mismatch error
*/
int handle_arith_expression(t_parse_tree_node *pt, symbol_table **st, int type)
{
	//debug("here");
	// debug(pt->data->token);
	int i=0, f=0;
	for(i=0;i<pt->child_nodes_count;i++)
	{
		int ret = handle_arith_expression(pt->child_nodes[i], st, type);
		if(ret)
		{
			// debug("here1");
			return 1;
		}
	}
	/*process current node*/
	if(strcmp(pt->data->token,"TK_ID") == 0)
	{
		symbol_list sym = exist_in_scope(pt->data->lexem, pt->data->scope,*st);
		if(type != sym->type)
			return 1;		
	}
	else if(strcmp(pt->data->token, "TK_CCONST") == 0)
	{
		if(type!=3)
			return 1;
	}
	else if(strcmp(pt->data->token, "TK_NUM") == 0)
	{
		if(type!=1)
			return 1;
	}
	else if(strcmp(pt->data->token, "TK_RNUM") == 0)
	{
		if(type!=2)
			return 1;
	}
	else if(strcmp(pt->data->token, "TK_BCONST") == 0)
	{
		if(type!=4)
			return 1;
	}
	else if(strcmp(pt->data->token, "TK_STR") == 0)
	{
		if(type!=6)
			return 1;
	}

	return 0;
}

/*
*	return values
*		0=> no error
*		else type mismatch error
*/
int handle_assignment_single_stmt(t_parse_tree_node *pt, symbol_table **st)
{
	symbol_list sym = exist_in_scope(pt->child_nodes[0]->data->lexem, pt->child_nodes[0]->data->scope,*st);
	int flag=0;
	int target_type = sym->type, rec_type;
	t_parse_tree_node *t = pt->child_nodes[3]->child_nodes[0];
	if(strcmp(t->data->token, "<size>")==0)
	{
		flag=1;
		rec_type = T_INT;
	}
	else if(strcmp(t->data->token, "<arithExpr>")==0)
	{
		rec_type = handle_arith_expression(t, st, target_type);

		if(rec_type)
		{
			printf("Error: Type mismatch error on line number %d\n", pt->child_nodes[0]->data->line_no);
			return 1;
		}
	}
	// if(!flag && rec_type != target_type)
	if(rec_type != target_type && !flag && rec_type == T_INT)
	{
		printf("Error: Type Mismatch on line no %d\n", pt->child_nodes[0]->data->line_no);
		return 2;
	}
	return 0;
}

int handle_assignment_statement(t_parse_tree_node *pt, symbol_table **st)
{
	if(strcmp(pt->child_nodes[0]->data->token, "<assignStmtSingle>") == 0){
		return handle_assignment_single_stmt(pt->child_nodes[0], st);
	}
	return 0;
}

/*
*	handles all the element access
*	return value
*		0=> 
*		1=> error occurred
*/
int handle_derived_types(t_parse_tree_node *pt, symbol_table **st)
{
	if(strcmp(pt->child_nodes[0]->data->token, "EPSILON") == 0)
		return 0;
	if(!(pt->child_nodes[0]->child_nodes[1]->child_nodes[0]->data->type==T_INT
		||!strcmp(pt->child_nodes[0]->child_nodes[1]->child_nodes[0]->data->token, "TK_INT")==0))
	{
		return 1;
	}
	return 0;
}

/*
*	handles id token
*	return values
*		0=> access is semantically correct, added symbol table entry
*		1=> improper usage of derived type attribute
*/
int handle_tk_id(t_parse_tree_node *pt, symbol_table **st)
{
	int flag=0;
	if(strcmp(pt->parent->data->token,"<paramList>") == 0)
	{
		t_parse_tree_node *t = pt->parent;
		while(1)
		{
			if(strcmp(t->parent->data->token, "<restParams>") == 0)
			{
				t = t->parent->parent;
			}
			else
				break;
		}
		t = t->parent->parent;
		t = t->child_nodes[1]->child_nodes[0];
		pt->data->scope = t->data->scope;
	}
	symbol_list sys = exist_in_scope(pt->data->lexem, pt->data->scope, *st);
	if(sys==NULL)
	{
		printf("Error: Use of undeclared variable%s on line number %d\n",pt->data->lexem,pt->data->line_no);
	}
	t_parse_tree_node *t = pt->parent->child_nodes[1];
	if(pt->data->type == T_ARRAY)
	{
		if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_LC"))
		{
			printf("Error: expected '(' but found %s at line number %d\n", t->child_nodes[0]->child_nodes[0]->data->token, t->child_nodes[0]->child_nodes[0]->data->line_no);
			return 1;
		}
	}
	else if(pt->data->type == T_STRING)
	{
		if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_LS"))
		{
			printf("Error: expected '[' but found %s at line number %d\n", t->child_nodes[0]->child_nodes[0]->data->token, t->child_nodes[0]->child_nodes[0]->data->line_no);
			return 1;
		}
	}
	else if(pt->data->type == T_TUPLE)
	{
		if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_COLON"))
		{
			printf("Error: expected ':' but found %s at line number %d\n", t->child_nodes[0]->child_nodes[0]->data->token, t->child_nodes[0]->child_nodes[0]->data->line_no);
			return 1;
		}
	}
	return 0;
}

/*
*	handles function call statement
*	return values
*		0=> all the input parameter type matched, function is defined before being used
*		1=> undefined function
*		2=> passing undefined variable
*		3=> 
*
*/
int handle_function_call(t_parse_tree_node *pt, symbol_table **st)
{
	int flag=0;
	int p_count=0, type=0;
	int *p_list = (int*)malloc(MAX_DIM*sizeof(int));
	t_parse_tree_node *t = pt;

	/*check if function is declared*/
	symbol_list sym_funcid = exist_in_scope(t->child_nodes[0]->data->lexem, t->child_nodes[0]->data->scope,*st);
	if(sym_funcid == NULL)
	{
		printf("Error: call to an undefined function %s on line number %d\n",t->child_nodes[0]->data->lexem,t->child_nodes[0]->data->line_no);
		flag = 1;
		return flag;
	}

	symbol_list sym;
	/*extract type of all the parameters and check if they are declared*/
	t = t->child_nodes[2];
	while(1)
	{
		int type = 0;
		// printf("%s\n",temp->child_nodes[0]->child_nodes[0]->data->token);
		// fflush(stdout);
		if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_NUM") == 0)
			type = T_INT;
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_RNUM") == 0)
			type = T_REAL;
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_BCONST") == 0)
			type = T_BOOLEAN;
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_CCONST") == 0)
			type = T_CHAR;
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_STR") == 0)
			type = T_STRING;
		
		if(!type)
		{
			sym = exist_in_scope(t->child_nodes[0]->child_nodes[0]->data->lexem, t->child_nodes[0]->child_nodes[0]->data->scope,*st);
			fflush(stdout);
			if(sym == NULL)
			{
				printf("Error: passing an undefined variable %s to function in line number %d\n", t->child_nodes[0]->child_nodes[0]->data->lexem, t->child_nodes[0]->child_nodes[0]->data->line_no);
				flag = 2;
			}
			else
			{
				type = sym->type;
				if(!type)
				{
					printf("Error: on line number %d, only \"int\", \"real\", \"bool\", \"char\", \"string\" are allowed in a function call\n", t->child_nodes[0]->child_nodes[0]->data->line_no);
				 	flag = 3;
				}
			}
		}

		if((!type) || (sym!=NULL)){
			p_list[p_count++] = type;
		}

		if(strcmp(t->child_nodes[1]->child_nodes[0]->data->token, "EPSILON") == 0)
			break;

		t = t->child_nodes[1]->child_nodes[1];
	}
	
	p_list = (int*)realloc(p_list,sizeof(int)*p_count);

	/*check if types match to the function definition*/
	int *p_list_func_decl = sym_funcid->info->function_declration_info.parameter_type_list;
	int p_count_decl = sym_funcid->info->function_declration_info.parameter_count;

	if(p_count!=p_count_decl){
		printf("Error: on line number %d, Found %d parameters, expected %d parameters in function call\n", pt->data->line_no, p_count, p_count_decl);
		printf("\tFunction defined on line number %d\n", sym_funcid->line_declared);
		flag=4;
	}
	else
	{
		int i;
		for(i=0;i<p_count;i++){
			if(p_list[i]!=p_list_func_decl[i])
			{
				printf("Error: on line number %d, parameter number %d, expected type %s, given %s\n", pt->child_nodes[0]->data->line_no, i+1, type_arr[p_list_func_decl[i]],type_arr[p_list[i]]);
				flag=5;
			}
		}
	}
	return flag;
}

/*
*	handle function declaration
*	return values
*		0=> all the input parameters and function added to the symbol table
*		1=> error redeclaration of function id
*		2=> error redeclaration in parameter list
*		3=> return type not allowed or undefined
*	Logic:-
*
*/
int handle_function_declaration(t_parse_tree_node *pt, symbol_table **st)
{
	int flag=0;
	int scope = pt->child_nodes[1]->child_nodes[0]->data->scope;
	int p_count=0, r_count=0, type=0;
	int *p_list = (int*)malloc(MAX_DIM*sizeof(int));
	int *r_list = (int*)malloc(MAX_DIM*sizeof(int));

	t_parse_tree_node *t = pt;
	int ret = insert_into_symbol_table(*st, t->child_nodes[0]->child_nodes[1]->data->lexem, t->child_nodes[0]->child_nodes[1]->data->scope, t->child_nodes[0]->child_nodes[1]->data->line_no);
	if(ret == 1)
	{
		printf("Error: redeclaration of function %s on line number %d\n",t->child_nodes[0]->child_nodes[1]->data->lexem, t->child_nodes[0]->child_nodes[1]->data->line_no);
		return 1;
	}
	t->child_nodes[0]->child_nodes[1]->data->symbol_table_pointer = lookup_symbol_table(*st, t->child_nodes[0]->child_nodes[1]->data->lexem, t->child_nodes[0]->child_nodes[1]->data->scope);
	symbol *sym = t->child_nodes[0]->child_nodes[1]->data->symbol_table_pointer;
	sym->line_declared = t->child_nodes[0]->child_nodes[1]->data->line_no;
	sym->type = T_FUNC_IDENTIFIER;
	/*insert into parameter list and declare them*/
	t = t->child_nodes[0]->child_nodes[3];
	t_parse_tree_node *insert = t->child_nodes[1];
	while(1)
	{
		// printf("%s\n",temp->child_nodes[0]->child_nodes[0]->data->token);
		// fflush(stdout);
		if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_INT") == 0)
		{
			type = T_INT;
		}
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_REAL") == 0)
		{
			type = T_REAL;
		}
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_CHAR") == 0)
		{
			type = T_CHAR;
		}
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_BOOL") == 0)
		{
			type = T_BOOLEAN;
		}
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_STRING") == 0)
		{
			type = T_STRING;
		}
		else
		{
			printf("Error: on line number %d, only \"int\", \"real\", \"bool\", \"char\", \"string\" are allowed in a function declaration\n", t->child_nodes[0]->child_nodes[0]->data->line_no);
			flag = 2;
		}
		int ret = insert_into_symbol_table(*st, insert->data->lexem, scope, insert->data->line_no);
		if(ret == 1)
		{
			printf("Error: redeclaration of %s on line number %d\n",insert->data->lexem, insert->data->line_no);
			flag = 2;
		}
		p_list[p_count++] = type;
		/*insert rest variables of symbol table*/
		t->child_nodes[1]->data->symbol_table_pointer = lookup_symbol_table(*st, t->child_nodes[1]->data->lexem, scope);
		symbol *sym_in = t->child_nodes[1]->data->symbol_table_pointer;

		sym_in->type = type;
		sym_in->line_declared = insert->data->line_no;
		if(type == T_INT)
			sym_in->size = 4;
		else if(type == T_REAL)
			sym_in->size = 4;
		else if(type == T_CHAR)
			sym_in->size = 1;
		else if(type == T_BOOLEAN)
			sym_in->size = 1;
		if(strcmp(t->child_nodes[2]->child_nodes[0]->data->token, "EPSILON") == 0)
			break;
		t = t->child_nodes[2]->child_nodes[1];
		insert = t->child_nodes[1];
	}

	/*insert into return list*/
	t = pt->child_nodes[0]->child_nodes[7];
	while(1)
	{
		// printf("%s\n",temp->child_nodes[0]->child_nodes[0]->data->token);
		// fflush(stdout);
		if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_INT") == 0)
		{
			type = T_INT;
		}
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_REAL") == 0)
		{
			type = T_REAL;
		}
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_CHAR") == 0)
		{
			type = T_CHAR;
		}
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_BOOL") == 0)
		{
			type = T_BOOLEAN;
		}
		else
		{
			printf("Error: on line number %d, only \"int\", \"real\", \"bool\", \"char\" are allowed in a function declaration\n", t->child_nodes[0]->child_nodes[0]->data->line_no);
			flag = 3;
		}
		r_list[r_count++] = type;
		
		if(strcmp(t->child_nodes[1]->child_nodes[0]->data->token, "EPSILON") == 0)
			break;

		t = t->child_nodes[1]->child_nodes[1];
	}

	/*ensure that the function has at least one return statement*/
	t = pt->child_nodes[1]->child_nodes[1];
	int r_flag=0,rec_flag=0;
	while(1)
	{
		if(strcmp(t->child_nodes[0]->data->token, "EPSILON") == 0)
			break;
		else if(strcmp(t->child_nodes[0]->data->token, "<branchStmt>") == 0)
		{
			if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_RETURN") == 0)
			{
				r_flag = 1;
				break;
			}
		}
		else if(strcmp(t->child_nodes[0]->data->token, "<functionCallStmt>") == 0)
		{
			rec_flag = 1;
			break;
		}
		// else if(strcmp(t->child_nodes[0]->data->token, "<assignmentStmt>") == 0)
		// {
		// 	t_parse_tree_node *th = t->child_nodes[0];
		// 	if(strcmp(th->data->token, "<assignStmtList>") == 0)
		// 	{
		// 		if(strcmp(th->child_nodes[4]->child_nodes[0]->data->token, "<functionCallStmt") == 0)
		// 		{
		// 			th = th->child_nodes[4]->child_nodes[0];
		// 			if(strcmp(th->data->lexem, sym->identifier) == 0)
		// 				rec_flag = 1;
		// 		}
		// 	}
		// 	else if(strcmp(th->child_nodes[3]->child_nodes[0]->data->token, "<functionCallStmt") == 0)
		// 	{
		// 		th = th->child_nodes[3]->child_nodes[0];
		// 		if(strcmp(th->data->lexem, sym->identifier) == 0)
		// 			rec_flag = 1;
		// 	}
		// 	if(rec_flag)
		// 		break;
		// }
		t=t->child_nodes[1];
	}
	if(!r_flag)
	{
		printf("Error: no return statement in function %s, defined on line number %d\n", sym->identifier,sym->line_declared );
	}
	if(rec_flag)
	{
		printf("Error: recursion not allowed, used in %s function defined on line number %d\n", sym->identifier,sym->line_declared);
	}
	/*update sym for function_declaration*/
	p_list = (int*)realloc(p_list,sizeof(int)*p_count);
	r_list = (int*)realloc(r_list,sizeof(int)*r_count);
	sym->info = (t_info *)malloc(sizeof(t_info));
	sym->info->function_declration_info.parameter_count = p_count;
	sym->info->function_declration_info.parameter_type_list = p_list;
	sym->info->function_declration_info.return_count = r_count;
	sym->info->function_declration_info.return_type_list = r_list;

	return flag;
}

/*
*	handle tuple declaration
*	return values
*		0=> all the variables added to the symbol table
*		1=> error redeclaration of id
*		2=> type not allowed or undefined
*	Logic:-
*	We start by extracting the TK_ID of the tuple
*	Then we move to <typeList> and for termination check whether it is followed by EPSILON recursively
*
*/
int handle_tuple_dec(t_parse_tree_node *pt, symbol_table **st)
{
	int count=0,offset=0,flag;
	int *type_list;
	int *start_address_list = (int *)malloc(MAX_ELE *sizeof(int));
	type_list = (int*)malloc(MAX_ELE * sizeof(int));
	t_parse_tree_node *t = pt;

	int ret = insert_into_symbol_table(*st, t->child_nodes[2]->data->lexem, t->child_nodes[2]->data->scope, t->child_nodes[2]->data->line_no);
	if(ret == 1)
	{
		printf("Error: redeclaration of %s on line number %d\n",t->child_nodes[2]->data->lexem, t->child_nodes[2]->data->line_no);
		return 1;
	}
	t->child_nodes[2]->data->symbol_table_pointer = lookup_symbol_table(*st, t->child_nodes[2]->data->lexem, t->child_nodes[2]->data->scope);
	symbol *sym = t->child_nodes[2]->data->symbol_table_pointer;
	sym->line_declared = t->child_nodes[2]->data->line_no;
	sym->size = 0;

	t = t->child_nodes[1]->child_nodes[1];
	while(1)
	{
		start_address_list[count] = offset;
		if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_INT") == 0)
		{
			type_list[count] = T_INT;
			offset += 4;
		}
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_REAL") == 0)
		{
			type_list[count] = T_REAL;
			offset += 4;
		}
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_CHAR") == 0)
		{
			type_list[count] = T_CHAR;
			offset += 1;
		}
		else if(strcmp(t->child_nodes[0]->child_nodes[0]->data->token, "TK_BOOL") == 0)
		{
			type_list[count] = T_BOOLEAN;
			offset += 1;
		}
		else
		{
			printf("Error: on line number %d, only \"int\", \"real\", \"bool\", \"char\" are allowed in a tuple\n", t->child_nodes[0]->child_nodes[0]->data->line_no);
			return 2;
		}
		count++;
		if(strcmp(t->child_nodes[1]->child_nodes[0]->data->token, "EPSILON")==0)
		{
			/*insert into the symbol table*/
			start_address_list = (int*)realloc(start_address_list, count*sizeof(int));
			type_list = (int*)realloc(type_list, count*sizeof(int));
			sym->size = offset;
			sym->type = T_TUPLE;
			sym->info = (t_info *)malloc(sizeof(t_info));
			sym->info->tuple_info.num_element = count;
			sym->info->tuple_info.type_list = type_list;
			sym->info->tuple_info.start_address_list = start_address_list;
			break;
		}
		t = t->child_nodes[1]->child_nodes[1];
	}
	return 0;
}

/*
*	add all the declarations
*	return value:
*		0=> all the variables are successfully add to the symbol table
*		1=> redeclaration of the variables
*		2=> array element declaration was not corrects
*	Logic:-
*	After Extracting type and first TK_ID, we move to <varListFollow>
*	Then we handle case where it is array or another TK_ID recursively
*/
int handle_add_all(t_parse_tree_node *pt, symbol_table **st, int type)
{
	int flag=0, n=0;
	/*insert pt->child_nodes[0] into st*/
	t_parse_tree_node *insert = pt->child_nodes[0];
	//insert_into_symbol_table(*st, lex->lexem_str, curr_scope, lex->lexem_line);
	t_parse_tree_node *temp = pt->child_nodes[1];
	// printf("%s\n", temp->data->token);
	// fflush(stdout);
	while(1)
	{
		int ret = insert_into_symbol_table(*st, insert->data->lexem, insert->data->scope, insert->data->line_no);
		// printf("%s %s\n",insert->data->lexem,insert->data->token);
		// fflush(stdout);
	
		if(ret == 1)
		{
			printf("Error: redeclaration of %s on line number %d\n",insert->data->lexem, insert->data->line_no);
			flag = 1;
		}
		insert->data->symbol_table_pointer = lookup_symbol_table(*st, insert->data->lexem, insert->data->scope);
		symbol *sym = insert->data->symbol_table_pointer;
		if(strcmp(temp->child_nodes[0]->data->token, "<array>") == 0)
		{
			/*handle array declaration*/
			int *arr_dim = (int *)malloc(sizeof(int)*MAX_DIM);
			int count = 0;
			t_parse_tree_node *t = temp->child_nodes[0];
			while(strcmp(t->child_nodes[0]->data->token,"EPSILON") != 0)
			{
				if(strcmp(t->child_nodes[1]->child_nodes[0]->data->token, "TK_NUM") == 0)
				{
					arr_dim[count] = t->child_nodes[1]->child_nodes[0]->data->val.integer;
					count++;
				}
				else
				{
					flag = 2;
					printf("Error: expected type Integer Constant(TK_NUM) but got %s\n", t->child_nodes[1]->child_nodes[0]->data->token);
				}
				t = t->child_nodes[2];
			}
			if(!flag)
			{
				arr_dim = (int *)realloc(arr_dim,sizeof(int)*count);
			}
			sym->type = T_ARRAY;
			sym->line_declared = insert->data->line_no;
			if(type == T_INT)
				sym->size = 4*count;
			else if(type == T_REAL)
				sym->size = 4*count;
			else if(type == T_CHAR)
				sym->size = 1*count;
			else if(type == T_BOOLEAN)
				sym->size = 1*count;

			sym->info = (t_info *)malloc(sizeof(t_info));
			sym->info->array_info.type = type;
			sym->info->array_info.dimension_count = count;
			sym->info->array_info.dimension_max_limit = arr_dim;
			n=1;
		}
		else
		{
			/*handle rest*/
			sym->type = type;
			sym->line_declared = insert->data->line_no;
			if(type == T_INT)
				sym->size = 4;
			else if(type == T_REAL)
				sym->size = 4;
			else if(type == T_CHAR)
				sym->size = 1;
			else if(type == T_BOOLEAN)
				sym->size = 1;
			n=0;
		}
		// printf("%s %d\n",temp->data->token, temp);
		if(strcmp(temp->child_nodes[n]->child_nodes[0]->data->token, "EPSILON") == 0)
			break;
		if(strcmp(temp->child_nodes[0]->data->token, "<array>") == 0)
		{
			insert = temp->child_nodes[1]->child_nodes[1]->child_nodes[0];
			temp = temp->child_nodes[1]->child_nodes[1]->child_nodes[1];
		}
		else
		{
			insert = temp->child_nodes[0]->child_nodes[1]->child_nodes[0];
			temp = temp->child_nodes[0]->child_nodes[1]->child_nodes[1];
		}
		// display_symbol_table(*st, stdout);
	}
	// display_symbol_table(*st, stdout);
	// printf("\n\n\n");
	return flag;
}

/*
*	responsible for handling declaration nodes
*/
int handle_declaration(t_parse_tree_node *pt, symbol_table **st)
{
	int type = 0;
	int ret = 0;
	if(strcmp(pt->child_nodes[0]->data->token, "TK_TUPLE") == 0)
	{
		type = T_TUPLE;
		// printf("here %s %d\n", pt->child_nodes[0]->data->token, pt->child_nodes[0]->data->line_no);
		// fflush(stdout);
		// handle_tuple_dec(pt->child_nodes[1]->child_nodes[1], st);
		if(handle_tuple_dec(pt, st))
			ret = 1;
	}
	else if(strcmp(pt->child_nodes[0]->child_nodes[0]->data->token, "TK_INT") == 0)
	{
		type = T_INT;
	}
	else if(strcmp(pt->child_nodes[0]->child_nodes[0]->data->token, "TK_REAL") == 0)
	{
		type = T_REAL;
	}
	else if(strcmp(pt->child_nodes[0]->child_nodes[0]->data->token, "TK_CHAR") == 0)
	{
		type = T_CHAR;
	}
	else if(strcmp(pt->child_nodes[0]->child_nodes[0]->data->token, "TK_BOOL") == 0)
	{
		type = T_BOOLEAN;
	}
	else if(strcmp(pt->child_nodes[0]->child_nodes[0]->data->token, "TK_STRING") == 0)
	{
		type = T_STRING;
	}

	if(type != T_TUPLE)
	{
		if(handle_add_all(pt->child_nodes[1], st, type))
			ret = 1;
	}
	return 0;
}