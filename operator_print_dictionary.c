# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_print_dictionary.h"
# include "macro_operator_c.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c print_dictionary: print the dictionary on \c stdout.
 *
 * For example: \verbatim
vvvvvvvv dictionary vvvvvvvvvv
"Be" => true
"Fe" => {
"tt"
pop
}
^^^^^^^^ dictionary ^^^^^^^^^\endverbatim
 *
 * Nothing is modified.
 * 
 * assert is enforced.
 * 
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */
 static basic_type operator_print_dictionary_evaluate ( chunk const ch ,	va_list va ) {	
   interpretation_context ic = va_arg( va , interpretation_context);
   dictionary_print(ic->dic, stdout);
   return basic_type_void;
}
OPERATOR_BASIC_FULL(print_dictionary , "print_dictionary")


