## Automake file to produce wss program
optimizer_alias_LTLIBRARIES = optimizer_alias.la
optimizer_aliasdir = $(libdir)/iljit/optimizers

AM_CFLAGS=-Wall -Wextra $(ILJITU_CFLAGS) $(XAN_CFLAGS) $(ILJITIR_CFLAGS) $(ILJITIROPTIMIZER_CFLAGS)
if PROFILE
AM_CFLAGS 	+= -DNDEBUG -DPROFILE -g -O0
else 
if PRINTDEBUG
AM_CFLAGS 	+= -ggdb -DPRINTDEBUG -DDEBUG -O0
else
if DEBUG
AM_CFLAGS	+= -ggdb -DDEBUG -O0
else
AM_CFLAGS	+= -DNDEBUG -O3
endif
endif
endif

optimizer_alias_la_SOURCES=							\
		optimizer_alias.c		optimizer_alias.h

optimizer_alias_la_LDFLAGS= -module -avoid_version $(ILJITU_LIBS) $(XAN_LIBS) $(ILJITIROPTIMIZER_LIBS) $(ILJITIR_LIBS)
