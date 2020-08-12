AC_INIT([MicroEMACS], [3.6.1-beta1],, [me], [https://github.com/troglobit/MicroEMACS/])
AC_CONFIG_AUX_DIR(aux)
AM_INIT_AUTOMAKE([1.11 foreign])
AM_SILENT_RULES([yes])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_FILES([Makefile src/Makefile])

AC_PROG_CC
AC_PROG_INSTALL

AC_HEADER_STDC

AC_CANONICAL_HOST
case $host in
  *-linux*)
    AC_DEFINE([linux])
    ;;
  *-freebsd* | *-openbsd* | *-netbsd* | *-dragonfly* | *-solaris* | *-apple*)
    AC_DEFINE([BSD])
    ;;
  *)
    AC_MSG_WARN(Your operating system is not officially supported yet by MicroEMACS.)
    AC_MSG_WARN(Set DEFS=-Dfoo manually to one of: AMIGA, VMS, CPM, MSDOS, V7, etc.)
    ;;
esac

AC_OUTPUT
