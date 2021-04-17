include djyos.mk
V = 1
include $(DJYOS_SRC_LOCATION)/third/micropython1.13/py/mkenv.mk
DJYOSPORT = $(TOP)/ports/djyos

# qstr definitions (must come before including py.mk)
QSTR_DEFS = $(DJYOSPORT)/qstrdefsport.h

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 0

MICROPY_PY_USSL=1
MICROPY_SSL_MBEDTLS=1

# include py core make definitions
include $(TOP)/py/py.mk

CROSS_COMPILE ?= arm-djyos-eabi-

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(DJYOSPORT)

DFU = $(TOP)/tools/dfu.py
PYDFU = $(TOP)/tools/pydfu.py
CFLAGS = $(INC) $(DJYOS_FLAGS) $(COPT) $(CFLAGS_MOD)
LDFLAGS = -nostdlib -T stm32f405.ld -Map=$@.map --cref --gc-sections

CSUPEROPT = -Os # save some code space

# Tune for Debugging or Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -O0 -ggdb
else
CFLAGS += -Os -DNDEBUG
CFLAGS += -fdata-sections -ffunction-sections
endif

LIBS =
#DSRC_C = \
#	$(DJYOSPORT)/djympy.c \
#	$(DJYOSPORT)/djythread.c \
#	$(DJYOSPORT)/mphalport.c \
#	$(DJYOSPORT)/modusocket.c \


SRC_C = $(DSRC_C)\
	$(DJYOSPORT)/djympy.c \
	$(DJYOSPORT)/djythread.c \
	$(DJYOSPORT)/mphalport.c \
	$(DJYOSPORT)/modusocket.c \
 	$(DJYOSPORT)/module-example.c \
  	$(DJYOSPORT)/djy-fs.c \
  	$(DJYOSPORT)/machine.c \
  	$(DJYOSPORT)/pin.c \
  	$(DJYOSPORT)/uart.c \
  	$(DJYOSPORT)/modutime.c \
  	$(DJYOSPORT)/timer.c \
	lib/netutils/netutils.c \
	lib/utils/printf.c \
	lib/utils/stdout_helpers.c \
	lib/utils/pyexec.c \
	lib/utils/interrupt_char.c \
	lib/mp-readline/readline.c \
#	$(BUILD)/_frozen_mpy.c \

SRC_C += lib/libc/string0.c
SRC_MOD += $(DJYOSPORT)/mbedtls/mbedtls_port.c
SRC_QSTR += $(SRC_C) $(SRC_MOD)

ifeq ($(MICROPY_SSL_MBEDTLS),1)
CFLAGS_MOD += -DMBEDTLS_CONFIG_FILE='"mbedtls/mbedtls_config.h"'
endif

OBJ = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_MOD:.c=.o))

all: $(targetname).a

$(BUILD)/_frozen_mpy.c: frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC freezing bytecode"
	$(Q)$(PYTHON) $(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

$(targetname).a: $(OBJ)
	$(ECHO) "LINK $@"

	$(Q)$(AR) rcs $(targetname).a $^
	$(ECHO) "LINK END $@"

# Run emulation build on a POSIX system with suitable terminal settings
run:
	stty raw opost -echo
	build/firmware.elf
	@echo Resetting terminal...
# This sleep is useful to spot segfaults
	sleep 1
	reset

test: $(BUILD)/firmware.elf
	$(Q)/bin/echo -e "print('hello world!', list(x+1 for x in range(10)), end='eol\\\\n')\\r\\n\\x04" | $(BUILD)/firmware.elf | tail -n2 | grep "^hello world! \\[1, 2, 3, 4, 5, 6, 7, 8, 9, 10\\]eol"

include $(TOP)/py/mkrules.mk
