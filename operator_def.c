# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_def.h"
# include "macro_operator_c.h"

# include "value_protected_label.h"
# include "value_error.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c def: add an entry into the \c dictionary.
 *
 * The entry is composed of a protected label on the top of the stack and the value under it.
 * Both values are removed from the stack.
 * Nothing else is modified.
 * 
 * If the stack is not deep enough or the first one is not a \c value_protected_label, a \c basic_type_error is returned.
 *
 * assert is enforced.
 * 
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */
 static basic_type operator_def_evaluate ( chunk const ch , va_list va ) {
	interpretation_context ic = va_arg( va , interpretation_context);
    chunk ch1 = linked_list_chunk_pop_front(ic->stack);
    chunk ch2 = linked_list_chunk_pop_front(ic-> stack);
    if(!(ch1) || !(ch2)){
    	return basic_type_error;
    }
    if(value_is_protected_label(ch1)){
    	dictionary_set(ic->dic , basic_type_get_pointer(chunk_answer_message(ch1 , "value_get_value")) , ch2);
    	chunk_destroy(ch1);
    	chunk_destroy(ch2);
    	return basic_type_void;
    }
    chunk_destroy(ch1);
    chunk_destroy(ch2);
    return basic_type_error;
} 
OPERATOR_BASIC_FULL ( def , "def")




