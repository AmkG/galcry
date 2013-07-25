/*
inc/gui/a4f.h - TTF anti-aliased font renderer
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
#ifndef A4F_H_INCLUDED
#define A4F_H_INCLUDED

/* Declare here so that ordering of #include<allegro.h> doesn't matter.  */
struct BITMAP;

/* Initialize and shutdown the a4f library.  Initialization returns 0 on
success, non-zero on failure.  */
int a4f_init (void);
void a4f_shutdown (void);

struct A4F_s;
typedef struct A4F_s *A4F;

/*-----------------------------------------------------------------------------
Font Construction and Destruction
-----------------------------------------------------------------------------*/

/* Get the number of font faces in the given font file.  Return 0 if
   the file cannot be loaded.  */
unsigned int a4f_file_faces (char const *filename);
/* Construct a new font from the given font file, selecting the
   face indexed by FACE_NUM.  */
A4F a4f_file_load (char const *filename, unsigned int face_num);
/* Create a copy of the font.  The copy's settings are changed independently
   of the original.  */
A4F a4f_copy (A4F font);
/* Destroy a font.  */
void a4f_destroy (A4F font);

/*-----------------------------------------------------------------------------
Font Settings
-----------------------------------------------------------------------------*/

/* Set the desired height of the font in pixels.  */
void a4f_set_height_pixels (A4F font, unsigned int );

/*-----------------------------------------------------------------------------
Font Extent
-----------------------------------------------------------------------------*/

/* Get the size, in pixels, of the given TEXT as rendered in the FONT.
   The TEXT is assumed to be UTF-8 always, regardless of platform.  */
void a4f_get_text_extent (unsigned int *pw, unsigned int *ph,
			  A4F font, char const *text);
/* Get the width of the given CHAR1, assuming that the next character is
   CHAR2.  This automatically considers kerning.  */
unsigned int a4f_get_char_width (A4F font,
				 unsigned int char1, unsigned int char2);

/*-----------------------------------------------------------------------------
Render
-----------------------------------------------------------------------------*/

/* Render the specified TEXT in the specified FONT into the specified BITMAP
   at the given X and Y coordinates.  The TEXT is assumed to be UTF-8
   always.  Output is always "see-through".  Return 0 on success, non-zero on
   failure.  */
int a4f_textout (struct BITMAP *target,
		 int x, int y,
		 int color,
		 A4F font, char const *text);

#endif /* !defined (A4F_H_INCLUDED) */
