#include "defines.h"

#define STRING_SHORT    512
#define STRING_LONG     2048

int wsock = 0;
int sockerr = 0;
int optlen = sizeof(sockerr);
size_t nRandStuffs = 0;
long QUESTIONS = 0;
long ADDITIONS = 0;
long DELETIONS = 0;
long uptime = 0;
long NO_FLOOD = 0;
long NUM_SERV = 0;
long L_CLIENTS = 0;
long IRCOPS = 0;
long xtried = 0;
long G_USERS = 0;
long rt = 120;
long fc = 0;
long spr = 0;
long snr = 0;
long BP = 6667;
long CHECKED = 1;
long SEND_DELAY = 1;
long send_tog = 0;
long NUM_HELPER = 0;
long NUMLINESSEEN = 0;
long NUM_USER = 0;
long quiz_timer = 0;
long quiz_line = 0;
bool quiz_answer = 0;
bool quiz_halt = 0;
char quiz_target[STRING_SHORT] = { 0 };
long recent_questions[10] = { 0 };

#ifdef	RANDOM_STUFF
long Rand_Stuff = 0;
long Rand_Idle = 0;
#endif
long AIL4 = 0;
long Sleep_Toggle = 0;
long Sleep_Time = 0;
long AIL3 = 0;
long AIL2 = 0;
long AIL5 = 0;
long JOINs = 0;
long PERMBAN_counter = 0;
long ram_load_time = 0;
long AIL9 = 0;
long AIL666 = 0;
long AIL8 = 0;
long AIL13 = 0;
long LastInput = 0;
long AIL10 = 0;
long MARK_CHANGE = 0;
long html_counter = 0;
char NICK_COMMA[32] = { 0 };
char COLON_NICK[33] = { 0 };
char pass_data[512] = { 0 };
char pass_pass[STRING_SHORT] = { 0 };
char rword[STRING_SHORT] = { 0 };
char lc1[STRING_SHORT] = "0";
char lc2[STRING_SHORT] = "0";
char lc4[STRING_SHORT] = "0";
char lc3[STRING_SHORT] = "0";
char BPASS[STRING_SHORT] = "0";
char mask_tmp[STRING_LONG] = "0";
long lcn1 = 0;
long lcn2 = 0;
long lcn4 = 0;
long lcn3 = 0;
long SeeN = 0;
long DebuG = 0;
char slc1[STRING_SHORT] = "0";
char slc2[STRING_SHORT] = "0";
char slc4[STRING_SHORT] = "0";
char slc3[STRING_SHORT] = "0";
long slcn1 = 0;
long slcn2 = 0;
long slcn4 = 0;
long slcn3 = 0;

#ifdef	WIN32
char *rp391 = "niW-1cr8 tobkraD";
#else
char *rp391 = "1cr8 tobkraD";
#endif
char BCOLON_NICK[STRING_SHORT] = { 0 };
char DARKBOT_BIN[STRING_SHORT] = { 0 };
char URL2[STRING_SHORT] = { 0 };
char DBTIMERS_PATH[STRING_SHORT] = { 0 };
char LOG_DIR[STRING_SHORT] = { 0 };
char RDB_DIR[STRING_SHORT] = { 0 };
char STATS_FILE[STRING_SHORT] = { 0 };
char SEEN_FILE[STRING_SHORT] = { 0 };
char BACKUP_DUP[STRING_SHORT] = { 0 };
char ADD_DELETES[STRING_SHORT] = { 0 };
char AUTOTOPIC_F[STRING_SHORT] = { 0 };
char HELPER_LIST[STRING_SHORT] = { 0 };
char QUIZ_FILE[STRING_SHORT] = { 0 };
char PERFORM[STRING_SHORT] = { 0 };
char DEOP[STRING_SHORT] = { 0 };
char RAND_SAY[STRING_SHORT] = { 0 };
char RAND_FILE[STRING_SHORT] = { 0 };
char RANDQ_TEMPFILE[STRING_SHORT] = { 0 };
char RAND_BACKUP_FILE[STRING_SHORT] = { 0 };
char SERVERS[STRING_SHORT] = { 0 };
char PERMBAN[STRING_SHORT] = { 0 };
char SETUP[STRING_SHORT] = { 0 };
char DAT_DIR[STRING_SHORT] = { 0 };
char r_reply[STRING_SHORT] = { 0 };
char data[STRING_SHORT] = { 0 };
char g_chan[STRING_SHORT] = { 0 };
char dbVersion[STRING_SHORT] = { 0 };
char strbuff[STRING_SHORT] = { 0 };
char f_tmp[STRING_LONG] = { 0 };
char UID[STRING_SHORT] = "database";
char BS[STRING_SHORT] = "irc.undernet.org";
char CMDCHAR[2] = "!";
char CHAN[STRING_SHORT] = "#darkbot";
char s_Mynick[STRING_SHORT] = "darkbot";
char g_host[STRING_SHORT] = { 0 };
char Mynick[STRING_SHORT] = "darkbot";
char sleep_chan[STRING_SHORT] = { 0 };
char VHOST[STRING_SHORT] = "0";
char REALNAME[STRING_SHORT] = "http://www.darkbot.org";
char privmsg_log[STRING_SHORT] = { 0 };

#define PBOT "ArchFiend"

/* ------------ Below are structs ------------------ */

struct rusage r_usage;

struct ignorelist
{
    char nick[STRING_SHORT];
    struct ignorelist *next;
}
 *ignorehead = NULL;

struct sendq
{
	char data[STRING_SHORT];
	struct sendq *next;
}
 *sendqhead = NULL, *sendqtail = NULL;

struct userlist
{								/* internal userlist */
	char chan[STRING_SHORT];
	char nick[STRING_SHORT];
	char uh[STRING_SHORT];
	long level;					/* auth */
	short global;				/* Global user? */
	long idle;
	struct userlist *next;
}
 *userhead = NULL;

struct helperlist
{
	char chan[STRING_SHORT];
	char uh[STRING_SHORT];
	char nick[STRING_SHORT];
	long level;
	size_t num_join;
	char greetz[STRING_SHORT];
	char pass[STRING_SHORT];
	struct helperlist *next;
}
 *helperhead = NULL;

/**
 * 6/23/00 Dan
 * - Changed permbanlist to have dynamically allocated
 *   userhost and reason fields.
 * - Changed type of counter to size_t, this should be an
 *   unsigned type.
 */
struct permbanlist
{
	char *uh;
	char *reason;
	size_t counter;

	struct permbanlist *next;
}
 *permbanhead = NULL;

struct old
{
	char host[200];
	long time;
	int count;
	int value;
	int kick;
}
ood[STRING_SHORT];

struct sl124
{
	char name[STRING_SHORT];
	long port;
	char pass[STRING_SHORT];
	struct sl124 *next;
}
 *sh124 = NULL;

struct statslist
{
	char nick[STRING_SHORT];
	char uh[STRING_SHORT];
	long total;
	long added_time;
	long last_time;

	struct statslist *next;
}
 *statshead = NULL;

struct webinfo
{
	char trigger[STRING_SHORT];
	char host[STRING_SHORT];
	int port;
	char url[STRING_SHORT];
}
 GOOGLE_webinfo = { "GOOGLE", "www.google.com", 80, "/search?q=" } ,
 METAR_webinfo = { "METAR", "weather.noaa.gov", 80, "/cgi-bin/mgetmetar.pl?cccc=" } ,
 TAF_webinfo = { "TAF", "weather.noaa.gov", 80, "/cgi-bin/mgettaf.pl?cccc=" } ;
