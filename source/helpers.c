#include "defines.h"
#include "vars.h"
#include "prototypes.h"

/**
 * Add a channel helper.
 * 6/22/00 Dan
 * n now initialized where declared
 * All pointer arguments now received as pointer to const data.
 */
void
add_helper (const char *chan,
			const char *uh, long level, size_t num_join, const char *greetz, const char *pass, char mode)
{
	struct	helperlist *n = 0;
	char	*ptr = NULL;
#if	ENCRYPT_PASSWORDS == ON
	char	*salt = "8fei3k";
	
	if ( mode == 0 )
    {
        if ((ptr = crypt (pass, salt)) == NULL)	/* encrypt password */
		    return;
    }
    else
        ptr = (char *)pass;
#else
	ptr = (char *)pass;
#endif
    

	n = malloc (sizeof (struct helperlist));
	if (n == NULL)
	{
		db_log ("error.log", "AHHH! No ram left! in add_helper!\n");
		return;
	}

	memset (n, 0, sizeof (struct helperlist));
	NUM_HELPER++;
	if (chan[0] == '#')
	{
		strncpy (n->chan, chan, sizeof (n->chan));
	}
	else
	{
		strncpy (n->chan, "#*", sizeof (n->chan));
	}

	strncpy (n->uh, uh, sizeof (n->uh));
	strlwr (n->uh);
	strncpy (n->pass, ptr, sizeof (n->pass));
	n->num_join = num_join;
	n->level = level;
	strncpy (n->greetz, greetz, min (sizeof (n->greetz) - 1, strlen (greetz)));
	n->next = helperhead;
	helperhead = n;
}

/**
 * Output the helper list to a nickname.
 * 6/22, Dan:
 * - Changed helperlist* c to be a pointer to const data
 * - Changed initialization of DATA, and size to be a
 *   power of 2
 * - Added initialization of c
 * - Changed while loop to for loop
 * - Changed types of i, x to size_t since they should be
 *   unsigned.
 * - Added reinitialization of DATA using memset() (changed from
 *   strcpy(DATA,""))
 */
void
show_helper_list (const char *nick, long level)
{
	char	DATA	[STRING_SHORT * 7] = { 0 };
	char	tmp		[STRING_SHORT] = { 0 };
	size_t	i = 0,	x = 0;
	const	struct	helperlist *c = NULL;

	
	for (c = helperhead; c != NULL; c = c->next)
	{
		// If we're displaying users at all levels...
		if ((level == 0) || (level == c->level))
		{
			i++; x++;

			snprintf (tmp, sizeof(tmp), "%s", DATA);
			snprintf (DATA, (sizeof(DATA) + sizeof(tmp)),
				"%s %s[%s:%ld:%d]",
				tmp, c->uh, c->chan, c->level, c->num_join);

			memset (tmp, 0, sizeof(tmp));

			if (i > 6)
			{
				i = 0;
				S ("NOTICE %s :%s\n", nick, DATA);
				memset (DATA, 0, sizeof(DATA));
				db_sleep(2);
			}
		}
	}							/* for() */

	if (x != 0)
		S ("NOTICE %s :%s\n", nick, DATA);
	
	S ("NOTICE %s :End of Helper Userlist; %d user%s found.\n", 
		nick, x, 
		(x == 1) ? "" : "s");
}

void
load_helpers (void)
{
	FILE *fp;
	char b[STRING_LONG], *user_host, *greetz, *numb_join, *chan, *w_level, *pass;
	long num_join = 0, i = 0, level = 0;

	if ((fp = fopen (HELPER_LIST, "r")) == NULL)
	{
		printf ("Unable to open %s! Aborting connection.\n", HELPER_LIST);
		printf ("Please run ./configure to setup your darkbot.\n");
		exit (0);
	}
#ifndef	WIN32
	printf ("Loading %s file ", HELPER_LIST);
#endif
	while (fgets (b, STRING_LONG, fp))
	{
		if (b == NULL)
			continue;
		stripline (b);
		if (*b == '/')
			continue;
		i++;
		printf (".");
		fflush (stdout);
		chan = strtok (b, " ");
		if (chan == NULL)
			continue;
		user_host = strtok (NULL, " ");
		if (user_host == NULL)
			continue;
		w_level = strtok (NULL, " ");
		if (w_level == NULL)
			continue;
		numb_join = strtok (NULL, " ");
		if (numb_join == NULL)
			continue;
		pass = strtok (NULL, " ");
		if (pass == NULL)
		{
			pass = "0";			/* duh */
		}
		greetz = strtok (NULL, "");
		if (greetz == NULL)
			greetz = "I haven't used \2SETINFO\2 yet!";
		if (w_level != NULL)
			level = atoi (w_level);
		else
			level = 1;
		if (numb_join != NULL)
			num_join = atoi (numb_join);
		else
			num_join = 0;
		if (strlen (pass) > 25)
			pass[25] = '\0';
		if (DebuG == 1)
			printf
				("loading helperlist: %s %s l:%d j:%d %s\n",
				 chan, user_host, (int) level, (int) num_join, greetz);
		add_helper (chan, user_host, level, num_join, greetz, pass, 1);
	}
	printf ("done(%d), ", (int) i);
	fclose (fp);
	save_changes ();
	if (DebuG == 1)
		db_sleep (2);
}

void
set_pass (char *nick, char *uh, char *pass, char *newpass)
{
	struct	helperlist *c;
	char	*ptr = NULL;
#if	ENCRYPT_PASSWORDS == ON
	char	*salt = "8fei3k";
#endif

	c = helperhead;
	strlwr (uh);


#if	ENCRYPT_PASSWORDS == ON
	if ((ptr = crypt (pass, salt)) == NULL)	/* encrypt old password */
		return;
#else
	ptr = (char *)pass;
#endif

	while (c)
	{
		if (!match_wild (c->uh, uh) == 0)
		{
			if (strcmp (c->pass, ptr) == 0 || strcmp (c->pass, "0") == 0)
			{
#if				ENCRYPT_PASSWORDS == ON
				if ((ptr = crypt (newpass, salt)) == NULL)	/* encrypt new password */
					return;
#else
				ptr = (char *)newpass;
#endif
				strncpy (c->pass, ptr, sizeof (c->pass));
				L012 (nick, uh);
				save_changes ();
				return;
			}
			else
			{
				L013 (nick);
				return;
			}
		}
		c = c->next;
	}
	L014 (nick);
}

long
verify_pass (char *nick, char *chan, char *uh, char *pass)
{
	struct	helperlist *c;
	char	*ptr = NULL;
#if	ENCRYPT_PASSWORDS == ON
	char	*salt = "8fei3k";
#endif

	c = helperhead;
	strlwr (uh);

#if	ENCRYPT_PASSWORDS == ON
	if ((ptr = crypt (pass, salt)) == NULL)
		return 0;
#else
	ptr = (char *)pass;
#endif

	while (c)
	{
		if (!match_wild (c->uh, uh) == 0)
		{
			if (*c->pass == '0')
				return 0;		/* no pass set */
			if (strcmp (c->pass, ptr) == 0)
			{
				if (c->chan[0] == '#' && c->chan[1] == '*')
					return c->level;

				if (*chan == '*')
					return c->level;

				if (stricmp (c->chan, chan) == 0)
					return c->level;

				return 0;		/* don't match chan access */
			}
		}
		c = c->next;
	}
	return 0;
}

long
get_pass (char *data)
{
	/* returns 0 for no data */
	/* returns 1 for just pass */
	/* returns 2 for pass and data */
	char b[STRING_SHORT] = { 0 }, b2[STRING_SHORT] =
	{
	0}
	, *temp = NULL;
	long i = 0;

	strncpy (pass_data, "0", sizeof (pass_data));	/* init */
	strncpy (pass_pass, "0", sizeof (pass_pass));
	if (data == NULL)
		return 0;
	strncpy (b2, data, sizeof (b2));
	temp = strtok (data, " ");
	if (temp == NULL)
		return -1;
	strncpy (b, temp, sizeof (b));
	while (temp != NULL)
	{
		i++;
		strncpy (pass_pass, temp, sizeof (pass_pass));
		temp = strtok (NULL, " ");
		if (temp == NULL)
			break;
		snprintf (b, sizeof (b), "%s %s", b, temp);
	}
	strncpy (b, "", sizeof (b));	/* reinit */
	temp = strtok (b2, " ");
	strncpy (b, temp, sizeof (b));
	while (i > 2)
	{
		i--;
		temp = strtok (NULL, " ");
		snprintf (b, sizeof (b), "%s %s", b, temp);
	}
	if (stricmp (b, pass_pass) == 0)
	{
		strncpy (pass_data, "0", sizeof (pass_data));
		return 1;
	}
	strncpy (pass_data, b, sizeof (pass_data));
	return 2;
}

void
do_login (char *nick, char *pass)
{
	long i = 0, x = 0, D = 0;
	char Data[STRING_SHORT] = { 0 }, b[STRING_SHORT] =
	{
	0};
	struct userlist *c;

	c = userhead;
	while (c)
	{
		if (stricmp (nick, c->nick) == 0)
		{
			x = verify_pass (c->nick, c->chan, c->uh, pass);
			if (x > 0)
			{
				i++;
				if (c->level == 0 && x >= 2)
				{
#if OP_USERS_ON_LOGIN == 1
					/* only if not already authed */
					S ("MODE %s +ov %s %s\n", c->chan, c->nick, c->nick);
#endif
					D = 1;
				}
				c->level = x;
				snprintf (b, sizeof (b), "%s[%d] %s", c->chan, (int) c->level, Data);
				strncpy (Data, b, sizeof (Data));
			}
		}
		c = c->next;
	}
	if (i != 0)
	{
		if (!D)
		{
			S ("NOTICE %s :Already authed on %s\n", nick, Data);
		}
		else
			S ("NOTICE %s :Verified: %s\n", nick, Data);
	}
}

int
check_access (char *uh, char *chan, int toggle, char *nick)
{
	long i = 0, length = 0, A = 0, X = 0, Y = 0;
	struct helperlist *c;
	struct userlist *c2;
	char temp[STRING_LONG] = { 0 };

	c = helperhead;
	c2 = userhead;
	strlwr (uh);

	if (toggle == 0)
	{							/* get access level */
		while (c2)
		{
			if (stricmp (c2->uh, uh) == 0)
			{
				if (((stricmp (c2->chan, chan) == 0) || (stricmp ("#*", chan) == 0)))	/* privmsg */
				{
					if (stricmp (c2->nick, nick) == 0)
					{
						return c2->level;
					}
				}
			}
			c2 = c2->next;
		}
		return 0;				/* no matches? */
	}
	else
		while (c != NULL)
		{
			if (!match_wild (c->uh, uh) == 0)
			{
				if (*c->pass == '0')
				{
					L001 (nick, Mynick);
					return 0;
				}
				if (c->chan[1] != '*')
					if (stricmp (c->chan, chan) != 0)
						return 0;
				c->num_join++;
				if (*c->greetz == '+')
					A = 1;
				strncpy (data, "", sizeof (data));
				length = Y = strlen (c->greetz);
				if (length > 1)
				{
					while (length > 0)
					{
						length--;
						X++;
						if (c->greetz[length] == '^')
						{
							i++;
							snprintf (temp, sizeof (temp), "%s%s", nick, data);
						}
						else if (c->greetz[length] == '%')
						{
							i++;
							snprintf (temp, sizeof (temp), "%u%s", c->num_join, data);
						}
						else if (c->greetz[length] == '$')
						{
							i++;
							snprintf (temp, sizeof (temp), "%s%s", uh, data);
						}
						else if (c->greetz[length] == '&')
						{
							i++;
							snprintf (temp, sizeof (temp), "%s%s", chan, data);
						}
						else
							snprintf (temp, sizeof (temp), "%c%s", c->greetz[length], data);
						if (X == Y && A == 1)
							continue;
						strncpy (data, temp, sizeof (data));
					}			/* While */
#if JOIN_GREET == 1
					if (i == 0)
					{
						if (setinfo_lastcomm (uh) == 0)
						{
							S ("PRIVMSG %s :%ld\2!\2\37(\37%s\37)\37\2:\2 %s\n",
							   chan, c->num_join, nick, c->greetz);
						}
					}
					else if (A == 1)
					{
						if (setinfo_lastcomm (uh) == 0)
						{
							S ("PRIVMSG %s :\1ACTION %s\1\n", chan, data);
						}
					}
					else
					{
						if (setinfo_lastcomm (uh) == 0)
						{
							S ("PRIVMSG %s :%s\n", chan, data);
						}
					}
#endif
					return c->level;
				}
			}
			c = c->next;
		}
	return 0;
}
