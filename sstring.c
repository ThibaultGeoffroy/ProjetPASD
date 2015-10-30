# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <assert.h>

# include "sstring.h"

# undef NDEBUG    // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief This module provides a « safer » string.
 * 
 * These are not C-string but a \c struct that holds the length of the string and a pointer to the actual char sequence.
 * Please note that there is no \c '\0' to mark the end of the \c sstring.
 *
 * Empty string is encoded by 0 \c length and \c NULL \c pointer.
 *
 * assert is enforced.
 *
 * \note Cela ressemble au TDM 1, même s'il y a des différences.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


/*! \c sstring is a pointer to a hidden structure. */

typedef struct sstring_struct{
  unsigned int size;
  char * value ;
}sstring_struct;
/*!
 * Generate an empty \c sstring.
 *
 * \return an empty \c sstring
 * \author Thibault Geoffroy
 */
sstring sstring_create_empty ( void )  { 
	sstring s = (sstring)malloc(sizeof(struct sstring_struct));
	s->size = 0;
	s->value = NULL;
	return s; 
}


/*!
 * Generate a \c sstring with the same \c char sequence as a C-string.
 *
 * \param st C-string
 * \pre st is not \c NULL (assert-ed)
 * \return a sstring corresponding to st
 * \author Thibault Geoffroy
 */
sstring sstring_create_string ( char const * const st )  {
	sstring s = (sstring)malloc(sizeof(struct sstring_struct));
	s->size = (unsigned int)strlen(st);
	s->value = (char*)malloc(s->size*sizeof(char));
	for(unsigned int i = 0; i<s->size ;  i++){
		(s->value)[i] = st[i];
	}
	return s; 
}


/*!
 * Destroy a \c sstring and release related resources.
 *
 * \param ss C-string to destroy
 * \pre ss is a valid \c sstring (assert-ed)
 * \author Thibault Geoffroy
 */
void sstring_destroy ( sstring ss )  {
	if (ss != NULL){
		free(ss->value);
		ss->value = NULL;
		ss->size = 0;
		free(ss);
		ss = NULL;
	}
}


/*!
 * Print a \c sstring to a stream.
 * There is no line return (unless it is contained in \c ss).
 *
 * \param ss \c sstring to print
 * \param f stream to print to
 * \pre ss is a valid \c sstring (assert-ed)
 * \pre f is not \c NULL (assert-ed)
 * \author Thibault Geoffroy
 */
void sstring_print ( sstring ss ,
			    FILE * f )  {
	assert(ss != NULL);
	for (unsigned int i = 0; i < ss->size ; i++){
		fprintf(f, "%c", (ss->value)[i] );
	}
}

/*!
 * Concatenate a \c sstring at the end of another.
 *
 * \param ss1 \c sstring to be modified
 * \param ss2 \c sstring to concatenate to \c ss1
 * \pre \c ss1 and \c ss2 are valid \c sstring (assert-ed)
 * \author Thibault Geoffroy
 */
void sstring_concatenate ( sstring ss1,
				  sstring ss2 )  {

	unsigned int tailleTotale = ss1->size + ss2->size;
	char* s = malloc(sizeof(char)*tailleTotale);
	if (ss1->value == NULL && ss2->value == NULL){
		free(s);
		return;
	}
	else if(ss1->value != NULL && ss2->value == NULL){
		free(s);
		return;
	}
	else if(ss1->value == NULL && ss2->value != NULL){
		for (unsigned int i = 0  ; i<tailleTotale ; i++){
			s[i] = (ss2->value)[i];
		}
	}
	else if(ss1->value != NULL && ss2->value != NULL){
		int j = 0;
		for (unsigned int i = 0 ; i<ss1->size ; i++ ){
			s[i] = (ss1->value)[i];
		}
		for (unsigned int i = ss1->size ;i<tailleTotale; i++  ){
			s[i] = (ss2->value)[j];
			j++;
		}
	}
	free(ss1->value);
	ss1->value = s;
	ss1->size = tailleTotale;
	return;
}

/*!
 * Provide a copy of a string.
 *
 * \param ss \c sstring to copy
 * \pre ss is a valid \c sstring (assert-ed)
 * \return an independant copy of \c ss
 */
sstring sstring_copy ( sstring ss )  { 
	sstring s = sstring_create_string(ss->value);
	s->size = ss->size;
	return s ; 
}


/*!
 * Indicate how two \c sstring are ordered alphabetically.
 *
 * \param ss1 \c sstring 
 * \param ss2 \c sstring 
 * \pre ss1 and ss2 are valid \c sstring (assert-ed)
 * \return 
 * \li 0 if ss1 == ss2
 * \li -1 if ss1 < ss2
 * \li 1 otherwise
 */
int sstring_compare ( sstring ss1 ,
			     sstring ss2 )  {
	int i = strcmp(ss1->value , ss2->value);
	if( i < 0){
		i = -1;
	}
	if(i > 0){
		i = 1;
	}
	return i; 
}


/*!
 * Indicate whether a string is empty.
 *
 * This function has no side effect and can be safely used in asserts.
 *
 * \param ss \c sstring to test
 * \pre ss is a valid \c sstring (assert-ed)
 * \return true ssi \c ss is empty
 */
bool sstring_is_empty ( sstring ss )  { 
	return (ss->size == 0); 
}


