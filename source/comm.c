#include "defines.h"
#include "vars.h"
#include "prototypes.h"

/**
 * Delete one or more elements from the sendq
 * 1 = delete all pri/not's
 * 0 = delete first in queue
 * 6/23/00 Dan
 * - Updated to use head and tail pointer queue
 * - All variables now initialized when declared
 * - Optimized the main while loop a bit, reduced amount of code
 */
void
del_sendq (long toggle)
{
	struct sendq *pNode = sendqhead, *pPrev = 0;

	if (NULL == sendqhead)
	{
		return;
	}

	if (toggle == 0)
	{
		/* Just delete the head */
		pNode = sendqhead;
		sendqhead = sendqhead->next;

		free (pNode);
	}
	else
	{
		/* Iterate through the queue and delete each element which is
		 * a PRIVMSG or NOTICE
		 */
		for (; pNode != NULL; pPrev = pNode, pNode = pNode->next)
		{
			if (0 == strncmp (pNode->data, "PRI", 3) || 0 == strncmp (pNode->data, "NOT", 3))
			{
				/* Found one, let's delete it */
				if (pPrev != NULL)
				{
					pPrev->next = pNode->next;
				}
				else
				{
					sendqhead = pNode->next;
				}
				free (pNode);
				pNode = NULL;
				break;
			}
		}						/* for */
	}							/* else */

	/* Update the tail pointer if needed */
	if (NULL == sendqhead)
	{
		sendqtail = NULL;
	}
}

int
get_sendq_count (long toggle)
{
	struct sendq *c;
	long i = 0, x = 0;

	c = sendqhead;
	while (c != NULL)
	{
		i++;
		if (c->data[0] == 'P' && c->data[1] == 'R' && c->data[2] == 'I')
			x++;
		else if (c->data[0] == 'N' && c->data[1] == 'O' && c->data[2] == 'T')
			x++;
		c = c->next;
	}
	if (toggle == 1)
		clear_sendq (i, 1);
	if (toggle == 2)
		return i;

	if (i < OUTPUT1_COUNT)
		SEND_DELAY = OUTPUT1_DELAY;
	else if (i < OUTPUT2_COUNT)
		SEND_DELAY = OUTPUT2_DELAY;
	else
		SEND_DELAY = OUTPUT3_DELAY;
	if (x > OUTPUT_PURGE_COUNT)
		clear_sendq (x, 0);
	return i;
}

void
clear_sendq (long count, long toggle)
{
	long i = 0;

	i = count;
	while (i > 1)
	{
		i--;
		del_sendq (1);
	}
	send_tog = 1;
	if (toggle != 1)
		L090 (CHAN, count);
}

long
setinfo_lastcomm (char *rest)
{								/* Disallows joins to more than one channel (or the same chan)
								 * to artifically raise join counters */
	long c_uptime = 0;

	if (stricmp (rest, slc1) == 0)
		return 1;				/* don't reply if already asked in LASTCOMM_TIME sec */
	if (stricmp (rest, slc2) == 0)
		return 1;
	if (stricmp (rest, slc3) == 0)
		return 1;
	if (stricmp (rest, slc4) == 0)
		return 1;
	if (*slc1 == '0')
	{							/* init lastcomms */
		strncpy (slc1, rest, sizeof (slc1));
		slcn1 = time (NULL);
	}
	if (*slc2 == '0')
	{
		strncpy (slc2, rest, sizeof (slc2));
		slcn2 = time (NULL);
	}
	if (*slc3 == '0')
	{
		strncpy (slc3, rest, sizeof (slc3));
		slcn3 = time (NULL);
	}
	if (*slc4 == '0')
	{
		strncpy (slc4, rest, sizeof (slc4));
		slcn4 = time (NULL);
	}
	if ((c_uptime = time (NULL) - slcn1) > SLASTCOMM_TIME)
	{							/* reinit old data */
		slcn1 = 0;
		*slc1 = '0';
	}
	if ((c_uptime = time (NULL) - slcn2) > SLASTCOMM_TIME)
	{
		slcn2 = 0;
		*slc2 = '0';
	}
	if ((c_uptime = time (NULL) - slcn3) > SLASTCOMM_TIME)
	{
		slcn3 = 0;
		*slc3 = '0';
	}
	if ((c_uptime = time (NULL) - slcn4) > SLASTCOMM_TIME)
	{
		slcn4 = 0;
		*slc4 = '0';
	}
	strncpy (slc4, slc3, sizeof (slc4));	/* no matches, move em on
											   down */
	strncpy (slc3, slc2, sizeof (slc3));
	strncpy (slc2, slc1, sizeof (slc2));
	strncpy (slc1, rest, sizeof (slc1));
	slcn4 = slcn3;
	slcn3 = slcn2;
	slcn2 = slcn1;
	slcn1 = time (NULL);
	return 0;
}

long
do_lastcomm (char *nick, char *target, char *rest)
{
	long c_uptime = 0;

	if (stricmp (rest, lc1) == 0)
		return 1;				/* don't reply if already asked in LASTCOMM_TIME sec */
	if (stricmp (rest, lc2) == 0)
		return 1;
	if (stricmp (rest, lc3) == 0)
		return 1;
	if (stricmp (rest, lc4) == 0)
		return 1;
	if (*lc1 == '0')
	{							/* init lastcomms */
		strncpy (lc1, rest, sizeof (lc1));
		lcn1 = time (NULL);
	}
	if (*lc2 == '0')
	{
		strncpy (lc2, rest, sizeof (lc2));
		lcn2 = time (NULL);
	}
	if (*lc3 == '0')
	{
		strncpy (lc3, rest, sizeof (lc3));
		lcn3 = time (NULL);
	}
	if (*lc4 == '0')
	{
		strncpy (lc4, rest, sizeof (lc4));
		lcn4 = time (NULL);
	}
	if ((c_uptime = time (NULL) - lcn1) > LASTCOMM_TIME)
	{							/* reinit old data */
		lcn1 = 0;
		*lc1 = '0';
	}
	if ((c_uptime = time (NULL) - lcn2) > LASTCOMM_TIME)
	{
		lcn2 = 0;
		*lc2 = '0';
	}
	if ((c_uptime = time (NULL) - lcn3) > LASTCOMM_TIME)
	{
		lcn3 = 0;
		*lc3 = '0';
	}
	if ((c_uptime = time (NULL) - lcn4) > LASTCOMM_TIME)
	{
		lcn4 = 0;
		*lc4 = '0';
	}
	strncpy (lc4, lc3, sizeof (lc4));	/* no matches, move em on
										   down */
	strncpy (lc3, lc2, sizeof (lc3));
	strncpy (lc2, lc1, sizeof (lc2));
	strncpy (lc1, rest, sizeof (lc1));
	lcn4 = lcn3;
	lcn3 = lcn2;
	lcn2 = lcn1;
	lcn1 = time (NULL);
	return 0;
}

void
prepare_bot (void)
{
	int esc = 0;

	while (!esc)
	{
		gs26 ();
		printf (".: Connecting to %s:%ld\t\r", BS, BP);
		fflush (stdout);
		sleep (2);

		socketfd = create_connection (BS, VHOST, BP);

		switch (socketfd)
		{

			case ERR_TIMED_OUT:
				printf (".: Connection to %s:%ld timed out!\t\r", BS, BP);
				fflush (stdout);
				sleep (2);
				break;

			case ERR_CONN_REFUSED:
				printf (".: Connection to %s:%ld was refused!\t\r", BS, BP);
				fflush (stdout);
				sleep (2);
				break;

			case ERR_NOT_ADDR:
				printf ("Address not in range!\n");
				exit (1);

			case ERR_NO_REACH:
				printf ("Address not reachable!\n");
				exit (1);

			default:
				esc = 1;
				printf
					(".: Connected to %s:%ld! [%ld]\t\r", BS, BP, (long) getpid ());

				fflush (stdout);
				sleep (5);
				break;
		}
	}

	/* set the socket to BLOCK */
	if (fcntl (socketfd, F_SETFL, 0) < 0)
	{
		printf ("\n");
		perror ("fcntl");
		exit (EXIT_FAILURE);
	}
}

int
create_connection (char *server, char *virtualhost, long port)
{
	struct	sockaddr_in		name;
	struct	hostent			*hostname;
	struct	timeval			timeout;
	fd_set					set;
	int						sock = 0, vhost = 1, sockerr = 0,
							optlen = sizeof (sockerr);

	if ((!virtualhost) || (strlen (virtualhost) < 1))
		vhost = 0;

	if ((sock = socket (AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	memset (&name, 0, sizeof (struct sockaddr_in));

	setsockopt (sock, SOL_SOCKET, SO_LINGER, 0, 0);
	setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, 0, 0);
	setsockopt (sock, SOL_SOCKET, SO_KEEPALIVE, 0, 0);

	name.sin_family = AF_INET;
	name.sin_addr.s_addr = (vhost ? inet_addr (virtualhost) : INADDR_ANY);

	if ((name.sin_addr.s_addr == -1) && vhost)
	{
		hostname = gethostbyname (virtualhost);
		if (hostname)
		{
			name.sin_addr = *(struct in_addr *) hostname->h_addr;
		}
		else
		{
			name.sin_addr.s_addr = INADDR_ANY;
		}
	}
	if (bind (sock, (struct sockaddr *) &name, sizeof (struct sockaddr_in)) < 0)
	{
		printf ("\n");
		perror ("bind");
		close (sock);
		exit (EXIT_FAILURE);
	}

	memset (&name, 0, sizeof (struct sockaddr_in));
	name.sin_family = AF_INET;
	name.sin_port = htons (port);

	if(!(hostname = gethostbyname (server)))
	{
		printf ("\n");
		herror ("hostname");
		exit (EXIT_FAILURE);
	}

	name.sin_addr = *(struct in_addr *) hostname->h_addr;

	/* set the file descriptor to non blocking mode so that connect()
	   returns immediately.
	 */

	if (fcntl (sock, F_SETFL, O_NONBLOCK) < 0)
	{
		printf ("\n");
		perror ("fcntl");
		exit (EXIT_FAILURE);
	}
	
	if (connect (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
	{
		if ((errno != EINPROGRESS) && (errno != ENOENT))
		{
			printf ("\n");
			perror ("connect");
			exit (EXIT_FAILURE);
		}
	}
	
	alarm (0);

	while (1)
	{
		/* set the timeout */
		timeout.tv_sec = CONNECT_WAIT_TIMEOUT;
		timeout.tv_usec = 0;

		/* set the file descriptor to be empty */
		FD_ZERO (&set);

		/* add our socket to the file descriptor set */
		FD_SET (sock, &set);

		/* select will let us know when our socket is ready (connected) */
		switch (select (FD_SETSIZE, (fd_set *) 0, &set, (fd_set *) 0, &timeout))
		{
				/* if select returns 0, our timeout was reached */
			case 0:
				alarm (AIL);
				return ERR_TIMED_OUT;

				/* -1 means we are not connected */
			case -1:
				break;

				/* we MIGHT be connected */
			default:

				/* get the socket errno so we can check if we are connected */
				switch (getsockopt (sock, SOL_SOCKET, SO_ERROR, &sockerr, &optlen))
				{
					case -1:
						alarm (AIL);
						return ERR_SOCK_OPT;

					case 0:
						switch (sockerr)
						{
							case ECONNREFUSED:
								alarm (AIL);
								return ERR_CONN_REFUSED;

							case EADDRNOTAVAIL:
								alarm (AIL);
								return ERR_NOT_ADDR;

							case ENETUNREACH:
								alarm (AIL);
								return ERR_NO_REACH;

							case SUCCESS:
								alarm (AIL);
								return sock;
						}
				}
		}
	}
}


void
register_bot (void)
{
	get_sendq_count (1);
	Snow ("NICK %s\n", Mynick);
	strlwr (UID);
	Snow ("USER %s %d %d :%s \2%d\2\n", UID, time (NULL), time (NULL), REALNAME, NUM_HELPER);
	
	if (BPASS != NULL)
		Snow ("PASS :%s\n", BPASS);
}
