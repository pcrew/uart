
TOPDIR := $(shell /bin/pwd)
INCDIR := $(TOPDIR)/include

CC = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip
CFLAGS += -Wall -Os -I$(INCDIR) -D_FILE_OFFSET_BITS=64

export CC STRIP CFLAGS TOPDIR INCDIR

all:
	@$(MAKE) -C sources
	@$(MAKE) -C utils 

clean:
	@rm -v `find . -name *.o`

cleanall:
	@rm -vf `find . -name *.o`

	@$(MAKE) cleanall -C utils
