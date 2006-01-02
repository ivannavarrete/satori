
AS = avra

OBJFILES = monitor.hex


first: all						# needed for qmake Makefiles
all: $(OBJFILES)

monitor.hex: monitor.asm
	@$(AS) monitor.asm

clean:
	rm -rf *.hex *.obj *.cof
