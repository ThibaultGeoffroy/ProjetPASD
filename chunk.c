# include <stdio.h>
# include <string.h>
# include <assert.h>

# include "chunk.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION


basic_type chunk_answer_message ( chunk const ch ,
				  char const * const message ,
				  ...
				  ) {
  assert ( NULL != ch ) ;
  assert ( NULL != message ) ;
  va_list va ;
  message_action const * rep = ch -> reactions ;
  // Default is message not found
  basic_type bt = basic_type_error ;
  va_start ( va , message ) ;
  assert ( NULL != va ) ;
  while ( NULL != rep -> message ) {
    if ( 0 == strcmp ( message , rep -> message ) ) {
      bt = ( * ( rep -> action ) ) ( ch , va ) ;
      break ;
    }
    rep ++ ;
  }
  va_end ( va ) ;
  if (t_long_long_int !=  ((basic_type*) (ch->state))->type ) {
    printf("dans le ANSWER");
  }
  return bt ;
}


chunk chunk_copy ( chunk ch ) {
  assert ( NULL != ch ) ;
  assert (NULL != ch->state);
  basic_type bt = chunk_answer_message ( ch , MESSAGE_COPY ) ;
  return basic_type_get_pointer ( bt ) ;
}


basic_type chunk_destroy ( chunk ch ) {
  return ( NULL == ch )
    ? basic_type_error
    : chunk_answer_message ( ch , MESSAGE_DESTROY ) ;
}


basic_type chunk_print ( chunk const ch,
			 FILE * f ) {
  assert ( NULL != ch ) ;
  assert ( NULL != f ) ;

  if (t_long_long_int !=  ((basic_type*) (ch->state))->type ) {
    printf("avant le answer_message");
  }
  basic_type bt  = chunk_answer_message ( ch , MESSAGE_PRINT , f ) ;
  if (t_long_long_int !=  ((basic_type*) (ch->state))->type ) {
    printf("après le answer_message");
  }
  return bt;
}
