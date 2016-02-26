#include "defines.h"
#include "vars.h"
#include "prototypes.h"
#include "stdio.h"
int
web_post_query(char *trigger, char *source, char *uh, char *target, char *query, int size)
{
    char *ptr = NULL;
    char *mem = NULL;
    struct webinfo *wi = NULL;
    
    if(stricmp (trigger, GOOGLE_webinfo.trigger) == 0)
    {
	wi = (struct webinfo *) &GOOGLE_webinfo;
    }
    else if(stricmp (trigger, METAR_webinfo.trigger) == 0)
    { 
	wi = (struct webinfo *) &METAR_webinfo;
    }
    else if(stricmp (trigger, TAF_webinfo.trigger) == 0)
    { 
	wi = (struct webinfo *) &TAF_webinfo;
    }
    else
    {
	return SUCCESS;
    }
    
    size++; // for null

    if((ptr = calloc(size, sizeof(char))) == NULL)
    {
        return ERR_CANT_MALLOC;
    }
    mem = ptr;

    if(web_open_socket(wi->host, wi->port) != SUCCESS)
    {
	free (mem);
        return ERR_OPEN_SOCKET;
    }
    snprintf(ptr, size + 1, "%s", query);

    while(*ptr != '\0')
    {
        if(*ptr == ' ')
        {
            *ptr = '+';
        }
        ptr++;
    }

    if(web_write_server(wsock, "GET %s%s HTTP/1.0\n\n", wi->url, mem) != SUCCESS)
    {
	free (mem);
        return ERR_WRITE_SOCKET;
    }

    /* i'm passing the trigger instead of the host to web_read_server 
       because the hostnames could be equal (see METAR & TAF) */
    if(web_read_server(source, uh, target, wsock, wi->trigger) != SUCCESS)
    {
	free (mem);
        return ERR_READ_SOCKET;
    }
    close(wsock);
    free (mem);

    return SUCCESS;
}

void
init_sockaddr (struct sockaddr_in *name, char *host, unsigned short int port)
{
    struct hostent *hostinfo;

    name->sin_family = AF_INET;
    name->sin_port = htons (port);
    hostinfo = gethostbyname (host);

    if (!hostinfo)
    {
	return;
    }
    name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}

int
web_open_socket(char *host, int port)
{
    extern void init_sockaddr (struct sockaddr_in *name, char *host, unsigned short int port);
	
    struct sockaddr_in name;
    struct timeval timeout;
    int result = 0;
    int esc = 0;
    fd_set set;

    if((wsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
	return ERR_CANT_CONNECT;
    }

    if((result = fcntl(wsock, F_SETFL, O_NONBLOCK)) < 0)
    {
        return ERR_CANT_CONNECT;
    }

    init_sockaddr (&name, host, port);
	
    if(connect(wsock, (struct sockaddr *) & name, sizeof(name)) != -1)
    {
        return ERR_CANT_CONNECT;
    }

    if(errno != EINPROGRESS)
    {
        return ERR_CANT_CONNECT;
    }

    while(!esc)
    {
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	FD_ZERO(&set);
	FD_SET(wsock, &set);

	switch(select(FD_SETSIZE, (fd_set *) 0, &set, (fd_set *) 0, &timeout))
        {
	    case 0:
		return ERR_TIMED_OUT;

	    case -1:
		break;

	    default:
		esc++;

		switch(getsockopt(wsock, SOL_SOCKET, SO_ERROR, &sockerr, &optlen)) 
		{
		    case -1:
			break;

		    case 0:
			switch(sockerr) 
			{
			    case ECONNREFUSED:
				break;
							
			    case EADDRNOTAVAIL:
				break;

			    case ENETUNREACH:
				break;

			    case SUCCESS: 
				if((result = fcntl(wsock, F_SETFL, 0)) < 0)
                                {
                                    return ERR_CANT_CONNECT;
   				}
				return SUCCESS;
			}
		}
	}
    }
    return SUCCESS;
}

int
web_write_server (int filedes, char *format,...)
{
    int	nbytes = 0;
    va_list arglist;
    char message[STRING_LONG] = {0};
    struct timeval timeout;
    fd_set set;

    va_start(arglist, format);
    vsprintf(message, format, arglist);
    va_end(arglist);

    while(1)
    {
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	FD_ZERO(&set);
	FD_SET(filedes, &set);

	switch (select(FD_SETSIZE, (fd_set *) 0, &set, (fd_set *) 0, &timeout))
        {
	    case 0:
		close(filedes);
		return ERR_SERVER_BUSY;

	    case -1:
		if (!alarmed)
                {
		    sleep(RECHECK);
		}
                else
                {
		    alarmed = 0;
                }
		break;
			
	    default: 
		if((nbytes = write(filedes, message, strlen(message) + 1)) < 0)
                {
		    return ERR_SERVER_BUSY;
                }
		else
                {
		    return SUCCESS;
                }
	}
    }
}

int
web_read_server(char *source, char *uh, char *target, int filedes, char *host)
{
    int nbytes = 0;
    int esc = 0;
    char packet[STRING_LONG] = { 0 };
    char *mem = NULL;
    char *ptr = NULL;
    struct timeval timeout;
    fd_set set;

    if((mem = calloc(sizeof(packet) + 1, sizeof(char))) == NULL)
    {
        return ERR_READ_SOCKET;
    }
    alarm(0);

    while(!esc)
    {
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	FD_ZERO(&set);
	FD_SET(filedes, &set);

	switch(select(FD_SETSIZE, &set, (fd_set *) 0, (fd_set *) 0, &timeout))
        {
	    case 0:
		close(filedes);
		alarm(AIL);
                free (mem);
		return ERR_SERVER_BUSY;

	    case -1:
		break;
			
	    default:
		alarm(AIL);
		esc = 1;
		break;
	}
    }

    while((nbytes = recv(filedes, packet, sizeof(packet), 0)) > 0)
    {
        strncat(mem, packet, sizeof(packet));

	if((ptr = realloc(mem, strlen(mem) + sizeof(packet) + 1)) == NULL)
        {
            free ( mem );
            return ERR_CANT_MALLOC;
	}
        else
        {
	    mem = ptr;
        }
	memset(packet, 0, sizeof(packet));
    }

    if(nbytes < 0)
    {
	free ( mem );
        return ERR_SERVER_BUSY;
    }
    close(filedes);

    
    if(stricmp (host, GOOGLE_webinfo.trigger) == 0)
    {
	google_parse_query(source, uh, target, mem);
    }
    else if(stricmp (host, METAR_webinfo.trigger) == 0)
    { 
	metar_parse_query(source, uh, target, mem);
    }
    else if(stricmp (host, TAF_webinfo.trigger) == 0)
    { 
	taf_parse_query(source, uh, target, mem);
    }

    free(mem);
    
    return SUCCESS;
}

int
google_parse_query(char *source, char *uh, char *target, char *data)
{
    char *s1 = NULL;
    char *s2 = NULL;
    char *s3 = NULL;
    char *s4 = NULL;
    char url[STRING_LONG] = { 0 };
    char sub1[] = "<p class=g><a href=";
 
    if(strstr(data, "did not match any documents") != NULL)
    {
        S("PRIVMSG %s :Sorry, your search did not match any documents.\n", target);
        return ERR_NO_DOCUMENTS;
    }

    if(((s1 = strstr(data, "Google Search:")) != NULL) || ((s1 = strstr(data, "Searched pages from")) != NULL))
    {
        if((s2 = strstr(s1, sub1)) != NULL)
        {
            s2 += strlen(sub1);

            if((s4 = strstr(s2, "http")) == NULL)
            {
                S("PRIVMSG %s :Try again later.\n", target);
                return ERR_NO_DOCUMENTS;
            }
            if((s3 = strchr(s4, '>')) != NULL)
            {
                *s3 = '\0';
            }
            if((s3 = strstr(s4, "&e=")) != NULL)
            {
                *s3 = '\0';
            }
            snprintf(url, sizeof(url), "%s", s4);
        }
        if(url[0] != 'h')
        {
            S("PRIVMSG %s :Try again later.\n", target);
            return ERR_NO_DOCUMENTS;
        }
        
        S("PRIVMSG %s :%s%s\n", target, rand_reply(source), url);
        add_stats (source, uh, 1, time (NULL), time (NULL));
    }
    else
    {
        S("PRIVMSG %s :Sorry, your search did not match any documents.\n", target);
        return ERR_NO_DOCUMENTS;
    }
    return SUCCESS;
}

int
metar_parse_query(char *source, char *uh, char *target, char *data)
{
    char *s1 = NULL;
    char *s2 = NULL;
    char metardata[STRING_LONG] = { 0 };
    int i = 0;
 
    if((s1 = strstr(data, "The observation is:")) != NULL)
    {
	/* skip the next 3 html tags */
	while(i<3)
	{
	    while(*s1 != '>')
	    {
		s1++;
	    }
	    s1++;
	    i++;
	}
	while((*s1 == 0x0D) || (*s1 == 0x0A))
	{
	    s1++;
	}
        if((s2 = strchr(s1, '<')) != NULL)
        {
            *s2 = '\0';
        }
	s2 = s1;
	while (*s2 != '\0')
	{
	    if ((*s2 == 0x0D) || (*s2 == 0x0A))
	    {
		*s2 = ' '; 
	    }
	    s2++;
	}
        snprintf(metardata, sizeof(metardata), "%s", s1);
        
        S("PRIVMSG %s :%s%s\n", target, rand_reply(source), metardata);
        add_stats (source, uh, 1, time (NULL), time (NULL));
    }
    else
    {
        S("PRIVMSG %s :Sorry, no METAR data available.\n", target);
        return ERR_NO_DOCUMENTS;
    }
    return SUCCESS;
}

int
taf_parse_query(char *source, char *uh, char *target, char *data)
{
    char *s1 = NULL;
    char *s2 = NULL;
    char tafdata[STRING_LONG] = { 0 };
    int i = 0;
 
    if((s1 = strstr(data, "The observation is:")) != NULL)
    {
	/* skip the next 4 html tags */
	while(i<4)
	{
	    while(*s1 != '>')
	    {
		s1++;
	    }
	    s1++;
	    i++;
	}
	while((*s1 == 0x0D) || (*s1 == 0x0A))
	{
	    s1++;
	}
        if((s2 = strchr(s1, '<')) != NULL)
        {
            *s2 = '\0';
        }
	s2 = s1;
	while (*s2 != '\0')
	{
	    if ((*s2 == 0x0D) || (*s2 == 0x0A) || (*s2 == 0x09))
	    {
		*s2 = ' '; 
	    }
	    s2++;
	}
        snprintf(tafdata, sizeof(tafdata), "%s", s1);
        
        S("PRIVMSG %s :%s%s\n", target, rand_reply(source), tafdata);
        add_stats (source, uh, 1, time (NULL), time (NULL));
    }
    else
    {
        S("PRIVMSG %s :Sorry, no TAF data available.\n", target);
        return ERR_NO_DOCUMENTS;
    }
    return SUCCESS;
}
