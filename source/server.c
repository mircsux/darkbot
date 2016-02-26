#include "defines.h"
#include "vars.h"
#include "prototypes.h"

void
gs26 (void)
{
	long i = 0;
	struct sl124 *c;

	c = sh124;
	spr++;
	if (spr > snr)
		spr = 1;
	while (c != NULL)
	{
		i++;
		if (i == spr)
		{
			strncpy (BS, c->name, sizeof (BS));
			BP = c->port;
			strncpy (BPASS, c->pass, sizeof (BPASS));			
		}
		c = c->next;
	}
}

void	add_s25			(char *server, long port, char *pass)
{
	struct sl124 *n;

	n = malloc (sizeof (struct sl124));
	if (n == NULL)
	{
		db_log ("error.log", "AHHH! No ram left! in add_s25!\n");
		return;
	}
	memset (n, 0, sizeof (struct sl124));
	snr++;
	if (n != NULL)
	{
		strncpy (n->name, server, sizeof (n->name));
		n->port = port;
		if (pass != NULL)
		strncpy (n->pass, pass, sizeof (n->pass));
		n->next = sh124;
		sh124 = n;
	}
}
