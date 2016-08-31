#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([uz80as - Micro Z80 assembler],
	[1.01],
	[jorge.giner@hotmail.com],
	uz80as,
	[http://jorgicor.sdfeu.org/uz80as])

AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE(-Wall -Werror gnu check-news color-tests parallel-tests)
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AM_MISSING_PROG(HELP2MAN, help2man, $missing_dir)

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 doc/Makefile
		 tests/Makefile])
AC_OUTPUT
