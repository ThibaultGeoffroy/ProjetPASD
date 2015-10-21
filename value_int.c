# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "value_int.h"
# include "stdbool.h"
# include "macro_value_c.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION

/*!
 * \file 
 * \brief \c value to hold an \c int.
 * 
 * Integers are read and printed in the usual way:
 * \li '-' sign if negative (nothing for positive) followed by
 * \li sequence of digits.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

// FAIRE DECLARATION DANS .H

const message_action value_int_reactions [] = {
  MESSAGE_ACTION__BASIC_VALUE(int),
  {NULL, NULL}

};

typedef struct {
  basic_type basic_type;
} value_int_state;


basic_type value_int_print(chunk const toprint, va_list va){
  long long int value = basic_type_get_long_long_int( *(basic_type*) (toprint->state) );
  FILE * f = va_arg ( va , FILE * ) ;
  fprintf ( f
	    , "%lld"
	    , value );
  return basic_type_void; 
}

basic_type value_int_destroy(chunk const todestroy, va_list va){ 
  free((basic_type*)(todestroy->state));
  todestroy->state = NULL;
  todestroy->reactions = NULL;
  free(todestroy);
  return basic_type_void;
}

basic_type value_int_copy(chunk const origin, va_list va){
  chunk ch = value_int_create(basic_type_get_long_long_int(*(basic_type*)(origin->state)));
  return basic_type_pointer(ch);
}

basic_type value_int_get_value(chunk const ToGet, va_list va){
  if(ToGet->state != NULL){
    return *(basic_type*)ToGet->state;
  }else{
    basic_type res = basic_type_error; // a tester
    return res;
  }
}

VALUE_DECLARE( int, long long int )
