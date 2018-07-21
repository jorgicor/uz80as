; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; Datapoint 2200 version I.
; ===========================================================================


#define equ .equ
#define end .end

n:          equ 20h
nn:         equ 0584h
dddd:       equ 07h
a16:        equ $1234
hi8:        equ $ff11
port:       equ 3
imm8:       equ 56h    ;immediate data (8 bits)
offset:     equ 7
offset_neg: equ -7

; 0x

	HALT
	; HALT
	SLC
	RFC
	AD n
	; 5
	LA n
	RETURN
	; 8
	; 9
	SRC
	RFZ
	AC n
	; D
	LB n
	; F

; 1x

	; BETA ; *
	; 1
	; 2
	RFS
	SU n
	; 5
	LC n
	; 7
	; ALPHA ; *
	; 9
	; A
	RFP
	SB n
	; D
	LD n
	; F

; 2x

	; DI ; *
	; 1
	; 2
	RTC
	ND n
	; 5
	LE n
	; 7
	; EI ; *
	; 9
	; A
	RTZ
	XR n
	; D
	LH n
	; F

; 3x

	; POP ; *
	; 1
	; 2
	RTS
	OR n
	; 5
	LL n
	; 7
	; PUSH ; *
	; 9
	; A
	RTP
	CP n
	; D
	; E
	; F


; 4x
	JFC a16
	INPUT
	CFC a16
	; 3
	JMP a16
	; 5
	CALL a16
	; 7
	JFZ a16
	; 9
	CFZ a16
	; B
	; C
	; D
	; E
	; F

; 5x

	JFS a16
	EX ADR
	CFS a16
	EX STATUS
	; 4
	EX DATA
	; 6
	EX WRITE
	JFP a16
	EX COM1
	CFP a16
	EX COM2
	; C
	EX COM3
	; E
	EX COM4

; 6x

	JTC a16
	; 1
	CTC a16
	; 3
	; 4
	; 5
	; 6
	; 7
	JTZ a16
	EX BEEP
	CTZ a16
	EX CLICK
	; C
	EX DECK1
	; E
	EX DECK2
	
; 7x
	JTS a16
	EX RBK
	CTS a16
	EX WBK
	; 4
	; 5
	; 6
	EX BSP
	JTP a16
	EX SF
	CTP a16
	EX SB
	; C
	EX REWND
	; E
	EX TSTOP

; 8x

	ADA
	ADB
	ADC
	ADD
	ADE
	ADH
	ADL
	ADM
	ACA
	ACB
	ACC
	ACD
	ACE
	ACH
	ACL
	ACM

; 9x

	SUA
	SUB
	SUC
	SUD
	SUE
	SUH
	SUL
	SUM
	SBA
	SBB
	SBC
	SBD
	SBE
	SBH
	SBL
	SBM

; Ax

	NDA
	NDB
	NDC
	NDD
	NDE
	NDH
	NDL
	NDM
	XRA
	XRB
	XRC
	XRD
	XRE
	XRH
	XRL
	XRM

; Bx

	ORA
	ORB
	ORC
	ORD
	ORE
	ORH
	ORL
	ORM
	CPA
	CPB
	CPC
	CPD
	CPE
	CPH
	CPL
	CPM

; Cx

	NOP
	LAB
	LAC
	LAD
	LAE
	LAH
	LAL
	LAM
	LBA
	; 9
	LBC
	LBD
	LBE
	LBH
	LBL
	LBM

; Dx

	LCA
	LCB
	; 2
	LCD
	LCE
	LCH
	LCL
	LCM
	LDA
	LDB
	LDC
	; B
	LDE
	LDH
	LDL
	LDM

; Ex

	LEA
	LEB
	LEC
	LED
	; 4
	LEH
	LEL
	LEM
	LHA
	LHB
	LHC
	LHD
	LHE
	; D
	LHL
	LHM

; Fx

	LLA
	LLB
	LLC
	LLD
	LLE
	LLH
	; 6
	LLM
	LMA
	LMB
	LMC
	LMD
	LME
	LMH
	LML
	; HALT

	end
