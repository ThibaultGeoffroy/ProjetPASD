# include <stdlib.h>
# include <stdio.h>
# include <assert.h>


# include "value_sstring.h"

# include "macro_value_c.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION




/*!
 * \file 
 * \brief \c value to hold a \c sstring.
 *
 * \c value_sstring are read and printed with \c '"' delimiters like <tt>"ed fe"</tt>.
 * On input, use \c "\"" for a \c '"', \c '\n' for a newline, \c '\t' for a tabulation and \c '\\' for a backspace char.
 * Protective \c '\\' are not restored on printing (i.e. follows the \c print of \link sstring\endlink)
 *
 * There is \em no \em limit to the length of a string.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */
const message_action value_sstring_reactions [] = {
	MESSAGE_ACTION__BASIC_VALUE(sstring),
  { NULL, NULL }
};


typedef struct {
  basic_type basic_type;
} value_sstring_state;

basic_type value_sstring_print(chunk const toprint, va_list va){
  FILE * f = va_arg ( va , FILE * ) ;
  fprintf(f ,"\"");
  sstring_print(basic_type_get_pointer( *(basic_type*) (toprint->state) ),f);
  fprintf(f ,"\"");
  return basic_type_void; 
}

basic_type value_sstring_destroy(chunk const todestroy, va_list va){ 
  sstring_destroy((sstring)basic_type_get_pointer(*(basic_type*)(todestroy->state)));
  todestroy->state = NULL;
  todestroy->reactions = NULL;
  free(todestroy);
  return basic_type_void;
}

basic_type value_sstring_copy(chunk const origin, va_list va){
  chunk ch = value_sstring_create(sstring_copy(basic_type_get_pointer(*(basic_type*)(origin->state))));
  return basic_type_pointer(ch);
}

basic_type value_sstring_get_value(chunk const ToGet, va_list va){
  if(ToGet->state != NULL){
    return *(basic_type*)ToGet->state;
  }else{
    basic_type res = basic_type_error; // a tester
    return res;
  }
}
VALUE_DECLARE ( sstring , sstring ) 
