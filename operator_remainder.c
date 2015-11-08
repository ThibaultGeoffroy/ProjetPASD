# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_remainder.h"
# include "macro_operator_c.h"
# include "value_int.h"

# include "value_error.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c %: replace the two \c value_int's on top of the stack by the remainder of their integer division.
 *
 * The \c value's must be \c value_int.
 * Result is a \c value_int.
 * The \c value_int's are destroyed.
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
  static basic_type operator_remainder_evaluate ( chunk const ch , va_list va ) {	
  	interpretation_context ic = va_arg( va , interpretation_context);	
	chunk ch1 = linked_list_chunk_pop_front(ic->stack); 
	chunk ch2 = linked_list_chunk_pop_front(ic->stack); 
	chunk ch3 = NULL; 
	if (ch1 ==  NULL || ch2 == NULL){
		return basic_type_error;
	}
	if(value_is_int(ch1) && value_is_int(ch2)){
		ch3 = value_int_create(basic_type_get_long_long_int(value_get_value(ch1))%basic_type_get_long_long_int(value_get_value(ch2)));
		linked_list_chunk_add_front(ic->stack, ch3);
		chunk_destroy(ch1);
		chunk_destroy(ch2);
		return basic_type_void;
	}
	chunk_destroy(ch1);
	chunk_destroy(ch2);
	return basic_type_error;

}
//OPERATOR_NUMBER(remainder , %)
OPERATOR_BASIC_FULL( remainder , "remainder")
//OPERATOR_DECLARE(remainder)

