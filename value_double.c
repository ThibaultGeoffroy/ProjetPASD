# include <stdlib.h>
# include <stdio.h>
# include <assert.h>


# include "value_double.h"

# include "macro_value_c.h"



# undef NDEBUG   // FORCE ASSERT ACTIVATION!_



/*!
 * \file 
 * \brief \c value used to hold a \c double.
 *
 * For I/O these are just numbers with decimal point like 78.0 0.75 -568.58.
 * No exponential form is supported.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

const message_action value_double_reactions [] = {
	MESSAGE_ACTION__BASIC_VALUE(double),
  { NULL, NULL }
};

typedef struct {
  basic_type basic_type;
} value_double_state;

basic_type value_double_print(chunk const toprint, va_list va){
  long double value = basic_type_get_long_double( *(basic_type*) (toprint->state) );
  FILE * f = va_arg ( va , FILE * ) ;
  fprintf ( f
	    , "%.6Lf"
	    , value );
  return basic_type_void; 
}

basic_type value_double_destroy(chunk const todestroy, va_list va){ 
  free((basic_type*)(todestroy->state));
  todestroy->state = NULL;
  todestroy->reactions = NULL;
  free(todestroy);
  return basic_type_void;
}

basic_type value_double_copy(chunk const origin, va_list va){
  //assert(t_long_long_int ==  ((basic_type*) (origin->state))->type);
	chunk ch = value_double_create(basic_type_get_long_double(*(basic_type*)(origin->state)));
	return basic_type_pointer(ch);
}

basic_type value_double_get_value(chunk const ToGet, va_list va){
  if(ToGet->state != NULL){
    return *(basic_type*)(ToGet->state);
  }else{
    basic_type res = basic_type_error; // a tester
    return res;
  }
}

VALUE_DECLARE( double , long double ) 

