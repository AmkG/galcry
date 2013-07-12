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
void loop_set_ticks_per_second (unsigned int i);
unsigned int loop_get_ticks_per_second (void);
/* Sets whether refresh routines should be given a video bitmap or
   a memory bitmap.  */
void loop_set_direct_video_refresh (int setting);
int loop_get_direct_video_refresh (void);

/* Used by game loop functions.  */
typedef void (*LoopRefresh) (BITMAP *buffer, void const *state);
typedef void (*LoopInput) (void *state);
typedef LOOPRETURN (*LoopLogic) (void *state);

/* Execute a game loop.  Game loops may be nested by the logic
   routine (i.e. a game loop logic routine).
   INPUT is expected to only check inputs (keyboard, mouse, etc.)
   and set flags in the game state, while LOGIC is expected to
   modify the game state based on the input and clear any input
   flags.  The reason for separating them has to do with
   loop_subexecute below.  */
void loop_execute (void *state,
		   LoopRefresh refresh,
		   LoopInput input,
		   LoopLogic logic);
/* Like loop_execute, except that the REFRESH and LOGIC of
   higher calls to loop_subexecute or loop_execute are
   also executed.
   Basically, loop_subexecute puts the REFRESH and LOGIC
   of any current loop execution onto a stack.  REFRESH
   and LOGIC of earlier calls is called first before REFRESH
   of later calls; the latest LOGIC controls whether or not
   to stop, and if any LOGIC requests a refresh, all
   REFRESH routines are called.
   Only the latest INPUT routine is called.
   The main purpose of this API is to allow a sub-menu to
   capture control of the mouse and/or keyboard.  */
void loop_subexecute (void *state,
		      LoopRefresh refresh,
		      LoopInput input,
		      LoopLogic logic);

#endif /* !defined (GUI_LOOP_H_INCLUDED) */
