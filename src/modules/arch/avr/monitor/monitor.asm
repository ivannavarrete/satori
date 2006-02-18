

.include "device.inc"

.equ DEVICE = AT908535
.equ OSC = 8000000


.include "/usr/include/avr/8535def2.inc"
.include "default.inc"


; Programmer commands. Must match those in commandengine.h
.equ	CmdReadMemory		= 1
.equ	CmdWriteMemory		= 2
.equ	CmdGetState			= 3
.equ	CmdSetState			= 4
.equ	CmdSetBreakpoint	= 5
.equ	CmdClearBreakpoint	= 6
.equ	CmdExecute			= 7

; Memory types. Must match those in memory.h
.equ	SRAM				= 1
.equ	EEPROM				= 2
.equ	FLASH				= 3

; must match to hdr_size in packet.h
.equ	header_size			= 12

; must match to value in execengine.h
.equ	num_breakpoints		= 4

; must match to state in state.h
.equ	state_size			= 37


.equ	DEBUG_STACK			= RAMEND

	.cseg
	.org $000
	rjmp	reset

	.org	OVF0addr
	rjmp	DebugEntryPoint

magic_str_1:	.db		"magic1"


	.org $011
	.db		$1A, $01		; movw		r2, r20
	.db		$07, $03		; mulsu		r16, r23
	cpc		r14, r21
	adiw	r26, $10
	ldi		r20, $5A
	cbi		$13, 5
	in		r5, $25
	out		$25, r5
	breq	debug_fake_jump
debug_fake_jump:
	rjmp	debug_fake_jump_2
debug_fake_jump_2:
	lds		r5, $55AA
	;jmp		debug_fake_jump_3
debug_fake_jump_3:
	;ldd		r5, $25
	;std		$25, r5



;===[ reset ]===================================================================
;
;===============================================================================
reset:
	; init stack
	ldi		tmp1, low(RAMEND)
	out		SPL, tmp1
	ldi		tmp1, high(RAMEND)
	out		SPH, tmp1

	; initialize uart
	ldi		tmp1, 1<<RXEN | 1<<TXEN
	out		UCR, tmp1

	;ldi	tmp1, 51					; 9600 baud at 8MHz
	;ldi	tmp1, 16					; 28800 baud at 8MHz
	ldi		tmp1, 12					; 38400 baud at 8MHz
	out		UBRR, tmp1

	; initialize timer
	;ldi		tmp1, 1<<TOIE0
	;out		TIMSK, tmp1

	;ldi		tmp1, 1<<TOV0
	;out		TIFR, tmp1

	; <<< enable debug leds >>>
	;ldi		tmp1, $FF
	;out		DDRC, tmp1
	;out		PORTC, tmp1

	;sei

	ldi		r31, $55
	mov		r0, r31
	ldi		r31, $AA
	mov		r2, r31
	ldi		r31, $5A

;	rjmp	DebugMain
	rjmp	DebugEntryPoint


;===[ Main ]====================================================================
;
;===============================================================================
;Main:


	;rjmp	Main


;=== [ DebugEntryPoint ]========================================================
; Save microcontroller state.
;===============================================================================
DebugEntryPoint:
	sts		state_regs+26, XL			; save working registers
	sts		state_regs+27, XH
	sts		state_regs+28, YL
	sts		state_regs+29, YH
	sts		state_regs+30, ZL
	sts		state_regs+31, ZH

	in		XL, SREG					; save status register
	sts		state_sreg, XL

	in		XH, SPH						; extract and save program counter
	in		XL, SPL
	ld		YL, X+
	ld		YL, X+
	ld		YH, X
	sts		state_pc, YL
	sts		state_pc+1, YH

	sts		state_sp, XL				; save original stack pointer
	sts		state_sp+1, XH

	;ldi		XL, low(DEBUG_STACK)		; change to local stack
	;ldi		XH, high(DEBUG_STACK)
	;out		SPL, XL
	;ous		SPH, XH

	ldi		r27, 26						; save the rest of the registers
	clr		YL							; Y = pointer to regs
	clr		YH
	ldi		ZL, low(state_regs)			; Z = pointer to mem buffer
	ldi		ZH, high(state_regs)

DebugEntryPoint_10:
	ld		r26, Y+
	st		Z+, r26
	dec		r27
	brne	DebugEntryPoint_10


;===[ DebugMain ]===============================================================
; Handle commands.
;===============================================================================
DebugMain:
	;in		tmp1, PINC
	;andi	tmp1, $BF
	;out		PORTC, tmp1

	rcall	GetPacketHeader
	lds		tmp1, packet_cmd
	
	cpi		tmp1, CmdReadMemory
	brne	DebugMain_10
	rjmp	ReadMemory

DebugMain_10:
	cpi		tmp1, CmdWriteMemory
	brne	DebugMain_20
	rjmp	WriteMemory

DebugMain_20:
	cpi		tmp1, CmdGetState
	brne	DebugMain_30
	rjmp	GetState

DebugMain_30:
	cpi		tmp1, CmdSetState
	brne	DebugMain_40
	rjmp	SetState

DebugMain_40:
	cpi		tmp1, CmdSetBreakpoint
	brne	DebugMain_50
	rjmp	SetBreakpoint

DebugMain_50:
	cpi		tmp1, CmdClearBreakpoint
	brne	DebugMain_60
	rjmp	ClearBreakpoint

DebugMain_60:
	cpi		tmp1, CmdExecute
	brne	DebugMain_70
	rjmp	Execute

DebugMain_70:
	rjmp	DebugMain


;===[ DebugExitPoint ]==========================================================
; Restore microcontroller state.
;===============================================================================
DebugExitPoint:

	lds		XL, state_sp				; restore original stack pointer
	out		SPL, XL
	lds		XL, state_sp+1
	out		SPH, XL

	lds		XL, state_sreg				; restore status register
	out		SREG, XL

	lds		XL, state_regs+26			; restore working registers
	lds		XH, state_regs+27
	lds		YL, state_regs+28
	lds		YH, state_regs+29
	lds		ZL, state_regs+30
	lds		ZH, state_regs+31

	ldi		tmp1, 1<<TOV0				; trigger overflow interrupt
	out		TIFR, tmp1
	
	reti


;===[ ReadMemory ]==============================================================
; Send SRAM/EEPROM/FLASH data to debugger.
;
; in:
;	packet_arg1		start address of data, 0-...
;	packet_arg2		number of bytes to transfer, 1-65535
;===============================================================================
ReadMemory:
	;ldi		XH, $FE
	;out		PORTC, XH

	lds		XH, packet_arg2+2			; number of bytes to transfer
	lds		XL, packet_arg2+3

	lds		ZH, packet_arg1+2			; start address
	lds		ZL, packet_arg1+3

	sts		packet_dsize, XH			; packet data size
	sts		packet_dsize+1, XL

	rcall	SendPacketHeader

	; send packet data
ReadMemory_entry_point_2:
	lds		sr1, packet_cmd_mod			; <<< can use in1 instead >>>
ReadMemory_10:
	cpi		sr1, SRAM
	brne	ReadMemory_20
	ld		in1, Z
	rjmp	ReadMemory_40

ReadMemory_20:
	cpi		sr1, FLASH
	brne	ReadMemory_30
	lpm
	mov		in1, r0
	rjmp	ReadMemory_40

ReadMemory_30:
	;ld		in1, Z			; <<< >>>
	;rjmp	ReadMemory_40	; <<< >>>

	sbic	EECR, EEWE					; wait until safe to modify E2P regs
	rjmp	ReadMemory_30
	
	out		EEARL, ZL					; load address into E2P address register
	out		EEARH, ZH

	sbi		EECR, EERE					; start read operation
	in		in1, EEDR					; get read byte

ReadMemory_40:
	rcall	Putc

	clr		r0							; inc address pointer
	clr		r1
	inc		r1
	add		ZL, r1
	adc		ZH, r0

	sub		XL, r1						; dec byte counter
	sbc		XH, r0
	brne	ReadMemory_10

	rjmp	DebugMain


;===[ WriteMemory ]=============================================================
; Due to the slow eeprom write time (2.5ms - 4ms) it is necessary that the
; debugger does not send data at speeds over 250B/s (2Kbit/s).
;===============================================================================
WriteMemory:
	;ldi		XH, $FD
	;out		PORTC, XH

	lds		XH, packet_dsize			; number of bytes to receive
	lds		XL, packet_dsize+1

	lds		ZH, packet_arg1+2			; memory address to store at
	lds		ZL, packet_arg1+3

	; receive and store packet data
WriteMemory_entry_point_2:
	lds		sr1, packet_cmd_mod
WriteMemory_10:
	rcall	Getc
	cpi		sr1, SRAM
	brne	WriteMemory_20
	st		Z, out1
	rjmp	WriteMemory_30

WriteMemory_20:
	cpi		sr1, EEPROM
	brne	WriteMemory_30				; ignore data if neither SRAM or E2P
WriteMemory_21:
	sbic	EECR, EEWE					; wait until safe to modify E2P regs
	rjmp	WriteMemory_21

	out		EEARL, ZL					; load address into E2P address reg
	out		EEARH, ZH
	out		EEDR, out1					; load data into E2P data reg

	ldi		out1, 1<<EEMWE				; start write operation
	out		EECR, out1
	sbi		EECR, EEWE

WriteMemory_30:
	rcall	Putc						; echo, to tell PC we're ready to go on

	clr		r0							; inc address pointer
	clr		r1
	inc		r1
	add		ZL, r1
	adc		ZH, r0

	sub		XL, r1						; dec byte counter
	sbc		XH, r0
	brne	WriteMemory_10

	rjmp	DebugMain


;===[ GetState ]================================================================
;
;===============================================================================
GetState:
	ldi		XL, low(state_size)
	ldi		XH, high(state_size)

	ldi		ZL, low(state)
	ldi		ZH, high(state)

	sts		packet_dsize, XH
	sts		packet_dsize+1, XL

	rcall	SendPacketHeader

	ldi		sr1, SRAM
	sts		packet_cmd_mod, sr1
	rjmp	ReadMemory_entry_point_2


;===[ SetState ]================================================================
;
;===============================================================================
SetState:
	ldi		XL, low(state_size)
	ldi		XH, high(state_size)

	ldi		ZL, low(state)
	ldi		ZH, high(state)

	ldi		sr1, SRAM
	sts		packet_cmd_mod, sr1
	rjmp	WriteMemory_entry_point_2


;===[ SetBreakpoint ]===========================================================
;
;===============================================================================
SetBreakpoint:
	; get breakpoint number and calculate index into breakpoint table
	lds		ZH, packet_arg2+2			; breakpoint number
	lds		ZL, packet_arg2+3

	clc
	rol		ZL
	rol		ZH

	ldi		XL, low(breakpoints)
	ldi		XH, high(breakpoints)

	add		ZL, XL
	adc		ZH, XH
	
	lds		XH, packet_arg1+2			; breakpoint address
	lds		XL, packet_arg1+3

	st		Z+, XL						; save new breakpoint address
	st		Z, XH

	rjmp	DebugMain


;===[ ClearBreakpoint ]=========================================================
;
;===============================================================================
ClearBreakpoint:
	clr		XL
	sts		packet_arg1+2, XL
	sts		packet_arg1+3, XL

	rjmp	SetBreakpoint


;===[ Execute ]=================================================================
;
;===============================================================================
Execute:

	rjmp	DebugMain


;===[ GetPacketHeader ]=========================================================
; Get command from computer.
;===============================================================================
GetPacketHeader:
	push	sr1
	push	XL
	push	XH

	ldi		XL, low(packet)
	ldi		XH, high(packet)
	ldi		sr1, header_size

GetPacketHeader_10:
	rcall	Getc
	st		X+, out1
	rcall	Putc					; echo, to tell PC we're ready to move on
	dec		sr1
	brne	GetPacketHeader_10

	pop		XH
	pop		XL
	pop		sr1

	ret


;===[ SendPacketHeader ]========================================================
;
;===============================================================================
SendPacketHeader:
	push	sr1
	push	XL
	push	XH

	ldi		XL, low(packet)
	ldi		XH, high(packet)
	ldi		sr1, header_size

SendPacketHeader_10:
	ld		in1, X+
	rcall	Putc
	dec		sr1
	brne	SendPacketHeader_10

	pop		XH
	pop		XL
	pop		sr1

	ret


;===[ Getc ]====================================================================
;
; out:
;	out1	received character
;===============================================================================
Getc:
	sbis	USR, RXC
	rjmp	Getc
	in		out1, UDR

	ret


;===[ Putc ]====================================================================
;
; in:
;	in1		character to send
;===============================================================================
Putc:
	sbis	USR, UDRE
	rjmp	Putc
	out		UDR, in1

	ret


magic_str_end:	.db	"magic_end", 0


	.dseg
	.org RAMSTART
; the packet structure must match the one in packet.h
packet:
packet_cmd:		.byte		1
packet_cmd_mod:	.byte		1
packet_dsize:	.byte		2
packet_arg1:	.byte		4
packet_arg2:	.byte		4

; the state structure must match the one in state.h
state:
state_regs:		.byte		32
state_pc:		.byte		2
state_sp:		.byte		2
state_sreg:		.byte		1

; breakpoint addresses
breakpoints:
bp1:			.byte		2
bp2:			.byte		2
bp3:			.byte		2
bp4:			.byte		2
bpn:			.byte		2

	.eseg
magic_e2p:		.db		"magic_e2p", 0
