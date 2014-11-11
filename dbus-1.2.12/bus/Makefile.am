
configdir=$(sysconfdir)/dbus-1

INCLUDES=-I$(top_srcdir) $(DBUS_BUS_CFLAGS) @PIE_CFLAGS@		\
	-DDBUS_SYSTEM_CONFIG_FILE=\""$(configdir)/system.conf"\"	\
	-DDAEMON_NAME=\"dbus-daemon\"  -DDBUS_COMPILATION

EFENCE=

CONFIG_IN_FILES=				\
	session.conf.in				\
	system.conf.in

config_DATA=					\
	session.conf				\
	system.conf

if DBUS_USE_LIBXML
XML_SOURCES=config-loader-libxml.c
endif
if DBUS_USE_EXPAT
XML_SOURCES=config-loader-expat.c
endif

if DBUS_BUS_ENABLE_KQUEUE
DIR_WATCH_SOURCE=dir-watch-kqueue.c
else
if DBUS_BUS_ENABLE_INOTIFY
DIR_WATCH_SOURCE=dir-watch-inotify.c
else
if DBUS_BUS_ENABLE_DNOTIFY_ON_LINUX
DIR_WATCH_SOURCE=dir-watch-dnotify.c
else
DIR_WATCH_SOURCE=dir-watch-default.c
endif
endif
endif

BUS_SOURCES=					\
	activation.c				\
	activation.h				\
	activation-exit-codes.h			\
	bus.c					\
	bus.h					\
	config-parser.c				\
	config-parser.h				\
	config-parser-common.c			\
	config-parser-common.h			\
	connection.c				\
	connection.h				\
	desktop-file.c				\
	desktop-file.h				\
	$(DIR_WATCH_SOURCE)			\
	dir-watch.h				\
	dispatch.c				\
	dispatch.h				\
	driver.c				\
	driver.h				\
	expirelist.c				\
	expirelist.h				\
	policy.c				\
	policy.h				\
	selinux.h				\
	selinux.c				\
	services.c				\
	services.h				\
	signals.c				\
	signals.h				\
	test.c					\
	test.h					\
	utils.c					\
	utils.h					\
	$(XML_SOURCES)

dbus_daemon_SOURCES=				\
	$(BUS_SOURCES)				\
	main.c					

dbus_daemon_LDADD=					\
	$(EFENCE)					\
	$(DBUS_BUS_LIBS)				\
	$(top_builddir)/dbus/libdbus-convenience.la

dbus_daemon_LDFLAGS=@R_DYNAMIC_LDFLAG@ @SECTION_LDFLAGS@ @PIE_LDFLAGS@

LAUNCH_HELPER_SOURCES=				\
	$(XML_SOURCES)				\
	config-parser-common.c			\
	config-parser-common.h			\
	config-parser-trivial.c			\
	config-parser-trivial.h			\
	desktop-file.c				\
	desktop-file.h				\
	utils.c					\
	utils.h					\
	activation-exit-codes.h			\
	activation-helper.h	  		\
	activation-helper.c

## This is the installed launch helper with the setuid checks
dbus_daemon_launch_helper_SOURCES=		\
	activation-helper-bin.c			\
	$(LAUNCH_HELPER_SOURCES)

dbus_daemon_launch_helper_LDADD=		\
	$(DBUS_LAUNCHER_LIBS)			\
	$(top_builddir)/dbus/libdbus-convenience.la

dbus_daemon_launch_helper_LDFLAGS=@R_DYNAMIC_LDFLAG@ @SECTION_LDFLAGS@

## we build another binary so we can do the launch testing without root privs.
## DO NOT INSTALL THIS FILE
dbus_daemon_launch_helper_test_SOURCES=		\
	activation-helper-bin.c			\
	$(LAUNCH_HELPER_SOURCES)

dbus_daemon_launch_helper_test_LDADD=		\
	$(DBUS_LAUNCHER_LIBS)			\
	$(top_builddir)/dbus/libdbus-convenience.la

dbus_daemon_launch_helper_test_LDFLAGS=@R_DYNAMIC_LDFLAG@ @SECTION_LDFLAGS@
dbus_daemon_launch_helper_test_CPPFLAGS=	\
	-DACTIVATION_LAUNCHER_TEST

## we build yet another binary so we can do the OOM tests
## DO NOT INSTALL THIS FILE
bus_test_launch_helper_SOURCES=		\
	test-launch-helper.c   		\
	$(LAUNCH_HELPER_SOURCES)

bus_test_launch_helper_LDADD=		\
	$(DBUS_LAUNCHER_LIBS)			\
	$(top_builddir)/dbus/libdbus-convenience.la

bus_test_launch_helper_LDFLAGS=@R_DYNAMIC_LDFLAG@ @SECTION_LDFLAGS@
bus_test_launch_helper_CPPFLAGS=	\
	-DACTIVATION_LAUNCHER_TEST	\
	-DACTIVATION_LAUNCHER_DO_OOM

## note that TESTS has special meaning (stuff to use in make check)
## so if adding tests not to be run in make check, don't add them to 
## TESTS
if DBUS_BUILD_TESTS
TESTS_ENVIRONMENT=DBUS_TEST_DATA=$(top_builddir)/test/data DBUS_TEST_HOMEDIR=$(top_builddir)/dbus DBUS_FATAL_WARNINGS=1 DBUS_BLOCK_ON_ABORT=1
TESTS=bus-test bus-test-system bus-test-launch-helper
else
TESTS=
endif

## we use noinst_PROGRAMS not check_PROGRAMS so that we build 
## even when not doing "make check"
noinst_PROGRAMS=$(TESTS) dbus-daemon dbus-daemon-launch-helper-test dbus-daemon-launch-helper

bus_test_system_SOURCES=			\
	$(XML_SOURCES)				\
	config-parser-common.c			\
	config-parser-common.h			\
	config-parser-trivial.c			\
	config-parser-trivial.h			\
	utils.c					\
	utils.h					\
	test-system.c

bus_test_system_LDADD=$(top_builddir)/dbus/libdbus-convenience.la $(DBUS_BUS_LIBS)
bus_test_system_LDFLAGS=@R_DYNAMIC_LDFLAG@

bus_test_SOURCES=				\
	$(BUS_SOURCES)				\
	test-main.c

bus_test_LDADD=$(top_builddir)/dbus/libdbus-convenience.la $(DBUS_BUS_LIBS)
bus_test_LDFLAGS=@R_DYNAMIC_LDFLAG@

## mop up the gcov files
clean-local:
	/bin/rm *.bb *.bbg *.da *.gcov || true

uninstall-hook:
	rm -f $(DESTDIR)$(DBUS_DAEMONDIR)/dbus-daemon
	rm -f $(DESTDIR)$(libexecdir)/dbus-daemon-launch-helper

install-data-hook:
	if test '!' -d $(DESTDIR)$(DBUS_DAEMONDIR); then \
		$(mkinstalldirs) $(DESTDIR)$(DBUS_DAEMONDIR); \
		chmod 755 $(DESTDIR)$(DBUS_DAEMONDIR); \
	fi
	$(INSTALL_PROGRAM) dbus-daemon $(DESTDIR)$(DBUS_DAEMONDIR)
	$(mkinstalldirs) $(DESTDIR)$(localstatedir)/run/dbus
	$(mkinstalldirs) $(DESTDIR)$(configdir)/system.d
	$(mkinstalldirs) $(DESTDIR)$(configdir)/session.d
	$(mkinstalldirs) $(DESTDIR)$(datadir)/dbus-1/services
	$(mkinstalldirs) $(DESTDIR)$(datadir)/dbus-1/system-services
	$(mkinstalldirs) $(DESTDIR)$(libexecdir)/dbus-1
	$(INSTALL_PROGRAM) dbus-daemon-launch-helper $(DESTDIR)$(libexecdir)
	if test `id -u` -eq 0; then \
		chown root:$(DBUS_USER) $(DESTDIR)$(libexecdir)/dbus-daemon-launch-helper; \
		chmod 4750 $(DESTDIR)$(libexecdir)/dbus-daemon-launch-helper; \
	else \
		echo "Not installing $(DESTDIR)$(libexecdir)/dbus-daemon-launch-helper binary setuid!"; \
		echo "You'll need to manually set permissions to root:$(DBUS_USER) and permissions 4750"; \
	fi

#### Init scripts fun
SCRIPT_IN_FILES=messagebus.in \
		rc.messagebus.in

## Red Hat start
if DBUS_INIT_SCRIPTS_RED_HAT

initddir=$(sysconfdir)/rc.d/init.d

initd_SCRIPTS= 	\
	messagebus

endif
 ## Red Hat end

## Slackware start
if DBUS_INIT_SCRIPTS_SLACKWARE

initddir=$(sysconfdir)/rc.d/

initd_SCRIPTS= 	\
	rc.messagebus

endif
## Slackware end

MAN_IN_FILES=dbus-daemon.1.in
man_MANS = dbus-daemon.1

#### Extra dist 

EXTRA_DIST=$(CONFIG_IN_FILES) $(SCRIPT_IN_FILES) $(man_MANS) $(MAN_IN_FILES)
