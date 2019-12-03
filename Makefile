## Select one of these
DEVICE=stm8s003f3

CC = sdcc
PROGRAMMER = stlinkv2

BUILD_DIR = ./build
SRC_DIR = ./src
INC_DIR = ./inc

## Get program name from enclosing directory name
PROGRAM = $(lastword $(subst /, ,$(CURDIR)))

SOURCES=$(wildcard $(SOURCEDIR)/*.c)
OBJECTS=$(notdir $(SOURCES:.c=.rel))
HEADERS=$(wildcard $(SOURCEDIR)/*.h)

DEFINES=
## Set MCU-type DEFINE
ifeq ($(DEVICE),stm8s003f3)
    DEFINES += -DSTM8S003
endif
ifeq ($(DEVICE),stm8s103f3)
    DEFINES += -DSTM8S103
endif

# CPPFLAGS = -I$(LIBDIR)/inc -I$(SOURCEDIR)
# VPATH = src:$(LIBDIR)/src

CFLAGS =  -mstm8 -I$(INC_DIR) -D$(DEVICE) $(DEFINES) -c
LFLAGS = -lstm8 -mstm8 --out-fmt-ihx

BUILD_ARTIFACTS = $(BUILD_DIR)

.PHONY: all default flash

all: $(BUILD_DIR)/main.ihx

default: $(BUILD_DIR)/main.ihx

$(BUILD_DIR)/main.ihx : $(SRC_DIR)/main.c $(addprefix $(BUILD_DIR)/, $(OBJ_FILES))
	@mkdir -p $(BUILD_DIR)
	sdcc -mstm8 -I$(INC_DIR) -D STM8S003 $(LFLAGS) src/main.c $(addprefix $(BUILD_DIR)/, $(OBJ_FILES)) -o $(BUILD_DIR)/main.ihx

clean:
	rm -rf $(BUILD_DIR)

flash: $(BUILDDIR)/$(PROGRAM).ihx
	stm8flash -c $(PROGRAMMER) -p $(DEVICE) -w $(BUILDDIR)/$(PROGRAM).ihx