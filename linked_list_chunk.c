# include <stdlib.h>   // malloc + free
# include <stdbool.h>
# include <assert.h>

# include "linked_list_chunk.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION




/*!
 * \file
 * \brief Structure used to store list of \link chunk\endlink 's.
 *
 * Please note that \c linked_list_chunk does not make any copy of \c chunk's.
 *
 * The list is bidirectional and can be accessed from both end.
 *
 * The structure is 2-level:
 * \li \c link's are used to provide the bidirectional links (with pointers) and points to \c chunk
 * \li the \c main structure has pointers to first and last \c link's.
 *
 * The empty list has the pointers of its \c main structure at \c NULL.
 *
 * \note Cela ressemble au TDM2 (sans être circulaire).
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


/*! 
 * \c linked_list_chunk is a pointer to a hidden structure (\c main structure). 
*/
typedef struct  inner_linked_chunk_struct{
  inner_linked_chunk* precedent ;
  inner_linked_chunk* suivant;
  chunk value;
} inner_linked_chunk_struct;


typedef struct linked_list_chunk_struct{
	inner_linked_chunk* first;
	inner_linked_chunk* last;
}linked_list_chunk_struct;

/*!
 * Generate an empty \c linked_list_chunk
 *
 * \return an empty \c linked_list_chunk
 */
linked_list_chunk linked_list_chunk_create ( void )  { 
	linked_list_chunk l = (linked_list_chunk)malloc(sizeof(struct linked_list_chunk_struct));
	l->first = NULL;
	l->last = NULL;
	return l ; 
}

/*!
 * Destroy the whole structure and the stored values.
 *
 * \param llc \c linked_list_chunk to destroy
 * \pre \c llc is valid (assert-ed)
 */
void linked_list_chunk_destroy ( linked_list_chunk llc )  {
	inner_linked_chunk* buffILC;
	while(llc->first != NULL){
		buffILC = llc->first;
		llc->first = llc->first->suivant;
		buffILC->precedent = NULL;
		chunk_destroy(buffILC->value);
		buffILC->value = NULL;
		buffILC->suivant = NULL;
		free(buffILC);
		buffILC = NULL;
	}
	free(llc);
	llc->first = NULL;
	llc->last = NULL;
	llc = NULL;

}


/*!
 * To know whether a \c linked_list_chunk is empty.
 *
 * \param llc \c linked_list_chunk to test
 * \pre \c llc is valid (assert-ed)
 * \return true iff \c llc is empty
 */
bool linked_list_chunk_is_empty ( linked_list_chunk llc)  { 
	return(llc->first == NULL);
}


/*!
 * To print a \c linked_list_chunk.
 * Each chink is printed on a separate line with \c chunk_print.
 *
 * \param llc \c linked_list_chunk to print
 * \param f stream to print to
 * \pre \c llc is valid (assert-ed)
 * \pre \c f is not \c NULL (assert-ed)
 */
void linked_list_chunk_print ( linked_list_chunk llc ,
				      FILE * f )  {
	assert(llc != NULL);
	inner_linked_chunk* ilc = llc->first;
	while(ilc != NULL){
		chunk_answer_message(ilc->value , "print", f);
		fprintf(f,"\n");
		ilc = ilc->suivant;
	}
}

/*!
 * Add a chunk at the beginning of the \c linked_list_chunk.
 *
 * \param llc \c linked_list_chunk to add to
 * \param ch \c chunk to add
 * \pre \c llc is valid (assert-ed)
 * \pre \c ch is not \c NULL (assert-ed)
 */
void linked_list_chunk_add_front ( linked_list_chunk llc ,
					  chunk ch )  {
	assert(llc != NULL);
	assert(ch != NULL);
	inner_linked_chunk* ilc = malloc(sizeof(inner_linked_chunk));
	ilc->precedent = NULL;
	ilc->suivant = llc->first;
	ilc->value = ch;
	if(llc->first !=  NULL){
		llc->first->precedent = ilc;
	}
	else{
		llc->last = ilc;
	}
	llc->first = ilc;
}

/*!
 * Add a \c chunk at the end of the \c linked_list_chunk.
 *
 * \param llc \c linked_list_chunk to add to
 * \param ch \c chunk to add
 * \pre \c llc is valid (assert-ed)
 * \pre \c ch is not \c NULL (assert-ed)
 */
void linked_list_chunk_add_back ( linked_list_chunk llc ,
					 chunk ch )  {
	assert(llc != NULL);
	assert(ch != NULL);
	inner_linked_chunk* ilc = (inner_linked_chunk*)malloc(sizeof(struct inner_linked_chunk_struct));
	ilc->suivant = NULL;
	ilc->precedent = llc->last;
	ilc->value = ch;
	
	if(llc->first !=  NULL){
		llc->last->suivant = ilc;
	}
	else{
		llc->first = ilc;
	}
	llc->last = ilc;
}

/*!
 * Return the \c chunk at the beginning of the \c linked_list_chunk.
 * The \c chunk is removed from the \c linked_list_chunk.
 *
 * \param llc \c linked_list_chunk to pop from
 * \return The removed \c chunk at the beginning or \c NULL if linked_list_chunk empty
 */
chunk linked_list_chunk_pop_front ( linked_list_chunk llc )  { 
	assert(llc != NULL);
	if(linked_list_chunk_is_empty(llc)){
		return NULL;
	}
	chunk ch = llc->first->value;
	if(llc->first == llc->last){
		llc->last = NULL;
		llc->first = NULL;
	}
	else{
		llc->first = llc->first->suivant;
		llc->first->precedent =  NULL;
	}
	return ch; 
}


/*!
 * Add a \b copy of the \c k first \c chunk at the beginning of the \c linked_list_chunk to it-self.
 * If there is less than \c k \c chunk then no copy is made.
 *
 * For \c k, the following \c linked_list_chunk
 * \verbatim [front]                                     ch0 ch_1 ch_2 ...  ch_k-2 ch_k-1   ch_k ch_k+1 ch_k+2 \endverbatim 
 * is transformed into
 * \verbatim [front]  ch0 ch_1 ch_2 ...  ch_k-2 ch_k-1   ch0 ch_1 ch_2 ...  ch_k-2 ch_k-1   ch_k ch_k+1 ch_k+2 \endverbatim 
 *
 * \param llc \c linked_list_chunk to add to
 * \param k size of the prefix to copy in front
 * \pre \c llc is valid (assert-ed)
 * \return false if there where less than k element. In such a case, no copy is made.
 */
bool linked_list_chunk_add_self_copy_front ( linked_list_chunk llc ,
						    unsigned int k )  { 
	inner_linked_chunk* buff = llc->first;
	for(unsigned int i = 0; i<k-1 ; i++){
		if(buff == NULL){
			return false;
		}
		buff = buff->suivant;
	}
	for(unsigned int i = 0; i<k; i++){
		linked_list_chunk_add_front(llc , chunk_copy(buff->value));
		buff = buff->precedent;
	}
	return true;
 }


/*!
 * Generate a copy of the \c linked_list_chunk.
 *
 * \param llc \c linked_list_chunk to copy
 * \pre \c llc is valid (assert-ed)
 */linked_list_chunk linked_list_chunk_copy ( linked_list_chunk llc )  {
 	linked_list_chunk llc2 = linked_list_chunk_create();
 	inner_linked_chunk* buff = llc->first;
 	while(buff != NULL){
	 	linked_list_chunk_add_back(llc2 , chunk_copy(buff->value));
	 	buff = buff->suivant;
 	}
 	return llc2; 
  }


