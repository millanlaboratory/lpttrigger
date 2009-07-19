# If the library source code has changed at all since the last update, then increment
#      revision.
# If any interfaces have been added, removed, or changed since the last update, increment
#      current, and set revision to 0.
# If any interfaces have been added since the last public release, then increment age.
# If any interfaces have been removed since the last public release, then set age to 0.

m4_define([lib_release],0)
m4_define([lib_current],0)
m4_define([lib_revision],0)
m4_define([lib_age],0)


AC_INIT(lpttrigger,[lib_release.lib_current.lib_age.lib_revision])
AC_CONFIG_SRCDIR([src/lpttrigger.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_LIBOBJ_DIR([lib])
AC_CONFIG_HEADERS([config/config.h])
AM_INIT_AUTOMAKE

AC_CANONICAL_HOST
AC_CANONICAL_BUILD


AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_PROG_CXX

LT_INIT([win32-dll])
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

AC_HEADER_STDC
AC_HEADER_TIME
AC_C_CONST
AC_C_VOLATILE



case $host in
	*-linux*)
		os_support=linux
		;;
	*win32* | *mingw32* | *cygwin* | *windows*)
		os_support=win32
		WINSYSDIR="/c/WINDOWS/system32"
		AM_LDFLAGS=-no-undefined
		;;
esac


AC_ARG_ENABLE(install-winio,
	AC_HELP_STRING([--disable-install-winio],[Do not install the winio library (available only on Windows)]),
	[case $enableval in
		yes) 
			installwinio=true
			;;
		no) 
			installwinio=false 
			;;
		*) AC_MSG_ERROR([bad value $enableval for --disable-install-winio]) ;;
	esac],
	[installwinio=true])

AC_ARG_VAR([WINSYSDIR],[Path to the system32 directory of Windows if applicable (default:/c/WINDOWS/system32)])
AM_CONDITIONAL([INSTALL_WINIO], [test "$installwinio" = "true" && test "$os_support" = "win32"])

AM_CONDITIONAL([LINUX_OS], [test "$os_support" = "linux"])
AM_CONDITIONAL([WIN32_OS], [test "$os_support" = "win32"])

AC_SEARCH_LIBS([pthread_create], [pthread posix4], [], AC_MSG_ERROR([The pthread library must be installed. Consider the installation of pthreads-win32 if on windows platform.]))
AC_SEARCH_LIBS(clock_gettime, [rt posix4])

AC_CHECK_DECLS([clock_gettime], [], [], [[#include <time.h>]])
AC_CHECK_TYPES([[struct timespec], [clockid_t]])
AC_REPLACE_FUNCS([clock_gettime])
AC_CHECK_FUNCS([gettimeofday ftime _ftime select])

AC_SUBST([RELEASE],[lib_release])
AC_SUBST([CURRENT],[lib_current])
AC_SUBST([REVISION],[lib_revision])
AC_SUBST([AGE],[lib_age])

AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile third/Makefile tests/Makefile doc/Makefile doc/doxygen.cfg])
AC_OUTPUT

