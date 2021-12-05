# Description of INI files

INI files must contain a single Global section, and 1 to 16 Log sections.

## Global Section Keys

    pidfile:  process id file, absolute path, required

## Log Section Keys

    command:  command line to run
              absolute path, optional, working directory is dirname

    dirname:  thread and postcmd working directory, logfile location
              absolute path, required

    subdirs:  option to search subdirectories, 1/true, 0/false
              default 0/false

    pipename: named pipe/fifo fifo location
              absolute or relative path, required when command is defined

    template: output file name, date(1) sequences %Y %m %d %H %M %S %s
              relative to dirname, required when command is set

    pcrestr:  perl-compatible regex naming files to manage
              valid perl-compatible regex, required

    uid:      worker thread username or uid, username root ok, uid 0 not ok
              default nobody

    gid:      worker thread groupname or gid, groupname root ok, gid 0 not ok
              default nogroup

    loglimit: rotate size, M = MiB, G = GiB; 0 = no rotate (off)
              default off

    diskfree: percent blocks free for unprivileged users; 0 = no monitor (off)
              default off

    inofree:  percent inodes free for unprivileged users; 0 = no monitor (off)
              default off

    expire:   logfile retention, units = m, H, D, W, M, Y; 0 = no expiration (off)
              default off

    retmin:   minimum number of logs to retain, regardless of expiration; 0 = none (off)
              default off

    retmax:   maximum number of logs to retain, regardless of expiration; 0 = no max (off)
              default off

    postcmd:  command to run after the log closes or rotates, %n = filename
              string passed to bash -c, optional, working directory is dirname
              unused when command is not defined
              default none

## Token Expansions

The `template` key can contain tokens similar to the date command:

          %Y:  year
          %m:  month
          %d:  day
          %H:  hour
          %M:  minute
          %S:  second
          %s:  seconds since epoch

The `postcmd` key can contain the current directory, file, and hostname:

          %h:  system hostname (nodename)
          %p:  current dirname (path)
          %n:  current filename

## Threads

Thread names have a kernel-imposed length limit of 16 characters (15 + nul).
Thread names are assigned `<sectionname>_<taskname>`.  Task names and purposes:

          wrk:  worker thread
          dfs:  filesystem free space monitor thread
          exp:  logfile expire monitor thread

Example: file expire thread name for the section called `console`: `console_exp`
