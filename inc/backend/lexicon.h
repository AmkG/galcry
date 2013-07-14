/*
inc/backend/lexicon.h - Alien Word Generator
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
#ifndef BACKEND_LEXICON_H_INCLUDED
#define BACKEND_LEXICON_H_INCLUDED

#include"stringutils.h"

#include"ustr.h"

void lexicon_init (void);
void lexicon_deinit (void);

/* Gets the language lexicons available.  */
void lexicon_get_languages (AUstr *languages);

/* Generates a random name from the selected lexicon.
   NOTE: since it uses random numbers, there is a small
   chance of duplicate names.  Caller is responsible for
   this.  */
void lexicon_generate (Ustr **, Ustr const *language);

/* Commands for main command loop.  */
void lexicon_lexicons_command (AUstr *command_line);
void lexicon_lexicon_command (AUstr *command_line);

#endif /* !defined (BACKEND_LEXICON_H_INCLUDED) */
