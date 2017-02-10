APPLICATION = default

BOARD ?= opencm9-04

RIOTBASE ?= $(CURDIR)/../RIOT

CFLAGS += -DDEVELHELP

QUIET ?= 0

MY_DEFINES += -DHAS_DXL=1 -DBOARD_opencm904 -DHAS_TERMINAL=1
CFLAGS += $(MY_DEFINES)
CXXEXFLAGS += $(MY_DEFINES)

CFLAGS += -I.
CXXEXFLAGS += -std=c++11

USEMODULE += xtimer

include $(RIOTBASE)/Makefile.include