dnl ---------------------------------
dnl configure.ac file for Darkbot.
dnl Written by ron <ron@darkbot.org>
dnl ---------------------------------

dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([Darkbot],[8rc2],[ron@darkbot.org])
AC_COPYRIGHT([Copyright (C) 2004 Darkbot Development Team])
AC_REVISION($Revision: 1.9 $)
AC_CONFIG_SRCDIR(source/defines.h)
AC_CONFIG_HEADER(source/config.h)
AC_PREFIX_DEFAULT([\${HOME}/darkbot])
AM_MAINTAINER_MODE

dnl config.h stuff
AH_TOP([#ifndef _DARKBOT_CONFIG_H
#define _DARKBOT_CONFIG_H])
AH_BOTTOM([#endif /* !_DARKBOT_CONFIG_H */])

AM_INIT_AUTOMAKE(darkbot, `cat VERSION`)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PATH_PROG(RM,rm)
AC_PATH_PROG(CP,cp)

dnl Checks for libraries.
dnl Replace `main' with a function in -lcrypt:
AC_CHECK_LIB(crypt, main)
dnl Replace `main' with a function in -luser32:
AC_CHECK_LIB(user32, main)

AC_CHECK_TYPES(clock_t size_t)

dnl Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC

AC_CHECK_HEADERS(fcntl.h sys/time.h unistd.h)
AC_CHECK_HEADER([windows.h],AC_DEFINE([WIN32],,[windows.h]))

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

AC_HEADER_TIME

dnl Checks for library functions.
AC_TYPE_SIGNAL
AC_FUNC_VPRINTF
AC_CHECK_FUNCS(select socket strspn strstr strtol snprintf)

AC_SUBST(CFLAGS)
AC_SUBST(PACKAGE)
AC_SUBST(VERSION)

AC_CONFIG_FILES([source/Makefile Makefile])
AC_OUTPUT

dnl Tell the user to run the Setup utility

echo
echo "You may now run ./Setup for configuration."

