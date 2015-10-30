# include <stdlib.h>
# include <stdio.h>
# include <assert.h>


# include "value_protected_label.h"

# include "macro_value_c.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file 
 * \brief \c value to hold a constant label protected/inactivated.
 *
 * A label is a letter followed by a limited number of characters among: letters, digits and '_'.
 * A label may not be a keyword (or reserved word of the language).
 *
 * \c value_protected_label are read and printed in the following form: \c '\\' followed by the label.
 * For example: \c \Bob and \c\in_34
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

const message_action value_protected_label_reactions [] = {
	MESSAGE_ACTION__BASIC_VALUE(protected_label),
  { NULL, NULL }
};

typedef struct {
  basic_type basic_type;
} value_protected_label_state;

basic_type value_protected_label_print(chunk const toprint, va_list va){
  FILE * f = va_arg ( va , FILE * );
  fprintf(f , "\\" );
  sstring_print(basic_type_get_pointer( *(basic_type*) (toprint->state) ),f);
  return basic_type_void; 
}

basic_type value_protected_label_destroy(chunk const todestroy, va_list va){ 
  sstring_destroy((sstring)basic_type_get_pointer(*(basic_type*)(todestroy->state)));
  todestroy->state = NULL;
  todestroy->reactions = NULL;
  free(todestroy);
  return basic_type_void;
}

basic_type value_protected_label_copy(chunk const origin, va_list va){
  chunk ch = value_protected_label_create(sstring_copy(basic_type_get_pointer(*(basic_type*)(origin->state))));
  return basic_type_pointer(ch);
}

basic_type value_protected_label_get_value(chunk const ToGet, va_list va){
  if(ToGet->state != NULL){
    return *(basic_type*)ToGet->state;
  }else{
    basic_type res = basic_type_error; // a tester
    return res;
  }
}
VALUE_DECLARE( protected_label , sstring ) 

