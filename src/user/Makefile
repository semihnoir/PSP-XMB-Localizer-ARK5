TARGET = localizeruser
OBJS = LocalizerLib.o main.o

INCDIR = 
CFLAGS = -O2 -G0 -Wall -fno-pic -fshort-wchar
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

LIBDIR = 
LDFLAGS = -nostartfiles
LIBS = -lpspsystemctrl_user


PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak


