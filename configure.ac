AC_INIT(lpttrigger,0.3)
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
#AC_DISABLE_SHARED
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

AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile third/Makefile doc/Makefile doc/doxygen.cfg])
AC_OUTPUT

