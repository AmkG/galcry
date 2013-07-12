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

#include<stdlib.h>
#include<string.h>

static bool initialized = false;

static BITMAP *target_bitmap = NULL;
static int direct_video = 0;
static unsigned loop_ticks = 100;

typedef struct Nested_s
{
  unsigned int cap;
  unsigned int len;
  void **a_state;
  LoopRefresh *a_refresh;
  LoopLogic *a_logic;
} Nested;
static Nested nested_calls = { 0, 0, 0, 0 };

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
loop_set_ticks_per_second (unsigned i)
{
  loop_ticks = i;
  if (initialized)
    {
      if (install_int_ex (&inc_ticks, BPS_TO_TIMER (loop_ticks)) != 0)
	gui_abort (0);
    }
}
unsigned int
loop_get_ticks_per_second (void)
{
  return loop_ticks;
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

static void
refresh_all (BITMAP *target)
{
  unsigned int i;
  for (i = 0; i < nested_calls.len; ++i)
    nested_calls.a_refresh[i] (target, nested_calls.a_state[i]);
}
static LOOPRETURN
logic_all (void)
{
  LOOPRETURN rv;
  unsigned int i;

  rv = LOOP_CONTINUE;
  for (i = 0; i < nested_calls.len; ++i)
    {
      switch (nested_calls.a_logic[i] (nested_calls.a_state[i]))
	{
	case LOOP_CONTINUE:
	  break;
	case LOOP_REFRESH:
	  rv = LOOP_REFRESH;
	  break;
	case LOOP_STOP:
	  if (i == nested_calls.len - 1)
	    rv = LOOP_STOP;
	  break;
	}
    }
  return rv;
}

void
loop_subexecute (void *state,
		 LoopRefresh refresh, LoopInput input, LoopLogic logic)
{
  bool needs_refresh;
  LOOPRETURN rv;

  if (!initialized)
    {
      initialized = true;
      LOCK_VARIABLE (ticks);
      LOCK_FUNCTION (inc_ticks);
      loop_set_ticks_per_second (loop_ticks);
      setup_target_bitmap ();
    }

  if (nested_calls.len == nested_calls.cap)
    {
      unsigned int ncap;
      void **na_state;
      LoopRefresh *na_refresh;
      LoopLogic *na_logic;

      ncap = nested_calls.cap + (nested_calls.cap / 2) + 4;

      na_state = (void **) malloc (ncap * sizeof (void *));
      if (!na_state)
	{
	  gui_abort (0);
	  abort ();
	}
      na_refresh = (LoopRefresh *) malloc (ncap * sizeof (LoopRefresh));
      if (!na_refresh)
	{
	  free (na_state);
	  gui_abort (0);
	  abort ();
	}
      na_logic = (LoopLogic *) malloc (ncap * sizeof (LoopLogic));
      if (!na_logic)
	{
	  free (na_state);
	  free (na_refresh);
	  gui_abort (0);
	  abort ();
	}

      memcpy (na_state, nested_calls.a_state,
	      nested_calls.len * sizeof (void *));
      memcpy (na_refresh, nested_calls.a_refresh,
	      nested_calls.len * sizeof (LoopRefresh));
      memcpy (na_logic, nested_calls.a_logic,
	      nested_calls.len * sizeof (LoopLogic));

      free (nested_calls.a_state);
      free (nested_calls.a_refresh);
      free (nested_calls.a_logic);

      nested_calls.cap = ncap;
      nested_calls.a_state = na_state;
      nested_calls.a_refresh = na_refresh;
      nested_calls.a_logic = na_logic;
    }

  nested_calls.a_state[nested_calls.len] = state;
  nested_calls.a_refresh[nested_calls.len] = refresh;
  nested_calls.a_logic[nested_calls.len] = logic;
  ++nested_calls.len;

  needs_refresh = true;
  rv = LOOP_CONTINUE;
  do
    {
      if (needs_refresh)
	{
	  refresh_all (target_bitmap);
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

      if (ticks)
	{
	  rv = LOOP_CONTINUE;
	  while (ticks > 0 && rv != LOOP_STOP)
	    {
	      --ticks;
	      input (state);
	      rv = logic_all ();
	      if (rv == LOOP_REFRESH)
		needs_refresh = true;
	    }
	}
      else
	rest (1);
    }
  while (rv != LOOP_STOP);

  --nested_calls.len;
}

void
loop_execute (void *state,
	      LoopRefresh refresh,
	      LoopInput input,
	      LoopLogic logic)
{
  Nested cached_nested;
  cached_nested = nested_calls;
  loop_subexecute (state, refresh, input, logic);
  nested_calls = cached_nested;
}
