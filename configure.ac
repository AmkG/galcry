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

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AM_PROG_CC_C_O

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
