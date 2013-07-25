/*
src/gui/utils.c - Minor utilities for GUI program.
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
#include"all_config.h"

#include"gui/a4f.h"
#include"gui/utils.h"

#include<allegro.h>

int res_width;
int res_height;
int res_depth;

A4F FreeSansNormal = 0;
A4F FreeSansBig = 0;

static void
common_gui_shutdown (int rc)
{
  if (FreeSansNormal)
    a4f_destroy (FreeSansNormal);
  if (FreeSansBig)
    a4f_destroy (FreeSansBig);
  a4f_shutdown ();
  cmdline_deinit ();
  allegro_exit ();
  exit (rc);
}

void
gui_exit (int rc)
{
  (void) set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
  common_gui_shutdown (rc);
}
void
gui_abort (Ustr **ps)
{
  (void) set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
  if (ps && *ps)
    {
      allegro_message ("Galactic Crisis GUI Abort!\nError: %s", 
		       ustr_cstr (*ps));
      ustr_sc_free (ps);
    }
  else
    {
      allegro_message ("Galactic Crisis GUI Abort!");
    }
  common_gui_shutdown (2);
}
