/* defines.h
 ***	Darkbot (c) 2002
 *	The Internet Relay Chat Talking Robot
 * 
 ***	Darkbot Creator  -   Jason Hamilton (play) Jason@superlink.net
 *	Project Administrator  -   LuizCB (Pincel) LuizCB@darkbot.info
 *      Recent Code Modifications -     Ron (`ron) ron@darkbot.org
 *
 *	Official Sites: http://www.darkbot.org 
 *	 		http://darkbot.sourceforge.net
 * 
 ***	For any questions, suggestions, malfunctions, etc
 *	Use our support forum at http://forum.darkbot.org
 *
 * 	*******************************---******************************* */
/*
 ***	NOTES - READ before you start:
 *** 	Lines beginning by # (hash) are peprocessor directives, they are not
 * 	comments. You should NOT remove them.
 *** 	#defines should only be changed to ON, OFF, #define, #undef
 * 	or it's value defined on the very right end.
 *** 	Areas between " " are messages Darkbot will send. Do not remove " ".
 *** 	Double slash characters on de beginning of a line or slaches followed
 *	or preceded by * on the beginning and end of lines envolving some
 *	portion of text are comment characters combinations with help
 *	information for the delelopers or users. They do not have any effect
 *	in the behavior of the program.
 *
 	,-----------------------------------------------------------------,
	|   Note that the defines bellow are the RECOMMENDED settings!    |
	| Modify it by changing it's values of the data. This file will   |
	| instruct the compiler to build the executable file. If you're   |
	|  not sure of what to do, leave the default settings alone :)    |
	`-----------------------------------------------------------------' */

/* ================ SYSTEM REQUIREMENTS ================ */

/* 
 * Change the Ansi C signal handling below if it's not in the standard place,
 * usually on include/. Leave it as is if you don't know what that is. Type 
 * 'man signal' on your unix box for more info.
 * (default = <signal.h>)
 */
#include <signal.h>

/*
 * Most BSD systems will not need this. If your bot will
 * connect but don't react at all try turning this ON.
 * (default = OFF)
 */

#define NEED_LIBC5 OFF

/*
 * SGI boxes (ie; IRIX system) need this
 * define to run. (default = OFF)
 */
#define	SGI OFF

/* ================ LANGUAGE ================ */
/*
 * Pick a number from the list below for the language you
 * want your darkbot to speak. Default is 1, English.
 * ------------------------------------------------------------------
 * 1 = ENGLISH	- play <jason@superlink.net>
 * 2 = FRENCH	- Inajira <inajira@videotron.ca>
 *		- EfX <michel.efx@globetrotter.net>
 *		- eCHaLoTTe <echalotte@cablevision.qc.ca>
 * 3 = SPANISH  - speed1 <speed@eduredes.edu.do>
 * 4 = DUTCH    - Asmodai <asmodai@wxs.nl>
 * 5 = LATIN	- Otaku <otaku@unixgeek.ml.org>
 * 6 = GREEK    - Chris_CY <chriscy@cylink.net>
 * 7 = EBONICS  - rapsux <bitter@ici.net>
 * 8 = PIG LATIN- Cloud <burtner@usa.net>
 * 9 = RUSSIAN	- KOI8 encoding
 * 10= RUSSIAN	- CP1251 encoding
 *              - Oleg Drokin <green@ccssu.crimea.ua>
 * 11= PORTUGUESE- Pincel <Pincel@darkbot.info>
 * 12= GERMAN	- C.Hoegl@gmx.net & marc@reymann.net
 * 13= ITALIAN  - daniele nicolucci <jollino@icemail.it>
 * 14= CHINESE  - James <jamespower@263.net>
 * 15= SWEDISH	- Ybznek <sunmo@seaside.se>
 * 16= NORWEGIAN- [SoHo] <soho@int19h.com>
 * 17= ROMANIAN - Radu <radu.negoescu@sante.univ-nantes.fr>
 * 18= ARABIC   - MrSiMo <mrsimo7@yahoo.fr>
 * 19= TAIWANISH- ?
 * ------------------------------------------------------------------
 * ? = Email juice@darkbot.org if you want to help add other langs!
 * ------------------------------------------------------------------
 */
#define	LANG 1

/* ================ STARTUP, CONNECTION, IRC ================ */

/* ==== ON START ==== */
/*
 * Your darkbot tell you (at startup) what the latest version of
 * darkbot is. If you DO NOT WANT THIS, then define this. We recommend
 * keeping this defined.
 */
#define DISALLOW_COUNT

/* ==== ON CONNECTION ==== */
/*
 * Print out extra details while starting up?
 */
#define	VERB

/*
 * Check if servers are not responding, is so
 * connect to next server in list. (recommended)
 * Some linux systems have problems with this.
 * You'll know if you're one of them if your bot
 * disconnects every 5 minutes.
 *
 * ALSO NOTE: If your bot seems to disconnect every 5 minutes, try
 * turning off the CHECK_STONED code.
 */
#define	CHECK_STONED ON

/*
 * This is the time that darkbot will wait for a connection
 * to the server, before trying the next server. You may need
 * to increase this if you are on a slow connection. 
 */
#define CONNECT_WAIT_TIMEOUT 10

/* ==== ON CONNECTED - IRC ==== */
/*
 * Send everything that's in PERFORM.INI
 * to the IRC server every 60 seconds?
 * Useful if the bot is banned, or X was
 * split at the time darkbot signed on.
 */
#define	PERFORM_TIMER ON

/*
 * What user modes do you want for darkbot?
 */
#define	DEFAULT_UMODE "+i-ds"

/* ==== ON IRC ==== */
/*
 * Reply to CTCP's? (PING/VERSION). 
 */
#define	CTCP ON

/*
 * Want your darkbot to always have less than
 * 10 min idle? This isn't usually needed since
 * darkbots tend to talk a lot. (default = undef)
 */
#undef	ANTI_IDLE

/* ==== ONJOIN ==== */
/*
 * When bot joins a channel, sometimes it's hard
 * to tell when it's "synced" and is no longer processing
 * stuff -- this tells the channel the bot has finished
 * syncing. For example, if you try to login while it's
 * still syncing, it may just ignore your login request
 * until it sees you in the channel. (default = OFF)
 */
#define DISPLAY_SYNC OFF

/* ================ DATABASE MANAGEMENT ================ */

/* ==== ON START ==== */
/*
 * Sort your info2.db on startup? Will take
 * forever if u have a large db. If you have dividers or
 * any sort of markers on your database and you want to
 * preserve their position leave this directive undefined.
 * Sorting the database will put all items listed in
 * alphabetic order. (default = undef)
 */
#undef	SORT

/*
 * When user's do the INFO command, and at
 * startup, do you want to find and remove
 * duplicates database entries? (default = undef)
 */
#undef	FIND_DUPS

/*
 * When duplicate topics are found, do you
 * want to save them? (in case some topics
 * are accidently deleted) (default = define)
 */
#define	SAVE_DUPS

/* ==== PERMISSIONS ==== */
/*
 * Do you want Darkbot to answer questions that match a topic
 * without addressing the bot? (default = ON)
 */
#define	GENERAL_QUESTIONS ON

/*
 * Turn this ON if you want to be able to use the ADD function
 * in private message to the bot. (default = OFF)
 */
#define ALLOW_ADD_IN_MSG        OFF
   
/*
 * Turn this ON if you want to be able to use the DEL function
 * in private message to the bot. (default = OFF)
 */
#define ALLOW_DEL_IN_MSG        OFF

/*
 * If you want your bot to be able to respond to things asked
 * in private messages, turn this option ON. Use the above two
 * defines to allow/disallow adding or deleting entries in private.
 * (default = OFF)
 */
#define MSG_RESPONSES OFF

/*
 * Toggles the use of encrypted passwords in the userlist.db file.
 * Turn this off if you don't want your passwords encrypted. Also
 * note that if you're using encrypted passwords and you decide to 
 * turn this option off, you'll need to redo your userlist.db file.
 * Note: If you're new to Darkbot, you may want to turn this option OFF.
 */
#define ENCRYPT_PASSWORDS OFF
 
/*
 * Minimal access level required to add
 * topics? (default = define)
 */
#define	REQ_ACCESS_ADD

/*
 * Minimal access level required to to delete
 * topics? (default = define)
 */
#define	REQ_ACCESS_DEL

/* ==== LOGS ==== */
/*
 * Do you want to log who adds/deletes topics? Log saved 
 * to dat/logs/add_delete.log (default = define)
 */
#define LOG_ADD_DELETES

/*
 * Do you want to log all privmsg's to your
 * darkbot? (will be saved to dat/logs/ dir)
 * (default = ON)
 */
#define LOG_PRIVMSG ON

/* ==== SLEEP DIRECTIVES ==== */
/*
 * Level at which user's can make darkbot shut up
 * (aka hush). This is useful for help channels when
 * they want darkbot to quit talking while they address
 * something important in the chan (default = 3)
 */
#define	SLEEP_LEVEL	3

/*
 * How many seconds to sleep for? (default = 300 (5 minutes)) 
 */
#define	SLEEP_TIME	300

/*
 * What you would want your Darkbot to say when sleep and
 * wake up commands are issued? (If you want you can change
 * what is between 'ACTION' and '\1')
 */
#define	GOSLEEP_ACTION	"\1ACTION falls asleep... ZzzZZzzZZzz\1"
#define	WAKEUP_ACTION	"\1ACTION wakes up from a snooze..\1"

/* ==== DATA SIZE ==== */
/*
 * The max length your database replies will be. This will be useful
 * to people loading their database into ram - if your data is all
 * less than 400 chars, then you can save some ram.... Longer replies
 * allows you to do more, for example, with bitwise operator PIPE
 * (default = 400)
 */
#define	MAX_DATA_SIZE	400

/*
 * The max length your database topics will be. (default = 50)
 */
#define	MAX_TOPIC_SIZE	50

/* ==== OUTPUTS - TIMERS ==== */
/*
 * LASTCOMM_TIME is the length of time (in seconds)
 * that your bot will not reply to a topic already
 * asked. Thus if someone asked your bot about "mirc"
 * that question could not be asked again in the
 * same format for N seconds (or till the question
 * is out of buffer). This prevents the bot from
 * falling prey to users who like to repeat.
 * (default = 5)
 */
#define LASTCOMM_TIME 5

/*
 * BELOW is the output timers. Darkbot does not
 * output text without first putting it into a
 * que list. If the bot has several lines of text
 * waiting to be sent, it starts to delay longer
 * and longer between output, so it can't flood
 * itself off of IRC. Explanation:
 *
 * If text in que is < OUTPUT1_COUNT, output it.
 * If text in que is > OUTPUT1_COUNT, delay
 * OUTPUT1_DELAY seconds. If que is > OUTPUT2_COUNT,
 * delay OUTPUT2_DELAY seconds. If number of text
 * in que is higher than OUTPUT_PURGE_COUNT, then
 * just delete all unneeded output (ie; any text
 * and notices, but leaving in stuff like kicks
 * and modes) The defaults below are recommended,
 * as the bot isn't going to flood off. If you are
 * having the bot delete output messages and you'd
 * like to increase the que, update the 
 * OUTPUT_PURGE_COUNT to a larger number. Just keep
 * in mind if someone floods your bot with a lot of
 * VERSION requests, the bot will sit there outputting
 * a lot of version replies instead of deleting them,
 * causing it to act as if it's just sitting there not
 * doing anything when you ask it something in your
 * channel. (defaults = 4) = 1) = 6) = 2)
 */
#define OUTPUT1_COUNT 4
#define OUTPUT1_DELAY 1
#define OUTPUT2_COUNT 6
#define OUTPUT2_DELAY 2

/*
 * If still < OUTPUT_PURGE_COUNT and > OUTPUT2_COUNT
 * then delay OUTPUT3_DELAY secs. (default = 3)
 */
#define OUTPUT3_DELAY 3

/*
 * When all else fails... if more than OUTPUT_PURGE_COUNT
 * delete them all! No use in making the bot output slowly
 * over a long period of time... imagine if you set this to
 * 50, and had an OUTPUT3_DELAY of 3 secs.. thats 50*3 secs
 * till the bot is ready to output any new data to you!
 * (default = 7)
 */
#define OUTPUT_PURGE_COUNT 7

/* ======== OUTPUT MESSAGES ======== */

/* ==== OUTPUT MESSAGES - ON TOPICS ==== */
/*
 * Message to give when adding an existent topic
 */ 
#define	EXISTING_ENTRY	"Sorry, there is an existing entry under keyword"

/*
 * Message to give when there is no mentioned topic
 */ 
#define	NO_ENTRY	"I was unable to find entry:"

/* ==== OUTPUT MESSAGES - ON SEARCH ==== */
/*
 * These are the messages Darkbot will give on unaccomplished
 * search queries. Change the areas between " " with what
 * you want.
 */
#define	CANT_FIND	"Was unable to find"	/* ... */
#define	NO_TOPIC	"Sorry, I don't have any entry for"	/* ... */
#define	TRY_FIND	"What am I trying to find"


/* ==== OUTPUT MESSAGES - ON NONEXISTENT TEXT ==== */
/*
 * Answer to give in case Darkbot's nick is mentioned
 * without any further text. Do not UNDEF. If you want
 * change only what is between " ". Check next directive.
 */
#define	WHUT	"hmmm?"

/*
 * Gives a random response from dat/whut.rdb instead what
 * is specified on 'define WHUT'. (default = ON)
 */
#define RANDOM_WHUT	ON

/*
 * Answer to give in case there is no reply on Darkbot's database
 * Do not UNDEF. If you want change only what is between " ". Check
 * next directive.
 */ 
#define	DONNO_Q	"*shrug*"

/*
 * For those wanting the DONNO_Q response to be random, turn this
 * option ON, and darkbot will ignore the default definition and
 * use a random line from dat/dunno.db
 */
#define RANDOM_DUNNO	ON

/* ======== RANDOM STUFF ========= */
/*
 * Will read from a random line in dat/randomstuff.ini and say
 * something random in the home channel. ALSO if nothing is said
 * in darkbot's home channel it will say a randomstuff every 
 * RAND_IDLE as long as no one says anything. (default = define)
 * UNDEFining this directive will not allow you to add RDBs or
 * randomstuff entries. If you really want your bot not to say
 * anything from randomstuff.ini and keed adding random replies,
 * increase the time _TIME and _IDLE defines to, for example,
 * 54750, which corresponds to 365 days :)
 */
#define	RANDOM_STUFF

/*
 * Time in seconds to randomly say something
 * from randomstuff.ini (default = 3600 (1 hour))
 */
#define	RAND_STUFF_TIME	3600

/*
 * Time in secs to say something in home chan when no
 * one says anything. This overrides the RAND_STUFF_TIME
 * counter. (default = 1800 (30 minutes))
 */
#define RAND_IDLE	1800

/*
 * Level at which user's can on-line add new
 * randomstuff topics, this also is the level at which
 * users will be able to add RDB topics (default = 2)
 */
#define	RAND_LEVEL 2

/*
 * This is a new feature which enables the RANDQ
 * channel command, which will output a random
 * quote from your randomstuff.ini containing the
 * specified search string. ( default = ON )
 */
#define RANDQ ON

/*
 * This define backs up your randomstuff file every time
 * you add a random quote, in case you would like to do so.
 * ( default = #undef )
 */
#undef BACKUP_RANDOMSTUFF


/* ================ IRC - CHANNEL RELATED ================ */

/* ==== REGISTERED USERS ON JOIN ACTIONS ==== */
/*
 * Have the bot do auto-greet (setinfo) to registered users?
 * (default = ON)
 */
#define	JOIN_GREET ON

/*
 * This is the length of time to NOT allow someone to be recounted
 * when they rejoin your channels, which tends to clutter up
 * everyone's screen with the setinfo. This basically keeps track
 * of who joined in the last ___ seconds, and does not do their
 * setinfo during that amount of time. (default = 60 (seconds))
 */
#define SLASTCOMM_TIME 60

/*
 * Have the bot VOICE users when they join? (default = OFF)
 */
#define VOICE_USERS_ON_JOIN OFF

/*
 * Have the bot OP users when they login? (default = OFF)
 */
#define OP_USERS_ON_LOGIN OFF

/* ==== USERS LOOKOUT ==== */
/*
 * Want to be alerted when a user who joins is in "questionable"
 * other channels? This option only works on networks which their
 * IRCD's allows NOTICE to be sent using /notice @#chan
 * (default = OFF)
 */
#define	DO_WHOIS OFF

/*
 * Max length in seconds to keep a lastseen
 * (default = 604800 (7 days))
 */
#define	MAX_LASTSEEN	604800

/*
 * If you change the above time (MAX_LASTSEEN), be sure to
 * update the SEEN_REPLY to the respective time length.
 * (Change for what you want between " ")
 */
#define	SEEN_REPLY	"in the last 7 days."

/* ==== COMPLAINT MESSAGES ==== */
/*
 * Message Darkbot will send to the channel where it was kicked
 * from. (Change for what you want between " ")
 */
#define	COMPLAIN_REASON	"grrr, kick me again and I'm going to..."

/*
 * will complain in the chan that the Darkbot is deoped in.
 * (default = OFF)
 */
#define BITCH_ABOUT_DEOP OFF

/*
 * What do you want your Darkbot to say when is not
 * oped? (Change for what you want between " ")
 */
#if BITCH_ABOUT_DEOP == 1
#define BITCH_DEOP_REASON "grr, someone op me!"
#endif

/* ==== CHAN OP DIRECTIVES ==== */
/*
 * Enable this if you want to make your darkbot bloated with
 * stuff like !KICK, !UP, !WACK, etc. Basically anything
 * dealing with channel modes and kicking commands. This
 * includes PERMBANS. On linux, without this, your darkbot
 * bin will shrink considerably. (default = ON)
 */
#define DO_CHANBOT_CRAP ON

/*
 * Autotopic time interval in seconds
 * (default = 1800 (30 minutes))
 */
#define	AUTOTOPIC_TIME	1800

/* ==== KICKS AND BANS ==== */

/*
 * What do you want your Darkbot to say when kicks someone?
 * Note when you apply a kick with reason this setting will
 * not overwrite it. (Change for what you want between " ")
 */
#define	DEFAULT_KICK	"Requested!"

/*
 * Kick people out when they are banned?
 */
#define KICK_ON_BAN OFF

/* ==== KICKS AND BANS - FLOODS ==== */
/*
 * Sometimes morons like to /notice flood channels.
 * This define will make the bot kick those people
 * when they do a channel notice. (default = ON)
 */
#define	KICK_ON_CHANNEL_NOTICE ON

/*
 * If you want to take it a step further, you can also
 * have the bot ban the user@host too. (default = OFF)
 */
#define BAN_ON_CHANNEL_NOTICE OFF

/*
 * Finally, we can ban by *@host. This requires the
 * above two to be turned on. (default = OFF)
 */
#define	BAN_BY_HOST OFF

/* ==== KICKS AND BANS - DARKBOT'S OWN FLOOD PROTECTION ==== */
/*
 * Define this if you want your Darkbot to KICK out the
 * people who flood it. NOTE: This means flooding your BOT,
 * not your CHANNEL. If you don't define this, it will just
 * ignore the user. (default = define)
 */
#define	FLOOD_KICK

/*
 * What do you want your Darkbot to say when someone is
 * flooding it? (Change for what you want between " ")
 */
#define	FLOOD_REASON	"Don't flood!"

/* ================ MISCELLANEOUS ================ */

/*
 * Google search feature (default = ON)
 */
#define GOOGLE ON

/*
 * METAR search feature (default = ON)
 */
#define METAR ON

/*
 * TAF search feature (default = ON)
 */
#define TAF ON

/*
 * Bot's online QUIZ (default = OFF)
 */
#define QUIZ OFF

/*
 * How long to wait before giving the answer? (default = 25)
 */
#define QUIZ_TIMER 25

/*
 * Don't allow another quiz until this time has expired (since
 * the last answer) (default = 20)
 */
#define QUIZ_REPEAT_TIMER 20

/*
 * Do you want your bot to do math commands? (default = define)
 */
#define	DO_MATH_STUFF

/*
 * Parse luser data? May cause SIG_SEGV on
 * some ircd's (ie; non ircu) Basically all this does
 * is display network info like opers, servers, avg # 
 * of users on the servers, etc.
 */
#define STATUS OFF

/* ================ AID DIRECTIVES ================ */

 /* 
 * Give user's who join a NOTICE with info on how to use the darkbot?
 * I don't bother with this anymore since my bot knows how to respond
 * to most general questions users ask. If your bot is new and doesn't
 * know much, then you may want to have it tell people how to use it.
 */
#define	HELP_GREET OFF

/* 
 * This define just sends a message to the channel, whenever a "Guest"
 * user joins, asking the user if they need any help. This is useful 
 * for help channels.
 */

#define	AUTOHELP_GUESTS OFF

/*
 * If you change anything on the next 5 directives be careful not to
 * remove or change the position of '%s' nor anything outside quotes.
 */
#define	L100(s,a,b,c,d,e,f) S("NOTICE %s :I can be triggered by various \
forms of speech, all which must be addressed to me, in one of the \
following formats:  %s %s %s or even %s .... In my database, you can \
find a topic by saying my nick, <topic> .  eg; \37%s nuke\37  ..... \
to do a search on a word, or partial text, just type:  <mynick>, search \
<text>   ... eg; \37%s search nuke\37\n",s,a,b,c,d,e,f)

#define	L101(a,b,c,d) S("NOTICE %s :I can also be triggered with even more \
human formats: \37%s who is bill gates?\37  .. You can also phrase it \
in a question: \37%s where is msie?\37 ...For more info \
about me, visit http://www.darkbot.org\n",a,b,c,d)

#define	L102(a,b,c,d) S("NOTICE %s :Welcome to %s, %s. Before \
asking your question, type %cHELP for a list of help topics.\n", a,b,c,d)

#define mySetinfo "My !setinfo variables are: ^ nick, % Number of joins, & \
Channel, $ user@host. Example: !setinfo ^ has joined & % times!!  (also, if \
you make the first char of your SETINFO a \"+\" the setinfo will be shown \
as an ACTION)"

/*(62)
 * Below are variables for the data replys
 */
#define myVariables "data variables are: N~ (Nick), C~ (Chan), T~ \
(Time/date) B~ (Botnick), Q~ (Question asked), R~ (random nick), !~ \
(command char), S~ (current Server), P~ (current port) V~ (botVer), W~ \
(db WWW site), H~ (u@h), t~ (unixtime), BAN (sets a ban), TEMPBAN (bans \
for 60 sec)"


/* *******************************---******************************* */
/* --------------- Don't change anything else below! --------------- */
/* ***************************************************************** */
#define	fr		3	/* these two are the # of lines per secs */
#define	ft		3
#define	AIL		1
#define	WSEC 		10
#define	USEC 		0
#define	RECHECK 	45
#define DEFAULT_DBTIMERS_PATH	"timers"
#define	DEFAULT_LOG_DIR		"logs/"
#define	DEFAULT_RDB_DIR		"rdb"
#define DEFAULT_STATS_FILE	"stats.db"
#define	DEFAULT_SEEN_FILE	"seen.db"
#define	DEFAULT_URL2	"info2.db"
#define DUNNO_FILE  "dunno"
#define WHUT_FILE   "whut"
#define	DEFAULT_BACKUP_DUP	"backup_dups.db"
#define	DEFAULT_ADD_DELETES	"logs/add_delete.log"
#define	TMP_URL		".info.tmp"
#define	TMP_FILE	".file.tmp"
#define	DEFAULT_AUTOTOPIC_F	"autotopic.ini"
#define	DEFAULT_HELPER_LIST	"userlist.db"
#define	DEFAULT_QUIZ_FILE	"quiz.db"
#define	DEFAULT_PERFORM		"perform.ini"
#define	DEFAULT_DEOP		"deop.ini"
#define	DEFAULT_RAND_SAY	"random.ini"
#define	DEFAULT_RAND_FILE	"randomstuff.ini"
#define DEFAULT_RANDQ_TEMPFILE  "randq.tmp"
#define DEFAULT_RAND_BACKUP_FILE "randomstuff.bak"
#define	DEFAULT_SERVERS		"servers.ini"
#define	DEFAULT_PERMBAN		"permbans.db"
#define	DEFAULT_SETUP		"setup.ini"
#define	MAX_SEARCH_LENGTH	350
#define ERR_SOCK_OPT		-1
#define ERR_TIMED_OUT		-1
#define ERR_CONN_REFUSED	-2
#define ERR_NOT_ADDR		-3
#define ERR_NO_REACH		-4
#define ERR_CANT_CONNECT	-5
#define ERR_SERVER_BUSY		-6
#define ERR_NO_DATA			-7
#define SUCCESS				0

#define ON	1
#define OFF	0

#define LEGAL_TEXT "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-"
#define SAFE_LIST "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
#define LEGAL_NICK_TEXT "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-\\|[]{}`"

#define NUMBER_LIST "1234567890"
#define	MAX_NICK_LENGTH		35

#ifndef	LANG	/* If not defined, default to english */
#define	LANG 1
#endif

#define	NORMALR				0
#define WHUTR				1
#define DUNNOR				2

#define	RAND_NORMAL			0
#define	RAND_ACTION			1
#define	RAND_RAW			2


/* -------------------------------------------------- */

#ifdef 	HAVE_CONFIG_H
#include "config.h"
#endif
#include "langs/lang.h"
#include <ctype.h>
#include <stdio.h>
#ifndef     HAVE_CLOCK_T
#include <time.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>

#define db_sleep(x) usleep(x*1000)

#ifndef __cplusplus
typedef enum
{ false, true }
bool;
#endif

#define ERR_CANT_MALLOC     -1
#define ERR_OPEN_SOCKET     -2
#define ERR_WRITE_SOCKET    -3
#define ERR_READ_SOCKET     -5
#define ERR_NO_DOCUMENTS    -6

#define RANDQ_NORMAL            0
#define RANDQ_CASE              1
#define	RANDQ_RAND				2

#define	RDB_NORMAL				0
#define	RDB_ACTION				1
#define	RDB_RAW					2

/* ////////////////////////////////////////////////////////////////////// */

#define RESERVED1           "EXPLAIN"
#define RESERVED2           "DECLARE"
