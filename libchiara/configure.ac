AC_INIT(libchiara, 1.0.0, simo.xan@gmail.com)
AM_INIT_AUTOMAKE
AM_PROG_AR
AM_CONFIG_HEADER(src/config.h)
AC_SUBST(VERSION)
ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)
AC_PREFIX_DEFAULT(/usr/local)
if test "${prefix}" == "NONE" ; then
	prefix=/usr/local
fi
AC_ARG_ENABLE(debug, [  --enable-debug    Enable debug compilation])
AC_ARG_ENABLE(optdebug, [  --enable-optdebug    Enable optimised debug compilation])
AC_ARG_ENABLE(printdebug, [  --enable-printdebug    Enable the print debug and the debug compilation])
AC_ARG_ENABLE(profile, [  --enable-profile    Enable the compilation for the automatically profiler tools])
AM_CONDITIONAL(DEBUG, test "$enable_debug" = "yes")
AM_CONDITIONAL(OPTDEBUG, test "$enable_optdebug" = "yes")
AM_CONDITIONAL(PRINTDEBUG, test "$enable_printdebug" = "yes")
AM_CONDITIONAL(PROFILE, test "$enable_profile" = "yes")
AC_DEFINE_UNQUOTED(PREFIX,		"${prefix}",				[Prefix directory])
AC_DEFINE_UNQUOTED(DATADIR,		"${prefix}/share",			[Data directory])
AC_CANONICAL_HOST

##############################################################################################################################
#						Initialize compiler default options
##############################################################################################################################
AM_INIT_AUTOMAKE(-Wall -Werror)
CFLAGS="$CFLAGS -Wall"
AC_SUBST(CFLAGS)


##############################################################################################################################
#						Checks for programs.
##############################################################################################################################
AC_PROG_INSTALL
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
if ! test -z "$MAKE" ; then
	AC_ERROR([Error: make program not exist, you have to install it])
fi
AC_CHECK_TOOL(BZIP2, bzip2, no)
if test "$BZIP2" = "no" ; then
	AC_ERROR([Error: you have to install bzip2 program])
fi
AC_CHECK_TOOL(PKGVAR, pkg-config)
if test -z "$PKGVAR" ; then
	echo "ERROR: I can't find the pkg-config program, then you have to install it" ;
	exit 1 ;
fi
AC_PROG_LEX
AC_CHECK_PROGS(XLEX,$LEX flex lex,none)
if test "$XLEX" = "none" ; then
	echo "*** Error: No suitable flex found. ***"
	echo "    Please install the 'flex' package."
	exit 1
fi

##############################################################################################################################
#						Checks for libraries.
##############################################################################################################################
PKG_CHECK_MODULES(XAN, libxan >= 0.2.0)
AC_SUBST(XAN_CFLAGS)
AC_SUBST(XAN_LIBS)

PKG_CHECK_MODULES(ILJITMM_ILJITU, libiljitu >= 0.3.1)
AC_SUBST(ILJITU_CFLAGS)
AC_SUBST(ILJITU_LIBS)

PKG_CHECK_MODULES(ILJITMM_ILJITSM, libiljitsm >= 0.3.1)
AC_SUBST(ILJITSM_CFLAGS)
AC_SUBST(ILJITSM_LIBS)

PKG_CHECK_MODULES(ILJITMM, libiljitmm >= 0.3.1)
AC_SUBST(ILJITMM_CFLAGS)
AC_SUBST(ILJITMM_LIBS)

PKG_CHECK_MODULES(LIBILJITIR, libirmanager >= 0.3.1)
AC_SUBST(LIBILJITIR_CFLAGS)
AC_SUBST(LIBILJITIR_LIBS)

PKG_CHECK_MODULES(DOMINANCE, libdominance >= 0.2.0)
AC_SUBST(DOMINANCE_CFLAGS)
AC_SUBST(DOMINANCE_LIBS)


##############################################################################################################################
##						Checks for headers.
##############################################################################################################################
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h sys/param.h sys/time.h time.h sys/mkdev.h sys/sysmacros.h string.h memory.h fcntl.h dirent.h sys/ndir.h ndir.h alloca.h locale.h )

AC_HEADER_MAJOR
AC_FUNC_ALLOCA
AC_STRUCT_TM
AC_STRUCT_ST_BLOCKS

AC_OUTPUT(
	Makefile 
	src/Makefile
	include/Makefile
	pkgconfig/libchiara.pc
	pkgconfig/Makefile
)
