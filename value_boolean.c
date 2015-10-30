# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "value_boolean.h"

# include "macro_value_c.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION




/*!
 * \file 
 * \brief \c value used to hold a boolean.
 *
 * For I/O, they appear as keyword \c true and \c false.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


/*!
 * Keyword for boolean constant \c true. 
 * Defined for I/O.
 */
# define VALUE_BOOLEAN_STRING_TRUE "true"


/*!
 * Keyword for boolean constant \c false. 
 * Defined for I/O.
 */
# define VALUE_BOOLEAN_STRING_FALSE "false"

const message_action value_boolean_reactions [] = {
  MESSAGE_ACTION__BASIC_VALUE(boolean)
};

typedef struct {
  basic_type basic_type;
} value_boolean_state;

basic_type value_boolean_print(chunk const toprint, va_list va){
  bool value = basic_type_get_boolean( *(basic_type*) (toprint->state) );
  FILE * f = va_arg ( va , FILE * ) ;
  fprintf ( f
	    , "%s"
	    , value ? VALUE_BOOLEAN_STRING_TRUE : VALUE_BOOLEAN_STRING_FALSE);
  return basic_type_void; 
}

basic_type value_boolean_destroy(chunk const todestroy, va_list va){ 
  free((basic_type*)(todestroy->state));
  todestroy->state = NULL;
  todestroy->reactions = NULL;
  free(todestroy);
  return basic_type_void;
}

basic_type value_boolean_copy(chunk const origin, va_list va){
  chunk ch = value_boolean_create(basic_type_get_boolean(*(basic_type*)(origin->state)));
  return basic_type_pointer(ch);
}

basic_type value_boolean_get_value(chunk const ToGet, va_list va){
  if(ToGet->state != NULL){
    return *(basic_type*)ToGet->state;
  }else{
    basic_type res = basic_type_error; 
    return res;
  }
}

VALUE_DECLARE ( boolean , bool )


