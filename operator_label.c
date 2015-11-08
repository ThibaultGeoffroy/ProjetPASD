# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_label.h"
# include "macro_operator_c.h"
# include "operator.h"
# include "value_block.h"
# include "dictionary.h"
# include "value_error.h"
# include "interpreter.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c label: when evaluated it is replaced by the corresponding \c value from the \c dictionary.
 *
 * If the associated \c value is a \c value_block, then the \c chunk's are processed in order.
 * Otherwise, it is processed as usually.
 *
 * Label can be created even if the entry is not defined in the dictionary.
 * It should be defined when evaluated.
 * 
 * assert is enforced.
 * 
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

/*!
 * Create an \c operator_label.
 *
 * \param ss non empty string
 * \pre \c ss in not \ NULL and non-empty (assert-ed)
 * \return a newly created \c operator_label
 */

 static basic_type operator_label_evaluate ( chunk const ch ,va_list va ) {
 	interpretation_context ic = va_arg( va , interpretation_context); 
 	chunk ch2 = dictionary_get_copy(ic->dic , (sstring)(basic_type_get_pointer(value_get_value(ch))));
 	if(ch2 == NULL){
 		return basic_type_error;
 	}
 	if(chunk_is_value(ch2)){
 		if(value_is_block(ch2)){
 			interprete_chunk_list(basic_type_get_pointer(value_get_value(ch2)), ic);
 		}
 		linked_list_chunk_add_front(ic->stack, ch2);
 	}
 	else if(chunk_is_operator(ch2)){
 		chunk_answer_message(ch2 , "operator_evaluate", ic);
 	}
	return basic_type_void;
}

static basic_type operator_label_print ( chunk const ch , va_list va ){
  FILE * f = va_arg(va, FILE *) ;
  sstring_print((sstring)(ch->state) , f);						
  return basic_type_void ;						
}

static basic_type operator_label_destroy ( chunk const ch , va_list va ) {
	sstring_destroy((sstring)(ch->state));
	free(ch);								
	return basic_type_void ;
}

static basic_type operator_label_copy ( chunk const ch , va_list va ) {	
    chunk ch2 = operator_label_create((sstring)(ch->state));		
    return basic_type_pointer(ch2);					
}

 static const message_action operator_label_reactions [] = {
 	MESSAGE_ACTION__BASIC_OPERATOR( label ),
	{NULL , NULL}
};
chunk operator_label_create ( sstring  ss ) {		
    chunk res = ( chunk ) malloc( sizeof(struct chunk_struct) );	
    res -> state = malloc ( sizeof (sstring) );
    res->reactions = operator_label_reactions;
    res->state = sstring_copy(ss);
	return res;							
  }			


/*!
 * Test whether a chunk is an operator_label
 *
 * \param ch cunk to test
 * \return true iff it is an operator_label
 */


bool operator_is_label ( chunk const ch )  { 
	return ch->reactions == operator_label_reactions ; 
}

