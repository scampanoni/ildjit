MAINTAINERCLEANFILES = Makefile.in config.h.in

libdla_LTLIBRARIES = libdla.la
libdladir = $(libdir)/iljit

AM_CFLAGS=-Wall $(XANLIB_CFLAGS) $(LIBILJITU_CFLAGS) $(LIBILJITIR_CFLAGS) $(LIBILJITIROPTIMIZER_CFLAGS) $(IRVIRTUALMACHINE_CFLAGS) $(CLIMANAGER_CFLAGS) $(PLATFORM_CFLAGS)
if CYGWIN
AM_CFLAGS += -shared
endif
if PROFILE
AM_CFLAGS 	+= -DNDEBUG -DPROFILE -O0 -g
else
if PRINTDEBUG
AM_CFLAGS	+= -ggdb -DDEBUG -DPRINTDEBUG -O0
else 
if DEBUG
AM_CFLAGS	+= -ggdb -DDEBUG -O0
else 
AM_CFLAGS	+= -O3 -DNDEBUG 
endif
endif
endif

libdla_la_SOURCES=					\
		dla.c		dla.h

libdla_la_LDFLAGS= -fPIC
libdla_la_LIBADD= $(XANLIB_LIBS) $(LIBILJITU_LIBS) $(LIBILJITIR_LIBS) $(LIBILJITIRPROFILER_LIBS) $(IRVIRTUALMACHINE_LIBS) $(CLIMANAGER_LIBS) $(ILJITIROPTIMIZER_LIBS) $(PLATFORM_LIBS)

