/*
src/backend/gamestate.c - Component for handling game state.
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

#include"backend/gamestate.h"
#include"stringutils.h"

#include<stdlib.h>

/* Game State variables.  */
static Mapping global;
static Mapping *country;
unsigned int ncountry;

void
gamestate_initialize (void)
{
  mapping_init (&global);
  country = 0;
  ncountry = 0;
}

void
gamestate_shutdown (void)
{
  unsigned int i;
  mapping_deinit (&global);
  for (i = 0; i < ncountry; ++i)
    mapping_deinit (&country[i]);
  free (country);
}

void
gamestate_clear (void)
{
  gamestate_shutdown ();
  gamestate_initialize ();
}

void
gamestate_save (Ustr const *filename)
{
  /* TODO */
  abort ();
}
void
gamestate_load (Ustr const *filename)
{
  /* TODO */
  abort ();
}

void
gamestate_global_aget (char const *key, AUstr *value)
{
  Ustr *tmp;

  tmp = ustr_dup_cstr (key);

  austr_clear (value);
  (void) mapping_find (&global, tmp, value);

  ustr_sc_free (&tmp);
}
void
gamestate_global_aput (char const *key, AUstr const *value)
{
  Ustr *tmp;

  tmp = ustr_dup_cstr (key);

  mapping_ins (&global, tmp, value);

  ustr_sc_free (&tmp);
}
void
gamestate_global_get (char const *key, Ustr **value)
{
  AUstr avalue;

  austr_init (&avalue);
  gamestate_global_aget (key, &avalue);
  if (austr_length (&avalue) >= 1)
    ustr_set (value, austr_i (&avalue, 0));
  else
    ustr_sc_del (value);
  austr_deinit (&avalue);
}
void
gamestate_global_put (char const *key, Ustr const *value)
{
  AUstr avalue;

  austr_init (&avalue);
  austr_push (&avalue, value);
  gamestate_global_aput (key, &avalue);
  austr_deinit (&avalue);
}

void
gamestate_countries_get (unsigned int *value)
{
  *value = ncountry;
}
void
gamestate_countries_put (unsigned int value)
{
  unsigned int i;
  Mapping *new_array;

  if (value == ncountry)
    return;

  new_array = malloc (value * sizeof (Mapping));
  if (!new_array)
    abort ();

  if (value > ncountry)
    {
      memcpy (new_array, country, ncountry * sizeof (Mapping));
      for (i = ncountry; i < value; ++i)
	mapping_init (&country[i]);
    }
  else
    {
      memcpy (new_array, country, value * sizeof (Mapping));
      for (i = value; i < ncountry; ++i)
	mapping_deinit (&country[i]);
    }
  free (country);
  country = new_array;
  ncountry = value;
}

void
gamestate_country_aget (unsigned int i, char const *key, AUstr *value)
{
  Ustr *tmp;

  tmp = ustr_dup_cstr (key);

  austr_clear (value);
  (void) mapping_find (&country[i], tmp, value);

  ustr_sc_free (&tmp);
}
void
gamestate_country_aput (unsigned int i, char const *key, AUstr const *value)
{
  Ustr *tmp;

  tmp = ustr_dup_cstr (key);

  mapping_ins (&country[i], tmp, value);

  ustr_sc_free (&tmp);
}

void
gamestate_country_get (unsigned int i, char const *key, Ustr **value)
{
  AUstr avalue;

  austr_init (&avalue);  
  gamestate_country_aget (i, key, &avalue);
  if (austr_length (&avalue) >= 1)
    ustr_set (value, austr_i (&avalue, 0));
  else
    ustr_sc_del (value);
  austr_deinit (&avalue);
}
void
gamestate_country_put (unsigned int i, char const *key, Ustr const *value)
{
  AUstr avalue;

  austr_init (&avalue);
  austr_push (&avalue, value);
  gamestate_country_aput (i, key, &avalue);
  austr_deinit (&avalue);
}
