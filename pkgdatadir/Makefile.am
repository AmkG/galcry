# Makefile.am - Compilation control file for Galactic Crisis data
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

DISTCLEANFILES =

lexicons =\
  lexicon/kanmanla\
  lexicon/gismu\
  lexicon/latin\
  lexicon/tagalog

nobase_dist_pkgdata_DATA =\
  $(lexicons) \
  lexicons \
  FreeSans.otf

lexicons : Makefile.am
	rm -f lexicons
	for file in $(lexicons); do\
	  echo $$file | sed -e 's*lexicon/**' >> lexicons; done
