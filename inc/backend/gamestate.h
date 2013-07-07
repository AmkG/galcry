/*
inc/backend/gamestate.h - Component for handling game state.
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
#ifndef BACKEND_GAMESTATE_H_INCLUDED
#define BACKEND_GAMESTATE_H_INCLUDED

#include"stringutils.h"

/* Initialize/shutdown gamestate system.  */
void gamestate_initialize (void);
void gamestate_shutdown (void);

/* Clear, Save, Load.  */
void gamestate_clear (void);
void gamestate_save (Ustr const *filename);
void gamestate_load (Ustr const *filename);

/* Set and lookup global variables.  */
void gamestate_global_get (char const *var, Ustr ** value);
void gamestate_global_put (char const *var, Ustr const *value);
void gamestate_global_aget (char const *var, AUstr *value);
void gamestate_global_aput (char const *var, AUstr const *value);

/* Set and get number of countries.  */
void gamestate_countries_get (unsigned int *value);
void gamestate_countries_put (unsigned int value);

/* Set and lookup country variables.  */
void gamestate_country_get (unsigned int country,
			    char const *var, Ustr ** value);
void gamestate_country_put (unsigned int country,
			    char const *var, Ustr const *value);
void gamestate_country_aget (unsigned int country,
			     char const *var, AUstr *value);
void gamestate_country_aput (unsigned int country,
			     char const *var, AUstr const *value);

#endif /* !defined (BACKEND_GAMESTATE_H_INCLUDED) */
