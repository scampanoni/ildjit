optimizer_mergeconversions_LTLIBRARIES = optimizer_mergeconversions.la
optimizer_mergeconversionsdir = $(libdir)/iljit/optimizers

AM_CFLAGS= -Wall -std=gnu99 -D_GNU_SOURCE		\
	$(ILJITU_CFLAGS) $(XAN_CFLAGS) $(ILJITIR_CFLAGS) $(ILJITIROPTIMIZER_CFLAGS)

if PROFILE
AM_CFLAGS 	+= -DNDEBUG -DPROFILE -g -O0
else 
if PRINTDEBUG
AM_CFLAGS 	+= -ggdb -DPRINTDEBUG -DDEBUG -O0
else
if DEBUG
AM_CFLAGS	+= -ggdb -DDEBUG -O0
else
if OPTDEBUG
AM_CFLAGS	+= -ggdb -DDEBUG -O3
else
AM_CFLAGS	+= -DNDEBUG -O3
endif
endif
endif
endif

optimizer_mergeconversions_la_SOURCES=							            \
        delete_conversions.c                delete_conversions.h            \
		optimizer_mergeconversions.c		optimizer_mergeconversions.h

optimizer_mergeconversions_la_LDFLAGS= -module -avoid_version

optimizer_mergeconversions_la_LIBADD = $(ILJITU_LIBS) $(XAN_LIBS) $(ILJITIROPTIMIZER_LIBS) $(ILJITIR_LIBS)

MAINTAINERCLEANFILES = Makefile.in config.h.in
