/*
inc/cmdline.c - Command line handling
Copyright (C) 2013 Alan Manuel K. Gloria

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CMDLINE_H_INCLUDED
#define CMDLINE_H_INCLUDED

#include"ustr.h"

/* Note: based on the command line, this function may exit
   without returning.  */
void cmdline_init (int argc, char **argv,
		   char const *progname,
		   char const *copyright,
		   char const *writtenby,
		   char const *desc);
void cmdline_deinit (void);

/* Extract information about directories, either specified on the
   command line, or as programmed.  */
Ustr const *cmdline_pkgdatadir (void);
Ustr const *cmdline_bindir (void);

#endif /* !defined (CMDLINE_H_INCLUDED) */
