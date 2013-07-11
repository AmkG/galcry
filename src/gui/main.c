/*
src/gui/main.c - Main driver for Galactic Crisis GUI
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

#include"cmdline.h"
#include"gui/utils.h"
#include"ustr.h"

#include<allegro.h>

int
main (int argc, char **argv)
{
  cmdline_init (argc, argv,
		"galcry-backend",
		"Copyright (C) 2013 Alan Manuel K. Gloria",
		"Written by Alan Manuel K. Gloria",
		"Launches the Galactic Crisis main GUI program.");
  if (allegro_init () != 0)
    {
      printf ("%s: Unable to launch allegro!\n", argv[0]);
      printf ("%s: Error: %s\n", argv[0], allegro_error);
      cmdline_deinit ();
      exit (1);
    }

  if (get_desktop_resolution (&res_width, &res_height) != 0)
    {
      res_width = 640;
      res_height = 480;
    }
  res_depth = desktop_color_depth ();
  if (res_depth == 0 || res_depth == 8)
    {
      res_depth = 32;
    }

  set_color_depth (res_depth);
  if (set_gfx_mode (GFX_AUTODETECT_FULLSCREEN, res_width, res_height, 0, 0)
      != 0)
    {
      printf ("%s: Allegro unable to switch to %d x %d screen.\n", argv[0],
	      res_width, res_height);
      printf ("%s: Error: %s\n", argv[0], allegro_error);
      allegro_message ("%s: Allegro unable to switch to %d x %d screen.\n"
		       "Error: %s.",
		       argv[0], res_width, res_height, allegro_error);
      cmdline_deinit ();
      exit (1);
    }

  gui_exit (0);
}
/* *INDENT-OFF* */
END_OF_MAIN ()
/* *INDENT-ON* */
