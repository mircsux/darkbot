## Makefile.am for Darkbot
## by ron <ron@darkbot.org>

bin_PROGRAMS = darkbot
EXTRA_PROGRAMS = convertdb

AM_CFLAGS = -DFORK

## Uncomment this line for  no-console mode on Windows.
## AM_LDFLAGS = -Wl,--subsystem,windows

## Uncomment this line to compile on Solaris.
## AM_LIBS = -lcrypt -lsocket -lnsl -lresolv

darkbot_SOURCES = chan.c general.c permbans.c reserved.c sockets.c \
                  users.c chansrv.c helpers.c quiz.c seen.c stats.c \
                  vars.c comm.c main.c random.c server.c topics.c web.c \
                  parse.c raw.c signals.c url.c

convertdb_SOURCES = convertdb.c
