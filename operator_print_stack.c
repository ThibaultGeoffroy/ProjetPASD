# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_print_stack.h"
# include "macro_operator_c.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c print_stack: print the stack on \c stdout.
 *
 * For example: \verbatim
vvvvvvvv stack  top  vvvvvvvvvv
-52.580000
"zet"
{
33
pop
print
}
true
^^^^^^^^ stack bottom ^^^^^^^^^ \endverbatim
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

static basic_type operator_print_stack_evaluate ( chunk const ch ,	va_list va ) {	
   interpretation_context ic = va_arg( va , interpretation_context);
   linked_list_chunk_print(ic->stack, stdout);
   return basic_type_void;
}
OPERATOR_BASIC_FULL(print_stack , "print_stack")


