## Select one of these
DEVICE=stm8s003f3
# DEVICE=stm8s103f3

## A directory for common include files
COMMONDIR = src

## A directory for user source files
SOURCEDIR = src

## A directory for library files
LIBDIR = libs

## Build Directory
BUILDDIR = build

## Get program name from enclosing directory name
PROGRAM = $(lastword $(subst /, ,$(CURDIR)))

SOURCES=$(wildcard $(SOURCEDIR)/*.c)
OBJECTS=$(notdir $(SOURCES:.c=.rel))
HEADERS=$(wildcard $(SOURCEDIR)/*.h)

CC = sdcc
PROGRAMMER = stlinkv2


DEFINES=
## Set MCU-type DEFINE
ifeq ($(DEVICE),stm8s003f3)
    DEFINES += -DSTM8S003
endif
ifeq ($(DEVICE),stm8s103f3)
    DEFINES += -DSTM8S103
endif

CPPFLAGS = -I$(LIBDIR)/inc -I$(SOURCEDIR)
VPATH = src:$(LIBDIR)/src

CFLAGS = --Werror --std-sdcc99 -mstm8 $(DEFINES)
LDFLAGS = -lstm8 -mstm8 --out-fmt-ihx

.PHONY: all clean flash

$(BUILDDIR)/$(PROGRAM).ihx: $(BUILDDIR)/$(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILDDIR)/%.rel : $(SOURCEDIR)/%.c $(HEADERS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILDDIR)

flash: $(BUILDDIR)/$(PROGRAM).ihx
	stm8flash -c $(PROGRAMMER) -p $(DEVICE) -w $(BUILDDIR)/$(PROGRAM).ihx