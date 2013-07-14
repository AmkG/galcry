/*
src/backend/lexicon.c - Alien Word Generator
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

#include"backend/lexicon.h"
#include"cmdline.h"
#include"stringutils.h"

#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* Lexicon.  */
typedef struct Lexicon_s
{
  /* Index 0=string start/end, Index 1 = A, etc.  */
  unsigned int histogram3[27][27][27];
  unsigned int histogram2[27][27];
} Lexicon;

/* List of lexicons.  */
typedef struct LexiconList_s
{
  Ustr *name;
  Lexicon *lexicon;
  struct LexiconList_s *next;
} LexiconList;

static LexiconList *lexicon_list = 0;

void
lexicon_init (void)
{
  Ustr *lexicons_path;
  FILE *lexicons;
  Ustr *line;
  AUstr words;

  lexicons_path = ustr_dup (cmdline_pkgdatadir ());
  if (!lexicons_path)
    abort ();
  if (!ustr_add_cstr (&lexicons_path, "/lexicons"))
    abort ();
  line = ustr_dup_empty ();
  if (!line)
    abort ();
  austr_init (&words);

  lexicons = fopen (ustr_cstr (lexicons_path), "r");
  if (!lexicons)
    abort ();

  lexicon_list = 0;
  while (errno = 0, ustr_sc_del (&line), ustr_io_getline (&line, lexicons))
    {
      unsigned int i;
      wordsplit (&words, line);
      for (i = 0; i < austr_length (&words); ++i)
	{
	  LexiconList *nlist;
	  nlist = malloc (sizeof (LexiconList));
	  if (!nlist)
	    abort ();
	  nlist->name = ustr_dup (austr_i (&words, i));
	  nlist->lexicon = 0;
	  nlist->next = lexicon_list;
	  lexicon_list = nlist;
	}
    }
  if (errno != 0)
    {
      perror ("galcry-backend, lexicons");
      fclose (lexicons);
      abort ();
    }

  fclose (lexicons);
  austr_deinit (&words);
  ustr_sc_free (&lexicons_path);
}

void
lexicon_deinit (void)
{
  while (lexicon_list)
    {
      LexiconList *n;
      n = lexicon_list->next;
      ustr_sc_free (&lexicon_list->name);
      if (lexicon_list->lexicon)
	free (lexicon_list->lexicon);
      free (lexicon_list);
      lexicon_list = n;
    }
}

void
lexicon_get_languages (AUstr *languages)
{
  LexiconList *n;

  austr_clear (languages);
  for (n = lexicon_list; n; n = n->next)
    austr_push (languages, n->name);
}

/* Learns a new lexicon.  */
static Lexicon *
learn (Ustr const *fname)
{
  Lexicon *rv;
  AUstr words;
  Ustr *line;
  FILE *lexicon;

  /* Initialize variables.  */
  rv = malloc (sizeof (Lexicon));
  if (!rv)
    abort ();
  austr_init (&words);
  line = ustr_dup_empty ();
  if (!line)
    abort ();
  lexicon = fopen (ustr_cstr (fname), "r");
  if (!lexicon)
    {
      perror ("galcry-backend, lexicon, learn");
      abort ();
    }

  /* Clear histogram.  */
  memset ((void *) rv, 0, sizeof (Lexicon));

  /* Learn words.  */
  while (errno = 0, ustr_sc_del (&line), ustr_io_getline (&line, lexicon))
    {
      unsigned int i;

      /* Skip comment lines.  */
      if (ustr_cstr (line)[0] == '#')
	continue;

      wordsplit (&words, line);
      for (i = 0; i < austr_length (&words); ++i)
	{
	  unsigned int l;
	  char const *s;

	  l = ustr_len (austr_i (&words, i));
	  if (l < 2)
	    continue;
	  s = ustr_cstr (austr_i (&words, i));

	  {
	    unsigned int i;
	    unsigned int c0, c1, c2;
	    c0 = 0;
	    c1 = (s[0] % 32) % 27;
	    ++rv->histogram2[c0][c1];
	    for (i = 1; i < l + 1; ++i)
	      {
		c2 = (s[i] % 32) % 27;
		++rv->histogram3[c0][c1][c2];
		++rv->histogram2[c1][c2];
		c0 = c1;
		c1 = c2;
	      }
	  }
	}
    }
  if (errno != 0)
    {
      perror ("galcry-backend, lexicon, learn, read");
      abort ();
    }

  fclose (lexicon);
  ustr_sc_free (&line);
  austr_deinit (&words);
  return rv;
}

void
lexicon_generate (Ustr **s, Ustr const *language)
{
  LexiconList *sp;
  char ch[2];

  ustr_sc_del (s);
  ch[0] = 0;
  ch[1] = 0;

  /* Linear search.  Simple and we don't expect dozens of lexicons
     anyway.  */
  sp = lexicon_list;
  while (sp && !ustr_cmp_eq (language, sp->name))
    sp = sp->next;
  if (!sp)
    return;

  /* Load lexicon if not existent yet.  */
  if (!sp->lexicon)
    {
      Ustr *path;

      path = ustr_dup (cmdline_pkgdatadir ());
      if (!path)
	abort ();
      if (!ustr_add_cstr (&path, "/lexicon/"))
	abort ();
      if (!ustr_add (&path, sp->name))
	abort ();

      sp->lexicon = learn (path);

      ustr_sc_free (&path);
    }

  /* Generate.  */
  {
    unsigned int c0, c1, c2;
    unsigned long int sel;
    unsigned long int lim;
    unsigned int i;
    Lexicon *l;

    c0 = 0;
    l = sp->lexicon;

    /* Generate first char.  */
    for (i = 1, lim = 0; i < 27; ++i)
      lim += l->histogram2[0][i];
    /* Random select.  */
    sel = random () % lim;
    /* Find histogram area that random selection got in.  */
    for (c1 = 1; sel >= l->histogram2[0][c1]; ++c1)
      sel -= l->histogram2[0][c1];
    /* Add random selection.  */
    ch[0] = 96 + c1;
    if (!ustr_add_cstr (s, ch))
      abort ();

    /* Generate subsequent chars.  */
    do
      {
	/* Random select.  */
	sel = random () % l->histogram2[c0][c1];
	for (c2 = 0; sel >=l->histogram3[c0][c1][c2]; ++c2)
	  sel -= l->histogram3[c0][c1][c2];
	if (c2 != 0)
	  {
	    ch[0] = 96 + c2;
	    if (!ustr_add_cstr (s, ch))
	      abort ();
	  }
	c0 = c1;
	c1 = c2;
      }
    while (c2 != 0);
  }
}

void
lexicon_lexicons_command (AUstr *command_line)
{
  AUstr lexes;
  unsigned int i;

  austr_init (&lexes);
  
  lexicon_get_languages (&lexes);

  printf ("OK ");
  for (i = 0; i < austr_length (&lexes); ++i)
    printf ("%s ", ustr_cstr (austr_i (&lexes, i)));
  printf ("\n");

  austr_deinit (&lexes);
}

void
lexicon_lexicon_command (AUstr *command_line)
{
  Ustr *word;

  if (austr_length (command_line) < 2)
    printf ("NG \"%s\"\n", "No lexicon specified.");

  word = ustr_dup_empty ();
  if (!word)
    {
      printf ("NG \"%s\"\n", "Out of memory.");
      return;
    }

  lexicon_generate (&word, austr_i (command_line, 1));

  if (ustr_len (word) == 0)
    printf ("NG \"%s\"\n", "Lexicon not found.");
  else
    printf ("OK %s\n", ustr_cstr (word));

  ustr_sc_del (&word);
}
