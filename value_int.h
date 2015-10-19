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
void chunk_int_print(chunk toprint, FILE *f);

void chunk_int_destroy(chunk *todestroy);

chunk chunk_int_copy(chunk origin);

bool chunk_is_int(chunk ToTest);

basic_type value_int_get_value(chunk ToGet);

message_action* getmessage_int();
# endif

