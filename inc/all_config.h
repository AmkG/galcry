/*
inc/all_config.h - Wrapper for config.h
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
#ifndef ALL_CONFIG_H_INCLUDED
#define ALL_CONFIG_H_INCLUDED

#if HAVE_CONFIG_H
#include"config.h"
#endif

/*-----------------------------------------------------------------------------
Boolean
-----------------------------------------------------------------------------*/

#if HAVE_STDBOOL_H
# include<stdbool.h>
#else /* !HAVE_STDBOOL_H */

# if !HAVE__BOOL
#  if defined(__cplusplus)
typedef bool _Bool;
#  else /* !defined(__cplusplus) */
#   define _Bool unsigned char
#  endif /* !defined(__cplusplus) */
# endif /* !HAVE__BOOL */

# define bool _Bool
# define false 0
# define true 1

#endif /* !HAVE_STDBOOL_H */

#endif
