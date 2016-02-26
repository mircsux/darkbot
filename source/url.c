#include "defines.h"
#include "vars.h"
#include "prototypes.h"

int
check_existing_url (const char *source, char *topic, char *target)
{
	FILE *fp;
	char b[STRING_LONG] = { 0 }, *subj = NULL;

	if ((fp = fopen (URL2, "r")) == NULL)
	{
		L003 (source, URL2);
		return 0;
	}
	while (fgets (b, STRING_LONG, fp))
	{
		stripline (b);
		subj = strtok (b, " ");
		if (stricmp (subj, topic) == 0)
		{
			fclose (fp);
			return 1;
		}
	}
	fclose (fp);
	return 0;
}

void
find_url (const char *nick, char *topic, char *target)
{
	FILE	*fp = NULL;
	long	i = 0, FOUND = 0, x = 0;
	char	b [STRING_LONG] = {0}, DATA [STRING_SHORT] = {0};
	char	*subj = NULL, *ptr2 = NULL;

	if (strlen (topic) > MAX_TOPIC_SIZE)
		topic[MAX_TOPIC_SIZE] = '\0';
	
	strlwr (topic);
	
	if ((fp = fopen (URL2, "r")) == NULL)
	{
		L003 (nick, URL2);
		return;
	}
	
	while (fgets (b, STRING_LONG, fp))
	{
		x++;
		
		if (*b == '\n')
			continue;		/* Just pass over empty lines */

		stripline (b);
		
		subj = strtok (b, " ");
		strlwr (subj);
		ptr2 = strstr (subj, topic);
	
		if (ptr2 != NULL)
		{
			i++;
			FOUND = 1;
			sprintf (DATA, "%s %s", DATA, subj);
			if (strlen (DATA) >= MAX_SEARCH_LENGTH)
				break;
		}
	}

	fclose (fp);
	
	if (FOUND == 0)
	{
		L021 (target, NO_TOPIC, topic, x);
	}
	else if (i > 19)
	{
		L022 (target, i, DATA);
	}
	else if (i == 1)
	{
		L023 (target, nick, DATA);
	}
	else
		L024 (target, i, nick, DATA);
}

void
display_url (char *target, char *nick, char *topic)
{
	FILE *fp;
	long x = 0;
	char b[STRING_LONG] = { 0 }, *subj = NULL, *ptr = NULL;

	strlwr (topic);
	if ((fp = fopen (URL2, "r")) == NULL)
	{
		S ("%s %s :%s, there was an error displaying data for \"%s\".\n",
			(target == nick) ? "notice" : "privmsg", target, 
			nick, topic);
		return;
	}
	while (fgets (b, STRING_LONG, fp))
	{
		x++;
		stripline (b);
		subj = strtok (b, " ");
		ptr = strtok (NULL, "");
		if (stricmp (subj, topic) == 0 || !match_wild (subj, topic) == 0)
		{
			QUESTIONS++;
			S ("PRIVMSG %s :Raw data for %s is: %s\n", target, topic, ptr);
			fclose (fp);
			return;
		}						/* Subject match */
	}
	fclose (fp);
	S ("PRIVMSG %s :%s, I do not know of any topic named %s\n", target, nick, topic);
}

void
delete_url (const char *nick, char *topic, char *target)
{
	FILE	*fp = 0; 
	size_t	i = 0;
	size_t	FOUND = 0;
	char b		[STRING_LONG] = { 0 };
	char DATA	[STRING_SHORT] = { 0 };
	char *subj = NULL;
	char *ptr = NULL;

	if (*topic == '~')
	{
		topic++;
		if (topic != NULL)
			strlwr (topic);
		snprintf (DATA, sizeof (DATA), "%s/%s.rdb", RDB_DIR, topic);
		if (strspn (topic, LEGAL_TEXT) != strlen (topic))
		{
			if (stricmp (nick, target) == 0)
				S ("NOTICE %s :%s, rdb files are made up of letters and or numbers, no other text is accepted.\n", nick, nick);
			else
				S ("PRIVMSG %s :%s, rdb files are made up of letters and or numbers, no other text is accepted.\n", target, nick);
			return;
		}

		if ((fp = fopen (DATA, "r")) == NULL)
		{
			if (stricmp (nick, target) == 0)
				S ("NOTICE %s :%s, %s.rdb does not exist.\n", nick, nick, topic);
			else
				S ("PRIVMSG %s :%s, %s.rdb does not exist.\n", target, nick, topic);
			return;
		}

		fclose (fp);
		remove(DATA);

		if (stricmp (nick, target) == 0)
			S ("NOTICE %s :I have unlinked %s.\n", nick, DATA);
		else
			S ("PRIVMSG %s :I have unlinked %s.\n", target, DATA);
		return;
	}

	if ((fp = fopen (TMP_URL, "w")) == NULL)	/* fixes the 'must have at least 1 topic' problem. */
	{
		L003 (nick, TMP_URL);
		return;
	}

	fclose (fp);

	if ((fp = fopen (URL2, "r")) == NULL)
	{
		L003 (nick, URL2);
		return;
	}
	
	while (fgets (b, STRING_LONG, fp))
	{
		stripline (b);
		subj = strtok (b, " ");
		ptr = strtok (NULL, "");
		i++;
		if (stricmp (subj, topic) == 0)
		{
			FOUND = 1;
			DELETIONS++;
			if (stricmp (nick, target) == 0)
				L029n (nick, nick, i, topic);
			else
				L029 (target, nick, i, topic);
		}
		else if (strstr (subj, " ") == NULL)
			db_log (TMP_URL, "%s %s\n", subj, ptr);
	}

	fclose (fp);

	remove(URL2);
	rename(TMP_URL, URL2);
	
	if (FOUND == 0)
	{
		if (stricmp (nick, target) == 0)
			L030n (nick, nick, topic);
		else
			L030 (target, nick, topic);
	}
}



void
show_url (char *nick, char *topic, char *target, long donno, long floodpro, char *uh, long no_pipe)
{
	FILE *fp;
	long x = 0, length = 0, toggle = 0, A = 0, gotit = 0, D = 0, F = 0, Tog = 0;
	char b[STRING_LONG] = { 0 }, *subj = NULL, *ptr = NULL;
	char temp[STRING_LONG] = { 0 };
	char Data[STRING_LONG] = { 0 }, *ptr8 = NULL;
	char crm1[STRING_LONG] = { 0 };
	char crm2[STRING_LONG] = { 0 }, bFirst = 1;

	strlwr (topic);

	/* removes the question mark */
	if ((ptr = strchr (topic, '?')) != NULL)
		memmove (ptr, ptr + 1, strlen (ptr + 1) + 1);

	if ((fp = fopen (URL2, "r")) == NULL)
	{
		if (donno == 1)
			L003 (nick, URL2);
		return;
	}
	while (fgets (b, STRING_LONG, fp))
	{
		x++;
		stripline (b);
		subj = strtok (b, " ");
		if (subj == NULL)
			continue;
		ptr = strtok (NULL, "");
		if (ptr == NULL)
			continue;
		if (stricmp (subj, topic) == 0 || !match_wild (subj, topic) == 0)
		{
			QUESTIONS++;
			if (floodpro == 1)
				if (cf (uh, nick, nick))
				{
					fclose (fp);
					return;
				}
			gotit = 1;
			if (*ptr == '+')
			{
				ptr++;
				A = 1;
			}
			else if (*ptr == '-')
			{
				if (strstr (nick, "|") != NULL)
					return;
				if (no_pipe == 1)
				{
					fclose (fp);
					return;
				}
				ptr++;
				D = 1;
			}
			else if (*ptr == '~')
			{
				ptr++;
				fclose (fp);
				do_randomtopic (NORMALR, target, ptr, nick, topic);
				return;
			}
			length = strlen (ptr);
			if (length > 3)
			{
				if (ptr[0] == 'i' && ptr[1] == 'l' && ptr[2] == 'c')
				{
					toggle++;
				}
			}
			while (length > 0)
			{
				length--;
				if (Tog == 1)
				{
					Tog = 0;
					if (ptr[length] == 'N')
					{			/* nick */
						toggle++;
						snprintf (temp, sizeof (temp), "%s%s", nick, Data);
					}
					else if (ptr[length] == 'C')
					{			/* chan */
						toggle++;
						snprintf (temp, sizeof (temp), "%s%s", target, Data);
					}
					else if (ptr[length] == '1'
							 || ptr[length] == '2'
							 || ptr[length] == '3'
							 || ptr[length] == '4'
							 || ptr[length] == '5'
							 || ptr[length] == '6'
							 || ptr[length] == '7' || ptr[length] == '8' || ptr[length] == '9')
					{
						toggle++;
						/* The first time around, we just store the topic in a
						   "safe place" */
						if (bFirst == 1)
						{
							strncpy (crm1, topic, sizeof (crm1));
							bFirst = 0;
						}
						/* Each time around, get a new copy from the "safe place." */
						strncpy (crm2, crm1, sizeof (crm2));
						snprintf (temp, sizeof (temp), "%s%s", get_word (ptr[length], crm2, "+"), Data);
					}
					else if (ptr[length] == 'H')
					{			/* u@h of user */
						toggle++;
						snprintf (temp, sizeof (temp), "%s%s", uh, Data);
					}
					else if (ptr[length] == 'h')
					{			/* u@h (no ident) */
						toggle++;
						if (*uh == '~')
						{
							uh++;
						}
						snprintf (temp, sizeof (temp), "%s%s", uh, Data);
					}
					else if (ptr[length] == 'T')
					{			/* time */
						toggle++;
						snprintf (temp, sizeof (temp), "%s%s", date (), Data);
					}
					else if (ptr[length] == 't')
					{			/* unixtime */
						toggle++;
						snprintf (temp, sizeof (temp), "%ld%s", (long)time (NULL), Data);
					}
					else if (ptr[length] == 'W')
					{			/* WWW page */
						toggle++;
						snprintf (temp, sizeof (temp), "http://darkbot.org%s", Data);
					}
					else if (ptr[length] == 'S')
					{			/* server */
						toggle++;
						snprintf (temp, sizeof (temp), "%s%s", BS, Data);
					}
					else if (ptr[length] == 'R')
					{			/* rand */
						toggle++;
						/* The first time around, we just store the topic in a
						   "safe place" */
						if (bFirst == 1)
						{
							strncpy (crm1, topic, sizeof (crm1));
							bFirst = 0;
						}
						// Don't do random nicknames if the target isn't a channel!!
						snprintf (temp, sizeof (temp), "%s%s",
							((*target != '#') ? "R~" : 	get_rand_nick (target)),
							 Data);
							
					}
					else if (ptr[length] == 'P')
					{			/* port */
						toggle++;
						snprintf (temp, sizeof (temp), "%ld%s", BP, Data);
					}
					else if (ptr[length] == 'Q')
					{			/* question */
						toggle++;
						snprintf (temp, sizeof (temp), "%s%s", revert_topic (topic), Data);
					}
					else if (ptr[length] == 'V')
					{			/* version */
						toggle++;
						snprintf (temp, sizeof (temp), "%s%s", dbVersion, Data);
					}
					else if (ptr[length] == '!')
					{			/* cmdchar */
						toggle++;
						snprintf (temp, sizeof (temp), "%c%s", *CMDCHAR, Data);
					}
					else if (ptr[length] == 'B')
					{			/* mynick */
						toggle++;
						snprintf (temp, sizeof (temp), "%s%s", Mynick, Data);
					}
					else
						snprintf (temp, sizeof (temp), "%c~%s", ptr[length], Data);
				}
				else if (ptr[length] == '~')
				{
					Tog = 1;
				}
				else
					snprintf (temp, sizeof (temp), "%c%s", ptr[length], Data);
				strncpy (Data, temp, sizeof (Data));
			}					/* While */
			if (D == 1)
			{
				ptr8 = strtok (Data, "|");
				while (ptr8 != NULL)
				{
					if (ptr8[0] == ' ')
						ptr8++;
					if (ptr8[0] == 'B' && ptr8[1] == 'A' && ptr8[2] == 'N')
					{			/* ban user */
						S ("MODE %s +b *%s\n", target, uh);
					}
					if (ptr8[0] == 'T' && ptr8[1] == 'E' && ptr8[2] == 'M' &&
						ptr8[3] == 'P' && ptr8[4] == 'B' && ptr8[5] == 'A' && ptr8[6] == 'N')
					{			/* temp ban user for 60 sec */
						S ("MODE %s +b *%s\n", target, uh);
						snprintf (temp, sizeof (temp), "%s/%ld", DBTIMERS_PATH, (long)time (NULL) + 60);
						db_log (temp, "MODE %s -b *%s\n", target, uh);
					}
					ptr8[0] = tolower (ptr8[0]);
					ptr8[1] = tolower (ptr8[1]);
					if (ptr8[0] == 'p' && ptr8[1] == 'r')
						F = 1;
					if (ptr8[0] == 'k' && ptr8[1] == 'i')
						F = 1;
					if (ptr8[0] == 'n' && ptr8[1] == 'o')
						F = 1;
					if (ptr8[0] == 't' && ptr8[1] == 'o')
						F = 1;
					if (F == 1)
						S ("%s\n", ptr8);
					F = 0;
					ptr8 = strtok (NULL, "|");
				}
				fclose (fp);
				return;
			}
			if (toggle == 0)
			{
				add_stats (nick, uh, 1, time (NULL), time (NULL));

				if (A == 0)
				{
					if ((*target == '#') || (*target == '+') || (*target == '&'))
						S ("PRIVMSG %s :%s%s\n", target, rand_reply (nick), Data);
					else
					{
						strncpy (temp, rand_reply (target), sizeof (temp));
						S ("NOTICE %s :%s [%s] %s [sent by %s]\n", target, temp, topic, Data, nick);
						S ("NOTICE %s :Messaged %s: \"%s [%s] %s [sent by %s]\"\n", nick, target,
						   temp, topic, Data, nick);
					}
				}
				else if ((*target == '#') || (*target == '+') || (*target == '&'))
					S ("PRIVMSG %s :\1ACTION %s\1\n", target, Data);
				else
				{
					S ("NOTICE %s :[%s] %s [sent by %s]\n", target, topic, Data, nick);
					S ("NOTICE %s :Messaged %s: \"[%s] %s [sent by %s]\"\n",
					   nick, target, topic, Data, nick);
				}
			}
			else if (A == 0)
			{
				add_stats (nick, uh, 1, time (NULL), time (NULL));

				if ((*target == '#') || (*target == '+') || (*target == '&'))
					S ("PRIVMSG %s :%s\n", target, Data);
				else
				{
					S ("NOTICE %s :[%s] %s [sent by %s]\n", target, topic, Data, nick);
					S ("NOTICE %s :Messaged %s: \"[%s] %s [sent by %s]\"\n",
					   nick, target, topic, Data, nick);
				}
			}
			else if ((*target == '#') || (*target == '+') || (*target == '&'))
			{
				add_stats (nick, uh, 1, time (NULL), time (NULL));
				S ("PRIVMSG %s :\1ACTION %s\1\n", target, Data);
			}
#if			MSG_RESPONSES == ON
			else
			{
				add_stats (nick, uh, 1, time (NULL), time (NULL));

				S ("NOTICE %s :[%s] %s [sent by %s]\n", target, topic, Data, nick);
			}
#endif
			fclose (fp);
			return;
		}						/* Subject match */
	}
	fclose (fp);
	if (donno == 1)
	{
		if (strlen (topic) > 3)
		{
			strlwr (topic);
			if (topic[0] == 'i' && topic[1] == 'l' && topic[2] == 'c')
			{
				if ((*target == '#') || (*target == '+') || (*target == '&'))
					S ("PRIVMSG %s :%s, I found no matching ILC for that channel.\n", target, nick);
				return;
			}
		}
		if ((*target == '#') || (*target == '+') || (*target == '&'))
#if     RANDOM_DUNNO == 1
			do_randomtopic (DUNNOR, target, DUNNO_FILE, nick, topic);
#else
			S ("PRIVMSG %s :%s, %s\n", target, nick, DONNO_Q);
#endif
		else
			S ("NOTICE %s :%s, %s\n", nick, nick, DONNO_Q);
	}
}

char *
get_multiword_topic (char *first)
{
	char *tmp2 = NULL;

	tmp2 = strtok (NULL, " ");
	if (tmp2 != NULL)
	{
		sprintf (f_tmp, "%s", first);
		while (tmp2 != NULL)
		{
			sprintf (f_tmp, "%s+%s", f_tmp, tmp2);
			tmp2 = strtok (NULL, " ");
		}
		return f_tmp;
	}
	else
		return first;
}

void
datasearch (const char *nick, char *topic, char *target)
{
	FILE *fp = NULL;
	size_t i = 0, FOUND = 0, x = 0;
	char b[STRING_LONG] = { 0 }, *dorf = NULL, *subj = NULL, *ptr2 = NULL, DATA[STRING_SHORT] =
	{
	0};

	/* Make sure topic is not NULL.*/
	if (topic == NULL)
	{
		return;
	}

	if (strlen (topic) > MAX_TOPIC_SIZE)
		topic[MAX_TOPIC_SIZE] = '\0';
	
	strlwr (topic);
	
	if ((fp = fopen (URL2, "r")) == NULL)
	{
		L003 (nick, URL2);
		return;
	}
	while (fgets (b, STRING_LONG, fp))
	{
		x++;
	
		if (*b == '\n')
			continue;	/* Just pass over blank lines */
		
		stripline (b);
		strlwr (b);
		
		subj = strtok (b, " ");
		dorf = strtok (NULL, "");
		ptr2 = strstr (dorf, topic);
		if (ptr2 != NULL)
		{
			i++;
			FOUND = 1;
			sprintf (DATA, "%s %s", DATA, subj);
			if (strlen (DATA) >= MAX_SEARCH_LENGTH)
				break;
		}
	}
	fclose (fp);
	if (FOUND == 0)
	{
		L021 (target, NO_TOPIC, topic, x);
	}
	else if (i > 19)
	{
		L022 (target, i, DATA);
	}
	else if (i == 1)
	{
		L023 (target, nick, DATA);
	}
	else
		L024 (target, i, nick, DATA);
}

char *
get_word (long number, char *string, char *seperator)
{								/* gets a specific word requested */
	long i = 0;
	char *ptr = NULL;

	number = number - 49;

	ptr = strtok (string, "+");

	strncpy (f_tmp, ptr, sizeof (f_tmp));
	if (ptr != NULL)
	{
		while (ptr != NULL)
		{
			i++;				/* word number */
			ptr = strtok (NULL, seperator);
			if (ptr != NULL)
			{
				if (i == number)
				{
					snprintf (f_tmp, sizeof (f_tmp), "%s", ptr);
					return f_tmp;
				}
			}
		}
		return f_tmp;
	}
	else
	{							/* only one word */
		if (number == 1)
		{
			return f_tmp;
		}
		else
			return "";			/* no match */
	}
}
