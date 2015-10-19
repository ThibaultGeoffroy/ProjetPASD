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
VALUE_DECLARE( int, long long int )


void chunk_int_print(chunk toprint, FILE *f){ // Que doit on print ? state / methode associer au message ... ?
  
}

void chunk_int_destroy(chunk *todestroy){ // fonction a tester, voir destruction necessaire pour message action et state, verif interférence avec copy)
  /* param const comment le free , besoins de le free ?
  free((*todestroy)->reactions);
  */
(*todestroy)->reactions = NULL;
  free((*todestroy)->state);
  (*todestroy)->state = NULL;
  free((*todestroy));
  *todestroy = NULL;
  free(todestroy);
  todestroy = NULL;
  return ;
}

chunk chunk_int_copy(chunk origin){
  chunk res = malloc(sizeof(struct chunk_struct));
  res->reactions = origin->reactions;
  res->state = origin->state;
  return res;
}

bool chunk_is_int(chunk ToTest){ 
  if(chunk_is_value(ToTest)){ // Test acces a la méthode
    return value_is_int(ToTest);
  }else{
    return false;
  }
}

basic_type value_int_get_value(chunk ToGet){
  if(ToGet->state != NULL){
    return *(basic_type*)ToGet->state;
  }else{
    basic_type res = basic_type_error; // a tester
    return res;
  }
}

message_action* getmessage_int(){
 message_action tabaction [] = {MESSAGE_ACTION__BASIC_PARAM(chunk_, int)};
  return tabaction;
}
