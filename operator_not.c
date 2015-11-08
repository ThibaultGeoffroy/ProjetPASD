# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_not.h"
# include "macro_operator_c.h"

# include "value.h"
# include "value_boolean.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c !: replace the \c value_boolean on top of the stack by its negation.
 *
 * The \c value_boolean is destroyed.
 * 
 * If the stack is not deep enough or a wrong kind of \c value is found, then a \c basic_type_error is returned.
 *
 * assert is enforced.
 * 
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


static basic_type operator_not_evaluate ( chunk const ch , va_list va ) { 
	interpretation_context ic = va_arg( va , interpretation_context);
	chunk ch1 =linked_list_chunk_pop_front(ic->stack);
	if(!(ch1)){
    	return basic_type_error;
    }
	if(value_is_boolean(ch1)){
		linked_list_chunk_add_front(ic->stack ,value_boolean_create(!basic_type_get_boolean(value_get_value(ch1))));
		chunk_destroy(ch1);
		return basic_type_void;
	}
	return basic_type_error;
}
OPERATOR_BASIC_FULL(not , "not")


