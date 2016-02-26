#include "defines.h"
#include "vars.h"
#include "prototypes.h"

/**
 * Removed trailing newline and carriage returns.
 * 6/22/00 Dan
 * Rewrote to be more efficient, reduced from O(2n) to O(n)
 */
void
stripline (char *ptr)
{
	for (; ptr && *ptr; ++ptr)
	{
		if ('\r' == *ptr || '\n' == *ptr)
		{
			*ptr = 0;
			return;
		}
	}
}

void
db_log (const char *filename, const char *format,...)
{
	va_list arglist;
	FILE *fp;

	fp = fopen (filename, "a");
	if (NULL == fp)
	{
		/* I guess there's no sense in trying to log the error :) */
		return;
	}

	va_start (arglist, format);
	vfprintf (fp, format, arglist);
	va_end (arglist);
	fclose (fp);
}

/**
 * Convert a character array to all lowercase.
 * 6/23/00 Dan:
 * - Rewrote to be more compact and a bit more efficient
 */
char *
strlwr (char *buf)
{
	char *ptr = buf;

	for (; ptr && *ptr; ++ptr)
	{
		*ptr = tolower (*ptr);
	}
	return buf;
}

/* stristr: case insensitive strstr. */
char	*db_stristr		(char *pHaystack, char *pNeedle)
{
	char	szHaystack	[STRING_LONG] = {0};
	char	szNeedle	[STRING_LONG] =	{0};
	
	strcpy(szHaystack, pHaystack);
	strcpy(szNeedle, pNeedle);

	strlwr(szHaystack);
	strlwr(szNeedle);

	return(strstr(szHaystack, szNeedle));
}


/* 
 * Added cast to str[i -1] to prevent warnings on Solaris.
 * -ron
 */

void
trailing_blanks (char *str)
{
	int i = 0;

	if (str == NULL)
		return;
	for (i = strlen (str); i > 0; i--)
	{
		if (isspace ( (int) str[i - 1]))
			str[i - 1] = '\0';
		else
			return;
	}
}


#ifndef WIN32
int
stricmp (const char *s1, const char *s2)
{
	return strcasecmp (s1, s2);
}
#else
int
stricmp (const char *s1, const char *s2)
{
	register int c = 0;

	while ((c = tolower (*s1)) == tolower (*s2))
	{
		if (c == 0)
			return 0;
		s1++;
		s2++;
	}
	if (c < tolower (*s2))
		return -1;
	return 1;
}
#endif

void
save_changes (void)
{
	long i = 0;
	struct helperlist *c;
	struct statslist *d;

	c = helperhead;
	d = statshead;
	remove (TMP_FILE);
	while (c != NULL)
	{
		i++;
		db_log (TMP_FILE, "%s %s %d %d %s %s\n",
			 c->chan, c->uh, c->level, c->num_join, c->pass, c->greetz);
		c = c->next;
	}
	rename (TMP_FILE, HELPER_LIST);

	while (d != NULL)
	{
		i++;
		db_log (TMP_FILE, "%s %s %ld %ld %ld\n",
			 d->nick, d->uh, d->total, d->added_time, d->last_time);
		d = d->next;
	}
	rename (TMP_FILE, STATS_FILE);
}

void
save_setup (void)
{
#ifdef	WIN32
	printf ("*** Writing setup file: %s (%s)\n", SETUP, date ());
#endif
	remove (TMP_FILE);
	db_log (TMP_FILE, "NICK=%s\n", s_Mynick);
	db_log (TMP_FILE, "USERID=%s\n", UID);
	db_log (TMP_FILE, "CHAN=%s\n", CHAN);
	db_log (TMP_FILE, "VHOST=%s\n", VHOST);
	db_log (TMP_FILE, "REALNAME=%s\n", REALNAME);
	db_log (TMP_FILE, "CMDCHAR=%c\n", *CMDCHAR);
	db_log (TMP_FILE, "SEEN=%d\n", SeeN);
	rename (TMP_FILE, SETUP);
}

char *
date (void)
{
	time_t timer;

	time (&timer);
	strncpy (strbuff, ctime (&timer), sizeof (strbuff));
	stripline (strbuff);
	return strbuff;
}

/**
 * Allocate a new character array.  Copy into it at most
 * maxBytes bytes.
 */
char *
db_strndup (const char *dupMe, size_t maxBytes)
{
	char *ptr = NULL;
	char *retMe = NULL;

	/* Configure maxBytes to be the number of bytes to copy */
	maxBytes = min (strlen (dupMe), maxBytes);
	/* Allocate the return buffer. */
	retMe = malloc (maxBytes + 1);
	/* Was the allocation successful? */
	if (NULL == retMe)
	{
		return NULL;
	}

	/*
	 * ptr will point to the byte to which to copy the next
	 * source byte.
	 */
	ptr = retMe;
	/*
	 * Continue while dupMe is valid and we are < maxBytes number
	 * of bytes copied. This is typecase here because size_t is
	 * unsigned, so comparing against > 0 *should* produce a
	 * warning :)
	 */
	while (dupMe && (int) maxBytes-- > 0)
	{
		*ptr++ = *dupMe++;
	}

	/* Be sure to NULL terminate the array */
	*ptr = 0;
	return retMe;
}

int
match_wild (const char *pattern, const char *str)
{
	char c = 0;
	const char *s = NULL;

	for (;;)
	{
		switch (c = *pattern++)
		{
			case 0:
				if (!*str)
					return 1;
				return 0;
			case '?':
				++str;
				break;
			case '*':
				if (!*pattern)
					return 1;
				s = str;
				while (*s)
				{
					if (*s == *pattern && match_wild (pattern, s))
						return 1;
					++s;
				}
				break;
			default:
				if (*str++ != c)
					return 0;
				break;
		}						/* switch */
	}
}

#ifndef WIN32
size_t
min (const size_t a, const size_t b)
{
	return ((a < b) ? a : b);
}
#endif

char **
tokenize (char *theString, size_t * numTokens)
{
	static char *tokens[STRING_SHORT] = { 0 };

	assert (numTokens != NULL && theString != NULL);
	memset (tokens, 0, STRING_SHORT * sizeof (char *));

	tokens[(*numTokens = 0)] = strtok (theString, " ");
	if (NULL == tokens[0])
	{
		/* 0 tokens */
		return tokens;
	}

	while ((tokens[++(*numTokens)] = strtok (NULL, " ")) != NULL)
	{
		/* NO-OP */ ;
	}
	tokens[*numTokens] = 0;

	return tokens;
}

void
get_s (void)
{
	char temp[50] = { 0 };
	long i = 0;

	i = strlen (rp391);
	while (i > 0)
	{
		i--;
		snprintf (temp, sizeof (temp), "%s%c", dbVersion, rp391[i]);
		strncpy (dbVersion, temp, sizeof (dbVersion));
	}
}

#ifndef WIN32
const char *
run_program (const char *input)
{
	FILE *read_fp;

	read_fp = popen (input, "r");
	if (read_fp != NULL)
	{
		while ( fgets(f_tmp, sizeof(f_tmp), read_fp) );

        pclose (read_fp);
		if (f_tmp)
		{
			return f_tmp;
		}
		return "No match";
	}
	return NULL;
}
#endif

/**
 * 6/22/00 Dan
 * - Removed srand(), should only be done once, in main()
 * - Changed while to for loop
 */
char *
get_rand_nick (const char *chan)
{
	size_t x = 0;
	size_t i = 0;
	const struct userlist *c = userhead;

	/* Iterate through the userlist */
	for (; c != NULL; c = c->next)
	{
		/* Check if this user is on the channel */
		if (stricmp (chan, c->chan) == 0)
		{
			if (stricmp (Mynick, c->nick) != 0)
			{
				strncpy (f_tmp, c->nick, sizeof (f_tmp));
				i++;
			}
		}
	}

	x = rand () % i + 2;
	i = 0;						/* reinit! */

	for (c = userhead; c != NULL; c = c->next)
	{
		if (stricmp (chan, c->chan) == 0)
		{
			i++;
			if (i == x)
			{
				if (*c->nick == '0')
				{
					return f_tmp;
				}
				if (stricmp (Mynick, c->nick) != 0)
				{
					strncpy (f_tmp, c->nick, sizeof (f_tmp));
					return f_tmp;
				}
			}
		}
	}
	return f_tmp;
}

void
check_dbtimers (void)
{
	DIR *dp;
	long i = 0;
	char filename[STRING_SHORT] = { 0 };
	struct dirent *entry;
	struct stat statbuf;
	FILE *fp;
	char b[STRING_LONG] = { 0 }, output[STRING_LONG] =
	{
	0};

	if ((dp = opendir (DBTIMERS_PATH)) == NULL)
	{
		return;
	}
	while ((entry = readdir (dp)) != NULL)
	{
		stat (entry->d_name, &statbuf);
		if (S_ISDIR (statbuf.st_mode) && *entry->d_name == '.')
		{
			continue;			/* it's a dir, ignore it */
		}
		if (S_ISDIR (statbuf.st_mode) && stricmp(entry->d_name, "CVS") == 0)
		{
			/* Ignore the CVS directory */
			continue;
		}
		i = time (NULL);
		if (i >= atoi (entry->d_name))
		{
			snprintf (filename, sizeof (filename), "%s/%s", DBTIMERS_PATH, entry->d_name);
			if ((fp = fopen (filename, "r")) == NULL)
			{
				return;
			}
			while (fgets (b, STRING_LONG, fp))
			{
				stripline (b);
				snprintf (output, sizeof (output), "%s\n", b);
				S (output);
			}
			fclose (fp);
			remove (filename);
		}
	}
	closedir (dp);
}

#ifndef	HAVE_SNPRINTF
int
snprintf (char *buff, size_t size, const char *fmt, ...)
{
	static char temp[BUFSIZ];	/* a temp (large buffer) */
	int result;
	va_list ap;
	va_start (ap, fmt);
	result = vsprintf (temp, fmt, ap);
	strncpy (buff, temp, size - 1);	/* copy at most n */
 	buff[size - 1] = '\0';		/* ensure \0 at end */
	va_end (ap);
	return result;
}
#endif

int
add_ignore_user_ram (char *nick)
{
	struct ignorelist *n, *c;

	c = ignorehead;

    while (c)
	{
		if (stricmp (c->nick, nick) == 0)
		{
			return 1;
		}
		c = c->next;
	}
	
    if ( ( n = malloc (sizeof (struct ignorelist))) == NULL )
	{
		db_log ("error.log", "AHHH! No ram left! in add_ignore_user!\n");
		return 0;
	}
	
    memset (n, 0, sizeof (struct ignorelist));

    if (n != NULL)
	{
		strncpy (n->nick, nick, sizeof (n->nick));

		n->next = ignorehead;
		ignorehead = n;
	}
    return 1;
}

int
check_ignore_user_ram (char *nick)
{
	struct ignorelist *c;

	c = ignorehead;

    while (c)
	{
        if (stricmp (c->nick, nick) == 0)
		{
			return 1;
		}
		c = c->next;
	}
    return 0;
}

int
delete_ignore_user_ram (char *nick)
{
	struct ignorelist *pNode, *pPrev;

	pNode = ignorehead;
	pPrev = NULL;

	while (pNode)
	{
		if (stricmp (pNode->nick, nick) == 0)
		{
			if (pPrev != NULL)
			{
				pPrev->next = pNode->next;
			}
			else
				ignorehead = pNode->next;
			free (pNode);
			pNode = NULL;
			return 1;
		}
		pPrev = pNode;
		pNode = pNode->next;
	}
    return 0;
}

/* Count lines in a given filename. */
size_t			count_lines	(char *filename)
{
	FILE	*fp = NULL;
	
	size_t		lines = 0;
	char	b	[STRING_LONG] = {0};
	
	if((fp = fopen(filename, "r")) == NULL)
	{
		db_log("error.log", "Failed to open %s in count_lines()", 
			filename);
		return (-1);
	}

	while(fgets(b, STRING_LONG, fp))
	{
			/* Ignore comments! */
			if((*b != '/') && (*b != '\n'))
				lines++;
			
			if(*b == '\n')
				continue;
	}

	/* Close the file. */
	fclose(fp);

	return(lines);
}

/* Self explanatory. */
void    reverse  (char   *pString)
{
        size_t  nLength = strlen(pString);
        char    *endptr = pString + nLength - 1;

        for(;pString < endptr; ++pString, --endptr)
        {
                 char c = *pString;
                 *pString = *endptr;
                 *endptr = c;
        }
}
	

/* Count how many times the character nChar exists in szStuff
 * return that number. 
 */

size_t		count_char		(const char *pStuff, const char nChar)
{
	char	szStuff [STRING_LONG] = {0};
	int		nIndex = 0;
	int		nCount = 0;
	
	strcpy(szStuff, pStuff);

	for(nIndex = 0; nIndex <= (size_t) strlen(szStuff); nIndex++)
	{
		if(szStuff[nIndex] == nChar)
			nCount++;
	}

	return(nCount);
}
