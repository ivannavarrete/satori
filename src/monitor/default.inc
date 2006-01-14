
;
; Default definitions and macros that should be included by all assembly files.
; This file should be included *after* the specific MCU include file has been
; included.
;
; This file also requires RAMSTART to be defined by the main program.
;


.ifndef DEFAULT_ASM
.define DEFAULT_ASM

.include "/usr/include/avr/device.inc"


; The length of a clock cycle in milli/micro/nanoseconds.
.equ    CYCLE_MS    = 1000/OSC
.equ    CYCLE_US    = 1000000/OSC
.equ    CYCLE_NS    = 1000000000/OSC


; Definition that should have been in the *def2.inc files but aren't always.
; Devices that have a 8-bit stack pointer (128 bytes of SRAM or less) don't
; define SPH, which makes code less portable.
.ifndef SPH
.equ	SPH			= $3E
.endif


; SREG should always be defined by the specific MCU include file. If it's
; undefined it most likely means a module was compiled separately, for no
; particular MCU. This should never be the case in a proper program, thus
; we write an error.
.ifndef SREG
.warning "SREG undefined.. make sure the specific MCU include file is included"
.equ	SREG		= $3f
.endif

; if SREG is undefined then so are X,Y and Z
.ifndef XL
.def	XL			= r26
.def	XH			= r27
.def	YL			= r28
.def	YH			= r29
.def	ZL			= r30
.def	ZH			= r31
.endif


; register aliases.. registers r12-r31 are never to be used directly
; use these aliases instead
.def	sr3			= r12		; sr3, sr4 are semi-general purpose saved regs
.def	sr4			= r13
.def	in3			= r14		; in3, in4 are semi-general purpose input regs
.def	in4			= r15
.def	in1			= r16		; in1, in2 are general purpose input regs
.def	in2			= r17
.def	out1		= r18		; out1, out2 are general purpose output regs
.def	out2		= r19
.def	sr1			= r20		; sr1, sr2 are general purpose saved regs
.def	sr2			= r21
.def	tmp1		= r22		; tmp1-tmp3 are general purpose temp regs
.def	tmp2		= r23
.def	tmp3		= r24
.def	errcode		= r25		; error code register, updated by some functions


; constants that refer to MCU memories
.equ	MEM_SRAM		= 1
.equ	MEM_FLASH		= 2
.equ	MEM_EEPROM		= 3

; reset sources (aux.asm module)
.equ	RESET_SRC_PO	= 1
.equ	RESET_SRC_EXT	= 2
.equ	RESET_SRC_WDT	= 3
.equ	RESET_SRC_UNKN	= 4		; happens for devices with no MCUSR

; error codes (uio.asm module)
.equ	OK				= 0
.equ	ERR_SYNTAX		= 1
.equ	ERR_MEMORY		= 2
.equ	ERR_DIV_BY_ZERO	= 3

; spi modes (spi.asm module)
.equ	SPI_MASTER	= 1
.equ	SPI_SLAVE	= 2


; this fragment allows all variables in RAM from now on to be declared after
; RAMSTART, unless specifically specified otherwise with an .org statement
.equ	RAMSTART	= $060

	.dseg
	.org RAMSTART
random_filler_variable_1234567890:


.endif DEFAULT_ASM
