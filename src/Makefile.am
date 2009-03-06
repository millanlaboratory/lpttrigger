AUTOMAKE_OPTIONS = subdir-objects
AM_CFLAGS = -I$(top_srcdir)/lib

lib_LTLIBRARIES = liblpttrigger.la
bin_PROGRAMS = testport
include_HEADERS = lpttrigger.h

liblpttrigger_la_SOURCES = lpttrigger.h lpttrigger.c lptwriter.h
if LINUX_OS
liblpttrigger_la_SOURCES += lptwriter-linux.c
endif
if WIN32_OS
liblpttrigger_la_SOURCES += lptwriter-win32.cpp
endif
liblpttrigger_la_LIBADD = $(LTLIBOBJS)

testport_SOURCES = testport.c
testport_LDADD = liblpttrigger.la
