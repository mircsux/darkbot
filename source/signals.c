#include "defines.h"
#include "vars.h"
#include "prototypes.h"

void
sig_alrm (int notUsed)
{
	alarmed = 1;
	alarm (AIL);
	check_dbtimers ();			/* timers :) */
	AIL8 += AIL;

	if (quiz_halt == 1)
	{
		AIL13++;
		if (AIL13 >= QUIZ_REPEAT_TIMER)
		{
			AIL13 = 0;
			quiz_halt = 0;
		}
	}

	if (quiz_answer == 1 && quiz_halt == 0)
	{
		if (quiz_timer >= QUIZ_TIMER)
		{
			quiz_answer = 0;
			quiz_timer = 0;
			quiz_halt = 1;
			run_quiz_answer ();
		}
		else
			quiz_timer++;
	}
	if (AIL8 >= SEND_DELAY)
	{
		AIL8 = 0;
		Send ();
	}
	LastInput += AIL;
	if (LastInput >= 500)
	{
		LastInput = 0;
#if CHECK_STONED == 1
		L088 (BS);
#ifdef	WIN32
		printf ("\nNo response from %s in 5 mins, reconnecting...\n", BS);
#endif
		prepare_bot ();
		register_bot ();
#endif
	}
	AIL10 += AIL;
	if (AIL10 >= 900)
	{							/* 15 mins */
		AIL10 = 0;
		if (MARK_CHANGE == 1)
		{
			MARK_CHANGE = 0;
			save_setup ();		/* save settings */
		}
	}
	AIL666 += AIL;
	if (AIL666 >= 60)
	{							/* 60 sec timer */
		AIL666 = 0;
		S ("PING :%s\n", BS);

	}
	AIL9 += AIL;
	if (AIL9 >= 30)
	{
		AIL9 = 0;
		if (stricmp (s_Mynick, Mynick) != 0)
		{
			S ("NICK %s\n", s_Mynick);
			strncpy (Mynick, s_Mynick, sizeof (Mynick));
			snprintf (NICK_COMMA, sizeof (NICK_COMMA), "%s,", Mynick);
			snprintf (COLON_NICK, sizeof (COLON_NICK), "%s:", Mynick);
			snprintf (BCOLON_NICK, sizeof (BCOLON_NICK), "%s\2:\2", Mynick);
		}
	}
	if (Sleep_Toggle == 1)
	{
		AIL4 += AIL;
		if (AIL4 >= Sleep_Time)
		{
			Sleep_Toggle = 0;
			AIL4 = 0;
			L089 (sleep_chan);
		}
	}
	AIL2 += AIL;
	AIL3 += AIL;
#ifdef	RANDOM_STUFF
	Rand_Idle++;
	if (RAND_IDLE <= Rand_Idle)
	{
		Rand_Idle = 0;
		do_random_stuff ();
		get_rand_stuff_time ();
	}
	Rand_Stuff -= AIL;
	if (Rand_Stuff <= 0)
	{
		if (Sleep_Toggle != 1)
			do_random_stuff ();
		get_rand_stuff_time ();
	}
#endif
	if (AIL3 >= AUTOTOPIC_TIME)
	{
		AIL3 = 0;
		do_autotopics ();
	}
	AIL5 += AIL;
	if (AIL5 >= 600)
	{
#ifdef	ANTI_IDLE
		S ("PRIVMSG ! :\2\n");
#endif
		AIL5 = 0;
	}
	if (AIL2 >= 300)
	{
		AIL2 = 0;
#if STATUS == 1
		S ("LUSERS\n");
#endif
		S ("MODE %s %s\n", Mynick, DEFAULT_UMODE);
		S ("JOIN %s\n", CHAN);
		reset_ ();
		save_changes ();

#if PERFORM_TIMER == 1
                run_perform ();
#endif

	}
}

void
sig_segv (int notUsed)
{
	long uptime2 = 0, p = 0;

	uptime2 = time (NULL) - uptime;
	printf
		("ERROR! Aborting program. (SIG_SEGV) Uptime: %d hour%s, %d min%s\n",
		 (int) (uptime2 / 3600),
		 (uptime2 / 3600 == 1) ? "" : "s",
		 (int) ((uptime2 / 60) % 60), ((uptime2 / 60) % 60) == 1 ? "" : "s");
	Snow ("QUIT :Caught SIG_SEGV! Aborting connection. Uptime: %d hour%s, %d min%s\n",
		  uptime2 / 3600,
		  uptime2 / 3600 == 1 ? "" : "s", (uptime2 / 60) % 60, (uptime2 / 60) % 60 == 1 ? "" : "s");
	db_sleep (2);
	p = getpid ();
	if (fork () > 0)
	{
		db_log ("error.log", "Caught SIGSEGV.. Sent kill -3 and kill -9...\n");
		kill (p, 3);
		kill (p, 9);
	}
	db_sleep (1);
	exit (0);
}

void
sig_hup (int notUsed)
{
	char temp[STRING_LONG];

	S ("QUIT :SIGHUP - Restarting %s ...\n", dbVersion);
	snprintf (temp, sizeof (temp), "sleep 2; %s", DARKBOT_BIN);
	system (temp);
	db_sleep (1);
	exit (0);
}
