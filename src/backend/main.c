/*
src/backend/main.c - Main driver for Galactic Crisis Backend
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

#include<stdio.h>
#include<stdlib.h>

struct Command_s
{
  char const *command;
  void (*handler) (int argc, char const **argv);
};
typedef struct Command_s Command;

static void quit_command (int argc, char const **argv);
static bool quit_flag;
static int quit_value;

Command command_list[] = {
  {"quit", &quit_command},
  {0, 0}
};

static void command_loop (void);

int
main (int argc, char **argv)
{
  quit_flag = false;
  quit_value = 0;

  command_loop ();

  return quit_value;
}

static void
quit_command (int argc, char const **argv)
{
  quit_flag = true;
}

static char *
get_line (void)
{
  char *rv;
  unsigned int capacity;
  unsigned int line_length;
  int c;

  rv = 0;
  capacity = 0;
  line_length = 0;

  for (;;)
    {
      c = getchar ();
      if (c == EOF && line_length == 0)
	return 0;

      if (line_length + 1 >= capacity)
	{
	  unsigned int ncap = capacity + (capacity / 2) + 4;
	  char *nrv = malloc (ncap);
	  if (!nrv)
	    abort ();
	  if (rv)
	    memcpy (nrv, rv, line_length);
	  free (rv);
	  capacity = ncap;
	  rv = nrv;
	}

      if (c == '\n' || c == EOF)
	{
	  rv[line_length] = 0;
	  break;
	}
      else
	{
	  rv[line_length] = (char) c;
	  ++line_length;
	}
    }

  return rv;
}

static void
command_loop (void)
{
  char *cmd;
  while (!quit_flag)
    {
      printf ("galcry> ");
      cmd = get_line ();
      if (!cmd)
	{
	  quit_flag = true;
	  return;
	}

      /* Divide into words.  */
      /* Search command list.  */

      free (cmd);
    }
}
