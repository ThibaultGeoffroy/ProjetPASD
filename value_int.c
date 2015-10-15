# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "value_int.h"

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
VALUE_DECLARE( int , long long int ) 

void print();
void destroy();
chunk copy();

boolean chunk_is_int(chunk ToTest){ 
  if(chunk_is_value(ToTest)){ // Test acces a la méthode
    return value_is_int(ToTest);
  }else{
    return false;
  }
}

boolean value_is_int(chunk ToTest){
  if(isdigit(ToTest->state->value) != 0){ // tester acces champs value sur basic_type
    return true;
  }else{
    return false;
  }
}

basic_type value_int_get_value(chunk ToGet){
  if(ToGet->state != NULL){
    return ToGet->state;
  }else{
    basic_type res = basic_type_error; // a tester
    return res;
  }
}

chunk value_int_create(basic_type Type_c){
  chunk res = malloc(sizeof(struct chunk_struct));
  res->message_action = MESSAGE_ACTION__BASIC_VALUE(int); // Verif acces au #define de value
  res->state = NULL; // Verif initialisation de la valeur
  return res;
}
