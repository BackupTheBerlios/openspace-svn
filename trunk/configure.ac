#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(openspace, 0.0.0.1, compbatant@t-nas.org)
AC_CONFIG_SRCDIR(src/openspace.cpp)
AC_CONFIG_HEADER(src/config.h)
AM_INIT_AUTOMAKE
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP


AM_PROG_LIBTOOL
# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([fcntl.h stdlib.h sys/mount.h sys/vfs.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_CHECK_MEMBERS([struct stat.st_rdev])

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_STAT
AC_CHECK_FUNCS([dup2 endgrent endpwent mkdir mkfifo rmdir strtoul])

AC_CONFIG_FILES([
		 openspacerc
		 Makefile
		 src/Makefile
		 icons/Makefile
		 src/plugins/Makefile
		 src/plugins/filelist/Makefile
		 src/plugins/cmddialog/Makefile
                 ])
AC_OUTPUT
