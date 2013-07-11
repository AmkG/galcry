/*
inc/gui/utils.h - Minor utilities for GUI program.
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
#ifndef GUI_UTILITIES_H_INCLUDED
#define GUI_UTILITIES_H_INCLUDED

#include "ustr.h"

void gui_exit (int rc);
void gui_abort (Ustr **ps);

extern int res_width;
extern int res_height;
extern int res_depth;


#endif /* !defined (GUI_UTILITIES_H_INCLUDED) */
