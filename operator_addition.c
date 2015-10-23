# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_addition.h"
# include "macro_operator_c.h"

# include "value_double.h"
# include "value_int.h"

# include "value_error.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c +: replace the two \c value's on top of the stack by their sum.
 *
 * The \c value's must be integer or float.
 * The result is an integer \c value iff both \c value's are integer.
 * The \c value's are destroyed.
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




static basic_type operator_addition_print ( chunk const ch ,	
						     va_list va ) {	
    return basic_type_error ;						
  }									
									
  static basic_type operator_addition_destroy ( chunk const ch ,	
						       va_list va ) {	
    return basic_type_error ;						
  }									
									
  static basic_type operator_addition_copy ( chunk const ch ,	
						    va_list va ) {	
    return basic_type_void ;						
  }	
static basic_type operator_addition_evaluate( chunk const ch , va_list va ) {
	interpretation_context ic = va_arg( va , interpretation_context);
	chunk ch1 = linked_list_chunk_pop_front(ic->stack);
	chunk ch2 = linked_list_chunk_pop_front(ic->stack);
	chunk ch3;
	if(value_is_int(ch1) && value_is_int(ch2) ){
		ch3 = value_int_create(basic_type_get_long_long_int(value_get_value(ch1)) + basic_type_get_long_long_int(value_get_value(ch2)));	}
	else if(value_is_double(ch1) && value_is_double(ch2)){
		ch3 = value_double_create(basic_type_get_long_double(value_get_value(ch1)) + basic_type_get_long_double(value_get_value(ch2)));
	}
	else if(value_is_double(ch1) && value_is_int(ch2)){
		ch3 = value_double_create(basic_type_get_long_double(value_get_value(ch1)) + basic_type_get_long_long_int(value_get_value(ch2)));
	}
	else if(value_is_int(ch1) && value_is_double(ch2)){
		ch3 = value_double_create(basic_type_get_long_long_int(value_get_value(ch1)) + basic_type_get_long_double(value_get_value(ch2)));
	}
	else{
		return basic_type_error ;						
	}
	chunk_destroy(ch1);
	chunk_destroy(ch2);
	linked_list_chunk_add_front(ic->stack, ch3);
	return basic_type_void;
  }

OPERATOR_DECLARE ( addition ) 

static const message_action operator_addition_reactions [] = {
    MESSAGE_ACTION__BASIC_OPERATOR( addition ) ,				
    { NULL, NULL }							
  } ;


