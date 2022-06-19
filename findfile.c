/*
 * findfile.c
 * Check dir and possibly subdirs for files matching pcrestr (pcrecmp).
 * Return the number of files matching pcrestr.
 *
 * In cases when we are interested in file size with no other conditions,
 * stop searching after the first match.
 *
 * In cases including other conditions (dir sizes, fs usage, retention),
 * search for the oldest file.
 *
 * Copyright (c) 2021, 2022 jjb
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found
 * in the root directory of this source tree.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "sentinal.h"

long findfile(struct thread_info *ti, short top, char *dir, struct dir_info *di)
{
	DIR    *dirp;
	char    filename[PATH_MAX];
	long    subfound;								/* matching files in subdir */
	short   anyfound;								/* existing files flag */
	short   exponly = TRUE;							/* conditional search */
	static long matches;							/* matching files */
	struct dirent *dp;
	struct stat stbuf;
	time_t  curtime;

	if((dirp = opendir(dir)) == NULL)
		return (0);

	rewinddir(dirp);

	if(top) {										/* reset */
		*di->di_file = '\0';
		di->di_time = di->di_size = di->di_bytes = 0;
		anyfound = matches = FALSE;
	}

	if(ti->ti_dirlimit || ti->ti_diskfree || ti->ti_inofree ||
	   ti->ti_retmin || ti->ti_retmax) {
		/*
		 * if one of these conditions is set, search for the oldest file,
		 * else any regex-matched file qualifies
		 */
		exponly = FALSE;
	}

	time(&curtime);

	while(dp = readdir(dirp)) {
		if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
			continue;

		fullpath(dir, dp->d_name, filename);

		if(lstat(filename, &stbuf) == -1)
			continue;

		if(S_ISLNK(stbuf.st_mode)) {
			anyfound = TRUE;
			continue;
		}

		if(S_ISDIR(stbuf.st_mode) && ti->ti_subdirs) {
			/* search subdirectory */

			subfound = findfile(ti, FALSE, filename, di);

			if(subfound == EOF)						/* empty dir removed */
				continue;

			anyfound = TRUE;

			if(exponly == FALSE)					/* continue searching */
				continue;
		}

		anyfound = TRUE;

		if(!S_ISREG(stbuf.st_mode))
			continue;

		if(!mylogfile(dp->d_name, ti->ti_pcrecmp))
			continue;

		/* match */

		if(exponly == TRUE) {
			/* if expired, remove now and continue searching */
			/* when loglimit and expire are given, both conditions must be true */

			if(stbuf.st_mtim.tv_sec + ti->ti_expire < curtime)
				if(ti->ti_loglimit == 0 || stbuf.st_size > ti->ti_loglimit)
					rmfile(ti, filename, "expired");

			continue;
		}

		if(ti->ti_dirlimit)							/* request total size of files found */
			di->di_bytes += stbuf.st_size;

		/* save the oldest file */

		if(di->di_time == 0 || stbuf.st_mtim.tv_sec < di->di_time) {
			strlcpy(di->di_file, filename, PATH_MAX);
			di->di_time = stbuf.st_mtim.tv_sec;
			di->di_size = stbuf.st_size;
		}

		matches++;
	}

	closedir(dirp);

	if(anyfound == FALSE && !top) {					/* ok to remove */
		rmfile(ti, dir, "rmdir");
		return (EOF);
	}

	return (matches);
}

/* vim: set tabstop=4 shiftwidth=4 noexpandtab: */
