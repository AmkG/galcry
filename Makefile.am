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
  src/backend/lexicon.c \
  src/backend/lloyd.c \
  src/backend/voronoi.cxx \
  inc/backend/gamestate.h \
  inc/backend/lexicon.h \
  inc/backend/lloyd.h \
  inc/backend/voronoi.h
galcry_backend_CPPFLAGS = $(CGAL_CPPFLAGS) $(BOOST_CPPFLAGS) $(AM_CPPFLAGS)
galcry_backend_LDADD = $(CGAL_LDFLAGS) $(BOOST_LDFLAGS) $(LDADD)

galcry_gui_SOURCES =\
  src/gui/main.c\
  src/gui/a4f.c\
  src/gui/loop.c\
  src/gui/utils.c\
  inc/gui/a4f.h\
  inc/gui/loop.h\
  inc/gui/utils.h
galcry_gui_CFLAGS = $(allegro4_CFLAGS) $(freetype2_CFLAGS)
galcry_gui_LDADD = $(allegro4_LIBS) $(freetype2_LIBS) $(LDADD)

# useful common sublibraries
noinst_LIBRARIES =\
  libstringutils.a\
  libcmdline.a

libstringutils_a_SOURCES =\
  src/stringutils/austr.c\
  src/stringutils/mapping.c\
  src/stringutils/stringhash.c\
  src/stringutils/wordsplit.c\
  inc/stringutils.h

libcmdline_a_SOURCES =\
  src/cmdline.c\
  inc/cmdline.h
libcmdline_a_CPPFLAGS =\
  -DPKGDATADIR="\"$(pkgdatadir)\""\
  -DBINDIR="\"$(bindir)\"" \
  $(AM_CPPFLAGS)

LDADD = $(noinst_LIBRARIES)

# subdirectories
SUBDIRS = ustr pkgdatadir

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

TEXI2DVI = texi2dvi --clean

# needed support
ACLOCAL_AMFLAGS = -I m4
