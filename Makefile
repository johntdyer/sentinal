# Makefile
#
# Copyright (c) 2021 jjb
# All rights reserved.
#
# This source code is licensed under the MIT license found
# in the root directory of this source tree.

all:		sentinal		\
			sentinalpipe

SENOBJS=	sentinal.o		\
			dfsthread.o		\
			expthread.o		\
			findmnt.o		\
			fullpath.o		\
			ini.o			\
			iniget.o		\
			logname.o		\
			logretention.o	\
			logsize.o		\
			mylogfile.o		\
			oldestfile.o	\
			pcrecheck.o		\
			postcmd.o		\
			runcmd.o		\
			signals.o		\
			slmthread.o		\
			strlcat.o		\
			strlcpy.o		\
			strreplace.o	\
			threadname.o	\
			verifyids.o		\
			version.o		\
			workthread.o

SPMOBJS=	sentinalpipe.o	\
			fullpath.o		\
			ini.o			\
			iniget.o		\
			strlcpy.o		\
			version.o

SEN_HOME=	/opt/sentinal
SEN_BIN=	$(SEN_HOME)/bin
SEN_ETC=	$(SEN_HOME)/etc
SEN_DOC=	$(SEN_HOME)/doc
SEN_TEST=	$(SEN_HOME)/tests

CC=			gcc
WARNINGS=	-Wno-unused-result -Wunused-variable -Wunused-but-set-variable
CFLAGS=		-g -O2 -pthread $(WARNINGS)

LIBS=		-lpthread -lpcre -lm

$(SENOBJS):	sentinal.h basename.h ini.h

$(SPMOBJS):	sentinal.h basename.h ini.h

sentinal:	$(SENOBJS)
			$(CC) $(LDFLAGS) -o $@ $(SENOBJS) $(LIBS)

sentinalpipe:	$(SPMOBJS)
				$(CC) $(LDFLAGS) -o $@ $(SPMOBJS) $(LIBS)

install:	all
			mkdir -p $(SEN_HOME) $(SEN_BIN) $(SEN_ETC) $(SEN_DOC)
			chown root:root $(SEN_HOME) $(SEN_BIN) $(SEN_ETC) $(SEN_DOC)
			install -o root -g root -m 755 sentinal -t $(SEN_BIN)
			install -o root -g root -m 755 sentinalpipe -t $(SEN_BIN)
			install -b -o root -g root -m 644 tests/test4.ini -T $(SEN_ETC)/example.ini
			cp -p README.* $(SEN_DOC)
			chown -R root:root $(SEN_DOC)

tests:		all
			mkdir -p $(SEN_TEST)
			cp -p tests/test?.* $(SEN_TEST)
			cp -p tests/testmultilog.ini tests/testmultipipe.ini $(SEN_TEST)
			chown -R root:root $(SEN_TEST)

systemd:
			sed "s,INI_FILE,$(SEN_ETC),"	\
				services/sentinal.systemd > sentinal.service
			install -o root -g root -m 644 sentinal.service -t /etc/systemd/system
			sed "s,INI_FILE,$(SEN_ETC),"	\
				services/sentinalpipe.systemd > sentinalpipe.service
			install -o root -g root -m 644 sentinalpipe.service -t /etc/systemd/system
			# systemctl daemon-reload
			# systemctl start sentinal sentinalpipe

deb:
			bash packaging/debian/debian.sh

rpm:
			bash packaging/redhat/redhat.sh

clean:
			rm -f $(SENOBJS) $(SPMOBJS) sentinal sentinalpipe
			rm -f sentinal.service sentinalpipe.service
			rm -fr packaging/debian/sentinal_*
			rm -fr packaging/redhat/sentinal-* packaging/redhat/rpmbuild

# vim: set tabstop=4 shiftwidth=4 noexpandtab:
