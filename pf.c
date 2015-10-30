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
char* buff2 = "-t";
FILE * input = fopen(argv[1], "r");
interprete(input, false);
/*if(argc == 4){

if(strcmp(argv[1], buff1) == 0 || strcmp(argv[2], buff1) == 0){
help_message ( argv [ 0 ] ) ;
}
if(strcmp(argv[1], buff2) == 0 || strcmp(argv[2], buff2) == 0){
interprete(input, false);
}else{
interprete(input, false);
}

}else{

    if(strcmp(argv[1], buff1) == 0){
      help_message ( argv [ 0 ] ) ;
    }

    if(strcmp(argv[1], buff2) == 0){
interprete(input, false);
    }else{
interprete(input, false);
} 	      
}*/
return 0;
}
