/*
src/stringutils/austr.c - Implementation of Ustr arrays.
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

#include<stdlib.h>
#include<string.h>

void
austr_init (AUstr *parray)
{
  parray->array = 0;
  parray->length = 0;
  parray->capacity = 0;
}
void
austr_deinit (AUstr *parray)
{
  unsigned int i;
  for (i = 0; i < parray->length; ++i)
    ustr_sc_free (&parray->array[i]);
  free (parray->array);
}

void
austr_resize (AUstr *parray, unsigned int length)
{
  if (length < parray->length)
    {
      unsigned int i;
      for (i = length; i < parray->length; ++i)
	ustr_sc_free (&parray->array[i]);
      parray->length = length;
    }
  else if (length == parray->length)
    return;
  else
    {
      unsigned int i;
      if (length > parray->capacity)
	{
	  Ustr **array;

	  parray->capacity = length + (length / 2);
	  if (parray->capacity < 4)
	    parray->capacity = 4;

	  array = malloc (sizeof (Ustr *) * parray->capacity);
	  if (!array)
	    abort ();

	  memcpy (array, parray->array, sizeof (Ustr *) * parray->length);

	  free (parray->array);
	  parray->array = array;
	}

      for (i = parray->length; i < length; ++i)
	parray->array[i] = USTR ("");
      parray->length = length;
    }
}

void
austr_push (AUstr *parray, Ustr const *string)
{
  austr_resize (parray, parray->length + 1);
  if (!ustr_set (austr_wi (parray, parray->length - 1), string))
    abort ();
}
void
austr_pop (AUstr *parray, Ustr **string)
{
  if (!ustr_set (string, austr_i (parray, parray->length - 1)))
    abort ();
  austr_resize (parray, parray->length - 1);
}
