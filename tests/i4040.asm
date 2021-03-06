; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; Intel 4040.
; ===========================================================================


#define equ .equ
#define end .end

n:     equ 56h

; 0x

	NOP
	HLT ; *
	BBS ; *
	LCR ; *
	OR4 ; *
	OR5 ; *
	AN6 ; *
	AN7 ; *
	DB0 ; *
	DB1 ; *
	SB0 ; *
	SB1 ; *
	EIN ; *
	DIN ; *
	RPM ; *
	; F

; 1x

	JCN 0,*
	JCN 1,*
	JCN 2,*
	JCN 3,*
	JCN 4,*
	JCN 5,*
	JCN 6,*
	JCN 7,*
	JCN 8,*
	JCN 9,*
	JCN 10,*
	JCN 11,*
	JCN 12,*
	JCN 13,*
	JCN 14,*
	JCN 15,*

; 2x

	FIM 0,n
	FIM 0P,n
	SRC 0
	SRC 0P
	FIM 2,n
	FIM 1P,n
	SRC 2
	SRC 1P
	FIM 4,n
	FIM 2P,n
	SRC 4
	SRC 2P
	FIM 6,n
	FIM 3P,n
	SRC 6
	SRC 3P
	FIM 8,n
	FIM 4P,n
	SRC 8
	SRC 4P
	FIM 10,n
	FIM 5P,n
	SRC 10
	SRC 5P
	FIM 12,n
	FIM 6P,n
	SRC 12
	SRC 6P
	FIM 14,n
	FIM 7P,n
	SRC 14
	SRC 7P

; 3x

	FIN 0
	FIN 0P
	JIN 0
	JIN 0P
	FIN 2
	FIN 1P
	JIN 2
	JIN 1P
	FIN 4
	FIN 2P
	JIN 4
	JIN 2P
	FIN 6
	FIN 3P
	JIN 6
	JIN 3P
	FIN 8
	FIN 4P
	JIN 8
	JIN 4P
	FIN 10
	FIN 5P
	JIN 10
	JIN 5P
	FIN 12
	FIN 6P
	JIN 12
	JIN 6P
	FIN 14
	FIN 7P
	JIN 14
	JIN 7P
; 4x

	JUN $077
	JUN $177
	JUN $277
	JUN $377
	JUN $477
	JUN $577
	JUN $677
	JUN $777
	JUN $877
	JUN $977
	JUN $A77
	JUN $B77
	JUN $C77
	JUN $D77
	JUN $E77
	JUN $F77

; 5x

	JMS $077
	JMS $177
	JMS $277
	JMS $377
	JMS $477
	JMS $577
	JMS $677
	JMS $777
	JMS $877
	JMS $977
	JMS $A77
	JMS $B77
	JMS $C77
	JMS $D77
	JMS $E77
	JMS $F77

; 6x

	INC 0
	INC 1
	INC 2
	INC 3
	INC 4
	INC 5
	INC 6
	INC 7
	INC 8
	INC 9
	INC 10
	INC 11
	INC 12
	INC 13
	INC 14
	INC 15

; 7x

	ISZ 0,*
	ISZ 1,*
	ISZ 2,*
	ISZ 3,*
	ISZ 4,*
	ISZ 5,*
	ISZ 6,*
	ISZ 7,*
	ISZ 8,*
	ISZ 9,*
	ISZ 10,*
	ISZ 11,*
	ISZ 12,*
	ISZ 13,*
	ISZ 14,*
	ISZ 15,*

; 8x

	ADD 0
	ADD 1
	ADD 2
	ADD 3
	ADD 4
	ADD 5
	ADD 6
	ADD 7
	ADD 8
	ADD 9
	ADD 10
	ADD 11
	ADD 12
	ADD 13
	ADD 14
	ADD 15

; 9x

	SUB 0
	SUB 1
	SUB 2
	SUB 3
	SUB 4
	SUB 5
	SUB 6
	SUB 7
	SUB 8
	SUB 9
	SUB 10
	SUB 11
	SUB 12
	SUB 13
	SUB 14
	SUB 15

; Ax

	LD 0
	LD 1
	LD 2
	LD 3
	LD 4
	LD 5
	LD 6
	LD 7
	LD 8
	LD 9
	LD 10
	LD 11
	LD 12
	LD 13
	LD 14
	LD 15

; Bx

	XCH 0
	XCH 1
	XCH 2
	XCH 3
	XCH 4
	XCH 5
	XCH 6
	XCH 7
	XCH 8
	XCH 9
	XCH 10
	XCH 11
	XCH 12
	XCH 13
	XCH 14
	XCH 15

; Cx

	BBL 0
	BBL 1
	BBL 2
	BBL 3
	BBL 4
	BBL 5
	BBL 6
	BBL 7
	BBL 8
	BBL 9
	BBL 10
	BBL 11
	BBL 12
	BBL 13
	BBL 14
	BBL 15

; Dx

	LDM 0
	LDM 1
	LDM 2
	LDM 3
	LDM 4
	LDM 5
	LDM 6
	LDM 7
	LDM 8
	LDM 9
	LDM 10
	LDM 11
	LDM 12
	LDM 13
	LDM 14
	LDM 15

; Ex

	WRM
	WMP
	WRR
	WPM
	WR0
	WR1
	WR2
	WR3
	SBM
	RDM
	RDR
	ADM
	RD0
	RD1
	RD2
	RD3

; Fx

	CLB
	CLC
	IAC
	CMC
	CMA
	RAL
	RAR
	TCC
	DAC
	TCS
	STC
	DAA
	KBP
	DCL
	; E
	; F

	end
