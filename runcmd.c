/*
 * runcmd.c
 * Create a command in zargv[] for execv().
 *
 * Copyright (c) 2021 jjb
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found
 * in the root directory of this source tree.
 */

#include <string.h>
#include "sentinal.h"
#include "basename.h"

static char *compprogs[] = {
	"bzip2",
	"gzip",
	"pbzip2",
	"zstd"
};

static int ncprogs = (sizeof(compprogs) / sizeof(compprogs[0]));

int runcmd(int argc, char *argv[], char *zargv[])
{
	int     i;
	int     n = 0;

	zargv[n++] = base(argv[0]);

	/* sorry, we're going to add/enforce -f for compression programs */

	for(i = 0; i < ncprogs; i++)
		if(strcmp(zargv[0], compprogs[i]) == 0) {
			zargv[n++] = "-f";
			break;
		}

	/* copy the remaining command line options from the INI file */

	for(i = 1; i < argc; i++)
		if(n < (MAXARGS - 1))
			zargv[n++] = argv[i];

	zargv[n] = (char *)NULL;
	return (n);
}

/* vim: set tabstop=4 shiftwidth=4 noexpandtab: */
