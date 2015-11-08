# include <string.h>
# include <ctype.h>
# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include "sstring.h"
# include "basic_type.h"
# include "read_chunk_io.h"
# include "linked_list_chunk.h"
# include "value.h"
# include "value_boolean.h"
# include "value_sstring.h"
# include "value_double.h"
# include "value_int.h"
# include "value_block.h"
# include "value_protected_label.h"
# include "value_error.h"
# include "operator.h"
# include "operator_nop.h"
# include "operator_pop.h"
# include "operator_print.h"
# include "operator_print_stack.h"
# include "operator_stop_trace.h"
# include "operator_start_trace.h"
# include "operator_copy.h"
# include "operator_def.h"
# include "operator_print_dictionary.h"
# include "operator_if.h"
# include "operator_if_else.h"
# include "operator_while.h"
# include "operator_addition.h"
# include "operator_division.h"
# include "operator_multiplication.h"
# include "operator_subtraction.h"
# include "operator_remainder.h"
# include "operator_less.h"
# include "operator_less_equal.h"
# include "operator_equal.h"
# include "operator_different.h"
# include "operator_and.h"
# include "operator_or.h"
# include "operator_not.h"
# include "operator_label.h"
# include "operator_creator_list.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Read \c chunk from a stream.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

/*!
 * Maximal length of a token or label.
 * Larger length labels or keywords are not allowed.
 */
# define TOKEN_KEYWORD_MAX_LENGTH 50
# define OPERATOR_CREATOR_LIST_SIZE 12

/*!
 * Read a token from a stream.
 *
 * The first (non-space) character is read and depending on it, only some \c chunk are possible.
 * More characters are read until:
 * \li chunk is fully read, and
 * \li there is no ambiguity  (\c"!" from \c"!=", minus operation from sign of number…).
 * If one (or more) \c char that could belong to another \c chunk are read to settle ambiguity, they are put back (with \c ungetc).
 *
 * Label (protected or not) are distinguished from keywords by using the table in \c operator_creator_list.
 *
 * Static functions are used to handle separately numbers (int or float), label or keyword, sstring, block…
 *
 * Following spaces are discarded.
 *
 * If the OEF is reached before anything is read, then a \c value_error with \c VALUE_ERROR_IO_EOF is returned.
 *
 * If the input cannot form a legal \c chunk, then a corresponding  \c value_error is returned.
 *
 * \param f \c stream to read from
 * \pre \c f is not \c NULL (assert-ed)
 * \return chunk read (should be a \c value or an \c operator) or a \c value_error if reading fail.
 */

chunk read_chunk_io ( FILE * f )  {



	/*
	Values :
	chiffre = int/double => différencié par '.'
	true/false = boolean
	\ = label
	" = string
	{ = value block

	Operators :
	nop
	pop
	+
	-
	*
	/
	%
	<
	<=
	!
	==
	!=
	&&
	||
	if
	if_else
	while
	label
	copy
	print
	print_dictionary
	print_stack
	start_trace
	stop_trace
	*/
	int c;
	char * buff;
	bool overflow = false;
	bool is_label = true;
	char * s = malloc(TOKEN_KEYWORD_MAX_LENGTH * sizeof(char));
	for(int j = 0; j<50;j++){s[j]=0;}
	sstring ss;
	sstring longss = sstring_create_empty();

	while((c = fgetc(f)) != EOF){
		// Vraiment vérifier les interactions avec les espaces, fins de ligne.
		s[0] = c;
		if(c == ' ' || c == '\n'){
		}else if ('0'<=c && c<='9'){
			//différencier int/double
			//On met le premier chiffre
			bool d = false;
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					overflow = true;
					continue;
				}
				s[i] = c;
				if ('0'<=c && c<='9'){
				}else if (c == '.'){
					if (d)
						//deux points, erreur
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
					else
						d = true;
				}else if (c == '}'){
					if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
						while((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){}
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
					}
					ungetc(' ',f);
					ungetc('}',f);
					break;
				}else{
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
				}
			}
			if (overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			if (d)
				return value_double_create(atof(s));
			else
				return value_int_create(atoi(s));
		}else if (c == '\\'){
			//value protected_label
			if (((c = fgetc(f)) <= 'z' && 'a'<= c) || (c <= 'Z' && 'A'<= c)){
				s[0] = c;
				for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
					if(i>=TOKEN_KEYWORD_MAX_LENGTH){
						continue;
						overflow = true;
					}
					s[i] = c;
					if ((c!='_') && !('0'<=c && c<='9') && !(c <= 'z' && 'a'<= c) && !(c <= 'Z' && 'A'<= c)){
						if(c=='}'){
							if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
								while((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){}
								return value_error_create(VALUE_ERROR_IO_SYNTAX);
							}
							ungetc(' ',f);
							ungetc('}',f);
							break;
						}
						while((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){}
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
					}
				}
				if(overflow)
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
			}else{
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			}
			for (int i = 1; i < OPERATOR_CREATOR_LIST_SIZE; ++i){
				if (0 == strcmp(s, operator_creator_list[i].keyword)){
					return value_error_create(VALUE_ERROR_IO_LABEL_IS_KEYWORD);
				}
			}
			//return chunk correspondant
			return value_protected_label_create(sstring_create_string(s));
		}else if (c == 't'){
			//value true
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if((c!='_') && !('0'<=c && c<='9') && !(c <= 'z' && 'a'<= c) && !(c <= 'Z' && 'A'<= c))
					is_label = false;
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					continue;
					overflow = true;
				}
				s[i] = c;
			}
			if(overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			buff = "true";
			if(0 == strcmp(s,buff))
				//return chunk correspondant
				return value_boolean_create(true);
			else{
				buff = "true}";
				if(0 == strcmp(s,buff)){
					ungetc(' ',f);
					ungetc('}',f);
					return value_boolean_create(true);
				}else{
				//if(){

				//}else
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
				//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
				}
			}
		}else if (c == 'f'){
			//value false
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if((c!='_') && !('0'<=c && c<='9') && !(c <= 'z' && 'a'<= c) && !(c <= 'Z' && 'A'<= c))
					is_label = false;
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					continue;
					overflow = true;
				}
				s[i] = c;
			}
			if(overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			buff = "false";
			if(0 == strcmp(s,buff))
				return value_boolean_create(false);
				//return chunk correspondant
			else{
				buff = "true}";
				if(0 == strcmp(s,buff)){
					ungetc(' ',f);
					ungetc('}',f);
					return value_boolean_create(true);
				}else{
				//if(){

				//}else
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
				//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
				}
			}
		}else if (c == '\"'){
			//value string
			bool backslash = false;
			/*if((c = fgetc(f)) != EOF)
			  s[0]=c;
			else
			return value_error_create(VALUE_ERROR_IO_MALFORMED_STRING);*/
			for (int i = 0;(c = fgetc(f)) != EOF; ++i){
			  if((i!=0) && (i%TOKEN_KEYWORD_MAX_LENGTH)==0){
					ss = sstring_create_string(s);
					sstring_concatenate(longss, ss);
					free(s);
					s = malloc(TOKEN_KEYWORD_MAX_LENGTH * sizeof(char));
					for(int j = 0; j<50;j++){s[j]=0;}
					overflow = true;
				}
				s[i%TOKEN_KEYWORD_MAX_LENGTH] = c;
				if(c == '\\')
				  backslash = true;
				if(c == 'n' && backslash){
				  s[(i%TOKEN_KEYWORD_MAX_LENGTH)-1] = '\n';
				  s[i%TOKEN_KEYWORD_MAX_LENGTH] = 0;
				  i--;
				}
				if(c == 't' && backslash){
				  s[(i%TOKEN_KEYWORD_MAX_LENGTH)-1] = '\t';
				  s[i%TOKEN_KEYWORD_MAX_LENGTH] = 0;
				  i--;
				}
				if (c == '\"'){
					if(!backslash){
					  s[i%TOKEN_KEYWORD_MAX_LENGTH]=0;
					  i--;
						if((c = fgetc(f)) == EOF || c == ' ' || c == '\n'){
							//return chunk correspondant
						  if (overflow){
						    ss = sstring_create_string(s);
						    sstring_concatenate(longss, ss);
						    free(s);
						    return value_sstring_create(longss);
						  }else
						    return value_sstring_create(sstring_create_string(s));
						}else{
							if(c=='}'){
								if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
									while((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){}
									return value_error_create(VALUE_ERROR_IO_SYNTAX);
								}
								ungetc(' ',f);
								ungetc('}',f);
								if (overflow)
									return value_sstring_create(longss);
								else
									return value_sstring_create(sstring_create_string(s));
							}
							while((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){}
							return value_error_create(VALUE_ERROR_IO_SYNTAX);
						}
							//value_error : input cannot form a legal chunk
					}else{
					  s[(i%TOKEN_KEYWORD_MAX_LENGTH)-1] = '\"';
					  s[i%TOKEN_KEYWORD_MAX_LENGTH] = 0;
					  i--; 
					}
					  
				}
				if(c!='\\')
				  backslash = false;
			}
			return value_error_create(VALUE_ERROR_IO_MALFORMED_STRING);
			//return value_error : input cannot form a legal chunk
		}else if (c == '{'){
			//value block
			linked_list_chunk lls;
			chunk ch;
			lls = linked_list_chunk_create();
			while(true){
				ch = read_chunk_io(f);
				if(((chunk_is_value(ch))) && (value_is_error(ch))){
					
					switch(basic_type_get_long_long_int(value_get_value(ch))){
					case 2 :
						return value_error_create(VALUE_ERROR_IO_UNFINISHED_BLOCK);
					break;
					case 10 :
						return value_block_create(lls);
					break;
					default:
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
					}
				}else{
					linked_list_chunk_add_back(lls, ch);
				}
			}
		}else if (c == '}'){
			//Utilisé pour terminer l'appel récursif de read_chunk_io pour remplir un block.
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				while((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){}
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			}
			return value_error_create(VALUE_ERROR_END_OF_BLOCK);
		}else if (c == 'n'){
			//operator nop
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if((c!='_') && !('0'<=c && c<='9') && !(c <= 'z' && 'a'<= c) && !(c <= 'Z' && 'A'<= c))
					is_label = false;
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					continue;
					overflow = true;
				}
				s[i] = c;
			}
			if(overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			buff = "nop";
			if(0 == strcmp(s,buff))
				return operator_nop_create();
				//return chunk correspondant
			else{
				buff = "nop}";
				if(0 == strcmp(s,buff)){
					ungetc(' ',f);
					ungetc('}',f);
					return operator_nop_create();
				}else{
					//if(){

					//}else
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
					//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
				}
			}
		}else if (c == 'd'){
			//operator def
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if((c!='_') && !('0'<=c && c<='9') && !(c <= 'z' && 'a'<= c) && !(c <= 'Z' && 'A'<= c))
					is_label = false;
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					continue;
					overflow = true;
				}
				s[i] = c;
			}
			if(overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			buff = "def";
			if(0 == strcmp(s,buff))
				return operator_def_create();
				//return chunk correspondant
			else{
				buff = "def}";
				if(0 == strcmp(s,buff)){
					ungetc(' ',f);
					ungetc('}',f);
					return operator_def_create();
				}else{
					//if(){

					//}else
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
					//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
				}
			}
		}else if (c == '+'){
			//operator +
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					continue;
					overflow = true;
				}
				s[i] = c;
			}
			if(overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			buff = "+";
			if(0 == strcmp(s,buff))
				//return chunk correspondant
				return operator_addition_create();
			else{
				buff = "+}";
				if(0 == strcmp(s,buff)){
					ungetc(' ',f);
					ungetc('}',f);
					return operator_addition_create();
				}else
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
			}
		}else if (c == '-'){
			//int / double / operator -
			if((c = fgetc(f)) == EOF || c == ' '|| c =='\n' || c == '{'){
				if(c == '{'){
					if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
						while((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){}
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
					}
					ungetc(' ',f);
					ungetc('}',f);
				}
				//return operator -
				return operator_subtraction_create();
			}else{
				if ('0' <= c && c <= '9'){
					s[1] = c;
					bool d = false;
					for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
						if(i>=TOKEN_KEYWORD_MAX_LENGTH){
							overflow = true;
							continue;
						}
						s[i+1] = c;
						if ('0'<=c && c<='9'){
						}else if (c == '.'){
							if (d)
								//deux points, erreur
								return value_error_create(VALUE_ERROR_IO_SYNTAX);
							else
								d = true;
						}else if (c == '}'){
							if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
								while((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){}
								return value_error_create(VALUE_ERROR_IO_SYNTAX);
							}
							ungetc(' ',f);
							ungetc('}',f);
							break;
						}else{
							return value_error_create(VALUE_ERROR_IO_SYNTAX);
						}
					}
					if (overflow)
						return value_error_create(VALUE_ERROR_IO_SYNTAX);							
					if (d){
						return value_double_create(atof(s));
					}else{
						return value_int_create(atoi(s));
					}
				}else{
					while((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){}
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
				}
			}
		}else if (c == '/'){
			//operator /
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					overflow = true;
					continue;
				}
				s[i] = c;
			}
			if(overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			buff = "/";
			if(0 == strcmp(s,buff))
				return operator_division_create();
				//return chunk correspondant
			else{
				buff = "/}";
				if(0 == strcmp(s,buff)){
					ungetc(' ',f);
					ungetc('}',f);
					return operator_division_create();
				}else
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
			}
		}else if (c == '*'){
			//operator *
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					overflow = true;
					continue;
				}
				s[i] = c;
			}
			if(overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			buff = "*";
			if(0 == strcmp(s,buff))
				return operator_multiplication_create();
				//return chunk correspondant
			else{
				buff = "*}";
				if(0 == strcmp(s,buff)){
					ungetc(' ',f);
					ungetc('}',f);
					return operator_multiplication_create();
				}else
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
			}
		}else if (c == '%'){
			//operator %
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					overflow = true;
					continue;
				}
				s[i] = c;
			}
			if(overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			buff = "%";
			if(0 == strcmp(s,buff))
				return operator_remainder_create();
				//return chunk correspondant
			else{
				buff = "%}";
				if(0 == strcmp(s,buff)){
					ungetc(' ',f);
					ungetc('}',f);
					return operator_remainder_create();
				}else
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
			}
		}else if (c == '<'){
			//operator < / operator <=
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				if (c == '}'){
					if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
						while((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){}
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
					}
					ungetc(' ',f);
					ungetc('}',f);
					return operator_less_create();
				}
				for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
					if(i>=TOKEN_KEYWORD_MAX_LENGTH){
						overflow = true;
						continue;
					}
					s[i] = c;
				}
				if(overflow)
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
				buff = "<=";
				if(0 == strcmp(s,buff))
					return operator_less_equal_create();
					//return operator <=
				else{
					buff = "<=}";
					if(0 == strcmp(s,buff)){
						ungetc(' ',f);
						ungetc('}',f);
						return operator_less_equal_create();
					}else
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
				}
			}
			//return operator <
			return operator_less_create();
		}else if (c == '!'){
			//operator ! / operator !=
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				if (c == '}'){
					if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
						while((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){}
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
					}
					ungetc(' ',f);
					ungetc('}',f);
					return operator_not_create();
				}
				for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
					if(i>=TOKEN_KEYWORD_MAX_LENGTH){
						overflow = true;
						continue;
					}
					s[i] = c;
				}
				if(overflow)
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
				buff = "!=";
				if(0 == strcmp(s,buff))
					return operator_different_create();
					//return operator !=
				else{
					buff = "!=}";
					if(0 == strcmp(s,buff)){
						ungetc(' ',f);
						ungetc('}',f);
						return operator_different_create();
					}else
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
				}
			}
			return operator_not_create();
			//return operator !
		}else if (c == '='){
			//operator ==
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
					if(i>=TOKEN_KEYWORD_MAX_LENGTH){
						overflow = true;
						continue;
					}
					s[i] = c;
				}
				if(overflow)
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
				buff = "==";
				if(0 == strcmp(s,buff))
					return operator_equal_create();
					//return operator ==
				else{
					buff = "==}";
					if(0 == strcmp(s,buff)){
						ungetc(' ',f);
						ungetc('}',f);
						return operator_equal_create();
					}else
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
				}
			}
			return value_error_create(VALUE_ERROR_IO_SYNTAX);
			//return value_error : input cannot form a legal chunk
		}else if (c == '&'){
			//operator &&
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
					if(i>=TOKEN_KEYWORD_MAX_LENGTH){
						overflow = true;
						continue;
					}
					s[i] = c;
				}
				if(overflow)
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
				buff = "&&";
				if(0 == strcmp(s,buff))
					return operator_and_create();
					//return operator &&
				else{
					buff = "&&}";
					if(0 == strcmp(s,buff)){
						ungetc(' ',f);
						ungetc('}',f);
						return operator_and_create();
					}else
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
				}
			}
			return value_error_create(VALUE_ERROR_IO_SYNTAX);
			//return value_error : input cannot form a legal chunk
		}else if (c == '|'){
			//operator ||
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
					if(i>=TOKEN_KEYWORD_MAX_LENGTH){
						overflow = true;
						continue;
					}
					s[i] = c;
				}
				if(overflow)
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
				buff = "||";
				if(0 == strcmp(s,buff))
					return operator_or_create();
					//return operator ||
				else{
					buff = "||}";
					if(0 == strcmp(s,buff)){
						ungetc(' ',f);
						ungetc('}',f);
						return operator_or_create();
					}else
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
				}
			}
			return value_error_create(VALUE_ERROR_IO_SYNTAX);
			//return value_error : input cannot form a legal chunk
		}else if (c == 'i'){
			//operator if / operator if_else
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if((c!='_') && !('0'<=c && c<='9') && !(c <= 'z' && 'a'<= c) && !(c <= 'Z' && 'A'<= c))
					is_label = false;
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					overflow = true;
					continue;
				}
				s[i] = c;
			}
			if(overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			buff = "if";
			if(0 == strcmp(s,buff))
				return operator_if_create();
				//return operator if
			else{
				buff = "if}";
				if(0 == strcmp(s,buff)){
					ungetc(' ',f);
					ungetc('}',f);
					return operator_if_create();
				}else{
					buff = "if_else";
					if (0 == strcmp(s,buff))
						return operator_if_else_create();
						//return operator if_else
					else{
						buff = "if_else}";
						if(0 == strcmp(s,buff)){
							ungetc(' ',f);
							ungetc('}',f);
							return operator_if_else_create();
						}else{
						//if(){

						//}else
						return value_error_create(VALUE_ERROR_IO_SYNTAX);
						//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
						}
					}
				}
			}
			//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
		}else if (c == 'w'){
			//operator while
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if((c!='_') && !('0'<=c && c<='9') && !(c <= 'z' && 'a'<= c) && !(c <= 'Z' && 'A'<= c))
					is_label = false;
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					overflow = true;
					continue;
				}
				s[i] = c;
			}
			if(overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			buff = "while";
			if(0 == strcmp(s,buff))
				return operator_while_create();
				//return chunk correspondant
			else{
				buff = "while}";
				if(0 == strcmp(s,buff)){
					ungetc(' ',f);
					ungetc('}',f);
					return operator_while_create();
				}else{
				//if(){

				//}else
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
				//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
				}
			}
		}else if (c == 'c'){
			//operator copy
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if((c!='_') && !('0'<=c && c<='9') && !(c <= 'z' && 'a'<= c) && !(c <= 'Z' && 'A'<= c))
					is_label = false;
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					overflow = true;
					continue;
				}
				s[i] = c;
			}
			if(overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			buff = "copy";
			if(0 == strcmp(s,buff))
				return operator_copy_create();
				//return chunk correspondant
			else{
				buff = "copy}";
				if(0 == strcmp(s,buff)){
					ungetc(' ',f);
					ungetc('}',f);
					return operator_copy_create();
				}else{
				//if(){

				//}else
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
				//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
				}
			}
		}else if (c == 's'){
			//operator start_trace / operator stop_trace
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				if((c!='_') && !('0'<=c && c<='9') && !(c <= 'z' && 'a'<= c) && !(c <= 'Z' && 'A'<= c))
					is_label = false;
				for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
					if((c!='_') && !('0'<=c && c<='9') && !(c <= 'z' && 'a'<= c) && !(c <= 'Z' && 'A'<= c))
						is_label = false;
					if(i>=TOKEN_KEYWORD_MAX_LENGTH){
						overflow = true;
						continue;
					}
					s[i] = c;
				}
				if(overflow)
					return value_error_create(VALUE_ERROR_IO_SYNTAX);
				buff = "start_trace";
				if(0 == strcmp(s,buff))
					return operator_start_trace_create();
					//return operator start_trace
				else{
					buff = "start_trace}";
					if(0 == strcmp(s,buff)){
						ungetc(' ',f);
						ungetc('}',f);
						return operator_start_trace_create();
					}else{
						buff = "stop_trace";
						if (0 == strcmp(s,buff))
							return operator_stop_trace_create();
							//return operator stop_trace
						else{
							buff = "stop_trace}";
							if(0 == strcmp(s,buff)){
								ungetc(' ',f);
								ungetc('}',f);
								return operator_stop_trace_create();
							}else{
							//if(){
			
							//}else
							return value_error_create(VALUE_ERROR_IO_SYNTAX);
							//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
							}
						}
					}
				}
			}
				//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
		}else if (c == 'p'){
			//operator pop / operator print / operator print_stack / operator print_dictionary
			for (int i = 1;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				if((c!='_') && !('0'<=c && c<='9') && !(c <= 'z' && 'a'<= c) && !(c <= 'Z' && 'A'<= c))
					is_label = false;
				if(i>=TOKEN_KEYWORD_MAX_LENGTH){
					overflow = true;
					continue;
				}
				s[i] = c;
			}
			if(overflow)
				return value_error_create(VALUE_ERROR_IO_SYNTAX);
			buff = "pop";
			if(0 == strcmp(s,buff))
				return operator_pop_create();
				//return operator pop
			else{
				buff = "if}";
				if(0 == strcmp(s,buff)){
					ungetc(' ',f);
					ungetc('}',f);
					return operator_if_create();
				}else{
					buff = "print";
					if (0 == strcmp(s,buff))
						return operator_print_create();
						//return operator print
					else{
						buff = "print}";
						if(0 == strcmp(s,buff)){
							ungetc(' ',f);
							ungetc('}',f);
							return operator_print_create();
						}else{
							buff = "print_stack";
							if (0 == strcmp(s,buff))
								return operator_print_stack_create();
								//return operator print_stack
							else{
								buff = "print_stack}";
								if(0 == strcmp(s,buff)){
									ungetc(' ',f);
									ungetc('}',f);
									return operator_print_stack_create();
								}else{
									buff = "print_dictionary";
									if (0 == strcmp(s,buff))
										return operator_print_dictionary_create();
									else{
										buff = "print_dictionary}";
										if(0 == strcmp(s,buff)){
											ungetc(' ',f);
											ungetc('}',f);
											return operator_print_dictionary_create();
										}else{
										//if(){
						
										//}else
										return value_error_create(VALUE_ERROR_IO_SYNTAX);
										//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
										}
									}
								}
							}
						}
					}
				}
			}
			//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
		}else{
			//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk}
		}
	}
	//return value_error :
	return NULL ;
}
