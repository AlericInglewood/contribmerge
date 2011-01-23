dnl Run autogen.sh to produce a configure script.

dnl CW_VERSION_MAJOR               : Increment when the whole application was rewritten; or some VERY MAJOR change was made.
dnl CW_VERSION_MINOR               : Increment when major changes have occured.
dnl CW_VERSION_REVISION            : Increment every public release; or set to 0 when CW_VERSION_MINOR was incremented.

define(CW_VERSION_MAJOR, 0)
define(CW_VERSION_MINOR, 0)
define(CW_VERSION_REVISION, 0)

define(CW_PACKAGE_NAME, [contribmerge])
define(CW_BUGREPORT, [Aleric.Inglewood@gmai.com])

dnl Package name and version
AC_INIT(CW_PACKAGE_NAME, CW_VERSION_MAJOR.CW_VERSION_MINOR.CW_VERSION_REVISION, CW_BUGREPORT)

dnl Put resulting configuration defines in this header file.
AC_CONFIG_HEADERS([config.h])

dnl Ensure that the directory specified with --srcdir was correct
dnl by demanding that it contains `src/contribmerge.cc'.
AC_CONFIG_SRCDIR([src/contribmerge.cc])

dnl Automake options.
AM_INIT_AUTOMAKE

dnl Include maintainer mode targets
AM_MAINTAINER_MODE

dnl Because we use cwautomacros.
CW_AUTOMACROS([20080318])

dnl Demand at least version 2.57 of autoconf.
AC_PREREQ(2.57)

dnl Check for compiler and preprocessor
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CXXCPP

dnl Add --enable-debug (DEBUG, DOXYGEN_DEBUG), --enable-libcwd (CWDEBUG, DOXYGEN_CWDEBUG),
dnl --enable-optimise and --enable-profile options. Update USE_LIBCWD, CWD_LIBS and CXXFLAGS accordingly.
CW_OPG_FLAGS([-W -Wall -Woverloaded-virtual -Wundef -Wpointer-arith -Wwrite-strings -Winline])

dnl Add --enable-pch (USE_PCH (macro)) option.
dnl Update USE_PCH (automake conditional) and PCHFLAGS accordingly.
CW_PCHFLAGS

dnl Each Makefile.am should use DEFS = @DEFS@. Set DEFS here.
DEFS="-DHAVE_CONFIG_H"
AC_SUBST(DEFS)

dnl Test if we are checked out by git.
if test -d "$srcdir/.git"; then
  if test x"$enable_maintainer_mode" != x"yes"; then
    AC_MSG_ERROR([Always use --enable-maintainer-mode if you are using git.])
  fi
  AC_DEFINE_UNQUOTED([USE_GIT], 1, [Define when this project is checked out using git.])
fi
AM_CONDITIONAL([USE_GIT], [test -d "$srcdir/.git"])

dnl Set this conditional when --enable-debug was given.
AM_CONDITIONAL([USE_DEBUG], [test x"$cw_config_debug" = x"yes"])

dnl Set this conditional if we're linking with libcwd.
AM_CONDITIONAL([USE_CWDEBUG], [test x"$cw_used_libcwd" = x"yes"])

dnl Checks for other programs.
AC_PROG_INSTALL

dnl Used in sys.h to force recompilation when the compiler version changes.
CW_PROG_CXX_FINGER_PRINTS
CC_FINGER_PRINT="$cw_prog_cc_finger_print"
AC_SUBST([CC_FINGER_PRINT])
CXX_FINGER_PRINT="$cw_prog_cxx_finger_print"
AC_SUBST([CXX_FINGER_PRINT])
CXXCPP_FINGER_PRINT="$cw_prog_cxxcpp_finger_print"
AC_SUBST([CXXCPP_FINGER_PRINT])

dnl Use in sys.h to force recompilation when CXXFLAGS change.
CXXFLAGS_FINGER_PRINT=[$(echo $CXXFLAGS | sed -e 's/-W[a-z-]* *//g')]
AC_SUBST([CXXFLAGS_FINGER_PRINT])

dnl Generate src/sys.h from src/sys.h.in
CW_CONFIG_FILE([src], [sys.h])

dnl Output the other configuration files.
AC_CONFIG_FILES(
        [Makefile]
        [src/Makefile]
        [src/tests/Makefile])

AC_OUTPUT
