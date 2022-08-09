/*
 * threadcheck.c
 * Considering the config data given, test if threads should run.
 *
 * Copyright (c) 2021, 2022 jjb
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found
 * in the root directory of this source tree.
 */

#include <stdio.h>
#include <string.h>
#include "sentinal.h"

#define	THRCHECK(ti,n)	threadcheck(ti, n) ? "true" : "false"

int threadcheck(struct thread_info *ti, char *tname)
{
	short   pass = FALSE;

	if(strcmp(tname, _DFS_THR) == 0)				/* filesystem free space */
		pass = ti->ti_pcrecmp && (ti->ti_diskfree || ti->ti_inofree);

	else if(strcmp(tname, _EXP_THR) == 0)			/* file expiration, retention, dirlimit */
		pass = ti->ti_pcrecmp && (ti->ti_dirlimit || ti->ti_expire || ti->ti_retmax);

	else if(strcmp(tname, _SLM_THR) == 0)			/* simple log monitor */
		pass = IS_NULL(ti->ti_command) &&
			NOT_NULL(ti->ti_template) && NOT_NULL(ti->ti_postcmd) && ti->ti_rotatesiz;

	else if(strcmp(tname, _WRK_THR) == 0)			/* worker (log ingestion) thread */
		pass = ti->ti_argc && NOT_NULL(ti->ti_pipename) && NOT_NULL(ti->ti_template);

	return (pass);
}

void activethreads(struct thread_info *ti)
{
	fprintf(stdout, "# threads");
	fprintf(stdout, "   %s: %s", _DFS_THR, THRCHECK(ti, _DFS_THR));
	fprintf(stdout, "   %s: %s", _EXP_THR, THRCHECK(ti, _EXP_THR));
	fprintf(stdout, "   %s: %s", _SLM_THR, THRCHECK(ti, _SLM_THR));
	fprintf(stdout, "   %s: %s", _WRK_THR, THRCHECK(ti, _WRK_THR));
	fprintf(stdout, "\n");
}

/* vim: set tabstop=4 shiftwidth=4 noexpandtab: */
