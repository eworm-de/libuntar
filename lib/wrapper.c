/*
**  Copyright 1998-2003 University of Illinois Board of Trustees
**  Copyright 1998-2003 Mark D. Roth
**  All rights reserved.
**
**  wrapper.c - libtar high-level wrapper code
**
**  Mark D. Roth <roth@uiuc.edu>
**  Campus Information Technologies and Educational Services
**  University of Illinois at Urbana-Champaign
*/

#include <internal.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

int
tar_extract_glob(TAR *t, char *globname, char *prefix)
{
	char *filename;
	char buf[MAXPATHLEN];
	int i;

	while ((i = th_read(t)) == 0)
	{
		filename = th_get_pathname(t);
		if (fnmatch(globname, filename, FNM_PATHNAME | FNM_PERIOD))
		{
			if (TH_ISREG(t) && tar_skip_regfile(t))
				return -1;
			continue;
		}
		if (t->options & TAR_VERBOSE)
			th_print_long_ls(t);
		if (prefix != NULL)
			snprintf(buf, sizeof(buf), "%s/%s", prefix, filename);
		else
			strlcpy(buf, filename, sizeof(buf));
		if (tar_extract_file(t, buf) != 0)
			return -1;
	}

	return (i == 1 ? 0 : -1);
}


int
tar_extract_all(TAR *t, char *prefix)
{
	char *filename;
	char buf[MAXPATHLEN];
	int i;

#ifdef DEBUG
	printf("==> tar_extract_all(TAR *t, \"%s\")\n",
	       (prefix ? prefix : "(null)"));
#endif

	while ((i = th_read(t)) == 0)
	{
#ifdef DEBUG
		puts("    tar_extract_all(): calling th_get_pathname()");
#endif
		filename = th_get_pathname(t);
		if (t->options & TAR_VERBOSE)
			th_print_long_ls(t);
		if (prefix != NULL)
			snprintf(buf, sizeof(buf), "%s/%s", prefix, filename);
		else
			strlcpy(buf, filename, sizeof(buf));
#ifdef DEBUG
		printf("    tar_extract_all(): calling tar_extract_file(t, "
		       "\"%s\")\n", buf);
#endif
		if (tar_extract_file(t, buf) != 0)
			return -1;
	}

	return (i == 1 ? 0 : -1);
}

