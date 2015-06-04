/*
*	Made By-
*	Group 37 Team 2: 	Shubham Gupta 2012A7PS086P
*						Ravi Shankar Pandey 2012C6PS676P
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hash_lex.h"
#include "lexer_helper.h"
#include "lexer.h"

//TODO change destination file
int lexer(char *filename)
{
	int flag = 0;
	FILE *fp = fopen(filename, "r");
	int len = strlen(filename);
	char *file = (char *)malloc((len+10)*sizeof(char));
	sprintf(file,"%s_temp_lex",filename);
	FILE *fout = fopen(file, "w");
	lexem_info *lex;
	Buffer *buff = create_dual_buffer(1000);
	hash_table_lex *ht = (hash_table_lex*)get_lexem_list((char *)"lexem_details.txt");
	character *ch = (character*)malloc(sizeof(character));
	while(1)
	{
		ch = get_non_blank_char(buff, fp);
		if(ch->type == -2 && buff->is_eof)
		{
			//end of file break the lexer loop
			break;
		}
		lex = get_lexem(buff, ch, ht, fp);
		fflush(stdout);
		if(lex->lexem == NULL)
		{
			continue;
		}
		if(strcmp(lex->token_id, "TK_ERR") == 0)
		{
			printf("Unrecognized token %s at line number %d\n", lex->lexem, lex->line);
			if(!flag)
			{
				fclose(fp);
				fclose(fout);
				remove(file);
			}
			flag = 1;
			// return 1;
		}
		if(!flag)
			fprintf(fout,"%s %d %s %d\n", lex->token_id, lex->lexem_len, lex->lexem, lex->line);
		free_pointer(lex->token_id);
		free_pointer(lex->lexem);
		free_pointer(lex);
	}
	//free_pointer all variables
	free_pointer(buff->buffer);
	free_pointer(buff);
	free_pointer(ch);
	clean_lexem_list(ht);
	free_pointer(file);
	if(!flag)
	{
		fclose(fp);
		fclose(fout);
	}
	return flag;
}

//TODO return proper errors
int get_identifier(Buffer *buff, lexem_info* lex, character *ch, hash_table_lex *ht, FILE *fp, int is_func)
{
	//TODO Handle return value
	add_char(lex, ch->c);
	free_pointer(ch);
	ch = get_char(buff, fp);
	while((ch->type==LETTER||ch->type==DIGIT||ch->c=='_')&&(ch->c!=EOF)) {
		//TODO Handle return value
		add_char(lex, ch->c);
		free_pointer(ch);
		ch = get_char(buff, fp);
	}
	//TODO Handle return value
	retract(buff);
	linked_list_lex node = lookup_hash_table_lex(ht, lex->lexem);
	if(node != NULL)
	{
		//token from hash table
		lex->token_id = strdup(node->token_id);
		free_pointer(node);
	}
	else if(is_func == 0)
	{
		//tk_id
		lex->token_id = strdup("TK_ID");
	}
	else
	{
		lex->token_id = strdup("TK_FUNCID");	
	}
	return 0;
}

//TODO error messages
int get_number(Buffer *buff, lexem_info* lex, character *ch, FILE *fp)
{
	// printf("inside get_number\n");
	// fflush(stdout);
	int decimal_flag = 0, count_after_decimal = 0;
	if(ch->type == DOT)
	{
		decimal_flag = 1;
	}
	add_char(lex, ch->c);
	free_pointer(ch);
	ch = get_char(buff, fp);
	while((ch->type == DIGIT || ch->type == DOT)&&(ch->c!=EOF))
	{
		if(decimal_flag)
		{
			count_after_decimal++;
		}
		if(ch->type == DOT)
		{
			decimal_flag++;
		}
		if(decimal_flag > 1)
			break;
		add_char(lex, ch->c);
		free_pointer(ch);
		ch = get_char(buff,fp);
	}
	retract(buff);
	if(decimal_flag == 1&&count_after_decimal>0)
	{
		lex->token_id = strdup("TK_RNUM");
	}
	else if(decimal_flag == 0)
	{
		lex->token_id = strdup("TK_NUM");
	}
	else
	{
		lex->token_id = strdup("TK_ERR");
		return 1;
	}
	//printf("here %s here %s\n", lex->lexem,lex->token_id);
	//fflush(stdout);
	return 0;
}

//TODO error messages
int get_string_literal(Buffer *buff, lexem_info* lex, character* ch, FILE* fp)
{
	int f=0;
	add_char(lex, ch->c);
	free_pointer(ch);
	ch = get_char(buff, fp);
	while((ch->type != DQUOTE)&&(ch->c != EOF))
	{
		add_char(lex, ch->c);
		free_pointer(ch);
		ch = get_char(buff, fp);
	}
	if(ch->c == EOF)
	{
		add_char(lex, ch->c);
		free_pointer(ch);
		lex->token_id = strdup("TK_ERR");
		return 1;
	}
	else if(ch->type == DQUOTE)
	{
		add_char(lex, ch->c);
		free_pointer(ch);
		lex->token_id = strdup("TK_STR");
	}
	return 0;
}

//TODO error messages
int get_char_literal(Buffer *buff, lexem_info *lex, character *ch, FILE *fp)
{
	add_char(lex, ch->c);
	free_pointer(ch);
	get_char(buff, fp);
	while(ch->type != SQUOTE)
	{
		add_char(lex, ch->c);
		free_pointer(ch);
		get_char(buff, fp);
	}
	add_char(lex, ch->c);
	free_pointer(ch);
	if(lex->lexem[1] == '\\' && lex->lexem[3] == '\'')
	{
		lex->token_id = strdup("TK_CCONST");
	}
	else if(lex->lexem[2] == '\'')
	{
		lex->token_id = strdup("TK_CCONST");
	}
	else
	{
		lex->token_id = strdup("TK_ERR");
		return 1;
	}
	return 0;
}

int get_main_tk(Buffer *buff, lexem_info *lex, character *ch, FILE *fp)
{
	free_pointer(ch);
	ch = get_char(buff, fp);
	while(ch->type == LETTER || ch->c == '_')
	{
		add_char(lex, ch->c);
		free_pointer(ch);
		ch = get_char(buff, fp);
	}
	retract(buff);
	if(strcmp(lex->lexem, "__main__") == 0)
	{
		lex->token_id = strdup("TK_MAIN");
		(buff->is_main)++;
	}
	else
	{
		lex->token_id = strdup("TK_ERR");
		return 1;
	}
	return 0;
}

int get_symbol(Buffer *buff, lexem_info *lex, character *ch, hash_table_lex *ht,FILE *fp)
{
	// printf("inside get_symbol %d\n",ch->type);
	// fflush(stdout);
	add_char(lex, ch->c);
	if(ch->c == '_')
	{
		//TODO Handle return value
		ch = get_char(buff, fp);
		if(ch->c == '_')
		{
			retract(buff);
			get_main_tk(buff, lex, ch, fp);
		}
		else
		{
			get_identifier(buff, lex, ch, ht, fp, 1);
		}
	}
	else if(ch->c == '(')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_LP");
	}
	else if(ch->c == ')')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_RP");
	}
	else if(ch->c == '{')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_LC");
	}
	else if(ch->c == '}')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_RC");
	}
	else if(ch->c == '[')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_LS");
	}
	else if(ch->c == ']')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_RS");
	}
	else if(ch->c == ',')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_COMMA");
	}
	else if(ch->c == ';')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_SEMICOLON");
	}
	else if(ch->c == '#')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_SIZE");
	}
	else if(ch->c == ':')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_COLON");
	}
	else if(ch->c == '|')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_PIPE");
	}
	else if(ch->c == '=')
	{
		free_pointer(ch);
		ch = get_char(buff, fp);
		if(ch->c == '=')
		{
			add_char(lex, ch->c);
			free_pointer(ch);
			lex->token_id = strdup("TK_EQ");
		}
		else
		{
			retract(buff);
			free_pointer(ch);
			lex->token_id = strdup("TK_ASSIGN");
		}
	}
	else if(ch->c == '<')
	{
		free_pointer(ch);
		ch = get_char(buff, fp);
		if(ch->c == '=')
		{
			add_char(lex, ch->c);
			free_pointer(ch);
			lex->token_id = strdup("TK_LE");
		}
		else
		{
			retract(buff);
			free_pointer(ch);
			lex->token_id = strdup("TK_LT");
		}
	}
	else if(ch->c == '>')
	{
		free_pointer(ch);
		ch = get_char(buff, fp);
		if(ch->c == '=')
		{
			add_char(lex, ch->c);
			free_pointer(ch);
			lex->token_id = strdup("TK_GE");
		}
		else
		{
			retract(buff);
			free_pointer(ch);
			lex->token_id = strdup("TK_GT");
		}
	}
	else if(ch->c == '-')
	{
		free_pointer(ch);
		ch = get_char(buff, fp);
		if(ch->c == '>')
		{
			add_char(lex, ch->c);
			free_pointer(ch);
			lex->token_id = strdup("TK_OPRET");
		}
		else
		{
			retract(buff);
			free_pointer(ch);
			lex->token_id = strdup("TK_MINUS");
		}
	}
	else if(ch->c == '+')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_PLUS");
	}
	else if(ch->c == '*')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_MUL");
	}
	else if(ch->c == '%')
	{
		free_pointer(ch);
		lex->token_id = strdup("TK_MOD");
	}
	else if(ch->c == '/')
	{
		free_pointer(ch);
		ch = get_char(buff, fp);
		if(ch->c == '/')
		{
			free_pointer(ch);
			remove_comment(buff, fp);
			free_pointer(lex->lexem);
			lex->lexem = NULL;
		}
		else
		{
			retract(buff);
			free_pointer(ch);
			lex->token_id = strdup("TK_DIV");
		}
	}
	else if(ch->c == '^')
	{
		free_pointer(ch);
		ch = get_char(buff, fp);
		if(ch->c == '^')
		{
			// free_pointer(ch);
			add_char(lex, ch->c);
			// printf("lex");
			free_pointer(ch);
			lex->token_id = strdup("TK_POW");
		}
		else
		{
			//TODO HANDLE ERROR
			add_char(lex, ch->c);
			free_pointer(ch);
			lex->token_id = strdup("TK_ERR");
			return 1;
			retract(buff);
			lex->token_id = strdup("TK_ERR");
		}
	}
	else
	{
		lex->token_id = strdup("TK_ERR");
	}
	return 0;
}

int remove_comment(Buffer *buff, FILE *fp)
{
	// printf("inside remove comment\n");
	// fflush(stdout);
	character *ch = get_char(buff, fp);
	while(ch->c != '\n')
	{
		free_pointer(ch);
		ch = get_char(buff, fp);
	}
	free_pointer(ch);
	return 0;
}

lexem_info* get_lexem(Buffer *buff, character *ch, hash_table_lex *ht, FILE* fp)
{
	//allocate a lexem node
	lexem_info *lex = (lexem_info*)malloc(sizeof(lexem_info));
	if(lex == NULL)
	{
		printf("Insufficent memory for lex.\n");
		return NULL;
	}
	memset(lex, 0, sizeof(lexem_info));
	lex->lexem = (char *)malloc(LEXEM_START_LEN*sizeof(char));
	if(lex->lexem == NULL)
	{
		printf("Insufficent memory for lexem.\n");
		free_pointer(lex);
		return NULL;
	}
	lex->max_lexem_len = LEXEM_START_LEN;
	lex->lexem_len = 0;
	buff->start = buff->curr;
	if(ch->type == LETTER)
	{
		//TODO
		lex->line = buff->line;
		//TODO handle error
		get_identifier(buff, lex, ch, ht, fp, 0);
	}
	else if(ch->type == DIGIT || ch->type == DOT)
	{
		//TODO
		lex->line = buff->line;
		//TODO handle error
		get_number(buff, lex, ch, fp);
	}
	//TODO get proper prototypes
	else if(ch->type == DQUOTE)
	{
		//TODO
		lex->line = buff->line;
		//TODO handle error
		get_string_literal(buff, lex, ch, fp);
	}
	else if(ch->type == SQUOTE)
	{
		//TODO
		lex->line = buff->line;
		//TODO handle error
		get_char_literal(buff, lex, ch, fp);
	}
	else if(ch->type == UNKNOWN)
	{
		//TODO
		lex->line = buff->line;
		//TODO handle error
		get_symbol(buff, lex, ch, ht, fp);
	}
	else if(ch->type == SPACE || ch->type == TAB || ch->type == NEW_LINE)
	{
		free_pointer(ch);
		ch = get_non_blank_char(buff, fp);
		return get_lexem(buff, ch, ht, fp);
	}
	return lex;
}

int retract(Buffer *buff)
{
	if(buff->curr == buff->buffer)
	{
		buff->curr = buff->buffer + 2*buff->len - 1;
	}
	else
	{
		buff->curr = buff->curr - 1;
	}
}

int add_char(lexem_info *lex, char c)
{
	//if lexem len is full
	if(lex->lexem_len == lex->max_lexem_len-1)
	{
		lex->lexem = (char*)realloc(lex->lexem, lex->max_lexem_len + LEXEM_LEN_INC);
		//TODO error handeling if retuurn value is NULL
		lex->max_lexem_len = lex->max_lexem_len + LEXEM_LEN_INC;
	}
	lex->lexem[(lex->lexem_len)++] = c;
	lex->lexem[lex->lexem_len] = '\0';
	return 0;
}

character* get_non_blank_char(Buffer *buff, FILE* fp)
{
	character* ch = get_char(buff, fp);
	while(isspace(ch->c))
	{
		free_pointer(ch);
		ch = get_char(buff, fp);
	}
	return ch;
}

character* get_char(Buffer *buff, FILE *fp)
{
	character *next = (character*)malloc(sizeof(character));
	if(next == NULL)
	{
		printf("Insufficient Memory.\n");
		return NULL;
	}
	int i = buff->curr - buff->buffer;
	if(i == 1)
	{
		buff->is_full2 = 0;
	}
	else if( i == buff->len + 1)
	{
		buff->is_full1 = 0;
	}
	if(i==buff->len && buff->is_full2 == 0)
	{
		fill_buffer(buff, fp);
	}
	else if(i==2*buff->len && buff->is_full1 == 0)
	{
		fill_buffer(buff, fp);
	}
	next->c = *(buff->curr);
	(buff->curr)++;
	if(next->c == '\0')
	{
		//ignore the null character which is read due to the use of fscanf
		free_pointer(next);
		return get_char(buff, fp);
	}
	if(isalpha(next->c))
	{
		next->type = LETTER;
	}
	else if(isdigit(next->c))
	{
		next->type = DIGIT;
	}
	else if(next->c == EOF)
	{
		next->type = -2;
		buff->is_eof = 1;
	}
	else if(next->c == '"')
	{
		next->type = DQUOTE;
	}
	else if(next->c == '\'')
	{
		next->type = SQUOTE;
	}
	else if(next->c == '.')
	{
		next->type = DOT;
	}
	else if(next->c == ' ')
	{
		next->type = SPACE;
	}
	else if(next->c == '\t')
	{
		next->type = TAB;
	}
	else if(next->c == '\n')
	{
		next->type = SPACE;
		(buff->line)++;
	}
	else
	{
		next->type = UNKNOWN;
	}
	// printf("\t\treutrning charater %c\n",next->c);
	return next;
}

int fill_buffer(Buffer *buff, FILE *fp)
{
	int len,i,current_location = (int)(buff->curr - buff->buffer);
	int s, e;
	char c;
	if(current_location == buff->len)
	{
		s = buff->len;
		e = 2*(buff->len);
	}
	else if(current_location == 2*(buff->len))
	{
		s = 0;
		e = buff->len;
	}
	for(i=s; i<e; i++)
	{
		buff->buffer[i] = fgetc(fp);
		if(buff->buffer[i] == EOF)
		{
			break;
		}
	}

	if(current_location == buff->len)
		buff->curr = buff->buffer + buff->len;
	else if(current_location == 2*buff->len)
		buff->curr = buff->buffer;
	return 0;
}

Buffer* create_dual_buffer(int buffer_size)
{
	Buffer *buff;
	buff = (Buffer *)malloc(sizeof(Buffer));
	if(buff == NULL)
	{
		printf("Insufficient space for buffer.\n");
		return NULL;
	}
	buff->buffer = (char *)malloc(2*buffer_size*sizeof(char));
	if(buff->buffer == NULL)
	{
		free_pointer(buff);
		printf("Insufficient space for buffer.\n");
		return NULL;
	}
	memset(buff->buffer, 0, 2*buffer_size*sizeof(char));
	buff->start = buff->buffer;
	buff->curr = buff->buffer + 2*buffer_size;
	buff->len = buffer_size;
	buff->line = 1;
	buff->is_eof = 0;
	buff->is_full1 = 0;
	buff->is_full2 = 0;
	buff->is_main = 0;
	return buff;
}
