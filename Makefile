APPLICATION = default

BOARD ?= samr21-xpro

RIOTBASE ?= $(CURDIR)/../RIOT

CFLAGS += -DDEVELHELP

QUIET ?= 0

CFLAGS += -I.
CXXEXFLAGS += -std=c++11

USEMODULE += xtimer
USEMODULE += dynamixel

include $(RIOTBASE)/Makefile.include
