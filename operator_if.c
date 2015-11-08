# include <stdlib.h>
# include <stdio.h>
# include <assert.h>


# include "basic_type.h"

# include "operator_if.h"

# include "macro_operator_c.h"
# include "interpreter.h"

# include "value_boolean.h"
# include "value_block.h"
# include "value_protected_label.h"

# include "value_error.h"

# include "operator_label.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c if: when evaluated depending the \c value_boolean on top of the stack activates or not the next value on the stack.
 *
 * Both values are destroyed.
 * Value on top must be a \c value_boolean and under should be a \c value_block or \c value_protected_label.
 * If this last \c value is a \c value_block, the chunks evaluated in order.
 * If this last \c value is a \c value_protected_label, then it as evaluated as the corresponding \c operator_label.
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
static basic_type operator_if_evaluate ( chunk const ch ,va_list va ) {
	interpretation_context ic = va_arg( va , interpretation_context); 
    chunk ch1 = linked_list_chunk_pop_front(ic->stack); 
    chunk ch2 = linked_list_chunk_pop_front(ic->stack); 
    if(ch1 == NULL || ch2 == NULL){
    	return basic_type_error;
    }
    if(value_is_boolean(ch1)){
    	if(basic_type_get_boolean(value_get_value(ch1))){
    		if(value_is_block(ch2)){
    			interprete_chunk_list(basic_type_get_pointer(value_get_value(ch2)), ic);
    		}
    		else if(value_is_protected_label(ch2)){
    			chunk_answer_message(ch2 , "operator_evaluate" , ic );
    		}
    		else{
    			chunk_destroy(ch1);
    			chunk_destroy(ch2);
    			return basic_type_error;
    		}
    	}
    	chunk_destroy(ch1);
    	chunk_destroy(ch2);
    	return basic_type_void;

    }
    chunk_destroy(ch1);
    chunk_destroy(ch2);
	return basic_type_error;
}
OPERATOR_BASIC_FULL(if , "if")


