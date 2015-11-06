# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <assert.h>
# include "interpreter.h"
# include <string.h>
# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief THE MAIN FILE!
 *
 * Arguments:
 * \li \c -h help message (and exit)
 * \li \c -t to trace (can be turned off by operator \c stop_trace)
 * \li \c file.pf file to interprete (otherwise it is stdin)
 *
 * \author Jérôme DURAND-LOSE
 * \year 2015
 */

/*!
 * print help message and exit.
 */
static void help_message ( char const * const prog_name ) {
  puts ( "USAGE:" ) ;
  printf ( " %s -h\n\tDisplay this message and exit\n" , prog_name ) ;
  printf ( " %s [OPTIONS] [FILE]\n\tRun the pf interpreter on [FILE] (standard input if void)\n" , prog_name ) ;
  puts ( "OPTIONS:" ) ;
  puts ( " -t to trace the execution" ) ;
  exit ( 0 ) ;
}


/*!
 * THE MAIN FUNCTION
 */
int main ( int const argc , char const * const argv [] ) {
  char* buff1 = "-h";
  bool trace = false;
  char* buff2 = "-t";
  FILE * input;
  for(int x = 1; x < argc; x++){
    if(strcmp(argv[x], buff1) == 0){
      help_message ( argv [ 0 ] ) ;
    }else{
      if(strcmp(argv[x], buff2) == 0){
	trace = true;
      }else{
	input = fopen(argv[x], "r");
      }
    }
  }
 interprete(input, trace);
 fclose(input);
 return 0;
}
 
