optimizer_branchpredictors_LTLIBRARIES = optimizer_branchpredictors.la
optimizer_branchpredictorsdir = $(libdir)/iljit/optimizers

AM_CFLAGS=$(ILJITU_CFLAGS) $(XAN_CFLAGS) $(ILJITIR_CFLAGS) $(ILJITIROPTIMIZER_CFLAGS)

if PROFILE
AM_CFLAGS 	+= -DNDEBUG -DPROFILE -ggdb3 -O2
else 
if PRINTDEBUG
AM_CFLAGS 	+= -ggdb -DPRINTDEBUG -DDEBUG -O0
else
if DEBUG
AM_CFLAGS	+= -ggdb3 -DDEBUG -O0
else
AM_CFLAGS	+= -DNDEBUG -O3
endif
endif
endif

optimizer_branchpredictors_la_SOURCES=							\
		optimizer_branchpredictors.c		optimizer_branchpredictors.h

optimizer_branchpredictors_la_LDFLAGS= -module -avoid_version 

optimizer_branchpredictors_la_LIBADD = $(ILJITU_LIBS) $(XAN_LIBS) $(ILJITIROPTIMIZER_LIBS) $(ILJITIR_LIBS)

MAINTAINERCLEANFILES = Makefile.in config.h.in

