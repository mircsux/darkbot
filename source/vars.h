#define STRING_SHORT    512
#define STRING_LONG     2048

extern int wsock;
extern int sockerr;
extern int optlen;
extern size_t nRandStuffs;
extern long QUESTIONS;
extern long ADDITIONS;
extern long DELETIONS;
extern long uptime;
extern long NO_FLOOD;
extern long NUM_SERV;
extern long L_CLIENTS;
extern long IRCOPS;
extern long xtried;
extern long G_USERS;
extern long rt;
extern long fc;
extern long spr;
extern long snr;
extern long BP;
extern long CHECKED;
extern long SEND_DELAY;
extern long send_tog;
extern long NUM_HELPER;
extern long NUM_USER;
extern long NUMLINESSEEN;
extern long Rand_Stuff;
extern long Rand_Idle;
extern long AIL4;
extern long Sleep_Toggle;
extern long Sleep_Time;
extern long AIL3;
extern long AIL2;
extern long AIL5;
extern long JOINs;
extern long PERMBAN_counter;
extern long	RAND_CHANS;
extern long ram_load_time;
extern long AIL9;
extern long AIL666;
extern long AIL8;
extern long AIL13;
extern long LastInput;
extern long AIL10;
extern long MARK_CHANGE;
extern long html_counter;
extern long lcn1;
extern long lcn2;
extern long lcn4;
extern long lcn3;
extern long SeeN;
extern long DebuG;
extern long quiz_timer;
extern long quiz_line;
extern bool quiz_answer;
extern bool quiz_halt;
extern long recent_questions[10];
extern char NICK_COMMA[32];
extern char COLON_NICK[33];
extern char BPASS[32];
extern char pass_data[512];
extern char pass_pass[STRING_SHORT];
extern char rword[STRING_SHORT];
extern char lc1[STRING_SHORT];
extern char lc2[STRING_SHORT];
extern char lc4[STRING_SHORT];
extern char lc3[STRING_SHORT];
extern char slc1[STRING_SHORT];
extern char slc2[STRING_SHORT];
extern char slc4[STRING_SHORT];
extern char slc3[STRING_SHORT];
extern char	mask_tmp[STRING_LONG];
extern char quiz_target[STRING_SHORT];
extern long slcn1;
extern long slcn2;
extern long slcn4;
extern long slcn3;
extern char *rp391;
extern char BCOLON_NICK[STRING_SHORT];
extern char DARKBOT_BIN[STRING_SHORT];
extern char URL2[STRING_SHORT];
extern char DBTIMERS_PATH[STRING_SHORT];
extern char LOG_DIR[STRING_SHORT];
extern char RDB_DIR[STRING_SHORT];
extern char STATS_FILE[STRING_SHORT];
extern char SEEN_FILE[STRING_SHORT];
extern char BACKUP_DUP[STRING_SHORT];
extern char ADD_DELETES[STRING_SHORT];
extern char AUTOTOPIC_F[STRING_SHORT];
extern char HELPER_LIST[STRING_SHORT];
extern char QUIZ_FILE[STRING_SHORT];
extern char PERFORM[STRING_SHORT];
extern char DEOP[STRING_SHORT];
extern char RAND_SAY[STRING_SHORT];
extern char RAND_FILE[STRING_SHORT];
extern char RANDQ_TEMPFILE[STRING_SHORT];
extern char RAND_BACKUP_FILE[STRING_SHORT];
extern char SERVERS[STRING_SHORT];
extern char PERMBAN[STRING_SHORT];
extern char SETUP[STRING_SHORT];
extern char DAT_DIR[STRING_SHORT];
extern char r_reply[STRING_SHORT];
extern char data[STRING_SHORT];
extern char g_chan[STRING_SHORT];
extern char dbVersion[STRING_SHORT];
extern char strbuff[STRING_SHORT];
extern char f_tmp[STRING_LONG];
extern char UID[STRING_SHORT];
extern char BS[STRING_SHORT];
extern char CMDCHAR[2];
extern char CHAN[STRING_SHORT];
extern char s_Mynick[STRING_SHORT];
extern char g_host[STRING_SHORT];
extern char Mynick[STRING_SHORT];
extern char sleep_chan[STRING_SHORT];
extern char VHOST[STRING_SHORT];
extern char REALNAME[STRING_SHORT];
extern char privmsg_log[STRING_SHORT];

extern struct rusage r_usage;

extern struct ignorelist
{
    char nick[STRING_SHORT];
    struct ignorelist *next;
}
 *ignorehead;

extern struct sendq
{
	char data[STRING_SHORT];
	struct sendq *next;
}
 *sendqhead, *sendqtail;

extern struct userlist
{								/* internal userlist */
	char chan[STRING_SHORT];
	char nick[STRING_SHORT];
	char uh[STRING_SHORT];
	long level;					/* auth */
	short global;				/* Global user? */
	long idle;
	struct userlist *next;
}
 *userhead;

extern struct helperlist
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
 *helperhead;

/**
 * 6/23/00 Dan
 * - Changed permbanlist to have dynamically allocated
 *   userhost and reason fields.
 * - Changed type of counter to size_t, this should be an
 *   unsigned type.
 */
extern struct permbanlist
{
	char *uh;
	char *reason;
	size_t counter;

	struct permbanlist *next;
}
 *permbanhead;

extern struct old
{
	char host[200];
	long time;
	int count;
	int value;
	int kick;
}
ood[STRING_SHORT];

extern struct sl124
{
	char name[STRING_SHORT];
	long port;
	char pass[STRING_SHORT];
	struct sl124 *next;
}
 *sh124;

extern struct statslist
{
	char nick[STRING_SHORT];
	char uh[STRING_SHORT];
	long total;
	long added_time;
	long last_time;

	struct statslist *next;
}
 *statshead;

extern struct	randstats
{
	char	chan	[STRING_SHORT];
	size_t	Rand_Stuff;
	size_t	Rand_Idle;
	size_t	refnum;
	size_t	nCount;		/* Number of times we outputted to this 
 				   channel. */

	struct	randstats	*next;
/*	struct	randstats	*prev; */
}	
 *randstatshead;

extern struct webinfo
{
	char trigger[STRING_SHORT];
	char host[STRING_SHORT];
	int port;
	char url[STRING_SHORT];
}
 GOOGLE_webinfo, METAR_webinfo, TAF_webinfo;
