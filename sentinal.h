/*
 * sentinal.h
 *
 * Copyright (c) 2021, 2022 jjb
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found
 * in the root directory of this source tree.
 */

#ifndef _SYS_TYPES_H
# include <sys/types.h>
#endif

#ifndef PCRE2_H_IDEMPOTENT_GUARD
# define	PCRE2_CODE_UNIT_WIDTH	8
# include <pcre2.h>
#endif

#define	TRUE		(short)1
#define	FALSE		(short)0
#define	MAXARGS		32
#define	MAXSECT		16							/* arbitrary, can be more */

#define	MAXFILES	128							/* max open files */

#ifndef PATH_MAX
# define	PATH_MAX	256
#endif

#ifndef	TASK_COMM_LEN
# define	TASK_COMM_LEN	16					/* task command name length */
#endif

#define	ONE_MINUTE	60							/* m */
#define	ONE_HOUR	(ONE_MINUTE * 60)			/* H or h */
#define	ONE_DAY		(ONE_HOUR * 24)				/* D or d */
#define	ONE_WEEK	(ONE_DAY * 7)				/* W or w */
#define	ONE_MONTH	(ONE_DAY * 30)				/* M */
#define	ONE_YEAR	(ONE_DAY * 365)				/* Y or y */

#define	ONE_KiB		1024						/* K or k */
#define	ONE_MiB		(ONE_KiB << 10)				/* M or m */
#define	ONE_GiB		(ONE_MiB << 10)				/* G or g */

#define	KiB(n)		((size_t) (n) >> 10)		/* convert bytes to KiB */
#define	MiB(n)		((size_t) (n) >> 20)		/* convert bytes to MiB */

#define	FIFOSIZ		(64L * ONE_MiB)				/* tunable, just a guess */

#define IS_NULL(s) !((s) && *(s))

/* postcmd tokens (1.3.0+) */

#define	_HOST_TOK	"%host"						/* hostname token */
#define	_PATH_TOK	"%path"						/* dirname (path) token */
#define	_FILE_TOK	"%file"						/* filename token */
#define	_SECT_TOK	"%sect"						/* INI section token */

/* deprecated postcmd tokens (pre-1.3.0) */

#define	_OLD_HOST_TOK	"%h"					/* hostname token */
#define	_OLD_DIR_TOK	"%p"					/* dirname (path) token */
#define	_OLD_FILE_TOK	"%n"					/* filename token */
#define	_OLD_SECT_TOK	"%t"					/* INI section token */

/* thread names */

#define	_DFS_THR	"dfs"						/* filesystem free space */
#define	_EXP_THR	"exp"						/* file expiration, retention */
#define	_SLM_THR	"slm"						/* simple log monitor */
#define	_WRK_THR	"wrk"						/* worker (log ingestion) thread */

/* execution environment */

#define	ENV		"/usr/bin/env"
#define	BASH	"/bin/bash"
#define	PATH	"/usr/bin:/usr/sbin"

struct thread_info {
	char   *ti_section;							/* section name */
	char   *ti_command;							/* thread command */
	int     ti_argc;							/* number of args in command */
	char   *ti_path;							/* path to command */
	char   *ti_argv[MAXARGS];					/* args in command */
	char   *ti_dirname;							/* FIFO directory name */
	char   *ti_subdirs;							/* subdirectory recursion flag */
	char   *ti_pipename;						/* FIFO name */
	char   *ti_template;						/* file template */
	char   *ti_pcrestr;							/* pcre for file match */
	pcre2_code *ti_pcrecmp;						/* compiled pcre */
	char   *ti_filename;						/* output file name */
	pid_t   ti_pid;								/* thread pid */
	uid_t   ti_uid;								/* thread uid */
	gid_t   ti_gid;								/* thread gid */
	int     ti_wfd;								/* worker stdin or EOF */
	int     ti_sig;								/* signal number received */
	off_t   ti_loglimit;						/* logfile rotate size */
	long double ti_diskfree;					/* desired percent blocks free */
	long double ti_inofree;						/* desired percent inodes free */
	int     ti_expire;							/* file expiration */
	int     ti_retmin;							/* file retention minimum */
	int     ti_retmax;							/* file retention maximum */
	char   *ti_terse;							/* notify file removal */
	char   *ti_postcmd;							/* command to run after log closes */
};

char   *convexpire(int, char *);
char   *findmnt(char *, char *);
char   *fullpath(char *, char *, char *);
char   *logname(char *, char *);
char   *threadname(char *, char *, char *);
gid_t   verifygid(char *);
int     logretention(char *);
int     oldestfile(struct thread_info *, short, char *, char *, time_t *);
int     postcmd(struct thread_info *, char *);
int     runcmd(int, char **, char **);
int     threadcheck(struct thread_info *, char *);
off_t   logsize(char *);
short   mylogfile(char *, pcre2_code *);
size_t  strlcat(char *, const char *, size_t);
size_t  strlcpy(char *, const char *, size_t);
uid_t   verifyuid(char *);
void    pcrecompile(struct thread_info *);
void    activethreads(struct thread_info *);
void    parentsignals(void);
void    rlimit(int);
void    strreplace(char *, char *, char *);
void   *dfsthread(void *);
void   *expthread(void *);
void   *slmthread(void *);
void   *workthread(void *);

/* vim: set tabstop=4 shiftwidth=4 noexpandtab: */
