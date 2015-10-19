# ifndef __MACRO_VALUE_C_H
# define __MACRO_VALUE_C_H


# undef VALUE_DECLARE
# define basic_type_linked_list_chunk basic_type_pointer
# define basic_type_block basic_type_pointer
# define basic_type_int basic_type_long_long_int
# define basic_type_double basic_type_long_double
# define basic_type_protected_label basic_type_pointer
# define basic_type_sstring basic_type_pointer
# define VALUE_DECLARE( type_name , type_C )				\
  chunk value_ ## type_name ## _create ( type_C const val ) {		\
    chunk res = ( chunk ) malloc(sizeof(struct chunk_struct));		\
    res->reactions = NULL;						\
    /* if(#type_C == "linked_list_chunk"){				\
      void * pt = &val;							\
      basic_type rep = basic_type_pointer(pt);				\
      }else{ */								\
    basic_type rep = basic_type_ ## type_name (val);			\
    void * value = &rep;						\
    res->state = value;							\
    return res;								\
  }									\
  bool value_is_ ## type_name ( chunk const ch ) {			\
    return true ;							\
  }


# define VALUE_IS_FULL( type_name )					\
  bool value_is_ ## type_name ( chunk const ch ) {			\
    assert ( NULL != ch ) ;						\
    return value_ ## type_name ## _reactions == ch -> reactions ;	\
  }

# endif

