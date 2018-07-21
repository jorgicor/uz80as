; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; Intel 8008.
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

	; HLT
	; HLT
	RLC
	RFC
	ADI n
	RST 0
	LAI n
	RET
	INB
	DCB
	RRC
	RFZ
	ACI n
	RST 1
	LBI n
	; RET

; 1x

	INC
	DCC
	RAL
	RFS
	SUI n
	RST 2
	LCI n
	; RET
	IND
	DCD
	RAR
	RFP
	SBI n
	RST 3
	LDI n
	; RET

; 2x

	INE
	DCE
	; 2
	RTC
	NDI n
	RST 4
	LEI n
	; RET
	INH
	DCH
	; A
	RTZ
	XRI n
	RST 5
	LHI n
	; RET

; 3x

	INL
	DCL
	; 2
	RTS
	ORI n
	RST 6
	LLI n
	; RET
	; 8
	; 9
	; A
	RTP
	CPI n
	RST 7
	LMI n
	; RET


; 4x
	JFC a16
	INP 0
	CFC a16
	INP 1
	JMP a16
	INP 2
	CAL a16
	INP 3
	JFZ a16
	INP 4
	CFZ a16
	INP 5
	; JMP a16
	INP 6
	; CAL a16
	INP 7

; 5x

	JFS a16
	OUT 8
	CFS a16
	OUT 9
	; JMP a16
	OUT 10
	; CAL a16
	OUT 11
	JFP a16
	OUT 12
	CFP a16
	OUT 13
	; JMP a16
	OUT 14
	; CAL a16
	OUT 15

; 6x

	JTC a16
	OUT 16
	CTC a16
	OUT 17
	; JMP a16
	OUT 18
	; CAL a16
	OUT 19
	JTZ a16
	OUT 20
	CTZ a16
	OUT 21
	; JMP a16
	OUT 22
	; CAL a16
	OUT 23
	
; 7x
	JTS a16
	OUT 24
	CTS a16
	OUT 25
	; JMP a16
	OUT 26
	; CAL a16
	OUT 27
	JTP a16
	OUT 28
	CTP a16
	OUT 29
	; JMP a16
	OUT 30
	; CAL a16
	OUT 31

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
	LBB
	LBC
	LBD
	LBE
	LBH
	LBL
	LBM

; Dx

	LCA
	LCB
	LCC
	LCD
	LCE
	LCH
	LCL
	LCM
	LDA
	LDB
	LDC
	LDD
	LDE
	LDH
	LDL
	LDM

; Ex

	LEA
	LEB
	LEC
	LED
	LEE
	LEH
	LEL
	LEM
	LHA
	LHB
	LHC
	LHD
	LHE
	LHH
	LHL
	LHM

; Fx

	LLA
	LLB
	LLC
	LLD
	LLE
	LLH
	LLL
	LLM
	LMA
	LMB
	LMC
	LMD
	LME
	LMH
	LML
	HLT

	end
