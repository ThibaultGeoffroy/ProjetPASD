# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "interpreter.h"
# include "operator_if_else.h"
# include "operator_label.h"
# include "macro_operator_c.h"

# include "value_protected_label.h"
# include "value_boolean.h"
# include "value_block.h"
# include "value_error.h"



# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c if_else: when evaluated depending the boolean on top of the stack activate next or following value on the stack.
 *
 * The \c value on top  must be a \c value_boolean and the two \c value's under mus be \c value_block or \c value_protected_label.
 * If the evaluated \c value is a \c value_block, the chunks evaluated in order.
 * If the evaluated \c value is a \c value_protected_label, then it as evaluated as the corresponding \c operator_label.
 * All three \c value's are destroyed.
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
static basic_type operator_if_else_evaluate ( chunk const ch ,va_list va ) {
	interpretation_context ic = va_arg( va , interpretation_context); 
    chunk ch1 = linked_list_chunk_pop_front(ic->stack); 
    chunk ch2 = linked_list_chunk_pop_front(ic->stack);
    chunk ch3 = linked_list_chunk_pop_front(ic->stack);
    if(ch1 == NULL || ch2 == NULL || ch3 == NULL){
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
    			chunk_destroy(ch3);
    			return basic_type_error;
    		}
    	}
    	else{
    		if(value_is_block(ch3)){
    			interprete_chunk_list(basic_type_get_pointer(value_get_value(ch3)), ic);
    		}
    		else if(value_is_protected_label(ch3)){
    			chunk_answer_message(ch3 , "operator_evaluate" , ic );
    		}
    		else{
    			chunk_destroy(ch1);
    			chunk_destroy(ch2);
    			chunk_destroy(ch3);
    			return basic_type_error;
    		}
    	}
    	chunk_destroy(ch1);
    	chunk_destroy(ch2);
    	chunk_destroy(ch3);
    	return basic_type_void;

    }
    chunk_destroy(ch1);
    chunk_destroy(ch2);
    chunk_destroy(ch3);
	return basic_type_error;
}
OPERATOR_BASIC_FULL(if_else , "if_else")


