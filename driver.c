/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include <stdio.h>
#include <stdlib.h>
#include "./lexer_helper.h"
#include "./lexer.h"
#include "./parse_tree.h"
#include "./parser_helper.h"
#include "./type_checker.h"
#include "./deco_ast.h"

#define show_pt 1
#define show_ast 1
#define debug(A) printf("%s\n",A);fflush(stdout)

int main(int argc, char *argv[])
{
	int ret;
	if(argc != 2)
	{
		printf("Inappropriate format\n");
		printf("\t%s filename\n",argv[0]);
		return 1;
	}
	/*get an empty parse tree*/
	t_parse_tree *parse_tree = NULL;
	parse_tree = create_parse_tree();
	if(parse_tree == NULL)
	{
		printf("Insufficient memory\n");
		return 1;
	}
	/*get an empty symbol table*/
	symbol_table *table = create_symbol_table(START_SIZE);
	if(table == NULL)
	{
		printf("Insufficient memory\n");
		return 2;
	}

	char *filename = argv[1];
	char pt_out[100];
	char ast_out[100];
	char lexer_out[100];
	sprintf(lexer_out, "%s_temp_lex", filename);
	sprintf(pt_out, "%s_pt.txt",filename);
	sprintf(ast_out, "%s_ast.txt",filename);
	ret = lexer(filename);
	if(ret)
	{
		printf("ERROR in the lexical phase exiting\n");
		return 1;
	}
	printf("lexer completed\n");


	FILE *fp_pt = fopen(pt_out,"w");
	FILE *fp_ast = fopen(ast_out, "w");
	ret = parser(lexer_out, fp_pt, fp_ast, &parse_tree, &table);
	if(ret)
	{
		printf("Parser ERROR\n");
		fclose(fp_pt);
		return 1;
	}
	printf("parser completed\n");
	display_parse_tree(parse_tree->root, 0, fp_pt);
	// t_parse_tree *ast = get_AST(parse_tree);
	// display_parse_tree(ast->root, 0, fp_ast);
	// free_parse_tree(parse_tree);
	// free_parse_tree(ast);
	fclose(fp_pt);
	// fclose(fp_ast);
	/**/
	ret = type_checker(parse_tree->root, &table);
	if(DISPLAY_SCOPE_GRAPH)
	{
		char file[100];
		sprintf(file,"%s_sg.txt",filename);
		FILE *fp_sg = fopen(file, "w");
		display_scope_graph(table->sg->root, 0,fp_sg);
		// display_scope_graph(table->sg->root, 0,stdout);
		fclose(fp_sg);
		//symbol table
		sprintf(file,"%s_symbol_table.txt", filename);
		FILE *fp_st = fopen(file, "w");
		display_symbol_table(table, fp_st);
		// display_symbol_table(table, stdout);
		// fclose(fp_st);
	}
	char file[100];
	parse_tree = get_decorated_ast(parse_tree);
	display_parse_tree(parse_tree->root, 0, fp_ast);
	fclose(fp_ast);
	sprintf(file, "%s_deco_ast.txt", filename);
	parse_tree = get_AST(parse_tree);
	fp_ast = fopen(file, "w");
	display_parse_tree(parse_tree->root, 0, fp_ast);
	fclose(fp_ast);

// 	symbol_list temp =  lookup_symbol_table(table, " j", 2);
// debug("in driver");
// 	printf("id-%s scope(%d) referred_count:- %d\n", temp->identifier, temp->scope, temp->referred_count);
// 	temp = exist_in_scope(" j", 5, table);
// 	printf("id-%s scope(%d) referred_count:- %d\n", temp->identifier, temp->scope, temp->referred_count);
	return 0;
}

// if(!ret && show_pt)
	// {
	// 	FILE *fp = fopen(pt_out,"w");
	// 	// printf("----------------------------------------------------------------------\n");
	// 	display_parse_tree(parse_tree->root, 0, fp);
	// 	// printf("----------------------------------------------------------------------\n");
	// 	close(fp);
	// }
	// // t_parse_tree *ast = get_AST(parse_tree);
	// if(!ret && show_ast)
	// {
	// 	FILE *fp = fopen(ast_out, "w");
	// 	// printf("----------------------------------------------------------------------\n");
	// 	// display_parse_tree(ast->root, 0, fp);
	// 	// printf("----------------------------------------------------------------------\n");
	// 	close(fp);
	// }
	// printf("%d\n",ret);