/*	Memory Allocation Library

    Added documentation and slightly tweaked prompt_yn due to design choice

    v2.0
    Ian Chiu (ichiu@andrew.cmu.edu)
    7/17/18

*/
#include <stdio.h>
#include "toolkit.h"


/*
 * prompt_yn - prompts user to respond with a y or n for yes or no,
 *             and on an ill-formed response, returns def
 *             (the default response)
 */
boolean prompt_yn ( char *prompt, boolean def )
{
    char response;

    printf ("%s (y/n)? ", prompt);
    fflush( stdout );
    response = getchar( );

    if  ( (response == 'Y') || (response == 'y') )
        return TRUE;
    if  ( (response == 'N') || (response =='n') )
        return FALSE;
    return def;
}


/*
 * prompt_yesno - prompts user to respond with yes or no, and on an ill-formed
 *                response, loops until user provides valid response.
 */
boolean prompt_yesno ( char *prompt )
{
    char response[4];

    while ( TRUE )  {
        printf ("%s? (yes or no) ", prompt );
        fgets  ( response, 4, stdin );
        if  ( !strncasecmp( response, "yes", 3 ) )
            return TRUE;
        if  ( !strncasecmp( response, "no", 2 ) )
            return FALSE;
    }
}


