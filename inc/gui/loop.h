/*
inc/gui/loop.h - Component for GUI game loop.
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
#ifndef GUI_LOOP_H_INCLUDED
#define GUI_LOOP_H_INCLUDED

#include<allegro.h>

/* Logic routine return values.  */
typedef enum LOOPRETURN_e
{
  /* Stop the current loop and exit it.  */
  LOOP_STOP,
  /* Just continue the current loop.  */
  LOOP_CONTINUE,
  /* Continue the current loop, and call the refresh routine.  */
  LOOP_REFRESH
} LOOPRETURN;

/* Set the ticks per second (number of times logic routine is called
   per second).  */
void loop_ticks_per_second (unsigned int i);
/* Sets whether refresh routines should be given a video bitmap or
   a memory bitmap.  */
void loop_set_direct_video_refresh (int setting);
int loop_get_direct_video_refresh (void);
/* Execute a game loop.  Game loops may be nested by the logic
   routine (i.e. a game loop logic routine).  */
void loop_execute (void *state,
		   void (*refresh) (BITMAP *buffer, void const *state),
		   LOOPRETURN (*logic) (void *state));

#endif /* !defined (GUI_LOOP_H_INCLUDED) */
