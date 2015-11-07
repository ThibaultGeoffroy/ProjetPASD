# ifndef __VALUE_INT_H
# define __VALUE_INT_H

# include <stdbool.h>

# include "value.h"

# include "macro_value.h"


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


VALUE_DECLARE( int , long long int ) 


/*!
 * Get the value from a value_int.
 * The value is a basic_type of type t_long_long_int, representing a long long int.
 */
basic_type value_int_get_value(chunk const ToGet, va_list va);

/*!
 * Copy the value_int, return his value.
 * The returned value is a basic_type of type t_long_long_int, containing a long long int.
 */
basic_type value_int_copy(chunk const origin, va_list va);

/*!
 * Destroy the value_int contained by the state of the chunk, then the chunk itself.
 */
basic_type value_int_destroy(chunk const todestroy, va_list va);

/*!
 * Print the value of the chunk state containging a long_long_int .
 * Printing is done on a specified output via the va_list
 */
basic_type value_int_print(chunk const toprint, va_list va);



# endif

