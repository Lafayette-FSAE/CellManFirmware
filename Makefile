BRANCH:=$(shell git rev-parse --abbrev-ref HEAD)
VERSION:=$(shell git describe --tags)

DEVICE=stm8s003f3

CC = sdcc
PROGRAMMER = stlinkv2

BUILD_DIR = ./build
SRC_DIR = ./src
INC_DIR = ./libs

## Get program name from enclosing directory name
PROGRAM = $(lastword $(subst /, ,$(CURDIR)))

SOURCES=$(wildcard $(SOURCEDIR)/*.c)
OBJECTS=$(notdir $(SOURCES:.c=.rel))
HEADERS=$(wildcard $(SOURCEDIR)/*.h)

# DEFINES=
# ## Set MCU-type DEFINE
# ifeq ($(DEVICE),stm8s003f3)
#     DEFINES += -DSTM8S003
# endif
# ifeq ($(DEVICE),stm8s103f3)
#     DEFINES += -DSTM8S103
# endif

DEFINES += -D BRANCH=$(BRANCH)
DEFINES += -D VERSION=$(VERSION)

# CPPFLAGS = -I$(LIBDIR)/inc -I$(SOURCEDIR)
# VPATH = src:$(LIBDIR)/src

CFLAGS =  -mstm8 -I$(INC_DIR) -D$(DEVICE) $(DEFINES) -c
LFLAGS = -lstm8 -mstm8 --out-fmt-ihx

BUILD_ARTIFACTS = $(BUILD_DIR)

.PHONY: all default flash clean

all: $(BUILD_DIR)/main.ihx

default: $(BUILD_DIR)/main.ihx

$(BUILD_DIR)/main.ihx : $(SRC_DIR)/main.c $(addprefix $(BUILD_DIR)/, $(OBJ_FILES)) clean
	@mkdir -p $(BUILD_DIR)
	sdcc -mstm8 -I$(INC_DIR) -D STM8S003 $(DEFINES) src/main.c $(addprefix $(BUILD_DIR)/, $(OBJ_FILES)) -o $(BUILD_DIR)/main.ihx

clean:
	rm -rf $(BUILD_DIR)

flash: $(BUILD_DIR)/main.ihx
	stm8flash -c $(PROGRAMMER) -p $(DEVICE) -w $(BUILD_DIR)/main.ihx