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

#if HAVE_STDINT_H
# include<stdint.h>
#endif
#if !(defined (UINT32_MAX) || defined (uint32_t))
typedef unsigned int uint32_t;
#endif /* !(defined (UINT32_MAX) || defined (uint32_t)) */

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

static inline unsigned int
austr_length (AUstr const *parray)
{
  return parray->length;
}
static inline Ustr const *
austr_i (AUstr const *parray, unsigned int i)
{
  return parray->array[i];
}
static inline Ustr **
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
static inline void
austr_clear (AUstr *parray)
{
  austr_resize (parray, 0);
}

/*-----------------------------------------------------------------------------
Mappings of Strings to Array of Strings
-----------------------------------------------------------------------------*/

struct MappingImpl_s;

struct Mapping_s
{
  struct MappingImpl_s *pimpl;
};
typedef struct Mapping_s Mapping;

void mapping_init (Mapping *pmapping);
void mapping_deinit (Mapping *pmapping);

/* Insert or delete mappings.  For insertion, VALUE is copied
   if non-null, otherwise an empty array is mapped.  */
void mapping_ins (Mapping *pmapping, Ustr const *key, AUstr const *value);
void mapping_del (Mapping *pmapping, Ustr const *key);

/* Lookup a KEY in the *PMAPPING.  If VALUE is non-null, copy
   the value into the array of strings.  */
bool mapping_find (Mapping const *pmapping, Ustr const *key, AUstr *value);

/* Copy all keys in the *PMAPPING.  */
void mapping_keys (Mapping const *pmapping, AUstr *keys);

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

/* Gets the hash of the string.  */
uint32_t stringhash (Ustr const *string);

#endif /* !defined(STRINGUTILS_H_INCLUDED) */
