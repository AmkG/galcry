/*
src/stringutils/stringhash.c - String hashing
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

uint32_t
stringhash (Ustr const *string)
{
  /* FNV-1a.  */
  uint32_t hash;
  char const *cstring;
  size_t i;
  size_t len;

  /* http://www.isthe.com/chongo/tech/comp/fnv/ */
  hash = 2166136261;
  cstring = ustr_cstr (string);
  len = ustr_len (string);

  for (i = 0; i < len; ++i)
    {
      hash = hash ^ ((uint32_t) cstring[i]);
      hash = hash * 16777619;
    }

  return hash;
}
