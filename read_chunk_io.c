# include <string.h>
# include <ctype.h>
# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include "read_chunk_io.h"

# include "value_boolean.h"
# include "value_sstring.h"
# include "value_double.h"
# include "value_int.h"
# include "value_block.h"
# include "value_protected_label.h"
# include "value_error.h"

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



	int c;
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
	char * s = malloc(TOKEN_KEYWORD_MAX_LENGTH * sizeof(char));
	char * buff;

	while((c = fgetc(f)) != EOF){
		// Vraiment vérifier les interactions avec les espaces, fins de ligne.
		if(c == ' ' || c == '\n'){
		}else if ('0'<c && c<'9'){
			//différencier int/double
			//On met le premier chiffre
			bool d = false;
				while((c = fgetc(f)) != EOF && c != ' ' && c !='\n'){
					if ('0'<c && c<'9'){
						//remplit le int/double
					}else if (c == '.'){
						if (d)
							//deux points, erreur
							return NULL;
						else
							d = true;
					}else
						return NULL;
						//erreur
				}
			//return int/double, bool pour déduire?
		/*}else if (c == '\\'){
			//value protected_label
			for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				s[i] = c;
			}
			for (int i = 0; i < OPERATOR_CREATOR_LIST_SIZE; ++i){
				if (0 == strcmp(s, operator_creator_list[i].keyword)){
					//return value_error
					return NULL;
				}
			}
			return NULL;
			//return chunk correspondant*/
		}else if (c == 't'){
			//value true
			for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				s[i] = c;
			}
			buff = "rue";
			if(0 == strcmp(s,buff))
				//return chunk correspondant
				return NULL;
			else
				return NULL;
				//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
		}else if (c == 'f'){
			//value false
			for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				s[i] = c;
			}
			buff = "alse";
			if(0 == strcmp(s,buff))
				return NULL;
				//return chunk correspondant
			else
				return NULL;
				//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
		}else if (c == '\"'){
			//value string
			while((c = fgetc(f)) != EOF && c!='\"'){
				if(c == '\"'){
					for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
						s[i] = c;
					}
					buff = "";
					if(0 == strcmp(s,buff))
						//return chunk correspondant
						return NULL;
					else
						return NULL;
						//value_error : input cannot form a legal chunk
				}else
					//remplit le string
					return NULL;
			}
			return NULL;
			//return value_error : input cannot form a legal chunk
		}else if (c == '{'){
			//value block
			int nb_brackets_left_to_close = 0;
			while((c = fgetc(f)) != EOF){
				if(c == '{'){
					//ajoute le {
					nb_brackets_left_to_close++;
				}
				if(c == '}'){
					//ajoute le }
					if (0 == nb_brackets_left_to_close){
						for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
							s[i] = c;
						}
						buff = "";
						if(0 == strcmp(s,buff))
							return NULL;
							//return chunk correspondant
					}else{
						nb_brackets_left_to_close--;
					}
				}else{
					return NULL;
					//ajoute le char
				}
			}
			//return value_error : input cannot form a legal chunk
		}else if (c == 'n'){
			//operator nop
			for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				s[i] = c;
			}
			buff = "op";
			if(0 == strcmp(s,buff))
				return NULL;
				//return chunk correspondant
			else
				return NULL;
				//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
		}else if (c == '+'){
			//operator +
			for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
					s[i] = c;
				}
				buff = "";
				if(0 == strcmp(s,buff))
					//return chunk correspondant
					return NULL;
				else
					return NULL;
					//value_error : input cannot form a legal chunk
		}else if (c == '-'){
			//int / double / operator -
			if((c = fgetc(f)) == EOF || c == ' '||c =='\n'){
					//return operator -
			}else{
				if ('0' <= c && c <= '9'){
					//remplit int/double
					bool d = false;
					while((c = fgetc(f)) != EOF && c != ' ' && c !='\n'){
						if ('0'<c && c<'9'){
							return NULL;
							//remplit le int/double
						}else if (c == '.'){
							if (d)
								return NULL;
								//deux points, erreur
							else
								d = true;
						}else
							return NULL;
							//erreur
					}
				//return int/double, bool pour déduire?
				}
				//return value_error : input cannot form a legal chunk
			}
		}else if (c == '/'){
			//operator /
			for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
					s[i] = c;
				}
				buff = "";
				if(0 == strcmp(s,buff))
					return NULL;
					//return chunk correspondant
				else
					return NULL;
					//value_error : input cannot form a legal chunk
		}else if (c == '*'){
			//operator *
			for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
					s[i] = c;
				}
				buff = "";
				if(0 == strcmp(s,buff))
					return NULL;
					//return chunk correspondant
				else
					return NULL;
					//value_error : input cannot form a legal chunk
		}else if (c == '%'){
			//operator %
			for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
					s[i] = c;
				}
				buff = "";
				if(0 == strcmp(s,buff))
					return NULL;
					//return chunk correspondant
				else
					return NULL;
					//value_error : input cannot form a legal chunk
		}else if (c == '<'){
			//operator < / operator <=
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i)
					s[i] = c;
				buff = "=";
				if(0 == strcmp(s,buff))
					return NULL;
					//return operator <=
				else
					return NULL;
					//return value_error : input cannot form a legal chunk
			}
			//return operator <
		}else if (c == '!'){
			//operator ! / operator !=
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i)
					s[i] = c;
				buff = "=";
				if(0 == strcmp(s,buff))
					return NULL;
					//return operator !=
				else
					return NULL;
					//return value_error : input cannot form a legal chunk
			}
			//return operator !
		}else if (c == '='){
			//operator ==
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i)
					s[i] = c;
				buff = "=";
				if(0 == strcmp(s,buff))
					return NULL;
					//return operator ==
				else
					return NULL;
					//return value_error : input cannot form a legal chunk
			}
			//return value_error : input cannot form a legal chunk
		}else if (c == '&'){
			//operator &&
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i)
					s[i] = c;
				buff = "&";
				if(0 == strcmp(s,buff))
					return NULL;
					//return operator &&
				else
					return NULL;
					//return value_error : input cannot form a legal chunk
			}
			//return value_error : input cannot form a legal chunk
		}else if (c == '|'){
			//operator ||
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i)
					s[i] = c;
				buff = "|";
				if(0 == strcmp(s,buff))
					return NULL;
					//return operator ||
				else
					return NULL;
					//return value_error : input cannot form a legal chunk
			}
			//return value_error : input cannot form a legal chunk
		}else if (c == 'i'){
			//operator if / operator if_else
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i)
					s[i] = c;
				buff = "f";
				if(0 == strcmp(s,buff))
					return NULL;
					//return operator if
				else{
					buff = "f_else";
					if (0 == strcmp(s,buff))
						return NULL;
						//return operator if_else
					else
						return NULL;
						//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
				}
			}
				//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
		}else if (c == 'w'){
			//operator while
			for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				s[i] = c;
			}
			buff = "hile";
			if(0 == strcmp(s,buff))
				return NULL;
				//return chunk correspondant, vérif char suivant?
			else
				return NULL;
				//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
		}else if (c == 'c'){
			//operator copy
			for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i){
				s[i] = c;
			}
			buff = "opy";
			if(0 == strcmp(s,buff))
				return NULL;
				//return chunk correspondant, vérif char suivant?
			else
				return NULL;
				//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
		}else if (c == 's'){
				//operator start_trace / operator stop_trace
				if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
					for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i)
						s[i] = c;
					buff = "tart_trace";
					if(0 == strcmp(s,buff))
						return NULL;
						//return operator start_trace
					else{
						buff = "top_trace";
						if (0 == strcmp(s,buff))
							return NULL;
							//return operator stop_trace
						else
							return NULL;
							//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
					}
				}
					//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
		}else if (c == 'p'){
			//operator pop / operator print / operator print_stack / operator print_trace
			if((c = fgetc(f)) != EOF && (c != ' ') && (c != '\n')){
				for (int i = 0;(c = fgetc(f)) != EOF && (c != ' ') && (c != '\n'); ++i)
					s[i] = c;
				buff = "op";
				if(0 == strcmp(s,buff))
					return NULL;
					//return operator pop
				else{
					buff = "rint";
					if (0 == strcmp(s,buff))
						return NULL;
						//return operator print
					else{
						buff = "rint_stack";
						if (0 == strcmp(s,buff))
							return NULL;
							//return operator print_stack
						else{
							buff = "rint_trace";
							if (0 == strcmp(s,buff))
								return NULL;
								//return operator print_trace
							else
								return NULL;
								//Vérifier si c'est une clef, si oui on retourne l'operateur label correspondant, sinon value_error : input cannot form a legal chunk
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


