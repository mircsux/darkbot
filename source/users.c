#include "defines.h"
#include "vars.h"
#include "prototypes.h"

void
scan_chan_users (char *chan, char *nick, char *banned)
{
	struct userlist *c;

	c = userhead;
	if (banned[0] == '*' && banned[1] == '!' && banned[2] == '*' && banned[3] == '\0')
	{
		S ("MODE %s -ob %s %s\n", chan, nick, banned);
		return;
	}
#if KICK_ON_BAN == 1
	while (c)
	{
		if (!match_wild (banned, c->uh) == 0)
		{
			if (stricmp (c->nick, Mynick) != 0)
			{
				S ("KICK %s %s :BANNED\n", chan, c->nick);
			}
			else
			{
				S ("MODE %s -ob %s %s\n", chan, nick, banned);
				return;
			}
		}
		c = c->next;
	}
#endif
}

void
delete_user (const char *nick, char *chan)
{
	struct userlist *pNode, *pPrev;

	pNode = userhead;
	pPrev = NULL;

	while (pNode)
	{
		if (stricmp (pNode->nick, nick) == 0 && stricmp (pNode->chan, chan) == 0)
		{
			save_seen (pNode->nick, pNode->uh, pNode->chan);
			if (pPrev != NULL)
			{
				pPrev->next = pNode->next;
			}
			else
			{
				userhead = pNode->next;
			}
			free (pNode);
			pNode = NULL;
			break;
		}
		pPrev = pNode;
		pNode = pNode->next;
	}
}

void
add_user (char *chan, char *nick, char *uh, long tog)
{
	/* toggle of 0 means to unidle the client */
	struct userlist *n, *c;

	c = userhead;
	if (strlen (uh) > 399)
		uh[399] = '\0';
	while (c)
	{							/* don't readd data that already exists */
		if (tog == 0)
		{
			if (stricmp (c->nick, nick) == 0 && stricmp (c->chan, chan) == 0)
			{
				c->idle = time (NULL);
			}
		}
		if (tog == 1)
		{
			if (stricmp (c->nick, nick) == 0 && stricmp (c->chan, chan) == 0)
			{
				/* If user is somehow already here, just update his data instead
				   of readding */
				strncpy (c->chan, chan, sizeof (c->chan));
				strncpy (c->uh, uh, sizeof (c->uh));
				strlwr (c->uh);
				strncpy (c->nick, nick, sizeof (c->nick));
				c->idle = time (NULL);
				c->level = 0;
				return;
			}
		}
		c = c->next;
	}
	if (tog == 0)
	{
		/* all we wanted to do was unidle, so we can quit now */
		return;
	}
	n = malloc (sizeof (struct userlist));
	if (n == NULL)
	{
		db_log ("error.log", "AHHH! No ram left! in add_user!\n");
		return;
	}
	memset (n, 0, sizeof (struct userlist));
	if (n != NULL)
	{
		strncpy (n->chan, chan, sizeof (n->chan));
		strncpy (n->uh, uh, sizeof (n->uh));
		strlwr (n->uh);
		strncpy (n->nick, nick, sizeof (n->nick));
		n->idle = time (NULL);
		n->level = 0;

		n->next = userhead;
		userhead = n;
	}
}

void
delete_user_ram (char *source, char *uh)
{
	struct helperlist *pNode, *pPrev;

	pNode = helperhead;
	pPrev = NULL;
	while (pNode)
	{
		if (stricmp (pNode->uh, uh) == 0)
		{
			L015 (source, pNode->uh, pNode->level, pNode->num_join);
			if (pPrev != NULL)
			{
				pPrev->next = pNode->next;
			}
			else
				helperhead = pNode->next;
			free (pNode);
			pNode = NULL;
			break;
		}
		pPrev = pNode;
		pNode = pNode->next;
	}
	save_changes ();
}

char	*mask_from_userhost (char *uh)
{
	char	*ptr1 = NULL;
	char	*ptr2 = NULL;
	char	*user	= NULL;
	char	*host	= NULL;
	char	userhost	[STRING_LONG] = {0};
	size_t	nParts		= 0;

	if (userhost == NULL)
		return(" ");
	
	strcpy (userhost, mask_tmp);
	
	if ((user = strtok (userhost, "@")) == NULL)
		return (" ");

	// Strip off any tilde on the username.
	if (*user ==  '~')
		user++;

	// Hostname
	host = strtok (NULL, " ");
	
	// If there are less than three parts to this hostname, return
	// the full hostname.
	if ((nParts = count_char	(host, '.')) < 3)
	{	
		snprintf (mask_tmp, sizeof (mask_tmp), "*%s@%s", user, host); 
		return (mask_tmp);
	}
	
	// If the host is composed entirely of numbers and dots, assume
	// it is a numeric IP. Parse that accordingly (203.203.203.*)
	if (strspn (host, "1234567890.") == strlen (host))
	{
		reverse (host);
		ptr1 = strtok (host, ".");
		ptr1 = strtok (NULL, " ");
		reverse (ptr1);
		snprintf (mask_tmp, sizeof (mask_tmp), "*%s@%s.*", user, ptr1);
		return (mask_tmp);
	}
	
	// It has to be an actual hostname now, so let's parse it.
	reverse (host);
	ptr2 = strtok (host, ".");
	ptr1 = strtok (NULL, ".");
	reverse (ptr2);
	reverse (ptr1);
	snprintf (mask_tmp, sizeof (mask_tmp), "*%s@*.%s.%s", user, ptr1, ptr2);
	return (mask_tmp);
}

char	*uh_from_nick (char *who, const char *chan)
{
	const struct	userlist *c = userhead;
	for (; c != NULL; c = c->next)
	{
		if (!stricmp (who, c->nick) && !stricmp (chan, c->chan))
		{
			strcpy (mask_tmp, c->uh);
			return (mask_tmp);
		}
	}
	return (0);
}

char	*mask_from_nick	(char *who, const char *chan)
{
	char	*s;

	if ((s = uh_from_nick(who, chan)) == NULL)
		return (who);
	return (mask_from_userhost (s));
}

