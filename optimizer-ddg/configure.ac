AC_INIT(optimizer-ddg, 0.3.1, simo.xan@gmail.com)
AM_INIT_AUTOMAKE(optimizer-ddg, 0.3.1)
AM_CONFIG_HEADER(src/config.h)
AC_SUBST(VERSION)
ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)
if test "${prefix}" == "NONE" ; then
	prefix=/usr/local
fi
AC_ARG_ENABLE(debug, [  --enable-debug    Enable debug compilation])
AC_ARG_ENABLE(printdebug, [  --enable-printdebug    Enable the print debug and the debug compilation])
AC_ARG_ENABLE(profile, [  --enable-profile    Enable the compilation for the automatically profiler tools])
AM_CONDITIONAL(DEBUG, test "$enable_debug" = "yes")
AM_CONDITIONAL(PRINTDEBUG, test "$enable_printdebug" = "yes")
AM_CONDITIONAL(PROFILE, test "$enable_profile" = "yes")
AC_DEFINE_UNQUOTED(PREFIX,		"${prefix}",				[Prefix directory])
AC_CANONICAL_HOST

##dnl Checks for programs.
AC_PROG_INSTALL
AC_PROG_CC
AC_PROG_LIBTOOL

##############################################################################################################################
#						Initialize compiler default options
##############################################################################################################################
AM_INIT_AUTOMAKE(-Wall -Werror)
CFLAGS="$CFLAGS -Wall"
AC_SUBST(CFLAGS)

##############################################################################################################################
#						Checks for libraries.
##############################################################################################################################
PKG_CHECK_MODULES(COMPILERMEMORYMANAGER, libcompilermemorymanager >= 0.3.0)
AC_SUBST(COMPILERMEMORYMANAGER_CFLAGS)
AC_SUBST(COMPILERMEMORYMANAGER_LIBS)

PKG_CHECK_MODULES(ILJITIROPTIMIZER, libiljitiroptimizer >= 0.3.0)
AC_SUBST(ILJITIROPTIMIZER_CFLAGS)
AC_SUBST(ILJITIROPTIMIZER_LIBS)

PKG_CHECK_MODULES(ILJITIR, libirmanager >= 0.3.1)
AC_SUBST(ILJITIR_CFLAGS)
AC_SUBST(ILJITIR_LIBS)

PKG_CHECK_MODULES(ILJITU, libiljitu >= 0.3.0)
AC_SUBST(ILJITU_CFLAGS)
AC_SUBST(ILJITU_LIBS)

PKG_CHECK_MODULES(XAN, libxan >= 0.2.0)
AC_SUBST(XAN_CFLAGS)
AC_SUBST(XAN_LIBS)

##dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h sys/param.h sys/time.h time.h sys/mkdev.h sys/sysmacros.h string.h memory.h fcntl.h dirent.h sys/ndir.h ndir.h alloca.h locale.h )

AC_OUTPUT(
	Makefile
	src/Makefile
)
