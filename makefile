
#	MAKEFILE for compiler project
#	Made by:
#		Ravi Shankar Pandey (2012C6PS676P)
#		Shubham Gutpa (2012A7PS086P)
#

#	Variables for makefile
CC = gcc
Cflags = -c
# if set compile in debug mode
is_debug = 1
no_debug = 0
debug_flag = -g
result_file = compiler_c--

#The default option
default: parser.o lexer.o
	$(CC) driver.c parser.a lexer.a -o $(result_file)
	rm *.a

#gets a library for the parser
parser.o:
	$(CC) $(Cflags)	./symbol_table.c ./scope_graph.c ./parser_helper.c ./hash_par.c ./stack.c ./parse_tree.c ./type_checker.c ./deco_ast.c
	ar cr parser.a *.o
	rm *.o

# gets a library for the lexer
lexer.o:
	$(CC) $(Cflags) ./lexer.c ./lexer_helper.c ./hash_lex.c
	ar cr lexer.a *.o
	rm *.o

# dec_ast:
# 	$(CC) $(Cflags) ./deco_ast.c ./parse_tree.c
# 	ar cr dec_ast.a *o
# 	rm *.o
##
#	To add type extractor 
#	To add symentic
#	To add code generator
##

# remove all the compilation and executable file
clean:
	rm $(result_file) *.a *.o