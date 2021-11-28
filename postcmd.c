/*
 * postcmd.c
 * Run command after the log closes or rotates.
 *
 * Copyright (c) 2021 jjb
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found
 * in the root directory of this source tree.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sentinal.h"

#define	BASH	"/bin/bash"
#define	PATH	"/usr/bin:/usr/sbin"

extern struct utsname utsbuf;

void postcmd(struct thread_info *ti, char *filename)
{
	char   *home;
	char    cmdbuf[BUFSIZ];
	int     i;
	pid_t   pid;
	struct passwd *p;

	if(IS_NULL(ti->ti_postcmd))
		return;

	switch (pid = fork()) {

	case -1:
		fprintf(stderr, "%s: can't fork postcmd\n", ti->ti_section);
		return;

	case 0:
		if(geteuid() == (uid_t) 0) {
			setgid(ti->ti_gid);
			setuid(ti->ti_uid);
		}

		if(chdir(ti->ti_dirname) == -1)
			exit(EXIT_FAILURE);

		strlcpy(cmdbuf, ti->ti_postcmd, BUFSIZ);
		substrstr(cmdbuf, _HOST_TOK, utsbuf.nodename);
		substrstr(cmdbuf, _DIR_TOK, ti->ti_dirname);
		substrstr(cmdbuf, _FILE_TOK, filename);
		fprintf(stderr, "%s: %s\n", ti->ti_section, cmdbuf);
#if 0
		fprintf(stderr, "%s: postcmd pid: %d\n", ti->ti_section, getpid());
#endif

		/* close parent's and unused fds */

		for(i = 3; i < 20; i++)
			close(i);

		nice(1);

		home = (p = getpwuid(ti->ti_uid)) ? p->pw_dir : "/tmp";
		setenv("HOME", home, TRUE);
		setenv("PATH", PATH, TRUE);
		setenv("SHELL", BASH, TRUE);

		execl("/usr/bin/env", "-iS", BASH, "--noprofile", "-l", "-c",
			  cmdbuf, (char *)NULL);

		exit(EXIT_FAILURE);

	default:
		return;
	}
}

/* vim: set tabstop=4 shiftwidth=4 noexpandtab: */