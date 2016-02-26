#include "defines.h"
#include "vars.h"
#include "prototypes.h"

#ifdef	DO_MATH_STUFF

/* Changed input to be unsigned instead of signed. This
 * suppressed warning messages when compiling on Solaris.
 */

void
do_math (const char *who, char *target, char *math)
{
	char number_string[STRING_SHORT] = { 0 };
	char op = 0;
	unsigned char input[STRING_SHORT] = { 0 };
	unsigned int index = 0;
	unsigned int to = 0;
	unsigned int input_length = 0;
	unsigned int number_length = 0;
	double result = 0.0;
	double number = 0.0;

	strncpy (input, math, sizeof (input));
	input_length = strlen (input);
	for (to = 0, index = 0; index <= input_length; index++)
		if (*(input + index) != ' ')
			*(input + to++) = *(input + index);
	input_length = strlen (input);
	index = 0;
	if (input[index] == '=')
		index++;
	else
	{
		number_length = 0;
		if (input[index] == '+' || input[index] == '-')
			*(number_string + number_length++) = *(input + index++);
		for (; isdigit (*(input + index)); index++)
			*(number_string + number_length++) = *(input + index);
		if (*(input + index) == '.')
		{
			*(number_string + number_length++) = *(input + index++);
			for (; isdigit (*(input + index)); index++)
				*(number_string + number_length++) = *(input + index);
		}
		*(number_string + number_length) = '\0';
		if (number_length > 0)
			result = atof (number_string);
	}
	for (; index < input_length;)
	{
		op = *(input + index++);
		number_length = 0;
		if (input[index] == '+' || input[index] == '-')
			*(number_string + number_length++) = *(input + index++);
		for (; isdigit (*(input + index)); index++)
			*(number_string + number_length++) = *(input + index);
		if (*(input + index) == '.')
		{
			*(number_string + number_length++) = *(input + index++);
			for (; isdigit (*(input + index)); index++)
				*(number_string + number_length++) = *(input + index);
		}
		*(number_string + number_length) = '\0';
		number = atof (number_string);
		switch (op)
		{
			case '+':
				result += number;
				break;
			case '-':
				result -= number;
				break;
			case '*':
				result *= number;
				break;
			case '/':
				if (number == 0)
				{
					L016 (target, who);
					return;
				}
				else
					result /= number;
				break;
			case '%':
				if ((long) number == 0)
				{
					L016 (target, who);
					return;
				}
				else
					result = (double) ((long) result % (long) number);
				break;
			default:
				L017 (target, who);
				return;
		}
	}
	S ("PRIVMSG %s :%s\2:\2 %f\n", target, who, result);
}
#endif

long
cf (char *host, char *nick, char *chan)
{
	int f_n = 0;

	if (check_access (host, chan, 0, nick) >= 3)
		return 0;
	f_n = f_f (host);
	if (f_n == -1)
	{
		a_f (host);
		return 0;
	}
	if (ood[f_n].value)
		return 1;
	ood[f_n].count++;
	if ((time (NULL) - ood[f_n].time) > ft)
		ood[f_n].count = 0;
	else if ((time (NULL) - ood[f_n].time) <= ft && ood[f_n].count >= fr)
	{
		ood[f_n].value = true;
		if (!ood[f_n].kick)
		{
			ood[f_n].kick = 1;
#ifdef	FLOOD_KICK
			if (*chan == '#' || *chan == '&')
			{
				L018 (chan, nick, FLOOD_REASON, fc, host);
			}
			else
				L019 (CHAN, fc, host);
#else
			if (*chan == '#' || *chan == '&')
			{
				L019 (CHAN, fc, host);
			}
			else
				L019 (CHAN, fc, host);
#endif
		}
		return 1;
	}
	ood[f_n].time = time (NULL);
	return 0;
}

/**
 * Update a nick's channel greeting and user@host.
 * 6/23/00 Dan:
 * - All method arguments are now pointers to const data
 * - Rewrote to use a for loop, and fewer variables
 * - Info is only saved to disk if changes are made
 */
void
update_setinfo (const char *new_uh, const char *new_greetz, const char *nick)
{
	struct helperlist *c = helperhead;
	bool madeChange = false;
	size_t i = 0;

	for (; c != NULL; c = c->next)
	{
		++i;
		if (!match_wild (c->uh, new_uh) == 0)
		{
			strncpy (c->greetz, new_greetz, sizeof (c->greetz));
			strlwr (c->uh);
			L020 (nick, i, c->uh, new_greetz);
			madeChange = true;
		}
	}
	if (madeChange)
	{
		save_changes ();
	}
}

/**
 * 6/23/00 Dan:
 * - All variables now initialized when declared
 * - Altered variable types to reflect usage
 */
void
info (const char *source, char *target)
{
	FILE *fp;
	clock_t starttime = 0;
	char b[STRING_LONG] = { 0 };
	size_t topics = 0, dup = 0;
	time_t t2time = 0, c_uptime = 0;

#ifdef FIND_DUPS
	char *ptr = NULL, *subj = NULL;
    char last[STRING_LONG] = { 0 };
#endif
	t2time = time (NULL);
	unlink (TMP_URL);
	starttime = clock ();
	fp = fopen (URL2, "r");
	if (NULL == fp)
	{
		L003 (source, URL2);
		return;
	}
	while (fgets (b, STRING_LONG, fp))
	{
		topics++;
#ifdef	FIND_DUPS
		if(*b == '\n')
            continue;

        stripline (b);
		subj = strtok (b, " ");
		ptr = strtok (NULL, "");
		strlwr (subj);
		if (stricmp (last, subj) == 0)
		{
			dup++;
#ifdef	SAVE_DUPS
			db_log (BACKUP_DUP, "%s %s\n", subj, ptr);
#endif
		}
		else
		{
			db_log (TMP_URL, "%s %s\n", subj, ptr);
		}
		strncpy (last, subj, sizeof (last));
		last[sizeof (last) - 1] = '\0';
#endif
	}

	fclose (fp);
	rename (TMP_URL, URL2);
#ifdef	FIND_DUPS
	if (dup > 0)
	{
		L025 (target, dup);
	}
#endif
	c_uptime = time (NULL) - uptime;
	topics -= dup;
	if (c_uptime > 86400)
	{
		L026 (target,
			  dbVersion,
			  topics,
			  c_uptime / 86400,
			  (c_uptime / 86400 ==
			   1) ? "" : "s",
			  (c_uptime / 3600) % 24,
			  (c_uptime / 60) % 60, QUESTIONS,
			  ADDITIONS, DELETIONS,
			  (double) (clock () -
						starttime) /
			  CLOCKS_PER_SEC, (((double) (clock () - starttime) / CLOCKS_PER_SEC) == 1) ? "" : "s");
	}
	else if (c_uptime > 3600)
	{
		L027 (target,
			  dbVersion,
			  topics,
			  c_uptime / 3600,
			  c_uptime / 3600 == 1 ? "" : "s",
			  (c_uptime / 60) % 60,
			  (c_uptime / 60) % 60 ==
			  1 ? "" : "s", QUESTIONS,
			  ADDITIONS, DELETIONS,
			  (double) (clock () -
						starttime) /
			  CLOCKS_PER_SEC, (((double) (clock () - starttime) / CLOCKS_PER_SEC) == 1) ? "" : "s");
	}
	else
	{
		L028 (target,
			  dbVersion,
			  topics,
			  c_uptime / 60,
			  c_uptime / 60 == 1 ? "" : "s",
			  c_uptime % 60,
			  c_uptime % 60 == 1 ? "" : "s",
			  QUESTIONS, ADDITIONS, DELETIONS,
			  (double) (clock () - starttime) / CLOCKS_PER_SEC, (((double)
																  (clock () -
																   starttime) /
																  CLOCKS_PER_SEC) == 1) ? "" : "s");
	}
/*    get_stats(target, NULL); */
}

/**
 * Output information about the bot's database to a target.
 * 6/22 Dan:
 * - Changed both method arguments to be pointers to const data,
 *   this is a read only method.
 */
void
show_info2 (const char *target, const char *source)
{
	S ("PRIVMSG %s :%s, compiled on %s. "
	   "I have processed %ld lines of text since startup...\n",
	   target, source, __DATE__, NUMLINESSEEN);
}

/**
 * 6/23/00 Dan:
 * - All method arguments are now pointer to const
 * - Return type is now time_t
 * - A for loop is now used instead of a while loop
 */
time_t
return_useridle (const char *chan, const char *who, int toggle)
{								/* toggle=0 is for idle time, toggle=1 is to check if user
								   is in the chan */
	const struct userlist *c = userhead;

	for (; c != NULL; c = c->next)
	{
		if (!stricmp (who, c->nick) && !stricmp (chan, c->chan))
		{
			if (toggle == 1)
			{
				/* If we only care if user is present or not.. */
				return 1;
			}
			else
				return c->idle;
		}						/* if */
	}							/* for */
	return 0;
}

void
process_nick (char *nick, char *newnick)
{
	struct userlist *c;

	c = userhead;
	newnick++;
	while (c)
	{
		if (stricmp (nick, c->nick) == 0)
		{
			strncpy (c->nick, newnick, sizeof (c->nick));
		}
		c = c->next;
	}
}

/**
 * 6/23/00 Dan:
 * - All method arguments are now pointer to const
 * - A for loop is now used instead of a while loop
 */
void
show_chaninfo (const char *nick, const char *chan, const char *target)
{
	size_t totalUsers = 0, foundUsers = 0;
	const struct userlist *c = userhead;

	for (; c != NULL; c = c->next)
	{
		++totalUsers;
		if (!stricmp (chan, c->chan))
			++foundUsers;
	}
	S ("PRIVMSG %s :%s, I see %d users in %s (%d users total in ram)\n",
	   	target, nick, foundUsers, chan, totalUsers);
}

/* 
 * This function displays a list of users that are listed in the bot's internal user
 * list as being on the channel pointed to by chan, to the nick pointed to by nick.
 * Each message sent to the target should be no more than 200 characters in length.
 */

void    show_chanusers  (const char *nick, const char *chan)
{
	const   struct  userlist        *c = userhead;
    char    DATA    [STRING_SHORT * 7] = {0},
            tmp     [STRING_SHORT] = {0};
    size_t  foundUsers = 0, len = 0;
        
    for (; c != NULL; c = c->next)
    {
		if (stricmp (chan, c->chan) == 0)
        {
			++foundUsers;
			

			snprintf (tmp, sizeof (tmp), "%s", DATA);
			
			/* The check for DATA being NULL is done to prevent ugly looking spaces
			 * at the beginning of the line when it's outputted.
			 */
			snprintf (DATA, (sizeof (DATA) + sizeof (tmp)), 
							"%s%s%s", 
							tmp, 
							(DATA == NULL ? "" : " "), 
							c->nick);

			/* Add the length of the new nick and room for a space to the length
			 * of the current buffer.
			 */

			len += (strlen (c->nick) + 1);

       		memset (tmp, 0, sizeof (tmp));

	        if (len >= 200)
		    {
				S ("NOTICE %s :%s\n", nick, DATA);
				len = 0;
				memset (DATA, 0, sizeof (DATA));
				db_sleep (2);
			}
		}
	}

	/* If there's any leftover data in our buffer after we've reached the end of the list,
	 * send that as well.
	 */

    if (len > 0)
		S ("NOTICE %s :%s\n", nick, DATA);

	/* Even if no users were found... */

	S ("NOTICE %s :End of CHANUSERS list; %d user%s found.\n",
					          nick, foundUsers, (foundUsers == 1 ? "" : "s"));
	
}

void
do_modes (char *source, char *data)
{
	char *chan = NULL, *mode = NULL, *nick = NULL, *ptr = NULL;
	long PM = 0, j = 0, i = 0;

	chan = strtok (data, " ");
	mode = strtok (NULL, " ");

	if ((ptr = strchr (source, '!')) != NULL)
		*ptr++ = '\0';
	j = strlen (mode);
	i = -1;						/* i needs to start at 0 */
	while (j > 0)
	{
		j--;
		i++;
		if (mode[i] == '+')
			PM = 1;
		if (mode[i] == '-')
			PM = 0;
		if (mode[i] == 'o')
		{
			nick = strtok (NULL, " ");
			continue;
		}
		if (mode[i] == 'v')
		{						/* voice sucks, ignore it */
			nick = strtok (NULL, " ");
			continue;
		}
		if (mode[i] == 'k' || mode[i] == 'b')
		{
			nick = strtok (NULL, " ");
			if (nick[0] == '*' && nick[1] == '!')
			{
				nick += 2;
			}
			strlwr (nick);
			if (PM == 1)
				scan_chan_users (chan, source, nick);
			continue;
		}
		if (mode[i] == 'l' && PM == 1)
		{						/* don't parse if -limit
								 * since no parms */
			nick = strtok (NULL, " ");
			continue;
		}
	}

}

/**
 * do_quit
 *
 * Purpose:
 * 1) delete all instances when a nick matches (nick)
 * 2) delete all users off a given channel
 * 2) delete everything (i.e., when the bot is disconnected from irc)
 *
 * toggle 1 = delete user.
 * toggle 2 = delete chan
 * toggle 3 = everything (when I'm killed).
 */
void
do_quit (const char *nick, long toggle)
{
	struct userlist *pNode = userhead;
	struct userlist *pPrev = NULL;

	if (toggle == 1)
	{
		/* delete user */
		while (pNode)
		{
			if (stricmp (pNode->nick, nick) == 0)
			{
				/* found a match, remove it */
				save_seen (pNode->nick, pNode->uh, pNode->chan);
				if (pPrev != NULL)
				{
					pPrev->next = pNode->next;
					free (pNode);
					pNode = pPrev->next;
				}
				else
				{
					/* first node in the list */
					userhead = pNode->next;
					free (pNode);
					pNode = userhead;
				}
			}
			else
			{
				/* No match, continue to next node */
				pPrev = pNode;
				pNode = pNode->next;
			}
		}
	}
	else if (toggle == 2)
	{
		/* delete channel */
		while (pNode)
		{
			if (stricmp (pNode->chan, nick) == 0)
			{
				/* found a match, remove it */
				save_seen (pNode->nick, pNode->uh, pNode->chan);
				if (pPrev != NULL)
				{
					pPrev->next = pNode->next;
					free (pNode);
					pNode = pPrev->next;
				}
				else
				{
					/* first node in the list */
					userhead = pNode->next;
					free (pNode);
					pNode = userhead;
				}
			}
			else
			{
				/* No match, continue to next node */
				pPrev = pNode;
				pNode = pNode->next;
			}
		}
	}
	else if (toggle == 3)
	{
		struct userlist *tempPtr = userhead;

		while (pNode)
		{
			tempPtr = pNode->next;
			free (pNode);
			pNode = tempPtr;
		}
	}
}

long
f_f (char *host)
{
	int i = 0;

	for (i = 0; i < fc; i++)
		if (!strcasecmp (ood[i].host, host))
			return i;

	return -1;
}

void
a_f (char *host)
{
	if (++fc > 100)
		fc = 0;
	fc--;
	strncpy (ood[fc].host, host, sizeof (ood[fc].host));
	ood[fc].time = time (NULL);
	ood[fc].count = 0;
	ood[fc].value = false;
	fc++;
}

void
reset_ (void)
{
	int i = 0;

	for (i = 0; i < fc; i++)
	{
		if (ood[i].value && (time (NULL) - ood[i].time) > rt)
		{
			ood[i].count = 0;
			ood[i].time = time (NULL);
			ood[i].value = false;
			ood[i].kick = 0;
		}
	}
}
