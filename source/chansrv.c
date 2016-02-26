#include "defines.h"
#include "vars.h"
#include "prototypes.h"

void
chanserv (char *source, char *target, char *buf)
{
	char *cmd = NULL, *s = NULL, *s2 = NULL, *s3 = NULL, *s4 = NULL, *s5 = NULL;
	char *ptr3 = NULL, temp[1024] = { 0 }, *userhost = NULL;
	long sn2 = 0, sn = 0, i = 0, unixtime = 0;
	struct stat statbuf;

#ifdef	RANDOM_STUFF
	if (stricmp (target, CHAN) == 0)
		Rand_Idle = 0;
#endif
	stripline (buf);
	stripline (source);
	if (buf == NULL || target == NULL || source == NULL)
		return;
	cmd = strtok (buf, " ");
	if (cmd == NULL)
		return;
	if (*cmd == ':')
		cmd++;
	if ((userhost = strchr (source, '!')) != NULL)
	{
		*userhost++ = '\0';
	}

    if ( check_ignore_user_ram ( source ) > 0 )
        return;
    
    /* ------ commands that require a privmsg ------ */
	if (*target != '#' && *target != '&' && *target != '+')
	{
		if (stricmp (cmd, "PASS") == 0
			|| stricmp (cmd, "PASSWORD") == 0 || stricmp (cmd, "PASSWD") == 0)
		{
			s = strtok (NULL, " ");
			s2 = strtok (NULL, " ");
			if (s == NULL || s2 == NULL)
			{
				L031 (source, Mynick);
				return;
			}
			if (strlen (s2) > 25)
				s2[25] = '\0';
			set_pass (source, userhost, s, s2);
			return;
		}
		else if (stricmp (cmd, "DBSIZE") == 0 || stricmp (cmd, "INFOSIZE") == 0)
		{
			if (check_access (userhost, "#*", 0, source) >= 2)
			{
				if (stat (URL2, &statbuf) == 0)
				{
					S ("NOTICE %s :My database file is presently %ld byte%s in size.\n", 
						source,
						statbuf.st_size,
						statbuf.st_size == 1 ? "" : "s");
				}
				else
				{
					return;
				}
			}
		}
		else if (stricmp (cmd, "OP") == 0)
		{
			s = strtok (NULL, " ");
			if (s == NULL)
				return;
			if (check_access (userhost, s, 0, source) >= 3)
			{
				s2 = strtok (NULL, "");
				if (s2 == NULL)
				{
					return;
				}
				S ("MODE %s +oooooo %s\n", s, s2);
			}

		}
#ifdef	RANDOM_STUFF
		else if (stricmp (cmd, "RANDOMSTUFF") == 0 || stricmp (cmd, "RANDSTUFF") == 0 ||
			stricmp (cmd, "RS") == 0)
		{
			s = strtok (NULL, " ");
			if (s == NULL)
				return;
			if (check_access (userhost, s, 0, source) >= RAND_LEVEL)
			{
				s2 = strtok (NULL, "");
				if (s2 == NULL)
				{
					L064n (source, source);
					return;
				}
				add_randomstuff (source, source, s2);
			}
		}
#endif
		else if (stricmp (cmd, "DEOP") == 0)
		{
			s = strtok (NULL, " ");
			if (s == NULL)
				return;
			if (check_access (userhost, s, 0, source) >= 3)
			{
				s2 = strtok (NULL, "");
				if (s2 == NULL)
				{
					return;
				}
				S ("MODE %s -oooooo %s\n", s, s2);
			}

		}
		else if (stricmp (cmd, "VOICE") == 0 || stricmp (cmd, "V") == 0)
		{
			s = strtok (NULL, " ");
			if (s == NULL)
				return;
			if (check_access (userhost, s, 0, source) >= 3)
			{
				s2 = strtok (NULL, "");
				if (s2 == NULL)
				{
					return;
				}
				S ("MODE %s +vvvvvv %s\n", s, s2);
			}

		}
		else if (stricmp (cmd, "DEV") == 0
				 || stricmp (cmd, "DV") == 0
				 || stricmp (cmd, "DEVOICE") == 0 || stricmp (cmd, "DVOICE") == 0)
		{
			s = strtok (NULL, " ");
			if (s == NULL)
				return;
			if (check_access (userhost, s, 0, source) >= 3)
			{
				s2 = strtok (NULL, "");
				if (s2 == NULL)
				{
					return;
				}
				S ("MODE %s -vvvvvv %s\n", s, s2);
			}
		}
		else if ((stricmp (cmd, "KICK") == 0
				  || stricmp (cmd, "WACK") == 0
				  || stricmp (cmd, "K") == 0 || stricmp (cmd, "NAIL") == 0))
		{
			s = strtok (NULL, " ");
			if (s == NULL)
				return;
			if (check_access (userhost, s, 0, source) >= 3)
			{
				s2 = strtok (NULL, " ");
				s3 = strtok (NULL, "");
				if (s2 == NULL)
				{
					S ("NOTICE %s :You must specity a nick to kick from %s!\n", source, s);
					return;
				}
				if (s3 == NULL)
				{
					S ("KICK %s %s %s\n", s, s2, DEFAULT_KICK);
				}
				else
				{
					S ("KICK %s %s %s\n", s, s2, s3);
				}
			}
		}

#if	ALLOW_DELETE_IN_MSG == ON
		else if (stricmp (cmd, "DELETE") == 0
				 || stricmp (cmd, "REMOVE") == 0
				 || stricmp (cmd, "FORGET") == 0 || stricmp (cmd, "DEL") == 0)
		{
#ifdef	REQ_ACCESS_DEL
			if (check_access (userhost, "#*", 0, source) >= 1)
			{
#endif
				s2 = strtok (NULL, " ");
				if (s2 == NULL)
				{
					S ("NOTICE %s :%s what, %s?\n", source, cmd, source);
					return;
				}
				if (strlen (s2) > MAX_TOPIC_SIZE)
					s2[MAX_TOPIC_SIZE] = '\0';
#ifdef	LOG_ADD_DELETES
				db_log (ADD_DELETES, "[%s] %s!%s DEL %s\n", date (), source, userhost, s2);
#endif
				if (*s2 == '~')
				{				/* need level 2 to delete .rdb files */
					if (check_access (userhost, "#*", 0, source) >= 2)
					{
						delete_url (source, s2, source);
					}
					return;
				}
				delete_url (source, s2, source);
#ifdef	REQ_ACCESS_DEL
			}
#endif
		}
#endif
		else if (stricmp (cmd, "TELL") == 0)
		{
			s2 = strtok (NULL, " ");
			if (s2 == NULL)
			{
				L085n (source, source);
				return;
			}
			s3 = strtok (NULL, " ");
			if (s3 == NULL)
			{
				L083n (source, source, s2);
				return;
			}
			if (stricmp (s3, Mynick) == 0)
				return;			/* don't bother telling
								   * myself about stuff */
			if (stricmp (s3, "ABOUT") == 0)
			{
				s4 = strtok (NULL, " ");
				if (s4 == NULL)
				{
					L084n (source, source, s2);
					return;
				}
				strlwr (s4);
				show_url (source, get_multiword_topic (s4), s2, 1, 0, userhost, 1);
			}
			else
			{
				strlwr (s3);
				show_url (source, get_multiword_topic (s3), s2, 1, 0, userhost, 1);
			}
		}

#if	ALLOW_ADD_IN_MSG == ON
		else if (stricmp (cmd, "ADD") == 0
				 || stricmp (cmd, "REMEMBER") == 0
				 || stricmp (cmd, "SAVE") == 0 || stricmp (cmd, "STORE") == 0)
		{
#ifdef	REQ_ACCESS_ADD
			if (check_access (userhost, "#*", 0, source) >= 1)
			{
#endif
				s2 = strtok (NULL, " ");
				
				if (s2 == NULL)
				{
					L065n (source, source);
					return;
				}
		
				// Fix for some segmentation fault problems
				// concerning topics consisting entirely of
				// wildcard characters.
				if (strspn(s2, "*?") == strlen(s2))
				{
					Lbadtopic2(source, source);
					return;
				}					
		
				if (strlen (s2) > MAX_TOPIC_SIZE)
				{
					s2[MAX_TOPIC_SIZE] = '\0';
					S ("NOTICE %s :%s, topic is over the limit, and has characters truncated.\n",
					   source, source);
				}
				s3 = strtok (NULL, "");
				if (s3 == NULL)
				{
					L066n (source, source, s2);
					return;
				}
				if (strlen (s3) > MAX_DATA_SIZE)
					s3[MAX_DATA_SIZE] = '\0';
				strlwr (s2);
				if (*s2 == '~')
				{
					S ("NOTICE %s :%s, rdb files can only be called from the data of a topic, they cannot be used in the topic itself.\n", source, source);
					return;
				}
				if (check_existing_url (source, s2, target) == 1)
				{
					S ("NOTICE %s :%s \37%s\37\n", source, EXISTING_ENTRY, s2);
					return;
				}
#ifdef	LOG_ADD_DELETES
				db_log (ADD_DELETES, "[%s] %s!%s ADD %s %s\n", date (), source, userhost, s2, s3);
#endif
				ADDITIONS++;
				if (s2[0] == 'i' && s2[1] == 'l' && s2[2] == 'c')
				{
					db_log (URL2, "%s ([%s] %s!%s): %s\n", s2, date (), source, userhost, s3);
				}
				else
					db_log (URL2, "%s %s\n", s2, s3);
				L067n (source, source);
#ifdef	REQ_ACCESS_ADD
			}
#endif
		}
#endif
		else if (stricmp (cmd, "WHERE") == 0
				 || stricmp (cmd, "WHO") == 0 || stricmp (cmd, "WHAT") == 0)
		{
			s2 = strtok (NULL, " ");
			if (s2 == NULL)
			{
				L086n (source, source);
				return;
			}
			s3 = strtok (NULL, " ");
			if (s3 == NULL)
				return;
			strlwr (s3);
			ptr3 = strchr (s3, '?');
			if (ptr3 != NULL)
				memmove (ptr3, ptr3 + 1, strlen (ptr3 + 1) + 1);
			ptr3 = strchr (s3, '!');
			if (ptr3 != NULL)
				memmove (ptr3, ptr3 + 1, strlen (ptr3 + 1) + 1);
			if (stricmp (s3, "A") == 0 || stricmp (s3, "AN") == 0)
			{
				s4 = strtok (NULL, " ");
				if (s4 == NULL)
				{
					L087n (source, cmd, s2, s3, *CMDCHAR);
					return;
				}
				show_url (source, get_multiword_topic (s4), source, 1, 0, userhost, 0);
			}
			else
				show_url (source, get_multiword_topic (s3), source, 1, 0, userhost, 0);
		}
		else if (stricmp (cmd, "ADDUSER") == 0)
		{
			if (check_access (userhost, "#*", 0, source) >= 3)
			{
				s4 = strtok (NULL, " ");
				s = strtok (NULL, " ");
				s2 = strtok (NULL, " ");
				s5 = strtok (NULL, " ");
				if (s == NULL || s4 == NULL || s2 == NULL || s5 == NULL)
				{
					L055 (source);
					return;
				}
				sn = atoi (s2);
				if (sn > 10 || sn <= 0)
					return;
				if (strlen (s) < 7)
					return;
				L056 (*CMDCHAR);
				add_helper (s4, mask_from_nick(s, target), sn, 0, temp, s5, 0);
				L057 (source, mask_from_nick(s, target), sn);
				save_changes ();
			}
		}
		else if (stricmp (cmd, "DIE") == 0 || stricmp (cmd, "QUIT") == 0)
		{
			s = strtok (NULL, "");
			if (check_access (userhost, "#*", 0, source) >= 3)
			{
				if (s == NULL)
				{
					L032 (source);
				}
				else
					Snow ("QUIT :K\2\2illed (%s (%s))\n", source, s);
				db_sleep (1);
#ifdef	WIN32
				printf ("\n\nGood-bye! %s (c) Jason Hamilton\n\n", dbVersion);
				uptime = time (NULL) - uptime;
				printf
					("Time elapsed: %ld hour%s, %ld min%s\n\n",
					 uptime / 3600,
					 uptime / 3600 == 1 ? "" : "s",
					 (uptime / 60) % 60, (uptime / 60) % 60 == 1 ? "" : "s");
				db_sleep (5);
#endif
				exit (0);
			}
#if CTCP == 1
		}
		else if (stricmp (cmd, "\1VERSION\1") == 0)
		{
			if (cf (userhost, source, target))
				return;
			if (cf (userhost, source, target))
				return;
			S ("NOTICE %s :\1VERSION Hi, I'm a Darkbot. Download me from http://www.darkbot.org\1\n", source);
		}
		else if (stricmp (cmd, "\1PING") == 0)
		{
			if (cf (userhost, source, target))
				return;
			if (cf (userhost, source, target))
				return;
			s2 = strtok (NULL, "");
			if (s2 != NULL)
			{
				if (strlen (s2) > 21)
					s2[21] = '\0';
				S ("NOTICE %s :\1PING %s\n", source, s2);
			}
#endif
		}
		else if (stricmp (cmd, "LOGIN") == 0)
		{
			s = strtok (NULL, " ");
			if (s == NULL)
				return;
			do_login (source, s);
		}
		else
			show_url (source, get_multiword_topic (cmd), source, 1, 0, userhost, 0);
		return;
	}
	add_user (target, source, userhost, 0);	/* Unidle */

	/* ------ Commands that require a CMDCHAR to activate ------ */
	if (*cmd == *CMDCHAR)
	{
		if (Sleep_Toggle == 1)
			return;
		cmd++;
		if (cf (userhost, source, target))
			return;
		if (stricmp (cmd, "USERLIST") == 0
			|| stricmp (cmd, "HLIST") == 0 || stricmp (cmd, "ACCESS") == 0)
		{
			if (check_access (userhost, target, 0, source) == 0)
				return;
		
			if ((s = strtok (NULL, " ")) != NULL)
			{
				show_helper_list (source, atoi (s));
			}
			else
				show_helper_list (source, 0);
		}
        else if (stricmp (cmd, "BANLIST") == 0)
		{
			if (check_access (userhost, target, 0, source) == 0)
				return;
			show_banlist (source);
		}
		else if (stricmp (cmd, "LANG") == 0 || stricmp (cmd, "LANGUAGE") == 0)
		{
			S ("PRIVMSG %s :%s, %s\n", target, source, I_SPEAK);
		}
		else if (stricmp (cmd, "MASK") == 0) 
		{
			s2 = strtok(NULL, " ");
			if(s2 == NULL)
				return;
			S ("privmsg %s :%s: %s\n", target, source, 
				mask_from_nick(s2, target));
			return;
		}
		else if (stricmp (cmd, "CHANINFO") == 0)
		{
			if ((s2 = strtok (NULL, " ")) == NULL)
			{
				show_chaninfo (source, target, target);
				return;
			}
			else
			{
				/* If s2 is not a valid channel name, just use the current channel */
				show_chaninfo (source, ((*s2 == '#' || *s2 == '&' || *s2 == '+') ? s2 : target), target);
				return;
			}

		}
		else if (stricmp (cmd, "CHANUSERS") == 0)
		{
			if ((s2 = strtok (NULL, " ")) == NULL)
			{
				show_chanusers (source, target);
				return;
			}
			else
			{
				/* If s2 is not a valid channel name, just use the current channel. */
				show_chanusers (source, ((*s2 == '#' || *s2 == '&' || *s2 == '+') ? s2 : target));
				return;
			}
		}
// --------------------- RANDQ STUFF --------------------- //

#if		RANDQ == ON
		else if ((stricmp (cmd, "RANDQ") == 0) ||
			(stricmp (cmd, "RANDQUOTE") == 0))
		{		// RANDQ_NORMAL
			s2 = strtok (NULL, "");
			do_randq (s2, RANDQ_NORMAL, target, source);
		}
		else if ((stricmp (cmd, "QUOTE") == 0))
		{
			do_randq (s2, RANDQ_RAND, target, source);
		}
		else if ((stricmp (cmd, "RANDQ2") == 0) ||
			(stricmp (cmd, "RANDQUOTE2") == 0))
		{		// RANDQ_CASE
			s2 = strtok (NULL, "");
			do_randq(s2, RANDQ_CASE, target, source);
		}
#endif
// ------------------------------------------------------- //

		else if (stricmp (cmd, "IDLE") == 0)
		{
			s2 = strtok (NULL, " ");
			if (s2 == NULL)
				return;
			if (stricmp (s2, source) == 0)
			{
				S ("PRIVMSG %s :%s, don't be lame.\n", target, source);
				return;
			}
			unixtime = return_useridle (target, s2, 0);
			if (unixtime == 0)
			{
				S ("PRIVMSG %s :%s, I do not see %s in %s.\n", target, source, s2, target);
				return;
			}
			unixtime = time (NULL) - unixtime;
			if (unixtime > 86400)
				S ("PRIVMSG %s :%s, %s has been idle %d day%s, %02d:%02d\n",
				   target, source, s2, unixtime / 86400,
				   (unixtime / 86400 == 1) ? "" : "s",
				   (unixtime / 3600) % 24, (unixtime / 60) % 60);
			else if (unixtime > 3600)
				S ("PRIVMSG %s :%s, %s has been idle %d hour%s, %d min%s\n",
				   target, source, s2, unixtime / 3600,
				   unixtime / 3600 == 1 ? "" : "s",
				   (unixtime / 60) % 60, (unixtime / 60) % 60 == 1 ? "" : "s");
			else
				S ("PRIVMSG %s :%s, %s has been idle %d minute%s, %d sec%s\n",
				   target, source, s2, unixtime / 60,
				   unixtime / 60 == 1 ? "" : "s", unixtime % 60, unixtime % 60 == 1 ? "" : "s");
		}
		else if (stricmp (cmd, "N") == 0 || stricmp (cmd, "NICK") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				// If no nick was specified...
				if((s = strtok (NULL, " ")) == NULL)
				{
					L036 (source);
					return;
				}
				
				// If the nick specified contains illegal characters...
				if(strspn(s, LEGAL_NICK_TEXT) != strlen(s))
				{
//					L036n (source);
					S ("notice %s :The nickname %s contains illegal characters.\n",
						source, s);
					return;
				}

				strncpy (Mynick, s, sizeof (Mynick));
				strncpy (s_Mynick, Mynick, sizeof (s_Mynick));
				snprintf (NICK_COMMA, sizeof (NICK_COMMA), "%s,", Mynick);
				snprintf (COLON_NICK, sizeof (COLON_NICK), "%s:", Mynick);
				snprintf (BCOLON_NICK, sizeof (BCOLON_NICK), "%s\2:\2", Mynick);
				L037 (source, Mynick);
				S ("NICK %s\n", Mynick);
			}
			else
				L038 (source, source);
		}
		else if (stricmp (cmd, "L") == 0
				 || stricmp (cmd, "PART") == 0
				 || stricmp (cmd, "LEAVE") == 0 || stricmp (cmd, "P") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 2)
			{
				s = strtok (NULL, "");
				if (s == NULL)
				{
					S ("PART %s\n", target);
				}
				else
				{
					S ("PART %s\n", s);
					L039 (target, s);
				}
			}
		}
		else if (stricmp (cmd, "VARIABLES") == 0)
		{
			S ("PRIVMSG %s :%s, %s\n", source, source, myVariables);
		}
		else if (stricmp (cmd, "JOIN") == 0 || stricmp (cmd, "J") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 2)
			{
				s = strtok (NULL, "");
				if (s == NULL)
				{
					S ("JOIN %s\n", target);
				}
				else
				{
					S ("JOIN %s\n", s);
					L040 (target, s);
				}
			}
#if DO_CHANBOT_CRAP == 1
		}
		else if (stricmp (cmd, "OP") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 2)
			{
				s = strtok (NULL, "");
				if (s == NULL)
				{
					S ("PRIVMSG %s :Specify a nick!\n", target);
					return;
				}
				else
				{
					S ("MODE %s +oooooo %s\n", target, s);
				}
			}
		}
		else if (stricmp (cmd, "DEOP") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 2)
			{
				s = strtok (NULL, "");
				if (s == NULL)
				{
					S ("PRIVMSG %s :Specify a nick!\n", target);
					return;
				}
				else
				{
					S ("MODE %s -oooooo %s\n", target, s);
				}
			}
		}
		else if (stricmp (cmd, "DOWN") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 2)
				S ("MODE %s -o %s\n", target, source);
		}
		else if (stricmp (cmd, "UP") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 2)
				S ("MODE %s +o %s\n", target, source);
		}
		else if ((stricmp (cmd, "KICK") == 0
				  || stricmp (cmd, "WACK") == 0
				  || stricmp (cmd, "K") == 0 || stricmp (cmd, "NAIL") == 0))
		{
			if (check_access (userhost, target, 0, source) >= 2)
			{
				s = strtok (NULL, " ");
				if (s == NULL)
				{
					S ("PRIVMSG %s :Specify a nick/chan!\n", target);
					return;
				}
				else
				{
					if (*s != '#' && *s != '&')
					{
						s2 = strtok (NULL, "");
						if (s2 == NULL)
						{
							if (stricmp (s, Mynick) == 0)
							{
								S ("KICK %s %s :hah! As *IF*\n", target, source);
							}
							else
								S ("KICK %s %s :\2%s\2'ed: %s\n", target, s, cmd, DEFAULT_KICK);
						}
						else if (stricmp (s, Mynick) == 0)
						{
							S ("KICK %s %s :%s\n", target, s, s2);
						}
						else
							S ("KICK %s %s :\2%s\2'ed: %s\n", target, s, cmd, s2);
					}
					else
					{
						s2 = strtok (NULL, " ");
						if (s2 == NULL)
						{
							S ("NOTICE %s :You must specify a nick to kick from %s!\n", source, s);
						}
						else
						{
							s3 = strtok (NULL, "");
							if (s3 == NULL)
							{
								if (stricmp (s2, Mynick) == 0)
								{
									S ("KICK %s %s :hah! As *IF*\n", s, source);
								}
								else
									S ("KICK %s %s :\2%s\2ed: %s\n", s, s2, cmd, DEFAULT_KICK);
							}
							else
							{

								if (stricmp (s2, Mynick) == 0)
								{
									S ("KICK %s %s :hah! As *IF* (%s)\n", s, source);
								}
								else
									S ("KICK %s %s :\2%s\2ed: %s\n", s, s2, cmd, s3);
							}
						}
					}
				}
			}
#endif
		}
		else if (stricmp (cmd, "CYC") == 0 || stricmp (cmd, "CYCLE") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 2)
			{
				s = strtok (NULL, "");
				if (s == NULL)
				{
					S ("PART %s\n", target);
					S ("JOIN %s\n", target);
				}
				else
				{
					S ("PART %s\n", s);
					S ("JOIN %s\n", s);
					S ("PRIVMSG %s :Cycling %s\n", target, s);
				}
			}
		}
		else if (stricmp (cmd, "DIE") == 0 || stricmp (cmd, "QUIT") == 0)
		{
			s = strtok (NULL, "");
			if (check_access (userhost, target, 0, source) >= 3)
			{
				if (s == NULL)
				{
					L032 (source);
				}
				else
					Snow ("QUIT :K\2\2illed (%s (%s))\n", source, s);
				db_sleep (1);
#ifdef	WIN32
				printf ("\n\nGood-bye! %s (c) Jason Hamilton\n\n", dbVersion);
				uptime = time (NULL) - uptime;
				printf
					("Time elapsed: %ld hour%s, %ld min%s\n\n",
					 uptime / 3600,
					 uptime / 3600 == 1 ? "" : "s",
					 (uptime / 60) % 60, (uptime / 60) % 60 == 1 ? "" : "s");
				db_sleep (5);
#endif
				exit (0);
			}
#if DO_CHANBOT_CRAP == 1
		}
		else if (stricmp (cmd, "DEV") == 0
				 || stricmp (cmd, "DV") == 0
				 || stricmp (cmd, "DEVOICE") == 0 || stricmp (cmd, "DVOICE") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 1)
			{
				s = strtok (NULL, "");
				if (s == NULL)
				{
					L041 (target);
					return;
				}
				else
					S ("MODE %s -vvvvvvv %s\n", target, s);
			}
		}
		else if (stricmp (cmd, "VOICE") == 0 || stricmp (cmd, "V") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 1)
			{
				s = strtok (NULL, "");
				if (s == NULL)
				{
					L041 (target);
					return;
				}
				else
					S ("MODE %s +vvvvvvv %s\n", target, s);
			}
		}
		else if (stricmp (cmd, "T") == 0 || stricmp (cmd, "TOPIC") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 2)
			{
				s = strtok (NULL, "");
				if (s == NULL)
				{
					S ("TOPIC %s :\n", target);
					return;
				}
				else
				{
					S ("TOPIC %s :%s\n", target, s);
				}
			}
#endif
		}
		else if (stricmp (cmd, "JUMP") == 0 || stricmp (cmd, "SERVER") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				s = strtok (NULL, " ");
				if (s == NULL)
				{
					S ("NOTICE %s :Syntax: JUMP <server> [port]\n", source);
					return;
				}
				s2 = strtok (NULL, " ");
				if (s2 == NULL)
				{
					sn = 6667;
				}
				else
					sn = atoi (s2);
				S ("QUIT :Jumping to %s:%d\n", s, sn);
				db_sleep (1);
				strcpy (BS, s);
				BP = sn;
				prepare_bot ();
				register_bot ();
			}
#if DO_CHANBOT_CRAP == 1
		}
		else if (stricmp (cmd, "DELBAN") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				s = strtok (NULL, " ");
				if (s == NULL)
				{
					L042 (source);
					return;
				}
				if (del_permban (source, s) == 1)
					S ("MODE %s -b %s\n", target, s);
				else
					L043 (source);
			}
#endif
		}
		else if (stricmp (cmd, "DELUSER") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				s = strtok (NULL, " ");
				if (s == NULL)
				{
					L044 (source);
					return;
				}
				delete_user_ram (source, s);
			}
#if DO_CHANBOT_CRAP == 1
		}
		else if (stricmp (cmd, "TEASEOP") == 0 || stricmp (cmd, "TO") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 2)
			{
				s = strtok (NULL, " ");
				if (s == NULL)
				{
					L036 (target);
					return;
				}
				if (stricmp (s, Mynick) == 0)
				{
					L045 (source);
				}
				else
					S ("MODE %s +o-o+o-o+o-o %s %s %s %s %s %s\n", target, s, s, s, s, s, s);
			}
#endif
#ifndef	WIN32
		}
		else if (stricmp (cmd, "BACKUP") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				snprintf (temp, sizeof (temp), "/bin/cp -rf %s \"%s.bak @ `date`\"\n", URL2, URL2);
				system (temp);
				L046 (target);
			}
#endif
		}
		else if (stricmp (cmd, "AUTOTOPIC") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				s = strtok (NULL, "");
				if (s == NULL)
				{
					L047 (source, *CMDCHAR);
					return;
				}
				set_autotopic (source, target, s);
			}
		}
		else if (stricmp (cmd, "SETCHAN") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				s = strtok (NULL, " ");
				if (s == NULL)
				{
					L048 (source);
					return;
				}
				strncpy (CHAN, s, sizeof (CHAN));
				L049 (source, CHAN);
				save_setup ();
			}
		}
		else if (stricmp (cmd, "SETCHAR") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				s = strtok (NULL, " ");
				if (s == NULL)
				{
					L050 (source);
					return;
				}
				*CMDCHAR = *s;
				L051 (source, *CMDCHAR);
				save_setup ();
			}
		}
		else if (stricmp (cmd, "SETUSER") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				s = strtok (NULL, " ");
				if (s == NULL)
				{
					L052 (source);
					return;
				}
				strncpy (UID, s, sizeof (UID));
				L053 (source, UID);
				save_setup ();
			}
		}
		else if (stricmp (cmd, "VHOST") == 0 || stricmp (cmd, "SETHOST") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				s = strtok (NULL, " ");
				if (s == NULL)
				{
					L091 (source);
					return;
				}
				strncpy (VHOST, s, sizeof (VHOST));
				L092 (source, VHOST);
				save_setup ();
			}
		}
		else if (stricmp (cmd, "PERFORMS") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				/* Set the default umodes */
				S ("MODE %s %s\n", Mynick, DEFAULT_UMODE);

				/* Run performs */
				run_perform ();
				S ("privmsg %s :%s, performs have been executed.\n",
					target, source);
				return;
			}
		}
		else if (stricmp (cmd, "SETNICK") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				if ((s = strtok (NULL, " ")) == NULL)
				{
					L054 (source);
					return;
				}

				if (strspn (s, LEGAL_NICK_TEXT) != strlen (s))
				{
					S ("notice %s :The nickname %s contains illegal characters.\n",
						source, s);
					return;
				}

				S ("NICK %s\n", s);
				strncpy (s_Mynick, s, sizeof (s_Mynick));
				strncpy (Mynick, s, sizeof (Mynick));
				snprintf (NICK_COMMA, sizeof (NICK_COMMA), "%s,", Mynick);
				snprintf (COLON_NICK, sizeof (COLON_NICK), "%s:", Mynick);
				snprintf (BCOLON_NICK, sizeof (BCOLON_NICK), "%s\2:\2", Mynick);
				save_setup ();
			}
		}
		else if (stricmp (cmd, "RAW") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				s = strtok (NULL, "");
				if (s != NULL)
					S ("%s\n", s);
			}
		}
		else if (stricmp (cmd, "SEEN") == 0 && SeeN == 1)
		{
			s = strtok (NULL, " ");
			if (s == NULL)
			{
				count_seen (source, target);
				return;
			}
			if (return_useridle (target, s, 1) == 1)
			{
				S ("PRIVMSG %s :%s is right here in the channel!\n", target, s);
				return;
			}
			show_seen (s, source, target);
#if STATUS == 1
		}
		else if (stricmp (cmd, "LUSERS") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 1)
				S ("LUSERS\n");
#endif
		}
		else if (stricmp (cmd, "ADDUSER") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				s4 = strtok (NULL, " ");
				s = strtok (NULL, " ");
				s2 = strtok (NULL, " ");
				s5 = strtok (NULL, " ");
				if (s == NULL || s4 == NULL || s2 == NULL || s5 == NULL)
				{
					L055 (source);
					return;
				}
				sn = atoi (s2);
				if (sn > 10 || sn <= 0)
					return;
				if (strlen (s) < 7)
					return;
				L056 (*CMDCHAR);
				add_helper (s4, mask_from_nick(s, target), sn, 0, temp, s5, 0);
				L057 (source, mask_from_nick(s, target), sn);
				save_changes ();
			}
#if DO_CHANBOT_CRAP == 1
		}
		else if (stricmp (cmd, "PERMBAN") == 0 || stricmp (cmd, "SHITLIST") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				s = strtok (NULL, " ");
				if (s == NULL)
				{
					L058 (source, *CMDCHAR, cmd);
					return;
				}
				s2 = strtok (NULL, "");
				if (s2 == NULL)
					s2 = "Permbanned!";
				add_permban (s, 0, s2);
				L059 (source, PERMBAN_counter, s, s2);
				save_permbans ();
				S ("MODE %s +b %s\n", target, s);
			}
#endif
		}
		else if (stricmp (cmd, "ALARM") == 0 || stricmp (cmd, "ALARMCLOCK") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 2)
			{
				s = strtok (NULL, " ");
				s2 = strtok (NULL, "");
				if (s == NULL || s2 == NULL)
				{
					S ("NOTICE %s :Syntax: <time type: d/h/m><time> <text to say>\n", source);
					return;
				}
				if (strlen (s) < 2)
				{
					S ("NOTICE %s :Syntax: <time type: d/h/m><time> <text to say>\n", source);
					return;
				}
				if (*s == 'd')
				{
					sn = 86400;
					s++;
				}
				else if (*s == 'h')
				{
					sn = 3600;
					s++;
				}
				else if (*s == 'm')
				{
					sn = 60;
					s++;
				}
				else
				{
					S ("NOTICE %s :Syntax: <time type: \2d/h/m\2><time> <text to say>\n", source);
					return;
				}
				if (strspn (s, NUMBER_LIST) != strlen (s))
				{
					S ("NOTICE %s :Time must be a number.\n", source);
					return;
				}
				i = (atoi (s) * sn) + time (NULL);
				snprintf (temp, sizeof (temp), "%s/%ld", DBTIMERS_PATH, i);
				db_log (temp,
					 "PRIVMSG %s :\2ALARMCLOCK\2 by %s!%s: %s\n", target, source, userhost, s2);
				unixtime = atoi (s) * sn;
				if (unixtime > 86400)
					S ("PRIVMSG %s :%s, alarmclock set to go off in %d day%s, %02d:%02d\n", target,
					   source, unixtime / 86400, (unixtime / 86400 == 1) ? "" : "s",
					   (unixtime / 3600) % 24, (unixtime / 60) % 60);
				else if (unixtime > 3600)
					S ("PRIVMSG %s :%s, alarmclock set to go off in %d hour%s, %d min%s\n", target,
					   source, unixtime / 3600, unixtime / 3600 == 1 ? "" : "s",
					   (unixtime / 60) % 60, (unixtime / 60) % 60 == 1 ? "" : "s");
				else
					S ("PRIVMSG %s :%s, alarmclock set to go off in %d minute%s, %d sec%s\n",
					   target, source, unixtime / 60, unixtime / 60 == 1 ? "" : "s", unixtime % 60,
					   unixtime % 60 == 1 ? "" : "s");
			}
		}
		else if (stricmp (cmd, "REPEAT") == 0 || stricmp (cmd, "TIMER") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				s = strtok (NULL, " ");
				s2 = strtok (NULL, " ");
				s3 = strtok (NULL, "");
				if (s == NULL || s2 == NULL || s3 == NULL)
				{
					L060 (source);
					return;
				}
				sn = atoi (s);
				sn2 = atoi (s2);
				while (sn > 0)
				{
					S ("%s\n", s3);
					sn--;
					db_sleep (sn2);
				}
			}
#ifndef	WIN32
		}
		else if (stricmp (cmd, "REHASH") == 0 || stricmp (cmd, "RESTART") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 3)
			{
				L062 (dbVersion);
				snprintf (temp, sizeof (temp), "sleep 2; %s", DARKBOT_BIN);
				system (temp);
				db_sleep (1);
				exit (0);
			}
#endif
		}
		else if (stricmp (cmd, "PING") == 0)
		{
			if (check_access (userhost, target, 0, source) == 0)
			{
				S ("NOTICE %s PONG!\n", source);
			}
			else
				S ("PRIVMSG %s :PONG!\n", target);
		}
		else if (stricmp (cmd, "HELP") == 0)
		{
			L100 (source, NICK_COMMA, COLON_NICK, BCOLON_NICK, Mynick, NICK_COMMA, NICK_COMMA);
			db_sleep (3);
			if (cf (userhost, source, target))
				return;
			L101 (source, NICK_COMMA, NICK_COMMA, NICK_COMMA);
			db_sleep (2);
		}
		else if (stricmp (cmd, "SETINFO") == 0)
		{
			if (check_access (userhost, target, 0, source) >= 1)
			{
				s = strtok (NULL, "");
				if (s == NULL)
				{
					S ("NOTICE %s :%s\n", source, mySetinfo);
					return;
				}
				update_setinfo (userhost, s, source);
				save_changes ();
			}
		}
#if CTCP == 1
	}
	else if (stricmp (cmd, "\1VERSION\1") == 0)
	{							/* these are #chan
								   * ctcp's */
		if (cf (userhost, source, target))
			return;
		if (cf (userhost, source, target))
			return;
		S ("NOTICE %s :\1VERSION %s\1\n", source, 
			"Hi, I'm a Darkbot. Download me from http://www.darkbot.org");
	}
	else if (stricmp (cmd, "\1PING") == 0)
	{
		if (cf (userhost, source, target))
			return;
		if (cf (userhost, source, target))
			return;
		s2 = strtok (NULL, "");
		if (s2 != NULL)
		{
			if (strlen (s2) > 21)
				s2[21] = '\0';
			S ("NOTICE %s :\1PING %s\n", source, s2);
		}
#endif
	}
	else if (stricmp (cmd, "\2\2DARKBOT") == 0)
	{
		if (Sleep_Toggle == 1)
			return;
		if (cf (userhost, source, target))
			return;
		S ("PRIVMSG %s :%s reporting! My cmdchar is %c\n", target, dbVersion, *CMDCHAR);
	}
	else if (stricmp (cmd, NICK_COMMA) == 0
			 || stricmp (cmd, COLON_NICK) == 0
			 || stricmp (cmd, BCOLON_NICK) == 0 || stricmp (cmd, Mynick) == 0)
	{
		s = strtok (NULL, " ");
		if (s != NULL)
		{
			if (stricmp (s, "WAKEUP") == 0)
			{
				if (Sleep_Toggle == 0)
					return;
				if (check_access (userhost, target, 0, source) >= SLEEP_LEVEL)
				{
					Sleep_Toggle = 0;
					AIL4 = 0;
					S ("PRIVMSG %s :%s\n", target, WAKEUP_ACTION);
					if (stricmp (sleep_chan, target) != 0)
						S ("PRIVMSG %s :%s\n", sleep_chan, WAKEUP_ACTION);
					return;
				}
			}
		}
		if (Sleep_Toggle == 1)
			return;
		if (cf (userhost, source, target))
			return;
		if (s != NULL)
		{
#ifdef	RANDOM_STUFF
			if (stricmp (s, "RANDOMSTUFF") == 0 || stricmp (s, "RANDSTUFF") == 0 ||
				stricmp (s, "RS") == 0)
			{
				if (check_access (userhost, target, 0, source) >= RAND_LEVEL)
				{
					s2 = strtok (NULL, "");
					if (s2 == NULL)
					{
						L064 (target, source);
						return;
					}
					add_randomstuff (source, target, s2);
				}
			}
			else
#endif
			if (stricmp (s, "ADD") == 0
					|| stricmp (s, "REMEMBER") == 0
					|| stricmp (s, "SAVE") == 0 || stricmp (s, "STORE") == 0)
			{
#ifdef	REQ_ACCESS_ADD
				if (check_access (userhost, target, 0, source) >= 1)
				{
#endif
					s2 = strtok (NULL, " ");
					if (s2 == NULL)
					{
						L065 (target, source);
						return;
					}

					// Fix for some segmenation fault problems
					// concerning topics consisting entirely of
					// wildcard characters.
					if (strspn(s2, "*?") == strlen(s2))
					{
						Lbadtopic(target,source);
						return;
					}

					if (strlen (s2) > MAX_TOPIC_SIZE)
					{
						s2[MAX_TOPIC_SIZE] = '\0';
						S ("PRIVMSG %s :%s, topic is over the limit, and has characters truncated.\n", target, source);
					}
					s3 = strtok (NULL, "");
					if (s3 == NULL)
					{
						L066 (target, source, s2);
						return;
					}
					if (strlen (s3) > MAX_DATA_SIZE)
						s3[MAX_DATA_SIZE] = '\0';
					strlwr (s2);
					if (*s2 == '~')
					{
						S ("PRIVMSG %s :%s, rdb files can only be called from the data of a topic, they cannot be used in the topic itself.\n", target, source);
						return;
					}
					if (check_existing_url (source, s2, target) == 1)
					{
						S ("PRIVMSG %s :%s \37%s\37\n", target, EXISTING_ENTRY, s2);
						return;
					}
#ifdef	LOG_ADD_DELETES
					db_log (ADD_DELETES, "[%s] %s!%s ADD %s %s\n", date (), source, userhost, s2, s3);
#endif
					ADDITIONS++;
					if (s2[0] == 'i' && s2[1] == 'l' && s2[2] == 'c')
					{
						db_log (URL2, "%s ([%s] %s!%s): %s\n", s2, date (), source, userhost, s3);
					}
					else
						db_log (URL2, "%s %s\n", s2, s3);
					L067 (target, source);
#ifdef	REQ_ACCESS_ADD
				}
#endif
			}
			else if (stricmp (s, "DATE") == 0 || stricmp (s, "TIME") == 0)
			{
				S ("PRIVMSG %s :%s, %s\n", target, source, date ());
			}
			else if (stricmp (s, "REPLACE") == 0)
			{
#ifdef	REQ_ACCESS_ADD
				if (check_access (userhost, target, 0, source) >= 1)
				{
#endif
					s2 = strtok (NULL, " ");
					if (s2 == NULL)
					{
						L068 (target, source);
						return;
					}
					if (strlen (s2) > MAX_TOPIC_SIZE)
						s2[MAX_TOPIC_SIZE] = '\0';
					s3 = strtok (NULL, "");
					if (s3 == NULL)
					{
						L069 (target, source, s2);
						return;
					}
					if (strlen (s3) > MAX_DATA_SIZE)
						s3[MAX_DATA_SIZE] = '\0';
					strlwr (s2);
					if (check_existing_url (source, s2, target) != 1)
					{
						S ("PRIVMSG %s :%s \37%s\37\n", target, NO_ENTRY, s2);
						return;
					}
					delete_url (source, s2, target);
#ifdef	LOG_ADD_DELETES
					db_log (ADD_DELETES,
						 "[%s] %s!%s REPLACE %s %s\n", date (), source, userhost, s2, s3);
#endif
					ADDITIONS++;
					db_log (URL2, "%s %s\n", s2, s3);
					L070 (target, source, s2);
#ifdef	REQ_ACCESS_ADD
				}
#endif
#if DO_CHANBOT_CRAP == 1
			}
			else if (stricmp (s, "PERMBANS?") == 0)
			{
				L071 (target,
					  (PERMBAN_counter ==
					   1) ? "is" : "are", PERMBAN_counter, (PERMBAN_counter == 1) ? "" : "s");
#endif
#ifdef	RANDOM_STUFF
			}
			else if (stricmp (s, "RANDOMSTUFF?") == 0 || stricmp (s, "RANDSTUFF?") == 0)
			{
				L073 (target, source, Rand_Stuff);
#endif
			}
// --------------------- RANDQ STUFF --------------------- //
#if		RANDQ == ON
			else if ((stricmp (s, "RANDQ") == 0) ||
					 (stricmp (s, "RANDQUOTE") == 0))
			{		// RANDQ_NORMAL
				s2 = strtok (NULL, "");
				do_randq (s2, RANDQ_NORMAL, target, source);
			}
			else if ((stricmp (s, "RANDQ2") == 0) ||
					 (stricmp (s, "RANDQUOTE2") == 0))
			{		// RANDQ_CASE
				s2 = strtok (NULL, "");
				do_randq(s2, RANDQ_CASE, target, source);
			}
#endif
// ------------------------------------------------------- //
			else if (stricmp (s, "LENGTH") == 0)
			{
				s2 = strtok (NULL, "");
				if (s2 == NULL)
					return;
				L074 (target, source, strlen (s2));
			}
			else if (stricmp (s, "CHAR") == 0)
			{
				s2 = strtok (NULL, " ");
				if (s2 == NULL)
					return;
				S ("PRIVMSG %s :%s, %c -> %d\n", target, source, s2[0], s2[0]);
			}
			else if (stricmp (s, "DBSIZE") == 0 || stricmp (s, "INFOSIZE") == 0)
			{
				if (check_access (userhost, target, 0, source) >= 2)
				{
					if (stat (URL2, &statbuf) == 0)
					{
						S ("PRIVMSG %s :My database file is presently %ld bytes big.\n", target,
						   statbuf.st_size);
					}
					else
					{
						return;
					}
				}
			}
			else if (stricmp (s, "SEEN") == 0 && SeeN == 1)
			{
				s2 = strtok (NULL, " ");
				if (s2 == NULL)
					return;
				show_seen (s2, source, target);
			}
			else if (stricmp (s, "SENDQ?") == 0 || stricmp (s, "QUE?") == 0)
			{
				L075 (target, source, get_sendq_count (2), (get_sendq_count (2) == 1) ? "" : "s");
			}
			else if (stricmp (s, "JOINS?") == 0)
			{
				L076 (target, JOINs);
			}
			else if (stricmp (s, "LOCATION?") == 0)
			{
				L077 (target, (snr == 1) ? "is" : "are", snr, (snr == 1) ? "" : "s", spr);
			}
			else if (stricmp (s, "CMDCHAR?") == 0)
			{
				L078 (target, source, *CMDCHAR);
			}
			else if (stricmp (s, "DATASEARCH") == 0
					 || stricmp (s, "DSEARCH") == 0 || stricmp (s, "DFIND") == 0)
			{
				s2 = strtok (NULL, "");
				if (s2 == NULL)
				{
					L079 (target, s, source);
					return;
				}
				datasearch (source, s2, target);
			}
			else if (stricmp (s, "SEARCH") == 0
					 || stricmp (s, "LOOK") == 0 || stricmp (s, "FIND") == 0)
			{
				s2 = strtok (NULL, " ");
				if (s2 == NULL)
				{
					if (stricmp (s, "FIND") == 0)
					{
						S ("PRIVMSG %s :%s, %s?\n", target, TRY_FIND, source);
					}
					else
						L079 (target, s, source);
					return;
				}
				find_url (source, s2, target);
			}
			else if (stricmp (s, "STATS") == 0)
			{
				s2 = strtok (NULL, "");
				if (s2 == NULL)
					get_stats (target, NULL);
				else
					get_stats (target, s2);
			}
			else if (stricmp (s, "QUIZ") == 0)
			{
#if QUIZ == 1
				if (quiz_halt == 0)
					run_quiz_question (target);
#endif
			}
#if GOOGLE == 1
            else if (stricmp (s, "GOOGLE") == 0)
            {
                s2 = strtok (NULL, "");
                if(s2 == NULL)
                {
                    S("PRIVMSG %s :Google what?\n", target);
                    return;
                }
                web_post_query(s, source, userhost, target, s2, strlen(s2));
            }
#endif
#if METAR == 1
            else if (stricmp (s, "METAR") == 0)
            {
                s2 = strtok (NULL, "");
                if(s2 == NULL)
                {
                    S("PRIVMSG %s :Metar what?\n", target);
                    return;
                }
                web_post_query(s, source, userhost, target, s2, strlen(s2));
            }
#endif
#if TAF == 1
            else if (stricmp (s, "TAF") == 0)
            {
                s2 = strtok (NULL, "");
                if(s2 == NULL)
                {
                    S("PRIVMSG %s :Taf what?\n", target);
                    return;
                }
                web_post_query(s, source, userhost, target, s2, strlen(s2));
            }
#endif
			else if (stricmp (s, "INFO2") == 0)
			{
				show_info2 (target, source);
			}
			else if (stricmp (s, "INFO") == 0)
			{
				info (source, target);
#ifdef	DO_MATH_STUFF
			}
			else if (stricmp (s, "CALC") == 0 || stricmp (s, "MATH") == 0)
			{
				s2 = strtok (NULL, "");
				if (s2 == NULL)
					return;
				if (strlen (s2) > 200)
					s2[200] = '\0';
				do_math (source, target, s2);
#endif
			}
			else if (stricmp (s, RESERVED1) == 0)
            {
                s2 = strtok (NULL, "");
                if ( s2 == NULL )
                    return;
                call_reserved_1 ( source, target, s2 );
            }
    		else if (stricmp (s, RESERVED2) == 0)
            {
                s2 = strtok (NULL, "");
                if ( s2 == NULL )
                    return;
                call_reserved_2 ( source, target, s2 );
            }
            else if (stricmp (s, "IGNORE") == 0)
            {
                if (check_access (userhost, target, 0, source) >= 1)
				{
					s2 = strtok (NULL, " ");
					if (s2 == NULL)
                    {
                        S ("PRIVMSG %s :ignore who %s?\n", target, source);
                        return;
                    }
                    if ( add_ignore_user_ram (s2) > 0 )
                        S ("PRIVMSG %s :%s, ignoring %s\n", target, source, s2 );
                    else
                        S ("PRIVMSG %s :%s, unable to ignore %s\n", target, source, s2 );
                }
            }
			else if (stricmp (s, "UNIGNORE") == 0)
            {
				if (check_access (userhost, target, 0, source) >= 1)
				{
					s2 = strtok (NULL, " ");
					if (s2 == NULL)
                    {
                        S ("PRIVMSG %s :unignore who %s?\n", target, source);
                        return;
                    }
                    if ( delete_ignore_user_ram (s2) > 0 )
                        S ("PRIVMSG %s :%s, unignoring %s\n", target, source, s2 );
                    else
                        S ("PRIVMSG %s :%s, unable to unignore %s :(\n", target, source, s2 );
                }
            }
            else if (stricmp (s, "SLEEP") == 0 || stricmp (s, "HUSH") == 0)
			{
				if (check_access (userhost, target, 0, source) >= SLEEP_LEVEL)
				{
					Sleep_Toggle = 1;
					
					if ((s2 = strtok (NULL, "")) == NULL)
						Sleep_Time = SLEEP_TIME;
					else
						if ((Sleep_Time = strtol (s2, (char **) NULL, Sleep_Time)) < 1)
							Sleep_Time = SLEEP_TIME;
				
					S ("PRIVMSG %s :%s\n", target, GOSLEEP_ACTION);
					strncpy (sleep_chan, target, sizeof (sleep_chan));

					/* If the user has specified a custom length of time to sleep for, send
					 * a notice reminding the user how long the bot will be asleep, in a 
					 * more readible format.
					 */

					if (Sleep_Time != SLEEP_TIME)
					{
						if (Sleep_Time > 86400)
							S ("NOTICE %s :Sleeping for %ld day%s, %02ld:%02ld.\n",
								source, Sleep_Time / 86400, 
								(Sleep_Time / 86400 == 1) ? "" : "s",
								(Sleep_Time / 3600) % 24, 
								(Sleep_Time / 60) % 60);
						else if (Sleep_Time > 3600)
							S ("NOTICE %s :Sleeping for %ld hour%s, %ld min%s.\n",
								source, Sleep_Time / 3600, 
								Sleep_Time / 3600 == 1 ? "" : "s",
								(Sleep_Time / 60) % 60, 
								(Sleep_Time/ 60) % 60 == 1 ? "" : "s");
						else
							S ("NOTICE %s :Sleeping for %ld minute%s, %ld sec%s.\n",
								source, Sleep_Time / 60,
								Sleep_Time / 60 == 1 ? "" : "s",
								Sleep_Time % 60,
								Sleep_Time % 60 == 1 ? "" : "s");
					}

				}
			}
			else if (stricmp (s, "UNIXTIME") == 0)
			{
				s2 = strtok (NULL, " ");
				if (s2 == NULL)
					return;
				unixtime = atoi (s2) - time (NULL);
				if (unixtime > 86400)
					S ("PRIVMSG %s :%s, %d day%s, %02d:%02d\n",
					   target, source, unixtime / 86400,
					   (unixtime / 86400 == 1) ? "" : "s",
					   (unixtime / 3600) % 24, (unixtime / 60) % 60);
				else if (unixtime > 3600)
					S ("PRIVMSG %s :%s, %d hour%s, %d min%s\n",
					   target, source, unixtime / 3600,
					   unixtime / 3600 == 1 ? "" : "s",
					   (unixtime / 60) % 60, (unixtime / 60) % 60 == 1 ? "" : "s");
				else
					S ("PRIVMSG %s :%s, %d minute%s, %d sec%s\n",
					   target, source, unixtime / 60,
					   unixtime / 60 == 1 ? "" : "s", unixtime % 60, unixtime % 60 == 1 ? "" : "s");
			}
			else if (stricmp (s, "CPU?") == 0)
			{
				getrusage (RUSAGE_SELF, &r_usage);
				S ("PRIVMSG %s :CPU usage: %ld.%06ld, System = %ld.%06ld\n",
				   target, r_usage.ru_utime.tv_sec, r_usage.ru_utime.tv_usec);
			}
			else if (stricmp (s, "DISPLAY") == 0)
			{
				s2 = strtok (NULL, " ");
				if (s2 == NULL)
					return;
				display_url (target, source, s2);
#ifndef	WIN32
			}
			else if (stricmp (s, "UPTIME") == 0)
			{
				snprintf (temp, sizeof (temp), "uptime\n");
				S ("PRIVMSG %s :Uptime: %s\n", target, run_program (temp));
			}
			else if (stricmp (s, "OS") == 0)
			{
				snprintf (temp, sizeof (temp), "uname\n");
				S ("PRIVMSG %s :I am running %s\n", target, run_program (temp));
			}
			else if (stricmp (s, "MEM") == 0 || stricmp (s, "RAM") == 0)
			{
				if (check_access (userhost, target, 0, source) >= 3)
				{
					snprintf (temp, sizeof (temp), "ps -u -p %d\n", getpid ());
					S ("PRIVMSG %s :ps: %s\n", target, run_program (temp));
				}
			}
			else if (stricmp (s, "RDB") == 0)
			{
				s2 = strtok (NULL, "");
				if (s2 == NULL)
				{
					snprintf (temp, sizeof (temp), "ls %s/*.rdb | wc\n", RDB_DIR);
					S ("PRIVMSG %s :RDB: %s\n", target, run_program (temp));
				}
				else
				{
					if (strspn (s2, SAFE_LIST) != strlen (s2))
					{
						S ("PRIVMSG %s :%s, rdb files are made up of letters and or numbers, no other text is accepted.\n", target, source);
						return;
					}
					snprintf (temp, sizeof (temp), "cat %s/%s.rdb | wc -l\n", RDB_DIR, s2);
					S ("PRIVMSG %s :%s\n", target, run_program (temp));
				}
#endif
			}
			else if (stricmp (s, "DELETE") == 0
					 || stricmp (s, "REMOVE") == 0
					 || stricmp (s, "FORGET") == 0 || stricmp (s, "DEL") == 0)
			{
#ifdef	REQ_ACCESS_DEL
				if (check_access (userhost, target, 0, source) >= 1)
				{
#endif
					s2 = strtok (NULL, " ");
					if (s2 == NULL)
					{
						S ("PRIVMSG %s :%s what, %s?\n", target, s, source);
						return;
					}
					if (strlen (s2) > MAX_TOPIC_SIZE)
						s2[MAX_TOPIC_SIZE] = '\0';
#ifdef	LOG_ADD_DELETES
					db_log (ADD_DELETES, "[%s] %s!%s DEL %s\n", date (), source, userhost, s2);
#endif
					if (*s2 == '~')
					{			/* need level 2 to delete .rdb files */
						if (check_access (userhost, target, 0, source) >= 2)
						{
							delete_url (source, s2, target);
						}
						return;
					}
					delete_url (source, s2, target);
#ifdef	REQ_ACCESS_DEL
				}
#endif
			}
			else if (stricmp (s, "TELL") == 0)
			{
				s2 = strtok (NULL, " ");
				if (s2 == NULL)
				{
					L085 (target, source);
					return;
				}
				s3 = strtok (NULL, " ");
				if (s3 == NULL)
				{
					L083 (target, source, s2);
					return;
				}
				if (stricmp (s3, Mynick) == 0)
					return;		/* don't bother telling
								   * myself about stuff */
				if (stricmp (s3, "ABOUT") == 0)
				{
					s4 = strtok (NULL, " ");
					if (s4 == NULL)
					{
						L084 (target, source, s2);
						return;
					}
					strlwr (s4);
					show_url (s2, get_multiword_topic (s4), target, 1, 0, userhost, 1);
				}
				else
				{
					strlwr (s3);
					show_url (s2, get_multiword_topic (s3), target, 1, 0, userhost, 1);
				}
			}
			else if (stricmp (s, "WHISPER") == 0)
			{
				s2 = strtok (NULL, " ");
				if (s2 == NULL)
				{
					L085 (target, source);
					return;
				}
				s3 = strtok (NULL, " ");
				if (s3 == NULL)
				{
					L083 (target, source, s2);
					return;
				}
				if (stricmp (s3, Mynick) == 0)
					return;		/* don't bother telling
								   * myself about stuff */
				if (stricmp (s3, "ABOUT") == 0)
				{
					s4 = strtok (NULL, " ");
					if (s4 == NULL)
					{
						L084 (target, source, s2);
						return;
					}
					strlwr (s4);
					show_url (source, get_multiword_topic (s4), s2, 1, 0, userhost, 1);
				}
				else
				{
					strlwr (s3);
					show_url (source, get_multiword_topic (s3), s2, 1, 0, userhost, 1);
				}
			}
			else if (stricmp (s, "WHERE") == 0
					 || stricmp (s, "WHO") == 0 || stricmp (s, "WHAT") == 0)
			{
				s2 = strtok (NULL, " ");
				if (s2 == NULL)
				{
					L086 (target, source);
					return;
				}
				s3 = strtok (NULL, " ");
				if (s3 == NULL)
					return;
				strlwr (s3);
				ptr3 = strchr (s3, '?');
				if (ptr3 != NULL)
					memmove (ptr3, ptr3 + 1, strlen (ptr3 + 1) + 1);
				ptr3 = strchr (s3, '!');
				if (ptr3 != NULL)
					memmove (ptr3, ptr3 + 1, strlen (ptr3 + 1) + 1);
				if (stricmp (s3, "A") == 0 || stricmp (s3, "AN") == 0)
				{
					s4 = strtok (NULL, " ");
					if (s4 == NULL)
					{
						L087 (target, s, s2, s3, *CMDCHAR);
						return;
					}
					show_url (source, get_multiword_topic (s4), target, 1, 0, userhost, 0);
				}
				else
					show_url (source, get_multiword_topic (s3), target, 1, 0, userhost, 0);
			}
			else
				show_url (source, get_multiword_topic (s), target, 1, 0, userhost, 0);
		}
#if RANDOM_WHUT == 1
		else
		{
			do_randomtopic (WHUTR, target, WHUT_FILE, source, s);
		}
#else
		else
			S ("PRIVMSG %s :%s\n", target, WHUT);
#endif

#if GENERAL_QUESTIONS == 1
	}
	else
	{
		if (Sleep_Toggle == 1)
			return;
		show_url (source, get_multiword_topic (cmd), target, 0, 1, userhost, 0);
#endif
		i = 0;
	}
}
