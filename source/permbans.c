#include "defines.h"
#include "vars.h"
#include "prototypes.h"

/**
 * Add a permban to the permban list.
 * 6/23/00 Dan:
 * - Both pointer variables are now received as pointer to const data
 * - Changed counter to type size_t, this should be an unsigned type
 * - Initialiazed n to 0 on declaration
 * - Added support for dynamically allocated uh and reason fields
 *   in the struct permban list
 * - Did some extra memory leak prevention
 */

void
add_permban (const char *uh, size_t counter, const char *reason)
{
	struct permbanlist *n = 0;

	n = malloc (sizeof (struct permbanlist));
	if (n == NULL)
	{
		db_log ("error.log", "AHHH! no ram left! in add_permban!\n");
		return;
	}

	memset (n, 0, sizeof (struct permbanlist));
	n->uh = db_strndup (uh, STRING_SHORT);
	if (NULL == n->uh)
	{
		db_log ("error.log", "add_permban> Memory allocation failure\n");
		/* Prevent memory leaks */
		free (n);
		return;
	}

	n->reason = db_strndup (reason, STRING_SHORT);
	if (NULL == n->reason)
	{
		db_log ("error.log", "add_permban> Memory allocation failure\n");
		/* Prevent memory leaks */
		free (n->uh);
		free (n);
		return;
	}

	strlwr (n->uh);
	n->counter = counter;
	PERMBAN_counter++;
	n->next = permbanhead;
	permbanhead = n;
}

/**
 * Check if a permban exists for a given uh/channel/nick set.
 * 6/23/00 Dan:
 * - Changed all method arguments to be pointers to const data
 * - Return type is now bool, returns true if ban is found,
 *   false otherwise
 */
bool
check_permban (const char *uh, const char *chan, const char *nick)
{
	char tmpBuf[STRING_SHORT + 1] = { 0 };
	struct permbanlist *c = permbanhead;
    size_t len = 0;

    len = min ( STRING_SHORT, strlen (uh) );

	strncpy (tmpBuf, uh, len);
    tmpBuf[len] = '\0';
	strlwr (tmpBuf);
	for (; c != NULL; c = c->next)
	{
		if (!match_wild (c->uh, tmpBuf) == 0)
		{
			c->counter++;
			S ("MODE %s +b %s\n", chan, c->uh);
			S ("KICK %s %s :\2[\2%d\2]\2: %s\n", chan, nick, c->counter, c->reason);
			return true;
		}
	}
	return false;
}

#if DO_CHANBOT_CRAP == 1

/**
 * Remove a permban based on nickname and user@host.
 * 6/23/00 Dan:
 * - Both arguments are now pointers to const data
 * - Added free() for both pNode->uh and pNode->reason now
 *   that the permbanlist has dynamically allocated fields
 * - Changed type of toggle from long to bool
 * - Changed name of toggle variable to (foundBan)
 * - permbanlist pointers are now initialized when declared
 * - Added if statement at end of method, this will only save
 *   the bans if a ban was removed
 */
int
del_permban (const char *nick, const char *uh)
{
	bool foundBan = false;
	struct permbanlist *pNode = permbanhead, *pPrev = 0;

	while (pNode)
	{
		if (stricmp (pNode->uh, uh) == 0)
		{
			L002 (nick, PERMBAN_counter, uh);
			PERMBAN_counter--;
			if (pPrev != NULL)
			{
				pPrev->next = pNode->next;
			}
			else
			{
				permbanhead = pNode->next;
			}

			free (pNode->uh);
			free (pNode->reason);
			free (pNode);
			foundBan = true;
			pNode = NULL;
			break;
		}
		pPrev = pNode;
		pNode = pNode->next;
	}

	if (foundBan)
	{
		/* Only need to save bans if ban list has changed */
		save_permbans ();
	}
	return foundBan;
}
#endif

#if DO_CHANBOT_CRAP == 1

/**
 * Save the permban list to file.
 * 6/23/00 Dan:
 * - the permbanlist pointer (c) is now pointer to const, because
 *   this is a read only method, and that is a read only variable
 * - Initialized c when it is declared
 */
void
save_permbans (void)
{
	const struct permbanlist *c = permbanhead;

	remove (TMP_FILE);

#ifdef	WIN32
	printf ("*** Writing permbans: %s (%s)\n", PERMBAN, date ());
#endif

	for (; c != NULL; c = c->next)
	{
		db_log (TMP_FILE, "%s %d %s\n", c->uh, c->counter, c->reason);
	}
	rename (TMP_FILE, PERMBAN);

	if (PERMBAN_counter == 0)
		remove (PERMBAN);
}
#endif

/**
 * 6/22 Dan
 * - Changed DATA to be 512 bytes, a power of 2
 * - DATA now initialized properly
 * - c is now a pointer to const, this is a read only method
 * - c is now initialized where declared
 * - Changed type of i, x to size_t, these variables should be
 *   unsigned.
 * - Changed while loop to for loop.
 * - Changed reinitialization of DATA to use memset()
 */

void
show_banlist (const char *nick)
{
	char DATA[STRING_SHORT * 7] = { 0 };
	char tmp [STRING_SHORT] = { 0 };
	size_t i = 0,	x = 0;
	const struct permbanlist *c = 0;

	for (c = permbanhead; c != NULL; c = c->next)
	{
		i++;
		++x;
		snprintf (tmp, sizeof(tmp), "%s", DATA);
		snprintf (DATA, (sizeof(DATA) + sizeof(tmp)), "%s %s:%ud", 
			tmp, c->uh, c->counter);
		memset (tmp, 0, sizeof(tmp));

		//if (i > 8)
		if (i > 6)
		{
			S ("NOTICE %s :%s\n", nick, DATA);
			i = 0;
			memset (DATA, 0, sizeof (DATA));
			db_sleep (2);
		}
	}
	S ("NOTICE %s :%s\n", nick, DATA);
	S ("NOTICE %s :End of PERMBAN list; %d ban%s found.\n", nick, x, (x == 1) ? "" : "s");
}

