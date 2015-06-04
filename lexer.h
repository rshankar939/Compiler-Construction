/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include <stdio.h>
#ifndef LEXER_H
#define LEXER_H

#define LETTER 0
#define DIGIT 1
#define UNKNOWN -1
#define SQUOTE 2 //'
#define DQUOTE 3 //"
#define DOT 4 //.
#define SPACE 5 //' '
#define TAB 6 //\t
#define NEW_LINE 7 //\n
#define NULL_CHAR 8 //'\0' read due to the use of fscanf

#define LEXEM_START_LEN 50
#define LEXEM_LEN_INC 50

typedef struct Buffer {
	char *buffer;
	char *start;
	char *curr;
	int len;
	int line;
	int is_eof;
	int is_full1;
	int is_full2;
	int is_main;
}Buffer;

typedef struct lexem_info {
	char *token_id;
	char *lexem;
	int lexem_len;
	int max_lexem_len;
	int line;
}lexem_info;

typedef struct character {
	char c;
	int type;
}character;

extern int remove_comment(Buffer *buff, FILE *fp);
extern int get_char_literal(Buffer *buff, lexem_info *lex, character *ch, FILE *fp);
extern int get_symbol(Buffer *buff, lexem_info *lex, character *ch, hash_table_lex *ht,FILE *fp);
extern int get_string_literal(Buffer *buff, lexem_info* lex, character* ch, FILE* fp);
extern int get_number(Buffer *buff, lexem_info* lex, character *ch, FILE *fp);
extern int get_identifier(Buffer *buff, lexem_info* lex, character *ch, hash_table_lex *ht, FILE *fp, int is_func);
extern int retract(Buffer *buff);
extern int add_char(lexem_info *lexem, char c);
extern character* get_non_blank_char(Buffer *buff, FILE *fp);
extern character* get_char(Buffer *buff, FILE *fp);
extern int fill_buffer(Buffer *buff, FILE *fp);
extern Buffer* create_dual_buffer(int buffer_size);
extern lexem_info* get_lexem(Buffer *buff, character *ch, hash_table_lex *ht, FILE* fp);
extern int get_main_tk(Buffer *buff, lexem_info *lex, character *ch, FILE *fp);

extern int lexer(char *filename);
#endif
