#include "defines.h"
#include "vars.h"
#include "prototypes.h"

#ifdef		RANDOM_STUFF
void		add_randomstuff		(char *source, char *target, char *data)
{
	char		*ptr = NULL;
	char		file	[STRING_LONG] = {0};
	size_t		nLines = 0;
	int			toggle = 1;
	
	
#ifdef	BACKUP_RANDOMSTUFF
#	ifndef		WIN32	
	// Backup randomstuff file to a temporary file.
	unlink(RAND_BACKUP_FILE);
	snprintf(file, sizeof(file), "cp %s %s\n", RAND_FILE, RAND_BACKUP_FILE);
	system(file);
#	endif
#endif

	if(*data == '~')
	{
		data++;
		ptr = strtok(data, " ");
		if(ptr != NULL)
			strlwr(ptr);

		if(strspn(ptr, LEGAL_TEXT) != strlen(ptr))
		{
			S("privmsg %s :%s, rdb files must be made up of letters and or numbers, no other text is accepted.\n",
				target, source);
			return;
		}
		
		snprintf(file, sizeof(file), "%s/%s.rdb", RDB_DIR, ptr);
		data = strtok(NULL, "");
		toggle = 0;
	}
	else
	{
		snprintf(file, sizeof(file), "%s", RAND_FILE);
		nRandStuffs++;
	}

	db_log(file, "%s\n", data);

	nLines = count_lines(file);

	L011(target, source, nLines);
}

void		get_rand_stuff_time			(void)
{
	Rand_Stuff = rand () % RAND_STUFF_TIME + 2;
	if (Rand_Stuff < RAND_STUFF_TIME / 2)
		Rand_Stuff = RAND_STUFF_TIME;
}


void		do_random_stuff					(void)
{
	FILE			*fp = 0;
	size_t			nLine = 0, nLength = 0, nIndex = 0;
	int				nIsAction = 0;
 	char			temp			[STRING_SHORT] = {0};
	char			szBuffer		[STRING_LONG] = {0};
	char			*b2 = NULL;
	
	// Count the randomstuffs.
	if ((nRandStuffs = count_lines(RAND_FILE)) < 1)
		return;

	// Get a random line number to display. If nLine is more than the number of RandStuffs,
	// nLine becomes equal to the number of RandStuffs.
	if ((nLine = 1 + (size_t) ((float)nRandStuffs * rand() / (RAND_MAX + 1.0))) > nRandStuffs)
		nLine = nRandStuffs;

	// If nLine is less than 1, nLine becomes equal to 1
	if(nLine < 1)
		nLine = 1;
	        
	// Complain if we can't open the randomstuff file.
	if((fp = fopen(RAND_FILE, "r")) == NULL)
	{
		S("privmsg %s :Unable to open %s for reading in do_random_stuff()\n",
			CHAN, RAND_FILE);
		
		return;
	}

	while(fgets(szBuffer, STRING_LONG, fp))
	{
		// Ignore comments that start with a /, treat as comments.
		if((*szBuffer != '/') && (*szBuffer != '\n'))
			nIndex++;
		else
			continue;

		stripline(szBuffer);

		if(nIndex == nLine)
		{
			nLength = strlen(szBuffer);
			nIndex = 0;

			memset(data, 0, sizeof(data));

			nIsAction = (*szBuffer == '+' ? 1 : 0);

			while(nLength > 0)
			{
				nIndex++;
				nLength--;
				
				// Parse data for channel name variable. replace accordingly.
				if(szBuffer[nLength] == '~')
					snprintf(temp, sizeof(temp), "%s%s", CHAN, data);
				else
					snprintf(temp, sizeof(temp), "%c%s", szBuffer[nLength], data);
				
				strncpy(data, temp, sizeof(data));
			}
			
			// Make sure the file is closed!
			fclose (fp);

			if(nIsAction == 1)
			{
				b2 = data;
				b2++;
				S("privmsg %s :\1ACTION %s\1\n", CHAN, b2);
				
				return;
			}
			else
			{
				S("privmsg %s :%s\n", CHAN, data);
				
				return;
			}
		}
	}
	fclose(fp);
}
#endif

void	do_randomtopic	(int type, char *target, char *file, char *nick, char *topic)
{
	FILE		*fp = 0;
	char		file2		[FILENAME_MAX] = {0};
	char		szBuffer2	[STRING_LONG] = {0};
	char		Data		[STRING_LONG] = {0};
	char		temp		[STRING_LONG] = {0};
	char		crm1		[STRING_LONG] = {0};
	char		crm2		[STRING_LONG] = {0};
	char		bFirst = 1;
	char		*pBuffer = NULL, *ptr = NULL, *pDefault = NULL;
	size_t		nLength = 0, nRandTopics = 0, nLine = 0,
				nIndex = 0;
	int			toggle = 0,	nType = 0, F = 0;
	
	// This ends up being the response, if we're doing a dunno, or whut reply, and the file
	// containing the random responses can't be opened.

	pDefault = (type == DUNNOR ? DONNO_Q : WHUT);

	if(file != NULL)
		snprintf(file2, sizeof(file2), "%s/%s.rdb", RDB_DIR, file);

	if ((nRandTopics = count_lines(file2)) < 1)
	{
		S("privmsg %s :Sorry, I cannot answer that topic because "
			"darkbot random text file (rdb) \"%s\" has no lines in it.\n",
			target, file2);
		return;
	}

	if ((fp = fopen(file2, "r")) == NULL)
	{
		// If this is a dunno, or whut response, instead we say our default reply.
		if (type == NORMALR)
		{
			S("privmsg %s :Sorry, I cannot answer that topic because "
			  "darkbot random text file (rdb) \"%s\" was not found.\n",
				target, file2);
		}
		else
		{
			S ("privmsg %s :%s\n", target, pDefault);
		}

		return;
	}
	
	/* db_sleep(1) */

	// Get line number that we are searching for in the file. If it ends up less than 
	// 1, for some reason, set it to 1.

	if ((nLine = (size_t) ((float)nRandTopics * rand() / (RAND_MAX+1.0))) < 1)
		nLine = 1;

	if(nLine > nRandTopics)
		nLine = nRandTopics;

	while(fgets(szBuffer2, STRING_LONG, fp))
	{
		if((*szBuffer2 != '/') && (*szBuffer2 != '\n'))
			nIndex++;
		else
			continue;

		stripline(szBuffer2);

		// This is our line!
		if(nIndex == nLine)
		{
			nIndex = 0;
			pBuffer = szBuffer2;
			
			/* if ((*pBuffer == '+') || (*pBuffer == '-'))
			{
				pBuffer++;
				nType = ((*pBuffer == '+') ? RDB_ACTION : RDB_RAW);
			}
			else
				nType = RDB_NORMAL;
			*/

			if (*pBuffer == '+')
			{
				pBuffer++; 
				nType = RDB_ACTION;
			}
			else if (*pBuffer == '-')
			{
				pBuffer++;
				nType = RDB_RAW;
			}
			else
			{
				nType = RDB_NORMAL;
			}

			nLength = (size_t) strlen(pBuffer);
			
			while(nLength > 0)
			{
				nLength--;
				if (toggle == 1)
				{
					toggle = 0;
					// Support for tokens
					if (pBuffer[nLength] == '1' || pBuffer[nLength] == '2' || pBuffer[nLength] == '3' ||
						pBuffer[nLength] == '4' || pBuffer[nLength] == '5' || pBuffer[nLength] == '6' ||
						pBuffer[nLength] == '7' || pBuffer[nLength] == '8' || pBuffer[nLength] == '9')
					{
						/* The first time around, we just store the topic in a "safe place" */
						
						if (bFirst == 1)
						{
							strncpy (crm1, topic, sizeof(crm1));
							bFirst = 0;
						}

						/* Each time around, get a new copy from the "safe place". */

						strncpy (crm2, crm1, sizeof(crm2));
						snprintf (temp, sizeof(temp), "%s%s", get_word (pBuffer[nLength], crm2, "+"), Data);
					}

					else if(pBuffer[nLength] == 'N')
					{				// nick 
						snprintf(temp, sizeof(temp), "%s%s", nick, Data);
					}				
					else if ((pBuffer[nLength] == 'H') && (*target == '#'))
					{			/* u@h of user */
						snprintf (temp, sizeof (temp), "%s%s", mask_from_nick (nick, target), Data);
	
					}
					else if ((pBuffer[nLength] == 'C') && (*target == '#'))
					{				// channel or nick.
						snprintf(temp, sizeof(temp), "%s%s", target, Data);
					}
					else if (pBuffer[nLength] == 'T')
					{				// time
						snprintf(temp, sizeof(temp), "%s%s", date(), Data);
					}
					else if (pBuffer[nLength] == 't')
					{			/* unixtime */
						snprintf (temp, sizeof (temp), "%ld%s", (long) time (NULL), Data);
					}
					else if ((pBuffer[nLength] == 'R') && (*target == '#'))
					{				// Random nick (not if this is not a channel
						snprintf(temp, sizeof(temp), "%s%s", 
							get_rand_nick(target), Data);
					}
					else if (pBuffer[nLength] == 'S')
					{				// Server
						snprintf(temp, sizeof(temp), "%s%s", BS, Data);
					}
					else if (pBuffer[nLength] == 'P')
					{				// Port
						snprintf(temp, sizeof(temp), "%d%s", (int) BP, Data);
					}
					else if(pBuffer[nLength] == 'Q')
					{				// Question 
						snprintf(temp, sizeof(temp), "%s%s", revert_topic(topic), Data);
					}
					else if(pBuffer[nLength] == 'W')
					{				// WWW page
						snprintf(temp, sizeof(temp), "http://darkbot.org%s", Data);
					}
					else if(pBuffer[nLength] == '!')
					{				// cmdchar
						snprintf(temp, sizeof(temp), "%c%s", *CMDCHAR, Data);
					}
					else if(pBuffer[nLength] == 'V')
					{				// Version 
						snprintf(temp, sizeof(temp), "%s%s", dbVersion, Data);
					}
					else if(pBuffer[nLength] == 'B')
					{				// Mynick
						snprintf(temp, sizeof(temp), "%s%s", Mynick, Data);
					}
					else
					{
						snprintf(temp, sizeof(temp), "%c~%s", pBuffer[nLength], Data);
					}
				}
				else if(pBuffer[nLength] == '~')
				{
					toggle = 1;
				}
				else
				{
					snprintf(temp, sizeof(temp), "%c%s", pBuffer[nLength], Data);
				}
				strncpy(Data, temp, sizeof(Data));
			}

			switch (nType)
			{
			case RDB_ACTION:
				S ("privmsg %s :\1ACTION %s\1\n", target, Data);
				break;

			case RDB_NORMAL:
				S ("privmsg %s :%s\n", target, Data);
				break;

			case RDB_RAW:
				ptr = strtok (Data, "|");

				while (ptr != NULL)
				{
					if (ptr[0] == ' ')
						ptr++;

					if (ptr[0] == 'B' && ptr[1] == 'A' && ptr[2] == 'N')
					{		// Ban the user, if the target is a channel.
						if (*target == '#')
							S ("mode %s +b *%s\n", target, mask_from_nick (nick, target));
						else 
							S ("privmsg %s :Sorry, %s, I can't respond to that topic in private.\n",
								target, nick);
					}
					
					if (ptr[0] == 'T' && ptr[1] == 'E' && ptr[2] == 'M' && ptr[3] == 'P' &&
						ptr[4] == 'B' && ptr[5] == 'A' && ptr[6] == 'N')
					{		// Ban user for 60 seconds, if the target is a channel.
						if (*target == '#')
						{
							S ("mode %s +b *%s\n", target, mask_from_nick (nick, target));
							snprintf (temp, sizeof (temp), "%s/%ld", DBTIMERS_PATH, (long) time (NULL) + 60);
							db_log (temp, "MODE %s -b *%s\n", target, mask_from_nick (nick, target));
			
						}
						else
							S ("privmsg %s :Sorry, %s, I can't respond to that topic in private.\n",
								target, nick);
					}

					ptr[0] = tolower (ptr[0]);
					ptr[1] = tolower (ptr[1]);

					if (ptr[0] == 'p' && ptr[1] == 'r')
						F = 1;
					if (ptr[0] == 'k' && ptr[1] == 'i')
						F = 1;
					if (ptr[0] == 'n' && ptr[1] == 'o')
						F = 1;
					if (ptr[0] == 't' && ptr[1] == 'o')
						F = 1;
					
					if (F == 1)
					{
						S ("%s\n", ptr);
						F = 0;
					}
					
					ptr = strtok (NULL, "|");
				}
				break;

			} // End switch.

			fclose (fp);
			return;
			
		}
	}
	fclose(fp);
}

/**
 * 05/27/02 Ron
 * - Recoded this function, random.ini no longer needs a number
 *   at the top.
 * 6/22/00 Dan
 * - Function argument is now pointer to const
 * - Fixed a problem where the file was never closed
 * - All variables are now initialized when declared
 * - Removed an unused variable
 * - Changed long variables to type size_t, they should be
 *   unsigned
 * - Changed reinitialization of data
 * - Moved the big if/else structure to a switch
 */

char	*rand_reply	(const char *nick)
{
	FILE		*fp = 0;
	char		temp	[STRING_SHORT] = {0};
	size_t		nIndex = 0, nRandTopics = 0, nLine = 0,
				nLength = 0;


	// Count how many lines are in the RAND_SAY file, if can't be
	// counted, just return.
	if((nRandTopics = count_lines(RAND_SAY)) < 1)
		return(0);

	// set nLine to be a random number between 1 and nRandTopics.
	nLine = 1 + (size_t) ((float)nRandTopics * rand() / (RAND_MAX+1.0));

	// If the RAND_SAY file can't be opened, just return.
	if((fp = fopen(RAND_SAY, "r")) == NULL)
		return(0);

	while(fgets(r_reply, STRING_SHORT, fp))
	{
		// Ignore comments
		if((*r_reply != '/' ) && (*r_reply != '\n'))
			nIndex++;
		else
			continue;
 
		stripline(r_reply);

		if(nIndex != nLine)
			continue;
		else
		{
			// This is our line.
			fclose(fp);
			nLength = (size_t) strlen(r_reply);
			nIndex = 0;
			data[0] = 0;

			while(nLength > 0)
			{
				nIndex++;
				nLength--;
				switch(r_reply[nLength])
				{
				case '^':
					snprintf(temp, sizeof(temp), "%s%s", nick, data);
					break;
				case '%':
					snprintf(temp, sizeof(temp), "\2%s", data);
					break;
				case '&':
					snprintf(temp, sizeof(temp), "\37%s", data);
					break;
				case '~':
					snprintf(temp, sizeof(temp), "\26%s", data);
					break;
				default:
					snprintf(temp, sizeof(temp), "%c%s", r_reply[nLength], data);
					break;
				}

				strncpy(data, temp, sizeof(data));
			}
			// Return our fully parsed line.
			return(data);
		}
	}	

	// Unable to find a match
	fclose(fp);
	
	// A space is returned to prevent crashing
	return(" ");
}

#if		RANDQ == ON

/* 
 * do_randq():
 *
 *  Types = RANDQ_NORMAL, RANDQ_CASE, RANDQ_RAND
 */

void		do_randq		(char *text, const int type, const char *target, const char *nick)
{
	FILE		*fp = 0;
	size_t		nLine = 0, length = 0, nNumMatches = 0,
				nIndex2 = 0, nIndex3 = 0;
	int			nIsAction = 0;
	char		*b2 = NULL;
	char		temp	[STRING_SHORT] = { 0 };
	char		szBuffer[STRING_LONG] = { 0 };

    /* 
	 * Check for RANDQ_RAND, and just do_random_stuff() if it's specified.
	 */
	if (type == RANDQ_RAND)
	{
		do_random_stuff ();
		return;
	}

    if (text == NULL)
	{
		if(((nNumMatches = count_lines(RAND_FILE)) == -1) || 
			(nNumMatches == 0))
		{
			S ("privmsg %s :%s, There are no randomstuffs, or %s cannot be accessed.\n", 
				target, nick, RAND_FILE);
			
			return;
		}
		
		S("privmsg %s :%s, there %s %d randomstuff%s\n", 
			target,
			nick,
			nNumMatches == 1 ? "is" : "are",
			nNumMatches,
			nNumMatches == 1 ? "." : "s."
		);

		return;
	}

	/* Remove the temporary file. */

	remove(RANDQ_TEMPFILE);

	if((fp = fopen(RAND_FILE, "r")) == NULL)
	{
		S("privmsg %s :%s, I was unable to open %s for reading.\n",
			target, nick, RAND_FILE);
		return;
	}

	while(!feof(fp))
	{
		while(fgets(szBuffer, STRING_LONG, fp))
		{
			if(*szBuffer == '/')
				continue;
	
			stripline(szBuffer);

			/*
			 * If type is specified as being case sensitive, use strstr,
			 * otherwise, use db_stristr
			 */

			if (type == RANDQ_CASE) 
			{
				if (strstr(szBuffer, text))
				{
					nNumMatches++;
					db_log(RANDQ_TEMPFILE, "%s\n", szBuffer);
				}
			}
			else /* if (type == RANDQ_NORMAL) */
			{
				if (db_stristr(szBuffer, text) != 0)
				{
					nNumMatches++;
					db_log(RANDQ_TEMPFILE, "%s\n", szBuffer);
				}
			}
		}
	}

	// Close randomstuff file.
	fclose(fp);

	// If no matches are present, return that.
	if(nNumMatches == 0)
	{
		S("privmsg %s :%s, I didn't find any matches for your search.\n",
			target,	nick);
		return;
	}

	nLine = 1 + (size_t) ((float)nNumMatches * rand() / (RAND_MAX + 1.0));
	
	// If we can't open the temporary file, complain about it.
	if((fp = fopen(RANDQ_TEMPFILE, "r")) == 0)
	{
		S("privmsg %s :%s, I was unable to open %s for reading.\n", 
			target, nick, RANDQ_TEMPFILE);
		return;
	}

	while(!feof(fp))
	{
		while(fgets(szBuffer, STRING_LONG, fp))
		{
			if((*szBuffer != '/' ) || (*szBuffer != '\n'))
				nIndex2++;
			else
				continue;

			stripline(szBuffer);

			// This is the line we have chosen.
			if(nIndex2 == nLine)
			{

				// Check if this is an action.
				nIsAction = (*szBuffer == '+' ? 1 : 0);
				 
				length = strlen(szBuffer);
				nIndex3 = 0;

				memset(data, 0, sizeof(data));
				while(length > 0)
				{
					nIndex3++;
					length--;
					if(szBuffer[length] == '~')
					{
						// Replace occurance of this char with chan name
						snprintf(temp, sizeof(temp), "%s%s", target, data);
					}
					else
					{
						snprintf(temp, sizeof(temp), "%c%s", szBuffer[length], data);
					}
					strncpy(data, temp, sizeof(data));
				}

				// Make sure the file is closed.
				fclose (fp);

				if(nIsAction == 1)
				{
					b2 = data;
					b2++;
				
					S("privmsg %s :\1ACTION %s (%d/%d)\1\n", 
							target, b2, nLine, nNumMatches);
					return;
				}
				else
				{
					S("privmsg %s :(%d/%d): %s\n", 
							target, nLine, nNumMatches, data);
					return;
				}
			}
		}
	}
	fclose(fp);
}
#endif
