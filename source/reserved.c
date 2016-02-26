#include "defines.h"
#include "vars.h"
#include "prototypes.h"

void
call_reserved_1 ( char *source, char *target, char *param )
{
    FILE *fp;
    char buffer[STRING_SHORT] = { 0 };
    char input[STRING_SHORT] = { 0 };
    
    if ( strlen ( param ) == 0 )
        return;

    snprintf ( buffer, sizeof ( buffer ), "./cdecl -a -q explain \"%s\"\n", param );

	if ( ( fp = popen ( buffer, "r" ) ) == NULL )
        return;

    fgets ( input, sizeof ( input ), fp );

    if ( strlen ( input ) > 0 )
        S ( "PRIVMSG %s :%s%s\n", target, rand_reply(source), input );

    pclose ( fp );
}

void
call_reserved_2 ( char *source, char *target, char *param )
{
    FILE *fp;
    char buffer[STRING_SHORT] = { 0 };
    char input[STRING_SHORT] = { 0 };
    
    if ( strlen ( param ) == 0 )
        return;

    snprintf ( buffer, sizeof ( buffer ), "./cdecl -a -q declare \"%s\"\n", param );

	if ( ( fp = popen ( buffer, "r" ) ) == NULL )
        return;

    fgets ( input, sizeof ( input ), fp );

    if ( strlen ( input ) > 0 )
        S ( "PRIVMSG %s :%s%s\n", target, rand_reply(source), input );

    pclose ( fp );
}
