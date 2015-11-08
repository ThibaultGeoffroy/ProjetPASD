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
# include "operator_print.h"
# include "operator_print_stack.h"
# include "operator_print_dictionary.h"
# include "operator_addition.h"
# undef NDEBUG   // FORCE ASSERT ACTIVATION

#define MAXADDEDLENGTH 11


void interprete_chunk ( chunk ch, interpretation_context ic ){

  if( chunk_is_value(ch) ){
    if( (*(basic_type*)ch->state).type == t_error ){
      // Gestion de l'erreur
    }else{
      
      linked_list_chunk_add_front(ic->stack, ch);
    }
    if(ic->do_trace){
      printf("==**== reading: ");
      chunk_print(ch, stdout);
      printf(" (value)\n"); 
      printf("vvvvvvvv stack  top  vvvvvvvvvv\n");
      linked_list_chunk_print( ic->stack , stdout);
      printf("^^^^^^^^ stack bottom ^^^^^^^^^\n");      
    }
    return;
  }


  if( chunk_is_operator(ch) ){

    chunk_answer_message(ch, "operator_evaluate", ic);
    if(ic->do_trace){
      printf("==**== reading: ");
      chunk_print(ch, stdout);
      printf(" (operator)\n"); 
      printf("vvvvvvvv stack  top  vvvvvvvvvv\n");
      linked_list_chunk_print( ic->stack , stdout);
      printf("^^^^^^^^ stack bottom ^^^^^^^^^\n");
    }
    return;
    }  
}



void interprete_chunk_list (linked_list_chunk ch, interpretation_context ic ){
  chunk chunk_l = (chunk)malloc(sizeof(struct chunk_struct));
  
  while(!linked_list_chunk_is_empty(ch)){
    chunk_l = linked_list_chunk_pop_front(ch); 
    if(chunk_is_value(chunk_l)){
      
      if( (*(basic_type*)chunk_l->state).type == t_pointer ){
	
	if( (value_block_reactions) == (chunk_l->reactions)){
	  basic_type tocast = *(basic_type*)chunk_l->state;
	  interprete_chunk_list(( (linked_list_chunk)tocast.value.val_pointer ), ic); 
	}
      }
    }
    interprete_chunk(chunk_l, ic);
  }
}


void interprete ( FILE * f, bool do_trace ){
  chunk ToInterprete = (chunk)malloc(sizeof(struct chunk_struct));
  ToInterprete = read_chunk_io(f);
  interpretation_context ic = malloc(sizeof(struct interpretation_context_struct));
  ic->stack = linked_list_chunk_create();
  ic->dic = dictionary_create();
  ic->do_trace = do_trace;
  while( chunk_is_value(ToInterprete) || chunk_is_operator(ToInterprete) ){
    /*  
      if(chunk_is_value(ToInterprete)){
      
	if( (*(basic_type*)ToInterprete->state).type == t_pointer ){
	  
	  if( (value_block_reactions) == (ToInterprete->reactions)){
	    basic_type tocast = *(basic_type*)ToInterprete->state;

	     if(ic->do_trace){
	       printf("==**== reading: ");
	       chunk_print(ToInterprete, stdout);
	       printf(" (value)\n"); 
	     }

	    interprete_chunk_list(( (linked_list_chunk)tocast.value.val_pointer ), ic); 
    
	    if(ic->do_trace){
	      printf("vvvvvvvv stack  top  vvvvvvvvvv\n");
	      linked_list_chunk_print( ic->stack , stdout);
	      printf("^^^^^^^^ stack bottom ^^^^^^^^^\n");
	    }
   
	  }
	}
      }
    */
    interprete_chunk(ToInterprete, ic);
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
}



