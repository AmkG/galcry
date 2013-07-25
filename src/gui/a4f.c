/*
src/gui/a4f.c - TTF anti-aliased font renderer
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

#include"gui/a4f.h"

#include<allegro.h>
#include<string.h>

/* FreeType2 inclusion.  */
#include<ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H

/*-----------------------------------------------------------------------------
FreeType2 Errors
-----------------------------------------------------------------------------*/

typedef struct ERRMSG_s
{
  int error_code;
  char const *message;
} ERRMSG;
#undef __FTERRORS_H__
#define FT_ERRORDEF(e,v,s)  {e, s},
#define FT_ERROR_START_LIST {
#define FT_ERROR_END_LIST   {0, 0} }

/* *INDENT-OFF* */
static ERRMSG const error_messages[] =
#include FT_ERRORS_H
;
/* *INDENT-ON* */

static void
set_allegro_error (int err)
{
  ERRMSG const *p;
  for (p = error_messages; p->error_code && p->message; ++p)
    {
      if (p->error_code == err)
	{
	  strncpy (allegro_error, p->message, ALLEGRO_ERROR_SIZE);
	  allegro_error[ALLEGRO_ERROR_SIZE - 1] = 0;
	  return;
	}
    }
  strncpy (allegro_error, "unknown freetype2 error", ALLEGRO_ERROR_SIZE);
  allegro_error[ALLEGRO_ERROR_SIZE - 1] = 0;
}

static FT_Error
check_error (FT_Error e)
{
  if (e != 0)
    set_allegro_error (e);
  return e;
}

/*-----------------------------------------------------------------------------
FreeType Globals
-----------------------------------------------------------------------------*/

static FT_Library lib;
static FTC_Manager face_cache;
static FTC_CMapCache cmap_cache;
static FTC_ImageCache image_cache;

/*-----------------------------------------------------------------------------
A4F Font Face
-----------------------------------------------------------------------------*/

typedef struct A4F_Face_s
{
  char const *filename;
  unsigned int face_num;

  unsigned int refcount;
} *A4F_Face;

static FT_Error
font_loader (FTC_FaceID face_id, FT_Library lib, FT_Pointer _, FT_Face* aface)
{
  A4F_Face font;

  font = (A4F_Face) (void *) face_id;

  return FT_New_Face (lib, font->filename, font->face_num, aface);
}

static A4F_Face
face_new (char const *filename, unsigned int face_num)
{
  A4F_Face rv;

  rv = malloc (sizeof (struct A4F_Face_s));
  if (!rv)
    return 0;
  filename = strdup (filename);
  if (!filename)
    {
      free (rv);
      return 0;
    }

  rv->filename = filename;
  rv->face_num = face_num;
  rv->refcount = 1;

  return rv;
}
static A4F_Face
face_ref (A4F_Face p)
{
  if (p)
    ++p->refcount;
  return p;
}
static void
face_delete (A4F_Face p)
{
  if (p)
    {
      --p->refcount;
      if (p->refcount == 0)
	{
	  free ((void *) p->filename);
	  free (p);
	}
    }
}

/*-----------------------------------------------------------------------------
A4F Fonts
-----------------------------------------------------------------------------*/

struct A4F_s
{
  A4F_Face face;
  bool has_kerning;
  unsigned int pixel_height;
};

/*-----------------------------------------------------------------------------
Library startup/shutdown
-----------------------------------------------------------------------------*/

int
a4f_init (void)
{
  FT_Error err;

  if (0 != check_error (FT_Init_FreeType (&lib)))
    return -1;
  if (0 != check_error (FTC_Manager_New (lib, 0, 0, 0, &font_loader, 0,
			&face_cache)))
    {
      (void) FT_Done_FreeType (lib);
      return -1;
    }
  if (0 != check_error (FTC_CMapCache_New (face_cache, &cmap_cache)))
    {
      (void) FTC_Manager_Done (face_cache);
      (void) FT_Done_FreeType (lib);
      return -1;
    }
  if (0 != check_error (FTC_ImageCache_New (face_cache, &image_cache)))
    {
      (void) FTC_Manager_Done (face_cache);
      (void) FT_Done_FreeType (lib);
      return -1;
    }

  return 0;
}

void
a4f_shutdown (void)
{
  (void) FTC_Manager_Done (face_cache);
  (void) FT_Done_FreeType (lib);
}

/*-----------------------------------------------------------------------------
Handling A4F Fonts
-----------------------------------------------------------------------------*/

unsigned int
a4f_file_faces (char const *filename)
{
  FT_Open_Args args;
  FT_Face f;
  unsigned int rv;

  args.flags = FT_OPEN_PATHNAME;
  args.pathname = strdup (filename);
  if (!args.pathname)
    return 0;

  if (0 != check_error (FT_Open_Face (lib, &args, -1, &f)))
    return 0;

  rv = f->num_faces;
  (void) FT_Done_Face (f);
  free (args.pathname);

  return rv;
}

A4F
a4f_file_load (char const *filename, unsigned int face_num)
{
  A4F rv;
  FT_Face face;

  rv = malloc (sizeof (struct A4F_s));
  if (!rv)
    return 0;

  rv->face = face_new (filename, face_num);
  if (!rv->face)
    {
      free (rv);
      return 0;
    }

  if (0 != check_error (FTC_Manager_LookupFace (face_cache,
						(FTC_FaceID) rv->face,
						&face)))
    {
      (void) FTC_Manager_RemoveFaceID (face_cache, (FTC_FaceID) rv->face);
      free (rv->face);
      free (rv);
      return 0;
    }

  rv->has_kerning = FT_HAS_KERNING (face);
  rv->pixel_height = 16;

  return rv;
}

A4F
a4f_copy (A4F font)
{
  A4F rv;

  rv = malloc (sizeof (struct A4F_s));
  if (!rv)
    return 0;

  rv->face = face_ref (font->face);
  rv->has_kerning = font->has_kerning;
  rv->pixel_height = font->pixel_height;

  return rv;
}

void
a4f_destroy (A4F font)
{
  face_delete (font->face);
  free (font);
}

/* Set the desired height of the font in pixels.  */
void
a4f_set_height_pixels (A4F font, unsigned int height)
{
  font->pixel_height = height;
}

/*-----------------------------------------------------------------------------
Render FreeType 2 Bitmap to Allegro 4 Bitmap
-----------------------------------------------------------------------------*/

/* Trans blenders from allegro 4 internals.  */
unsigned long _blender_trans15 (unsigned long, unsigned long, unsigned long);
unsigned long _blender_trans16 (unsigned long, unsigned long, unsigned long);
unsigned long _blender_trans24 (unsigned long, unsigned long, unsigned long);
static unsigned long
_blender_trans32 (unsigned long x, unsigned long y, unsigned long n)
{
  return _blender_trans24 (x & 16777215, y & 16777215, n);
}

static void
ft2_to_a4_blit (BITMAP *target, FT_Bitmap *psrc,
		int x, int y, int color)
{
  unsigned char *dat;
  unsigned int apitch;
  int dir;
  short max;
  int target_depth;

  int x1, y1, x2, y2;

  unsigned int r;
  unsigned int c;

  if (psrc->pixel_mode != FT_PIXEL_MODE_GRAY)
    abort ();
  if (bitmap_color_depth (target) == 8)
    abort ();

  if (psrc->pitch < 0)
    {
      dir = -1;
      apitch = -psrc->pitch;
    }
  else
    {
      dir = 1;
      apitch = psrc->pitch;
    }
  dat = (unsigned char *) psrc->buffer;
  max = psrc->num_grays - 1;

  target_depth = bitmap_color_depth (target);
  if (get_clip_state (target))
    {
      x1 = target->cl;
      y1 = target->ct;
      x2 = target->cr;
      y2 = target->cb;
    }
  else
    {
      x1 = 0;
      y1 = 0;
      x2 = target->w;
      y2 = target->h;
    }

#define RENDER_LOOP(bpp)\
  else if (target_depth == (bpp))\
    {\
      if (dir < 0)\
	y = y + psrc->rows;\
      for (r = 0; r < psrc->rows; ++r, y += dir, dat += apitch)\
	{\
	  unsigned char *ptr;\
	  ptr = dat;\
	  for (c = 0; c < psrc->width; ++c, ++ptr)\
	    {\
	      unsigned char v;\
	      int pix;\
	      if (!(x1 < x + c && x + c < x2 && y1 < y && y < y2))\
		continue;\
	      if (*ptr == 0)\
		continue;\
	      if (*ptr == max)\
		{\
		  _putpixel ## bpp (target, x + c, y, color);\
		  continue;\
		}\
	      v = *ptr;\
	      if (max != 255)\
		v = (((unsigned long) v) * 255) / max;\
	      pix = _getpixel ## bpp (target, x + c, y);\
	      pix = _blender_trans ## bpp (color, pix, v);\
	      _putpixel ## bpp (target, x + c, y, pix);\
	    }\
	}\
    }


  /* *INDENT-OFF* */
  if (0)
    { }
  RENDER_LOOP(32)
  RENDER_LOOP(24)
  RENDER_LOOP(16)
  RENDER_LOOP(15)
  /* *INDENT-ON* */
}

/*-----------------------------------------------------------------------------
Render Text
-----------------------------------------------------------------------------*/

/* Advances a UTF-8 character pointer.  */
static void
u8_adv (char const **ptext)
{
  unsigned char c;
  c = **ptext;
  if (c < 128)
    ++*ptext;
  else if (c > 252)
    *ptext += 6;
  else if (c > 248)
    *ptext += 5;
  else if (c > 240)
    *ptext += 4;
  else if (c > 224)
    *ptext += 3;
  else
    *ptext += 2;
}
static unsigned int
u8_rdb (unsigned char c)
{
  return c & 63;
}
/* Reads a UTF-8 character.  */
static unsigned int
u8_read (char const *text)
{
  unsigned char c;

  c = *text;
  if (c < 128)
    return c;
  else if (c > 252)
    return ((c & 1) << (6 * 5))
	   + (u8_rdb (text[1]) << (6 * 4))
	   + (u8_rdb (text[2]) << (6 * 3))
	   + (u8_rdb (text[3]) << (6 * 2))
	   + (u8_rdb (text[4]) << (6 * 1))
	   + (u8_rdb (text[5]));
  else if (c > 248)
    return ((c & 3) << (6 * 4))
	   + (u8_rdb (text[1]) << (6 * 3))
	   + (u8_rdb (text[2]) << (6 * 2))
	   + (u8_rdb (text[3]) << (6 * 1))
	   + (u8_rdb (text[4]));
  else if (c > 240)
    return ((c & 7) << (6 * 3))
	   + (u8_rdb (text[1]) << (6 * 2))
	   + (u8_rdb (text[2]) << (6 * 1))
	   + (u8_rdb (text[3]));
  else if (c > 224)
    return ((c & 15) << (6 * 2))
	   + (u8_rdb (text[1]) << (6 * 1))
	   + (u8_rdb (text[2]));
  else
    return ((c & 31) << (6 * 1))
	   + (u8_rdb (text[1]));
}

int
a4f_textout (BITMAP *target, int x, int y, int color,
	     A4F font, char const *text)
{
  unsigned int step_x;
  unsigned int c;
  bool has_kerning;
  bool first_char;
  FTC_FaceID face_id;
  FTC_ImageTypeRec imagetype;

  FT_Face face;

  FT_UInt previous;
  FT_UInt cur;
  FT_Glyph glyph;
  FT_BitmapGlyph bmpglyph;

  step_x = 0;
  has_kerning = font->has_kerning;
  first_char = true;
  face_id = (FTC_FaceID) font->face;
  imagetype.face_id = face_id;
  imagetype.width = 0;
  imagetype.height = font->pixel_height;
  imagetype.flags = FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL;

  if (0 != check_error (FTC_Manager_LookupFace (face_cache, face_id, &face)))
    return -1;
  (void) FT_Reference_Face (face);

  for (; *text; u8_adv(&text), first_char = false, previous = cur)
    {
      c = u8_read (text);
      cur = FTC_CMapCache_Lookup (cmap_cache, face_id, -1, c);

      if (has_kerning && !first_char && cur && previous)
	{
	  FT_Vector delta;

	  (void) FT_Get_Kerning (face, previous, cur, FT_KERNING_DEFAULT,
				 &delta);

	  step_x += delta.x;
	}

       if (0 != check_error (FTC_ImageCache_Lookup (image_cache,
						    &imagetype,
						    cur,
						    &glyph,
						    0)))
	{
	  (void) FT_Done_Face (face);
	  return -1;
	}

      bmpglyph = (FT_BitmapGlyph) glyph;
      ft2_to_a4_blit (target, &bmpglyph->bitmap,
		x + ((step_x + 31) >> 6) + bmpglyph->left,
		y - bmpglyph->top, color);
      step_x += glyph->advance.x >> 10;
    }

  (void) FT_Done_Face (face);
  return 0;
}
