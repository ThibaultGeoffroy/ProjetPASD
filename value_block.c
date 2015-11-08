# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <assert.h>

# include "value_block.h"

# include "macro_value_c.h"
# undef NDEBUG   // FORCE ASSERT ACTIVATION

const message_action value_block_reactions [] = {
 	MESSAGE_ACTION__BASIC_VALUE(block),
  { NULL, NULL }
};


typedef struct {
  basic_type basic_type;
} value_block_state;

/*!
 * \file 
 * \brief \c value used to hold an \c linked_list_chunk.
 *
 * The \c linked_list_chunk can be accessed and manipulated directly with \link value_block_get_list() \endlink.
 * Thus \c value_block is \em mutable.
 *
 * Copies are distinct (deep copy of the structure) so that acting on one does not modify any copy.
 *
 * Its output is like:
 * \verbatim
 { 
 "Bob"
 3
 +
 \Bob
 def
 true
 } \endverbatim
 *
 * In input, spaces and newlines are meaningless as long as the order of \c chunk's are preserved and they are separated, as in any \c pf program.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */
basic_type value_block_print(chunk const toprint, va_list va){
	FILE * f = va_arg ( va , FILE * ) ;
	fprintf(f, "{\n");
	linked_list_chunk_print(basic_type_get_pointer(*(basic_type*) (toprint->state)), f);	
	fprintf(f, "}");
	return basic_type_void; 
}

basic_type value_block_destroy(chunk const todestroy, va_list va){ 
  linked_list_chunk_destroy(basic_type_get_pointer(*(basic_type*) (todestroy->state)));
  todestroy->state = NULL;
  todestroy->reactions = NULL;
  free(todestroy);
  return basic_type_void;
}

basic_type value_block_copy(chunk const origin, va_list va){
  chunk ch = value_block_create(linked_list_chunk_copy(basic_type_get_pointer(*(basic_type*)(origin->state))));
  return basic_type_pointer(ch);
}

basic_type value_block_get_value(chunk const ToGet, va_list va){
  if(ToGet->state != NULL){
    return *(basic_type*)ToGet->state;
  }else{
    basic_type res = basic_type_error; // a tester
    return res;
  }
}

VALUE_DECLARE ( block , linked_list_chunk )


/*!
 * Return the \c linked_list_chunk held.
 *
 * This is not a copy but a direct access.
 *
 * \param vb chunk to query
 * \pre \c vb must be a \c value_block (assert-ed)
 * \return the \c linked_list_chunk held
 */
linked_list_chunk value_block_get_list ( chunk const vb )  {
	return((linked_list_chunk)basic_type_get_pointer(*(basic_type*)(vb->state)));
 }


