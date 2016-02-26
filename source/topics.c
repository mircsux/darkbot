#include "defines.h"
#include "vars.h"
#include "prototypes.h"

/**
 * Remove the autotopic for a particular channel.
 * 6/23/00 Dan:
 * - Method argument is now pointer to const data
 * - All variables are now initialized when declared
 * - Changed size of b to be power of 2
 * - Changed variable types of toggle and x in accordance
 *   with their use
 */
void
del_autotopic (const char *chan)
{
	FILE *fp;
	char b[STRING_LONG] = { 0 }, *r_chan = 0, *r_data = 0;
	bool toggle = false;
	size_t x = 0;

	remove (TMP_FILE);
	fp = fopen (AUTOTOPIC_F, "r");
	if (NULL == fp)
	{
		return;
	}

	while (fgets (b, STRING_LONG, fp))
	{
		x++;
		stripline (b);

		r_chan = strtok (b, " ");
		r_data = strtok (NULL, "");

		if (stricmp (r_chan, chan) == 0)
		{
			/* Found the channel */
			toggle = true;
		}
		else
		{
			db_log (TMP_FILE, "%s %s\n", r_chan, r_data);
		}
	}

	fclose (fp);
	if (x == 1 && toggle)
	{
		/* The autotopic file is now empty */
		remove (AUTOTOPIC_F);
		remove (TMP_FILE);
		return;
	}

	if (toggle)
	{
		/* We found the topic, change the temp file to the
		 * the name of the autotopic file */
		rename (TMP_FILE, AUTOTOPIC_F);
	}
	else
	{
		/* We were unable to find the channel, just
		 * return */
		remove (TMP_FILE);
	}
}

void
do_autotopics (void)
{
	FILE *fp;
	char b[STRING_LONG] = { 0 }, *r_chan = NULL, *r_data = NULL;

	if ((fp = fopen (AUTOTOPIC_F, "r")) == NULL)
		return;
	while (fgets (b, STRING_LONG, fp))
	{
		stripline (b);
		r_chan = strtok (b, " ");
		r_data = strtok (NULL, "");
		if (*r_data != '0')
		{
			db_sleep (1);
			S ("TOPIC %s :%s\n", r_chan, r_data);
		}
	}
	fclose (fp);
}

long
ifexist_autotopic (char *chan)
{
	FILE *fp;
	char b[STRING_LONG] = { 0 }, *r_chan = NULL;

	if ((fp = fopen (AUTOTOPIC_F, "r")) == NULL)
		return -1;
	while (fgets (b, STRING_LONG, fp))
	{
		stripline (b);
		if (*b == '/')
			continue;
		r_chan = strtok (b, " ");
		if (stricmp (r_chan, chan) == 0)
		{
			fclose (fp);
			return 1;			/* exists */
		}
	}
	fclose (fp);
	return 0;					/* doesn't exist */
}

void
set_autotopic (char *source, char *target, char *topic)
{
	long exist = 0;

	exist = ifexist_autotopic (target);
	if (exist == 0 && *topic == '0')
	{							/* never existed, lets humor
								   * the guy */
		L007 (source, target);
		return;
	}
	else if (exist == 1 && *topic == '0')
	{							/* delete it! */
		L008 (source, target);
		S ("TOPIC %s :\n", target);
		del_autotopic (target);
		return;
	}
	if (strlen (topic) >= 400)	/* make sure no overflow */
		topic[400] = '\0';
	if (exist == 0)
	{							/* no such autotopic, so add it */
		L009 (source, target, topic);
		db_log (AUTOTOPIC_F, "%s %s\n", target, topic);
		return;
	}
	/* only thing left is if topic exists and you want to update it */
	del_autotopic (target);
	L010 (source, target, topic);
	S ("TOPIC %s :%s\n", target, topic);
	db_log (AUTOTOPIC_F, "%s %s\n", target, topic);
}

char *
revert_topic (char *input)
{
	char *ptr = NULL, b[STRING_SHORT] = { 0 };

	ptr = strtok (input, "+");
	
	snprintf (f_tmp, sizeof (f_tmp), "%s", ptr);

	if (ptr != NULL)
	{
		while (ptr != NULL)
		{
			ptr = strtok (NULL, "+");
			if (ptr != NULL)
			{
				snprintf (b, sizeof (b), "%s %s", f_tmp, ptr);
				snprintf (f_tmp, sizeof (f_tmp), "%s", b);
			}
		}
		return f_tmp;
	}
	else
		return f_tmp;
}
