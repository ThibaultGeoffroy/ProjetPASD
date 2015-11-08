# ifndef __MACRO_OPERATOR_C_H
# define __MACRO_OPERATOR_C_H


# include "value_error.h"

# undef OPERATOR_DECLARE

# define OPERATOR_DECLARE( type_name )				\
  chunk operator_ ## type_name ## _create () {			\
    return NULL ;						\
  }								\
  bool operator_is_ ## type_name ( chunk const ch ) {		\
    return true ;						\
  }


/*
 * Except for operator label, all are identifcal.
 * This is simplified into making only one static instance of the operator for each kind.
 * Creation and destruction are thus handled regularly as any global variable.
 */
# define GIVE(a , b , c , op)  ch3 = value_ ## a ## _create(basic_type_get_ ## b ##(value_get_value(ch1)) op basic_type_get_ ## c ## (value_get_value(ch2)));



# define OPERATOR_BASIC_FULL( op_name , op )				\
  									\
  static basic_type operator_ ## op_name ## _print ( chunk const ch , va_list va ){ \
  FILE * f = va_arg(va, FILE *) ;					\
  char * printop = op;	  						\
  fprintf(f, "%s", printop);						\
  return basic_type_void ;						\
  }									\
									\
static basic_type operator_ ## op_name ## _destroy ( chunk const ch ,	\
						       va_list va ) {	\
  free(ch);								\
    return basic_type_void ;						\
  }									\
									\
static basic_type operator_ ## op_name ## _copy ( chunk const ch ,	\
						  va_list va ) {	\
    chunk ch2 = malloc(sizeof(struct chunk_struct));			\
    /*ch2 = &(operator_ ## op_name ## _instance); */			\
    return basic_type_pointer(ch2);					\
  }									\
									\
  static const message_action operator_ ## op_name ## _reactions [] = {	\
    MESSAGE_ACTION__BASIC_OPERATOR( op_name ) ,				\
    { NULL, NULL }							\
  } ;									\
									\
  static chunk_struct operator_ ## op_name ## _instance = {		\
    .state = NULL ,							\
    .reactions = operator_ ## op_name ## _reactions  } ;		\
  									\
  chunk operator_ ## op_name ## _create () {				\
    chunk ch = malloc(sizeof(struct chunk_struct));			\
    ch = &(operator_ ## op_name ## _instance);				\
    return ch;								\
  }									\
									\
  bool operator_is_ ## op_name ( chunk const ch ) {			\
    return (ch->reactions[0].action == (operator_ ## op_name ## _reactions)->action) ; 		\
  }



# define OPERATOR_NUMBER( op_name , op )				\
  static basic_type operator_ ## op_name ## _evaluate ( chunk const ch , \
						       va_list va ) {	\
    interpretation_context ic = va_arg( va , interpretation_context);	\
  chunk ch1 = linked_list_chunk_pop_front(ic->stack); \
  chunk ch2 = linked_list_chunk_pop_front(ic->stack); \
  chunk ch3;					      \
  if(value_is_int(ch1) && value_is_int(ch2) ){				\
    ch3 = value_int_create(basic_type_get_long_long_int(value_get_value(ch1)) op basic_type_get_long_long_int(value_get_value(ch2))); \
  }									\
  else if(value_is_double(ch1) && value_is_double(ch2)){ \
    ch3 = value_double_create(basic_type_get_long_double(value_get_value(ch1)) op basic_type_get_long_double(value_get_value(ch2))); \
  }									\
  else if(value_is_double(ch1) && value_is_int(ch2)){			\
    ch3 = value_double_create(basic_type_get_long_double(value_get_value(ch1)) op basic_type_get_long_long_int(value_get_value(ch2))); \
  } \
  else if(value_is_int(ch1) && value_is_double(ch2)){			\
    ch3 = value_double_create(basic_type_get_long_long_int(value_get_value(ch1)) op basic_type_get_long_double(value_get_value(ch2))); \
  }									\
  else{									\
    return basic_type_error ;						\
  } \
  chunk_destroy(ch1); \
  chunk_destroy(ch2);			       \
  linked_list_chunk_add_front(ic->stack, ch3); \
  return basic_type_void;		       \
  }

# define OPERATOR_BOOLEAN( op_name , op )				\
  static basic_type operator_ ## op_name ## _evaluate ( chunk const ch , \
							va_list va ) {	\
    return basic_type_error ;						\
  }									\
  									\
  OPERATOR_BASIC_FULL( op_name , op )



# define OPERATOR_COMPARATOR( op_name , op )				\
  static basic_type operator_ ## op_name ## _evaluate ( chunk const ch ,	\
						       va_list va ) {	\
    return basic_type_error ;						\
  }


# define OPERATOR_EQUALITY( op_name , op )				\
  static basic_type operator_ ## op_name ## _evaluate ( chunk const ch ,	\
						       va_list va ) {	\
    return basic_type_error ;					        \
  }									\
									\
  OPERATOR_BASIC_FULL( op_name , op )



# endif
