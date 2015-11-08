# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_pop.h"
# include "macro_operator_c.h"


# include "value_error.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c pop: remove the top \c value from the stack.
 *
 * The \c value is destroyed.
 * Nothing else is modified.
 * 
 * If the stack is empty, a \c basic_type_error is returned.
 *
 * assert is enforced.
 * 
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */
 static basic_type operator_pop_evaluate ( chunk const ch , va_list va ) {
 	interpretation_context ic = va_arg( va , interpretation_context); 
    chunk ch1 = linked_list_chunk_pop_front(ic->stack);
    if(!(ch1)){
    	return basic_type_error;
    }
    if(ch1){
    	chunk_destroy(ch1);
    	return basic_type_void;
    }
    return basic_type_error;
 }
 
OPERATOR_BASIC_FULL(pop, "pop")


