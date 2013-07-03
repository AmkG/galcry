/*
inc/stringutils.h - Various string utilities
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
#ifndef STRINGUTILS_H_INCLUDED
#define STRINGUTILS_H_INCLUDED

#include"ustr.h"

/*-----------------------------------------------------------------------------
Array of Strings
-----------------------------------------------------------------------------*/

struct AUstr_s
{
  Ustr **array;
  unsigned int length;
  unsigned int capacity;
};
typedef struct AUstr_s AUstr;

void austr_init (AUstr *parray);
void austr_deinit (AUstr *parray);

static unsigned int
austr_length (AUstr const *parray)
{
  return parray->length;
}
static Ustr const *
austr_i (AUstr const *parray, unsigned int i)
{
  return parray->array[i];
}
static Ustr **
austr_wi (AUstr *parray, unsigned int i)
{
  return &parray->array[i];
}

/* Stack-like.  */
void austr_push (AUstr *parray, Ustr const *string);
void austr_pop (AUstr *parray, Ustr **pstring);

/* Resize and clear.  Resizing to a larger size creates
   empty strings.  */
void austr_resize (AUstr *parray, unsigned int length);
static void
austr_clear (AUstr *parray)
{
  austr_resize (parray, 0);
}

/*-----------------------------------------------------------------------------
Various utility functions
-----------------------------------------------------------------------------*/

/* Splits the given string into words.  The given *PARRAY
   should be initialized by the caller.  This function
   will clear the array and then fill it.  The caller is
   responsible for deinitializing and freeing *PARRAY
   and the input string.

   Word splitting respects "", which supports C string
   escapes.  */
void wordsplit (AUstr *parray, Ustr const *string);

#endif /* !defined(STRINGUTILS_H_INCLUDED) */
