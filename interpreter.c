# include <stdio.h>
# include <assert.h>
# include <stdlib.h>


# include "value.h"
# include "operator.h"
# include "read_chunk_io.h"
# include "interpreter.h"
# include "value_block.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION




void interprete_chunk ( chunk ch, interpretation_context ic ){

  if( chunk_is_value(ch) ){
    if( (*(basic_type*)ch->state).type == t_error ){
      // Gestion de l'erreur
    }else{
      linked_list_chunk_add_front(ic->stack, ch);
    }

  }else{

    if( chunk_is_operator(ch) ){
      chunk_answer_message(ch, "operator_evaluate");
    }
  }
}

void interprete_chunk_list ( linked_list_chunk ch, interpretation_context ic ){


}

void interprete ( FILE * f, bool do_trace ){
  chunk ToInterprete;
  ToInterprete = read_chunk_io(f);
  interpretation_context ic = malloc(sizeof(struct interpretation_context_struct));
  ic->stack = linked_list_chunk_create();
  ic->dic = dictionary_create();
  while( ( (*(basic_type*)ToInterprete->state).type == t_error ) && 
	 (*(basic_type*)ToInterprete->state).value.val_long_long_int  == 2 ) {

    if( (*(basic_type*)ToInterprete->state).type == t_pointer ){
      /*
      if( (get_message_action_block()) == (ToInterprete->reactions) ){
	interprete_chunk_list(*(linked_list_chunk*)ToInterprete->state, ic); 
      }else{
	interprete_chunk(ToInterprete, ic); 
      }
      */
    }else{
      interprete_chunk(ToInterprete, ic);
    }
    ToInterprete = read_chunk_io(f);

  }  
}



