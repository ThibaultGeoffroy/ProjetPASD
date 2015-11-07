# ifndef __VALUE_SSTRING_H
# define __VALUE_SSTRING_H

# include "value.h"
# include "sstring.h"

# include "macro_value.h"


/*!
 * \file 
 * \brief \c value to hold a \c sstring.
 *
 * \c value_sstring are read and printed with \c '"' delimiters like <tt>"ed fe"</tt>.
 * On input, use \c "\"" for a \c '"', \c '\n' for a newline, \c '\t' for a tabulation and \c '\\' for a backspace char.
 * Protective \c '\\' are not restored on printing (i.e. follows the \c print of \link sstring\endlink)
 *
 * There is \em no \em limit to the length of a string.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


VALUE_DECLARE ( sstring , sstring) 
/*!
 * Get the value from a value_sstring.
 * The value is a basic_type_pointer, pointing to a sstring_struct.
 */
basic_type value_sstring_get_value(chunk const ToGet, va_list va);

/*!
 * Copy the value_sstring, return his value.
 * The returned value is a basic_type_pointer, pointing to a sstring_struct.
 */
basic_type value_sstring_copy(chunk const origin, va_list va);

/*!
 * Destroy the sstring contained by the state of the chunk, then the chunk itself.
 */
basic_type value_sstring_destroy(chunk const todestroy, va_list va);

/*!
 * Print the value of the chunk state containging an sstring.
 * Printing is done on a specified output via the va_list
 */
basic_type value_sstring_print(chunk const toprint, va_list va);


# endif
