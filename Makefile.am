# Makefile.am - Compilation control file for Galactic Crisis
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

bin_PROGRAMS = galcry-backend galcry-gui

# All programs use this, so just put it as
# an extra distributable.
EXTRA_DIST = inc/all_config.h

galcry_backend_SOURCES =\
  src/backend/main.c \
  src/backend/gamestate.c \
  inc/backend/gamestate.h

galcry_gui_SOURCES =\
  src/gui/main.c

# useful common sublibraries
noinst_LIBRARIES =\
  libstringutils.a

libstringutils_a_SOURCES =\
  src/stringutils/austr.c\
  src/stringutils/mapping.c\
  src/stringutils/stringhash.c\
  src/stringutils/wordsplit.c\
  inc/stringutils.h

LDADD = $(noinst_LIBRARIES)

# subdirectories
SUBDIRS = ustr

# ustr support
LDADD += ustr/libustr.a
AM_CPPFLAGS = -I $(srcdir)/ustr -I $(srcdir)/inc

# documentation
info_TEXINFOS = doc/galcry.texi
doc_galcry_TEXINFOS = \
  doc/galcry/intro.texi \
  doc/galcry/licenses.texi \
  doc/galcry/fdl.texi \
  doc/galcry/gpl.texi

TEXI2DVI = texi2dvi --tidy

# needed support
ACLOCAL_AMFLAGS = -I m4
