AUTOMAKE_OPTIONS = subdir-objects
AM_CFLAGS = -I$(top_srcdir)/lib

lib_LTLIBRARIES = liblpttrigger.la
bin_PROGRAMS = testport
include_HEADERS = lpttrigger.h lptwriter.h lptpwm.h

liblpttrigger_la_SOURCES = lpttrigger.h lpttrigger.c \
			   lptwriter.h               \
			   lptpwm.h lptpwm.c
if LINUX_OS
liblpttrigger_la_SOURCES += lptwriter-linux.c
endif
if WIN32_OS
liblpttrigger_la_SOURCES += lptwriter-win32.cpp winio_dll.h
endif
liblpttrigger_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(CURRENT):$(REVISION):$(AGE) -release $(RELEASE)
liblpttrigger_la_LIBADD = $(LTLIBOBJS)

testport_SOURCES = testport.c
testport_LDADD = liblpttrigger.la
