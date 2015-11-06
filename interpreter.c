# include <stdio.h>
# include <assert.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include "value.h"
# include "operator.h"
# include "sstring.h"
# include "read_chunk_io.h"
# include "interpreter.h"
# include "value_block.h"
# include "dictionary.h"
# include "chunk.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION

#define MAXADDEDLENGTH 11

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

void interprete_chunk_list (linked_list_chunk ch, interpretation_context ic ){


}

void interprete ( FILE * f, bool do_trace ){
  chunk ToInterprete = (chunk)malloc(sizeof(struct chunk_struct));
  ToInterprete = read_chunk_io(f);
  interpretation_context ic = malloc(sizeof(struct interpretation_context_struct));
  ic->stack = linked_list_chunk_create();
  ic->dic = dictionary_create();
  while( !( ( (*(basic_type*)ToInterprete->state).type == t_error)  && 
	    ( (*(basic_type*)ToInterprete->state).value.val_long_long_int  == 2 ) ) ) {
    
    if( (*(basic_type*)ToInterprete->state).type == t_pointer ){
      
      if( (value_block_reactions) == (ToInterprete->reactions)){
        basic_type tocast = *(basic_type*)ToInterprete->state;
        interprete_chunk_list(( (linked_list_chunk)tocast.value.val_pointer ), ic); 
      }else{
	interprete_chunk(ToInterprete, ic); 
      }
      
    }else{
      interprete_chunk(ToInterprete, ic);
    }
    
    // GESTION TRACE

    if(do_trace){
      basic_type toprint = *(basic_type*)ToInterprete->state;
      if(chunk_is_value(ToInterprete)){
	if( toprint.type == t_pointer ){
	  sstring toprint_s = (sstring)((toprint.value).val_pointer);
	    printf("==**== reading: \\");
	    sstring_print(toprint_s, stdout);
	    printf(" (value)\n");
	}
      
	if( toprint.type == t_true ){
	  printf("==**== reading: true (value)\n");
	} 
	if( toprint.type == t_false ){
	  printf("==**== reading: false (value)\n");
	}
	if( toprint.type == t_long_double ){
	  printf("==**== reading: %LF (value)\n", toprint.value.val_long_double);
	}
	if( toprint.type == t_long_long_int ){
	  printf("==**== reading: %lld (value)\n", toprint.value.val_long_long_int);
	}
      }else{  
	if(chunk_is_operator(ToInterprete)){
	  printf("==**== reading: (operator)\n" );
	}
      }
      
      printf("vvvvvvvv stack  top  vvvvvvvvvv\n");
      linked_list_chunk_print( ic->stack , stdout);
      printf("^^^^^^^^ stack bottom ^^^^^^^^^\n");
    }

    // FIN GESTION TRACE

    ToInterprete = read_chunk_io(f);
    if(ToInterprete == NULL){
      if(do_trace){
	printf("======= dictionnary ==============\n");
	dictionary_print(ic->dic, stdout);
      }
      printf("======== final stack =============\n");
      linked_list_chunk_print( ic->stack , stdout);
      chunk_destroy(ToInterprete);
      linked_list_chunk_destroy(ic->stack);
      dictionary_destroy(ic->dic);
      return ;
    }
  }
  chunk_destroy(ToInterprete);
  linked_list_chunk_destroy(ic->stack);
  dictionary_destroy(ic->dic);
}



