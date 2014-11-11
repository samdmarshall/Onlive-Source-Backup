# File : app/Makefile
#
# Copyright (C) 2003-2008, Marvell International Ltd.
# All Rights Reserved

# Path to the top directory of the wlan distribution
PATH_TO_TOP = ../..

# Determine how we should copy things to the install directory
ABSPATH := $(filter /%, $(INSTALLDIR))
RELPATH := $(filter-out /%, $(INSTALLDIR))
INSTALLPATH := $(ABSPATH)
ifeq ($(strip $(INSTALLPATH)),)
INSTALLPATH := $(PATH_TO_TOP)/$(RELPATH)
endif

# Override CFLAGS for application sources, remove __ kernel namespace defines
CFLAGS := $(filter-out -D__%, $(CFLAGS))

# Add the wlan driver directory to the preprocessor include path
CFLAGS +=  -I$(PATH_TO_TOP)/wlan

#
# List of application executables to create
#
TARGETS := wlanconfig

#
# Make target rules
#

# All rule compiles list of TARGETS using builtin program target from src rule
all : $(TARGETS)

# Update any needed TARGETS and then copy to the install path
build install: $(TARGETS)
	@cp -f $< $(INSTALLPATH)

clean:
	@rm -f $(TARGETS)

distclean: clean
	@rm -f *~ core
