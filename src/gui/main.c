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
#include"gui/a4f.h"
#include"gui/loop.h"
#include"gui/utils.h"
#include"ustr.h"

#include<allegro.h>

/* Simple GUI testing for loop.  */
typedef struct SIMPLE_s
{
  unsigned int mx;
  unsigned int my;
  bool quit_flag;
  A4F font;
  A4F font2;
  unsigned int x;
  unsigned int y;
} SIMPLE;

static void
simple_input (void *vs)
{
  SIMPLE *s;

  s = vs;

  poll_mouse ();
  s->mx = mouse_x;
  s->my = mouse_y;

  if (keypressed ())
    {
      (void) readkey ();
      s->quit_flag = true;
    }
  else if (mouse_b & 1 || mouse_b & 2 || mouse_b & 4)
    {
      s->quit_flag = true;
    }
}
static LOOPRETURN
simple_logic (void *vs)
{
  SIMPLE *s;

  s = vs;

  if (s->quit_flag)
    return LOOP_STOP;

  if (s->mx != s->x || s->my != s->y)
    {
      s->x = s->mx;
      s->y = s->my;
      return LOOP_REFRESH;
    }
  else
    return LOOP_CONTINUE;
}
static void
simple_refresh (BITMAP *target, void const *vs)
{
  SIMPLE const *s;
  int white;
  int cyan;

  set_color_depth (bitmap_color_depth (target));
  s = vs;
  white = makecol (255, 255, 255);
  cyan = makecol (0, 255, 255);

  clear_to_color (target, makecol (0,0,0));

  fastline (target, 0, 0, s->x, s->y, white);
  fastline (target, res_width - 1, 0, s->x, s->y, white);
  fastline (target, 0, res_height - 1, s->x, s->y, white);
  fastline (target, res_width - 1, res_height - 1, s->x, s->y, white);

  (void) a4f_textout (target, target->w / 2, target->h / 2, cyan,
		      s->font, "galcry-gui VAV");
  (void) a4f_textout (target, target->w / 4, target->h / 4, cyan,
		      s->font2, "galcry-gui VAV");
  fastline (target, 0, 0, target->w / 2, target->h / 2, cyan);
}

int
main (int argc, char **argv)
{
  cmdline_init (argc, argv,
		"galcry-gui",
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
  (void) install_keyboard ();
  (void) install_mouse ();
  (void) install_timer ();

  if (a4f_init () != 0)
    {
      printf ("%s: Unable to initialize font system.\n", argv[0]);
      printf ("%s: Error: %s\n", argv[0], allegro_error);
      allegro_message ("%s: Unable to initialize font system.\nError: %s",
		       argv[0], allegro_error);
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
      a4f_shutdown ();
      cmdline_deinit ();
      exit (1);
    }

  {
    SIMPLE s;
    Ustr *fname;

    s.quit_flag = false;
    s.x = 0;
    s.y = 0;

    fname = ustr_dup (cmdline_pkgdatadir ());
    if (!ustr_add_cstr (&fname, "/FreeSans.otf"))
      gui_abort (0);
    s.font = a4f_file_load (ustr_cstr (fname), 0);
    ustr_sc_free (&fname);
    if (!s.font)
      {
	fname = ustr_dup_cstr (allegro_error);
        gui_abort (&fname);
      }
    s.font2 = a4f_copy (s.font);
    a4f_set_height_pixels (s.font2, 32);

    loop_execute (&s, simple_refresh, simple_input, simple_logic);

    a4f_destroy (s.font);
    a4f_destroy (s.font2);
  }

  gui_exit (0);
}
/* *INDENT-OFF* */
END_OF_MAIN ()
/* *INDENT-ON* */
