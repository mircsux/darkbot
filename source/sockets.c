#include "defines.h"
#include "vars.h"
#include "prototypes.h"

/**
 * Write a character array to a socket connection.
 * 6/22/00 Dan
 * Method argument now pointer to const data.
 */
int
writeln (const char *b)
{
	return (write (socketfd, b, strlen (b)) < 0) ? 0 : 1;
}

int
readln (void)
{
	char ch = 0;
	int i = 0;

	do
	{
		if (read (socketfd, &ch, 1) < 1)
			return 0;
		if (ch >= 32 || ch <= 126)
			if (i < 524 - 1)
				L[i++] = ch;
	}
	while (ch != '\n');
	L[i] = '\0';
	return 1;
}

/**
 * 6/23/00 Dan
 * - Changed method argument to be pointer to const data
 * - Initialized variables when declared
 * - Changed b to a power of 2
 */
void
S (const char *format, ...)
{
	va_list arglist;
	char b[STRING_LONG] = { 0 };
	struct sendq *n = 0;

	va_start (arglist, format);
	vsprintf (b, format, arglist);
	va_end (arglist);

	if (send_tog == 0)
	{
		send_tog = 1;
		if (DebuG == 1)
		{
			printf ("OUT: %s\n", b);
		}
		writeln (b);
		return;
	}

	n = malloc (sizeof (struct sendq));
	if (n == NULL)
	{
		db_log ("error.log", "AHH! no ram left! in S!\n");
		return;
	}

	memset (n, 0, sizeof (struct sendq));
	strncpy (n->data, b, sizeof (n->data));

	if (sendqhead == NULL)
	{
		sendqhead = sendqtail = n;
	}
	else
	{
		sendqtail->next = n;
		sendqtail = sendqtail->next;
	}
}

/**
 * 6/23/00 Dan
 * - Changed method argument to be pointer to const data
 * - Initialized b
 */
int
Snow (const char *format, ...)
{
	va_list arglist;
	char b[STRING_LONG] = { 0 };

	va_start (arglist, format);
	vsprintf (b, format, arglist);
	va_end (arglist);
	if (DebuG == 1)
		printf ("OUT: %s\n", b);
	return (writeln (b));
}

int
Send ()
{
	struct sendq *c;
	char output[STRING_LONG];
	c = sendqhead;
	get_sendq_count (0);
	if (c == NULL)
	{
		send_tog = 0;
		return -1;
	}
	if (DebuG == 1)
		printf ("OUT: %s\n", c->data);
#ifdef	DEBUG2
	db_log ("darkbot_debug.log", "OUT: %s\n", c->data);
#endif
	strncpy (output, c->data, sizeof (output));
	del_sendq (0);
	return (writeln (output));
}
