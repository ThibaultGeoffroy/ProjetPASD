# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_while.h"

# include "macro_operator_c.h"

# include "linked_list_chunk.h"

# include "operator_label.h"
# include "value_boolean.h"
# include "value_block.h"
# include "value_protected_label.h"

# include "value_error.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c while: as long as the fist \c value on the top evaluates to true evaluate the previous argument. 
 *
 * The first two \c value's are removed and used each time on the remaining of the stack.
 * At the end, they are destroyed.
 *
 * The two values on top must be \c values_block or \c values_protected_label.
 * Any \c values_protected_label is evaluated as the corresponding \c operator_label.
 * They are re-evaluated each time.
 *
 * The first one should leave a \c value_boolean on top of the stack (any other modification due to the evaluation remains).
 * \li If the value is true, then the other \c value is evaluated and then the process is restarted.
 * \li If the value is false, then the evaluation is finished.
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
static basic_type operator_while_evaluate ( chunk const ch ,va_list va ) {
	interpretation_context ic = va_arg( va , interpretation_context); 
    chunk ch1 = linked_list_chunk_pop_front(ic->stack); 
    chunk ch2 = linked_list_chunk_pop_front(ic->stack); 
    if(ch1 == NULL || ch2 == NULL){
    	return basic_type_error;
    }
    if(value_is_block(ch1)){
    	interprete_chunk_list(basic_type_get_pointer(value_get_value(ch1)), ic);
    }
    else if( value_is_protected_label(ch1)){
    	chunk_answer_message(ch1, "operator_evaluate" , ic);
    }
    chunk_destroy(ch1);
    ch1 = linked_list_chunk_pop_front(ic->stack);
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
    	operator_while_evaluate(ch , va);
    	return basic_type_void;

    }
    chunk_destroy(ch1);
    chunk_destroy(ch2);
	return basic_type_error;
}
OPERATOR_BASIC_FULL(while , "while")


