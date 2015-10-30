# ifndef __CHUNK_VALUE_BOOLEAN_H
# define __CHUNK_VALUE_BOOLEAN_H

# include <stdbool.h>

# include "value.h"

# include "macro_value.h"


/*!
 * \file 
 * \brief \c value used to hold a boolean.
 *
 * For I/O, they appear as keyword \c true and \c false.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


/*!
 * Keyword for boolean constant \c true. 
 * Defined for I/O.
 */
# define VALUE_BOOLEAN_STRING_TRUE "true"


/*!
 * Keyword for boolean constant \c false. 
 * Defined for I/O.
 */
# define VALUE_BOOLEAN_STRING_FALSE "false"

basic_type value_boolean_print(chunk const toprint, va_list va);

basic_type value_boolean_destroy(chunk const todestroy, va_list va);

basic_type value_boolean_copy(chunk const origin, va_list va);

basic_type value_boolean_get_value(chunk const ToGet, va_list va);

VALUE_DECLARE ( boolean , bool )


# endif
