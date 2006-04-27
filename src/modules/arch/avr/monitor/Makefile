
# Currently multiple source files is not supported.

AS = avra
LOADER = avrdude -p 8535 -c stk500 -U flash:w:
RM = /bin/rm -f

OBJFILES = monitor.hex


first: all
all: $(OBJFILES)

%.hex: %.asm
	$(AS) $<
	#$(LOADER)$@

program: all
	$(LOADER)monitor.hex

clean: distclean
distclean:
	$(RM) *.hex *.obj *.cof
