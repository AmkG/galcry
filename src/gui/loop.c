/*
src/gui/loop.c - Component for GUI game loop.
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

#include"gui/loop.h"
#include"gui/utils.h"

static bool initialized = false;

static BITMAP *target_bitmap = NULL;
static int direct_video = 0;
static unsigned loop_ticks = 100;

/* Not sure if this is fine in recent machines... */
static volatile int ticks = 0;

static void
inc_ticks (void)
{
  ++ticks;
}
/* *INDENT-OFF* */
END_OF_STATIC_FUNCTION (inc_ticks)
/* *INDENT-ON* */

static void
setup_target_bitmap (void)
{
  int saved_depth;

  saved_depth = get_color_depth ();
  set_color_depth (bitmap_color_depth (screen));

  if (target_bitmap && target_bitmap != screen)
    destroy_bitmap (target_bitmap);

  if (direct_video)
    target_bitmap = screen;
  else
    target_bitmap = create_bitmap (screen->w, screen->h);

  set_color_depth (saved_depth);
}

void
loop_ticks_per_second (unsigned i)
{
  loop_ticks = i;
  if (initialized)
    {
      if (install_int_ex (&inc_ticks, BPS_TO_TIMER (loop_ticks)) != 0)
	gui_abort (0);
    }
}

void
loop_set_direct_video_refresh (int i)
{
  direct_video = i;
  if (initialized)
    setup_target_bitmap ();
}

int
loop_get_direct_video_refresh (void)
{
  return direct_video;
}

void
loop_execute (void *state,
	      void (*refresh) (BITMAP * buffer, void const *state),
	      LOOPRETURN (*logic) (void *state))
{
  bool needs_refresh;
  LOOPRETURN rv;

  if (!initialized)
    {
      initialized = true;
      loop_ticks_per_second (loop_ticks);
      setup_target_bitmap ();
    }

  needs_refresh = true;
  do
    {
      if (needs_refresh)
	{
	  refresh (target_bitmap, state);
	  if (direct_video)
	    rest (1);
	  else
	    {
	      vsync ();
	      blit (target_bitmap, screen, 0, 0, 0, 0,
		    target_bitmap->w, target_bitmap->h);
	    }
	  needs_refresh = false;
	}

      rv = LOOP_CONTINUE;
      while (ticks > 0 && rv != LOOP_STOP)
	{
	  --ticks;
	  rv = logic (state);
	  if (rv == LOOP_REFRESH)
	    needs_refresh = true;
	}
    }
  while (rv != LOOP_STOP);
}
