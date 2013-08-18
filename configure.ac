#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# Copyright (C) 2013 Alan Manuel K. Gloria
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.68])
AC_INIT([GalCry], [0.1A], [almkglor@gmail.com])
AC_CONFIG_SRCDIR([src/backend/main.c])
AC_CONFIG_AUX_DIR([auxdir])
AC_CONFIG_HEADERS([inc/config.h])
AC_CONFIG_LIBOBJ_DIR([libobj])
AM_INIT_AUTOMAKE([gnu check-news subdir-objects std-options silent-rules])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AM_PROG_AR

AC_LANG([C])

# Checks for libraries.
PKG_CHECK_MODULES([allegro4], [allegro],,[#else
  AC_PATH_TOOL([ALLEGRO_CONFIG], [allegro-config], [no])
  AS_IF([test x$ALLEGRO_CONFIG = xno],[#then
    AC_MSG_ERROR([
Cannot find allegro installation by either allegro-config or pkg-config.
The pkg-config utility reported the following errors:

$allegro4_PKG_ERRORS
])
  ],[#else
    allegro4_CFLAGS=`$ALLEGRO_CONFIG --cflags`
    allegro4_LIBS=`$ALLEGRO_CONFIG --libs`
  ])
])
PKG_CHECK_MODULES([freetype2], [freetype2],,[#else
  AC_PATH_TOOL([FREETYPE_CONFIG], [freetype-config], [no])
  AS_IF([test x$FREETYPE_CONFIG = xno],[#then
    AC_MSG_ERROR([
Cannot find freetype2 installation by either freetype-config or pkg-config.
The pkg-config utility reported the following errors:

$freetype2_PKG_ERRORS
])
  ],[#else
    freetype2_CFLAGS=`$FREETYPE_CONFIG --cflags`
    freetype2_LIBS=`$FREETYPE_CONFIG --libs`
  ])
])

# GMP is needed to properly link CGAL.
AC_CHECK_LIB([gmp],[__gmpz_init],,[#fail
AC_MSG_ERROR([GNU MP not found])
])
# On my system CGAL needs to have checking disabled
# to properly link using autotools.
CPPFLAGS="${CPPFLAGS} -DCGAL_NO_ASSERTIONS -DCGAL_NO_PRECONDITIONS -DCGAL_NO_POSTCONDITIONS"
AC_LANG([C++])
AX_LIB_CGAL_CORE(,[#fail
  AC_MSG_ERROR([
Cannot find CGAL installation.
  ])
])
AC_LANG([C])


# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AM_PROG_CC_C_O
AC_C_INLINE
AC_LANG([C++])
AC_PROG_CXX_C_O
AC_LANG([C])

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile ustr/Makefile pkgdatadir/Makefile])
AC_OUTPUT
