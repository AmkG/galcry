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

#include"stringutils.h"
#include"ustr.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Command_s
{
  char const *command;
  void (*handler) (AUstr *command_line);
};
typedef struct Command_s Command;

static void quit_command (AUstr *command_line);
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
quit_command (AUstr *command_line)
{
  quit_flag = true;
}

static Ustr *
get_line (void)
{
  Ustr *rv;
  int c;
  char charc;

  rv = USTR_NULL;

  for (;;)
    {
      c = getchar ();
      if (c == EOF && rv == USTR_NULL)
	return USTR_NULL;
      charc = (char) c;

      if (rv == USTR_NULL)
	rv = ustr_dup_cstr ("");

      if (c == '\n' || c == EOF)
	return rv;
      else
	{
	  if (!ustr_add_buf (&rv, &charc, 1))
	    abort ();
	}
    }

  return rv;
}

static void
command_loop (void)
{
  Ustr *cmd;
  AUstr words;

  austr_init (&words);

  while (!quit_flag)
    {
      printf ("galcry> ");
      cmd = get_line ();
      if (cmd == USTR_NULL)
	break;

      /* Divide into words.  */
      wordsplit (&words, cmd);

      /* Search command list.  */
      if (austr_length (&words) != 0)
	{
	  Command *pc;
	  bool found;
	  void (*handler) (AUstr *);

	  ustr_set (&cmd, austr_i (&words, 0));
	  found = false;
	  for (pc = command_list; pc->command; ++pc)
	    {
	      if (strcmp (ustr_cstr (cmd), pc->command) == 0)
		{
		  handler = pc->handler;
		  found = true;
		  break;
		}
	    }

	  if (!found)
	    printf ("NG \"%s\"\n", "Unrecognized Command");
	  else
	    handler (&words);
	}

      ustr_sc_free (&cmd);
    }
  austr_deinit (&words);
}
