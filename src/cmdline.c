/*
src/cmdline.c - Command line handling, implementation
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

#include"cmdline.h"

#include<string.h>

#ifndef PKGDATADIR
#define PKGDATADIR "."
#endif
#ifndef BINDIR
#define BINDIR "."
#endif

static Ustr *pkgdatadir;
static Ustr *bindir;

void
cmdline_init (int argc, char **argv,
	      char const *progname,
	      char const *copyright, char const *writtenby, char const *desc)
{
  int i;
  Ustr *opt;

  /* Initialize strings.  */
  pkgdatadir = ustr_dup_cstr (PKGDATADIR);
  bindir = ustr_dup_cstr (BINDIR);
  opt = ustr_dup_empty ();

  for (i = 1; i < argc; ++i)
    {
      if (!ustr_set_cstr (&opt, argv[i]))
	abort ();

      if (ustr_cmp_cstr_eq (opt, "--help"))
	{
	  printf ("Usage: %s [option]...\n", argv[0]);
	  printf ("\n%s\n\n", desc);

	  printf ("Options:\n");
	  /* *INDENT-OFF* */
	  printf (
"  --pkgdatadir=DIR    specify location of data files\n"
"  --bindir=DIR        specify location of executables\n"
	  );
	  printf ("\n");
	  printf (
"  --help              display this help and exit\n"
"  --version           output version information and exit\n"
	  );
	  /* *INDENT-ON* */
	  printf ("\n");
	  printf ("Report bugs to: <%s>\n", PACKAGE_BUGREPORT);

	  ustr_sc_free (&opt);
	  cmdline_deinit ();
	  exit (0);
	}
      else if (ustr_cmp_cstr_eq (opt, "--version"))
	{
	  printf ("%s (%s)\n", progname, PACKAGE_STRING);
	  printf ("%s\n", copyright);
	  printf ("This program is free software; "
		  "you are free to change and redistribute.\n");
	  printf ("There is NO WARRANTY, to the extent permitted by law.\n");
	  printf ("%s\n", writtenby);

	  ustr_sc_free (&opt);
	  cmdline_deinit ();
	  exit (0);
	}
      else if (ustr_cmp_prefix_cstr_eq (opt, "--pkgdatadir="))
	{
	  unsigned int off;
	  off = strlen ("--pkgdatadir=");
	  if (!ustr_set_subustr (&pkgdatadir, opt,
				 off + 1, ustr_len (opt) - off))
	    abort ();
	}
      else if (ustr_cmp_prefix_cstr_eq (opt, "--bindir="))
	{
	  unsigned int off;
	  off = strlen ("--bindir=");
	  if (!ustr_set_subustr (&bindir, opt,
				 off + 1, ustr_len (opt) - off))
	    abort ();
	}
      else
	{
	  printf ("%s: Unrecognized option `%s'.\n", argv[0], argv[i]);
	  printf ("Try `%s --help' for more information.\n", argv[0]);

	  ustr_sc_free (&opt);
	  cmdline_deinit ();
	  exit (1);
	}
    }

  ustr_sc_free (&opt);
}

void
cmdline_deinit (void)
{
  ustr_sc_free (&pkgdatadir);
  ustr_sc_free (&bindir);
}

Ustr const *
cmdline_pkgdatadir (void)
{
  return pkgdatadir;
}

Ustr const *
cmdline_bindir (void)
{
  return bindir;
}
