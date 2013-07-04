/*
src/stringutils/mapping.c - Implementation of mappings.
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

#define BUCKETS 256

struct SList_s
{
  Ustr *key;
  AUstr value;
  struct SList_s *next;
};
typedef struct SList_s SList;

struct MappingImpl_s
{
  SList *buckets[BUCKETS];
};
typedef struct MappingImpl_s MappingImpl;

void
mapping_init (Mapping *pmapping)
{
  unsigned int i;
  pmapping->pimpl = malloc (sizeof (MappingImpl));
  if (!pmapping->pimpl)
    abort ();
  for (i = 0; i < BUCKETS; ++i)
    pmapping->pimpl->buckets[i] = 0;
}
void
mapping_deinit (Mapping *pmapping)
{
  SList *pt;
  SList *npt;
  unsigned int i;

  for (i = 0; i < BUCKETS; ++i)
    {
      for (pt = pmapping->pimpl->buckets[i]; pt; pt = npt)
	{
	  npt = pt->next;
	  ustr_sc_free (&pt->key);
	  austr_deinit (&pt->value);
	  free (pt);
	}
    }

  free (pmapping->pimpl);
  pmapping->pimpl = 0;
}

/* Internal function to look for the pointer
   to the SList that matches the given key.

   Note that it returns a pointer to a pointer
   to an SList.  That pointer is the one that
   gets inserted into or deleted from.  */
static SList **
lookup (Mapping const *pmapping, Ustr const *key)
{
  MappingImpl *pimpl;
  uint32_t hash;
  SList **rv;

  pimpl = pmapping->pimpl;
  hash = stringhash (key);
  rv = &pimpl->buckets[hash % BUCKETS];

  while (*rv)
    {
      if (ustr_cmp_eq (key, (*rv)->key))
	break;
      rv = &((*rv)->next);
    }

  return rv;
}

void
mapping_ins (Mapping *pmapping, Ustr const *key, AUstr const *val)
{
  SList **ins_pt;
  SList *slist;
  unsigned int i;

  ins_pt = lookup (pmapping, key);

  if (*ins_pt)
    slist = *ins_pt;
  else
    {
      slist = malloc (sizeof (SList));
      if (!slist)
	abort ();

      slist->key = ustr_dup_buf (ustr_cstr (key), ustr_len (key));
      austr_init (&slist->value);
      slist->next = 0;

      *ins_pt = slist;
    }

  austr_clear (&slist->value);
  for (i = 0; i < austr_length (val); ++i)
    austr_push (&slist->value, austr_i (val, i));
}

void
mapping_del (Mapping *pmapping, Ustr const *key)
{
  SList **del_pt;
  SList *slist;

  del_pt = lookup (pmapping, key);

  if (*del_pt)
    {
      slist = *del_pt;
      *del_pt = slist->next;

      ustr_sc_free (&slist->key);
      austr_deinit (&slist->value);
      free (slist);
    }
}


bool
mapping_find (Mapping const *pmapping, Ustr const *key, AUstr *value)
{
  SList **find_pt;

  find_pt = lookup (pmapping, key);

  if (*find_pt)
    {
      if (value)
	{
	  SList *slist;
	  unsigned int i;

	  slist = *find_pt;

	  austr_clear (value);
	  for (i = 0; i < austr_length (&slist->value); ++i)
	    austr_push (value, austr_i (&slist->value, i));
	}
      return true;
    }
  else
    return false;
}

void
mapping_keys (Mapping const *pmapping, AUstr *keys)
{
  unsigned int i;
  SList *pt;

  austr_clear (keys);
  for (i = 0; i < BUCKETS; ++i)
    for (pt = pmapping->pimpl->buckets[i]; pt; pt = pt->next)
      austr_push (keys, pt->key);
}
