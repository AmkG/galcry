/*
src/stringutils/wordsplit.c - word-splitting algorithm
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

static bool
digit (char c)
{
  return ('0' <= c) && (c <= '9');
}

void
wordsplit (AUstr * parray, Ustr const *string)
{
  char const *cstring;
  size_t off;
  size_t len;
  size_t sublen;
  Ustr *word;
  bool finish_word;

  austr_clear (parray);
  cstring = ustr_cstr (string);
  off = 0;
  len = ustr_len (string);
  word = USTR ("");

  for (;;)
    {
      off += ustr_spn_cstr_fwd (string, off, " \t\n\r\v\f");
      if (off == len)
	{
	  ustr_sc_free (&word);
	  return;
	}

      if (!ustr_set_empty (&word))
	abort ();

      finish_word = false;
      while (!finish_word)
	{
	  sublen = ustr_cspn_cstr_fwd (string, off, "\" \t\n\r\v\f");
	  if (!ustr_add_subustr (&word, string, off + 1, sublen))
	    abort ();
	  off += sublen;

	  if (off < len && cstring[off] == '\"')
	    {
	      bool finish_quote;

	      ++off;
	      finish_quote = false;
	      while (!finish_quote)
		{
		  sublen = ustr_cspn_cstr_fwd (string, off, "\"\\");
		  if (!ustr_add_subustr (&word, string, off + 1, sublen))
		    abort ();
		  off += sublen;

		  if (off < len && cstring[off] == '\"')
		    {
		      ++off;
		      finish_quote = true;
		    }
		  else if (off < len)
		    {
		      /* \ code in quotes, skip backslash.  */
		      ++off;
		      if (off < len)
			{
			  char c[2];
			  c[0] = cstring[off];
			  c[1] = 0;
			  switch (c[0])
			    {
			    case '0':
			    case '1':
			    case '2':
			    case '3':
			    case '4':
			    case '5':
			    case '6':
			    case '7':
			    case '8':
			    case '9':
			      {
				size_t i;
				c[0] = 0;
				for (i = 0;
				     (digit (cstring[off + i])
				      && i < 3
				      && off + i < len);
				     ++i)
				  {
				    c[0] = c[0] * 8;
				    c[0] += cstring[off + i] - '0';
				  }
				off += i;
			      }
			      break;
			    /* *INDENT-OFF* */
			    case 'a': c[0] = '\a'; ++off; break;
			    case 'b': c[0] = '\b'; ++off; break;
			    case 'f': c[0] = '\f'; ++off; break;
			    case 'n': c[0] = '\n'; ++off; break;
			    case 'r': c[0] = '\r'; ++off; break;
			    case 't': c[0] = '\t'; ++off; break;
			    case 'v': c[0] = '\v'; ++off; break;
			    default:  c[0] = c[0]; ++off; break;
			    /* *INDENT-ON* */
			    }
			  if (!ustr_add_cstr (&word, c))
			    abort ();
			}
		    }

		  if (off == len)
		    finish_quote = true;
		}
	    }
	  else
	    finish_word = true;
	}

      austr_push (parray, word);
    }
}
