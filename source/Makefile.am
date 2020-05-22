## Makefile.am for Darkbot
## by ron <ron@darkbot.org>

bin_PROGRAMS = darkbot
EXTRA_PROGRAMS = convertdb

AM_CFLAGS = -DFORK

## Uncomment this line for  no-console mode on Windows.
## AM_LDFLAGS = -Wl,--subsystem,windows

## Uncomment this line to compile on Solaris.
## AM_LIBS = -lcrypt -lsocket -lnsl -lresolv

darkbot_SOURCES = chan.c chansrv.c comm.c general.c helpers.c main.c \
                  parse.c permbans.c quiz.c random.c raw.c reserved.c \
                  seen.c server.c signals.c sockets.c stats.c topics.c \
                  url.c users.c vars.c web.c 
                  
convertdb_SOURCES = convertdb.c
