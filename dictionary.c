# include <stdlib.h> // malloc + free
# include <assert.h>

# include "dictionary.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION





/*!
 * \file
 * \brief Provide a dictionary (associative table).
 * \li keys must be non-empty \c sstring's
 * \li values must be \c chunk's
 *
 * Keys and values are copied to get independent long term storage.
 * In the same spirit, queried values are \b copies. 
 * The <b>caller is in charge of destroying copies</b>; this is indicated by the \c _copy in the query function name.
 *
 * Values are stored in an ordered binary tree.
 * They are two data structures: one for the dictionary and one for nodes.
 *
 * \note Cela ressemble au TDM 3, même s'il y a des différences.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


/*! \c dictionary is a pointer to the hidden structure for dictionary. */
struct dictionary_struct {
	sstring key;
	void* val;
	dictionary pere;
	dictionary filsgauche;
	dictionary filsdroit;
};

/*!
 * Generate an empty \c dictionary.
 *
 * \return an empty \c dictionary
 */
dictionary dictionary_create ( void )  { 
	dictionary dic = (dictionary)malloc(sizeof(struct dictionary_struct));
	dic->key = NULL;
	dic->val = NULL;
	dic->pere = NULL;
	dic->filsgauche = NULL;
	dic->filsdroit = NULL;
	return dic;
}


/*!
 * Add an entry \c (key,val) into a \c dictionary.
 *
 * Copies of key and val are made.
 * The original values can be safely destroyed after the call.
 *
 * \param dic \c dictionary to modify
 * \param key insertion key
 * \param val inserted value
 * \pre no pointer is NULL (assert-ed)
 * \pre key is not an empty string  (assert-ed)
 */
void dictionary_set ( dictionary dic ,
			     sstring key ,
			     chunk val )  {
	if(dic->key == NULL){
		dic->key = sstring_copy(key);
		dic->val = chunk_copy(val);
		return;
	}
	else{
		int compare = sstring_compare(dic->key, key);
		if(compare == 0){
			return;
		}
		if(compare < 0){
			if(dic->filsgauche == NULL){
				dic->filsgauche = dictionary_create();
				dic->filsgauche->pere = dic;
			}
			dictionary_set(dic->filsgauche , key , val);
		}
		else if(compare > 0){
			if(dic->filsdroit == NULL){
				dic->filsdroit = dictionary_create();
				dic->filsdroit->pere = dic;
			}
			dictionary_set(dic->filsdroit , key , val);
		}
	}

}


/*!
 * Retrieve a \b copied value from a \c dictionary according to a \c key.
 *
 * \param dic \c dictionary to query from
 * \param key key to search a value for
 * \pre no pointer is NULL (assert-ed)
 * \pre key is not empty
 * \return a \b copy of the associated \c chunk or NULL if undefined 
 */
chunk dictionary_get_copy ( dictionary dic ,
				   sstring key )  { 
	int compare = sstring_compare(dic->key, key);
	if (compare == 0){
		return chunk_copy(dic->val);
	}
	if (compare < 0){
		if(dic->filsgauche == NULL){
			return NULL;
		}
		else{
			return dictionary_get_copy(dic->filsgauche, key);
		}
	}
	if (compare > 0){
		if(dic->filsdroit ==  NULL){
			return NULL;
		}
		else{
			return dictionary_get_copy(dic->filsdroit, key);
		}
	}
}


/*!
 * Destroy a \c dictionary and released associated resources.
 * All keys and values are destroyed.
 *
 * \param dic \c dictionary to destroy
 * \pre no pointer is NULL (assert-ed)
 */
void dictionary_destroy ( dictionary dic )  {
	if(dic != NULL){
		dictionary_destroy(dic->filsgauche);
		dictionary_destroy(dic->filsdroit);
		sstring_destroy(dic->key);
		chunk_destroy(dic->val);
		free(dic);
		dic = NULL;
	}
}


/*!
 * Print a \c dictionary to a stream.
 * Entries are printed in \c key alphabetical order.
 * The format is as in the following example with and \c value_integer and a \c value_blok
 \verbatim
 "Bob" => 3
 "Increment_Bob" => {
 Bob
 1
 +
 \Bob
 def
 } \endverbatim
 * \param dic \c dictionary to print
 * \param f stream to print to
 * \pre no pointer is NULL (assert-ed)
 */
void dictionary_print ( dictionary dic ,
			       FILE * f )  {
	if(dic != NULL){
		dictionary_print(dic->filsgauche, f);
		fprintf(f , "\"");
		sstring_print(dic->key, f);
		fprintf(f, "\" => " );
		chunk_print(dic->val, f);
		fprintf(f, "\n" );
		dictionary_print(dic->filsdroit, f);
	}
}





