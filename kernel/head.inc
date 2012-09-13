BASEADDR	EQU	0x00010000

G_1B		EQU	00000000b
G_4K		EQU	10000000b

DPL_0		EQU	00000000b
DPL_1		EQU	00100000b
DPL_2		EQU	01000000b
DPL_3		EQU	01100000b

;E = execute
;R = read-enable
;A = accessed
CODE_E		EQU	00000000b
CODE_EA		EQU	00000001b
CODE_ER		EQU	00000010b
CODE_ERA	EQU	00000011b
CODE_CE		EQU	00000100b
CODE_CEA	EQU	00000101b
CODE_CER	EQU	00000110b
CODE_CERA	EQU	00000111b

;R = read-enable
;W = write-enable
;A = accessed
;E = expansion-direction
DATA_R		EQU	00000000b
DATA_RA		EQU	00000001b
DATA_RW		EQU	00000010b
DATA_RWA	EQU	00000011b
DATA_ER		EQU	00000100b
DATA_ERA	EQU	00000101b
DATA_ERW	EQU	00000110b
DATA_ERWA	EQU	00000111b

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Data Descriptor
;
; %1 = BASE	0x00000000 ~ 0xFFFFFFFF
; %2 = LIMIT	0x00000 ~ 0xFFFFF
; %3 = G	G_*
; %4 = DPL	DPL_*
; %5 = ATTR	DATA_*
;
%macro m4Data 5
	dw	%2 & 0xFFFF
	dw	%1 & 0xFFFF
	db	(%1>>16) & 0xFF
	db	10010000b | %4 | %5
  	db	%3 | 01010000b | ((%2>>16) & 0x0F)
	db	(%1>>24) & 0xFF
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Code Descriptor
;
; %1 = BASE	0x00000000 ~ 0xFFFFFFFF
; %2 = LIMIT	0x00000 ~ 0xFFFFF
; %3 = G	G_*
; %4 = DPL	DPL_*
; %5 = CRA	CODE_*
;
%macro m4Code 5
	dw	%2 & 0xFFFF
	dw	%1 & 0xFFFF
	db	(%1>>16) & 0xFF
	db	10011000b | %4 | %5
  	db	%3 | 01010000b | ((%2>>16) & 0x0F)
	db	(%1>>24) & 0xFF
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; TSS Descriptor
;
; %1 = BASE	0x00000000 ~ 0xFFFFFFFF
; %2 = LIMIT	0x00000 ~ 0xFFFFF
; %3 = G	G_*
; %4 = DPL	DPL_*
;
%macro m4TSS 4
	dw	%2 & 0xFFFF
	dw	%1 & 0xFFFF
	db	(%1>>16) & 0xFF
	db	10001001b | %4
  	db	%3 | 00010000b | ((%2>>16) & 0x0F)
	db	(%1>>24) & 0xFF
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Call Gate Descriptor
;
; %1 = OFFSET	0x00000000 ~ 0xFFFFFFFF
; %2 = SELECTOR	0x00000 ~ 0xFFFFF
; %3 = DPL	DPL_*
;
%macro m4CallGate 3
	dw	%1 & 0xFFFF
	dw	%2 & 0xFFFF
	db	0x00
	db	10001100b | %3
	dw	(%1>>16) & 0xFFFF
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Interrupt Gate Descriptor
;
; %1 = OFFSET	0x00000000 ~ 0xFFFFFFFF
; %2 = SELECTOR	0x00000 ~ 0xFFFFF
; %3 = DPL	DPL_*
;
%macro m4IntGate 3
	dw	%1 & 0xFFFF
	dw	%2 & 0xFFFF
	db	0x00
	db	10001110b | %3
	dw	(%1>>16) & 0xFFFF
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Trap Gate Descriptor
;
; %1 = OFFSET	0x00000000 ~ 0xFFFFFFFF
; %2 = SELECTOR	0x00000 ~ 0xFFFFF
; %3 = DPL	DPL_*
;
%macro m4TrapGate 3
	dw	%1 & 0xFFFF
	dw	%2 & 0xFFFF
	db	0x00
	db	10001111b | %3
	dw	(%1>>16) & 0xFFFF
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Task Gate Descriptor
;
; %1 = SELECTOR	0x00000 ~ 0xFFFFF
; %2 = DPL	DPL_*
;
%macro m4TaskGate 2
	dw	0x0000
	dw	%2 & 0xFFFF
	db	0x00
	db	10000101b | %2
	dw	0x0000
%endmacro
