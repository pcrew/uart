
.PHONY: clean cleanall

SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)
PROGRAM = test
LDFLAGS = -ldl -lm

UTILS += $(TOPDIR)/sources/uart.o
UTILS += $(TOPDIR)/sources/modbus.o

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS) $(UTILS)
	      $(CC) -rdynamic $(OBJECTS) $(UTILS) -o $@ $(LDFLAGS)

clean:
	@rm -fv $(OBJECTS)

cleanall:
	@rm -fv $(OBJECTS) $(PROGRAM)
