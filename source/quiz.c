#include "defines.h"
#include "vars.h"
#include "prototypes.h"

void
run_quiz_question (char *target)
{
	FILE *fp;
	int i = 0;
	int j = 0;
	int k = 0;
	int size = 0;
	static int pos = 0;
	char b[STRING_LONG] = { 0 };
	char *ptr = NULL;

	if (quiz_timer > 0)
		return;					/* timer already running */

	srand (time (0));

	if ((fp = fopen (QUIZ_FILE, "r")) == NULL)
		return;

	while (fgets (b, sizeof (b), fp))
	{
		i++;					/* count questions */
	}

	fseek (fp, 0L, SEEK_SET);
	j = rand () % i;
	size = sizeof (recent_questions) / sizeof (recent_questions[0]);

	if (i < size)
		size = i / 2;

	while (k <= size)
	{
		srand (time (NULL));

		if (j == recent_questions[k])
		{
			j = rand () % i;
			k = 0;
			continue;
		}
		k++;
	}

	if (pos == size)
		pos = 0;

	recent_questions[pos] = j;
	quiz_line = j;
	pos++;
	i = 0;

	while (fgets (b, sizeof (b), fp))
	{
		if (i == quiz_line)
			break;
		i++;
	}

	if ((ptr = strtok (b, "@")) == NULL)
	{
		fclose (fp);
		return;
	}

	S ("PRIVMSG %s :%s\n", target, ptr);
	strncpy (quiz_target, target, sizeof (quiz_target));
	quiz_answer = 1;			/* switch on timer */

	fclose (fp);
}

void
run_quiz_answer (void)
{
	FILE *fp;
	int i = 0;
	char b[STRING_LONG] = { 0 };
	char *ptr = NULL;

	if ((fp = fopen (QUIZ_FILE, "r")) == NULL)
		return;

	while (fgets (b, sizeof (b), fp))
	{
		if (i == quiz_line)
			break;
		i++;
	}

	if ((ptr = strtok (b, "@")) == NULL)
	{
		fclose (fp);
		return;
	}
	if ((ptr = strtok (NULL, "")) == NULL)
	{
		fclose (fp);
		return;
	}

	S ("PRIVMSG %s :%s\n", quiz_target, ptr);

	fclose (fp);
}
