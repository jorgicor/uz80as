; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; Intel 8048.
; ===========================================================================


#define equ .equ
#define end .end

n:	equ 77

; 0x

	NOP
	; 1
	OUTL BUS,A
	ADD A,#n
	JMP $0FF
	EN I
	; 6
	DEC A
	INS A,BUS
	IN A,P1
	IN A,P2
	; B
	MOVD A,P4
	MOVD A,P5
	MOVD A,P6
	MOVD A,P7

; 1x

	INC @R0
	INC @R1
	JB0 *
	ADDC A,#n
	CALL $0FF
	DIS I
	JTF *
	INC A
	INC R0
	INC R1
	INC R2
	INC R3
	INC R4
	INC R5
	INC R6
	INC R7

; 2x

	XCH A,@R0
	XCH A,@R1
	; 2
	MOV A,#n
	JMP $1FF
	EN TCNTI
	JNT0 *
	CLR A
	XCH A,R0
	XCH A,R1
	XCH A,R2
	XCH A,R3
	XCH A,R4
	XCH A,R5
	XCH A,R6
	XCH A,R7

; 3x

	XCHD A,@R0
	XCHD A,@R1
	JB1 *
	; 3
	CALL $1FF
	DIS TCNTI
	JT0 *
	CPL A
	; 8
	OUTL P1,A
	OUTL P2,A
	; B
	MOVD P4,A
	MOVD P5,A
	MOVD P6,A
	MOVD P7,A

; 4x

	ORL A,@R0
	ORL A,@R1
	MOV A,T
	ORL A,#n
	JMP $2FF
	STRT CNT
	JNT1 *
	SWAP A
	ORL A,R0
	ORL A,R1
	ORL A,R2
	ORL A,R3
	ORL A,R4
	ORL A,R5
	ORL A,R6
	ORL A,R7
; 5x

	ANL A,@R0
	ANL A,@R1
	JB2 *
	ANL A,#n
	CALL $2FF
	STRT T
	JT1 *
	DA A
	ANL A,R0
	ANL A,R1
	ANL A,R2
	ANL A,R3
	ANL A,R4
	ANL A,R5
	ANL A,R6
	ANL A,R7

; 6x

	ADD A,@R0
	ADD A,@R1
	MOV T,A
	; 3
	JMP $3FF
	STOP TCNT
	; 6
	RRC A
	ADD A,R0
	ADD A,R1
	ADD A,R2
	ADD A,R3
	ADD A,R4
	ADD A,R5
	ADD A,R6
	ADD A,R7

; 7x

	ADDC A,@R0
	ADDC A,@R1
	JB3 *
	; 3
	CALL $3FF
	ENT0 CLK
	JF1 *
	RR A
	ADDC A,R0
	ADDC A,R1
	ADDC A,R2
	ADDC A,R3
	ADDC A,R4
	ADDC A,R5
	ADDC A,R6
	ADDC A,R7

; 8x

	MOVX A,@R0
	MOVX A,@R1
	; 2
	RET
	JMP $4FF
	CLR F0
	JNI *
	; 7
	ORL BUS,#n
	ORL P1,#n
	ORL P2,#n
	; B
	ORLD P4,A
	ORLD P5,A
	ORLD P6,A
	ORLD P7,A

; 9x

	MOVX @R0,A
	MOVX @R1,A
	JB4 *
	RETR
	CALL $4FF
	CPL F0	
	JNZ *
	CLR C
	ANL BUS,#n
	ANL P1,#n
	ANL P2,#n
	; B
	ANLD P4,A
	ANLD P5,A
	ANLD P6,A
	ANLD P7,A

; Ax

	MOV @R0,A
	MOV @R1,A
	; 2
	MOVP A,@A
	JMP $5FF
	CLR F1
	; 6
	CPL C
	MOV R0,A
	MOV R1,A
	MOV R2,A
	MOV R3,A
	MOV R4,A
	MOV R5,A
	MOV R6,A
	MOV R7,A

; Bx

	MOV @R0,#n
	MOV @R1,#n
	JB5 *
	JMPP @A
	CALL $5FF
	CPL F1
	JF0 *
	; 7
	MOV R0,#n
	MOV R1,#n
	MOV R2,#n
	MOV R3,#n
	MOV R4,#n
	MOV R5,#n
	MOV R6,#n
	MOV R7,#n

; Cx

	; 0
	; 1
	; 2
	; 3
	JMP $6FF
	SEL RB0
	JZ *
	MOV A,PSW
	DEC R0
	DEC R1
	DEC R2
	DEC R3
	DEC R4
	DEC R5
	DEC R6
	DEC R7

; Dx

	XRL A,@R0
	XRL A,@R1
	JB6 *
	XRL A,#n
	CALL $6FF
	SEL RB1
	; 6
	MOV PSW,A
	XRL A,R0
	XRL A,R1
	XRL A,R2
	XRL A,R3
	XRL A,R4
	XRL A,R5
	XRL A,R6
	XRL A,R7

; Ex

	; 0
	; 1
	; 2
	MOVP3 A,@A
	JMP $7FF
	SEL MB0
	JNC *
	RL A
	DJNZ R0,*
	DJNZ R1,*
	DJNZ R2,*
	DJNZ R3,*
	DJNZ R4,*
	DJNZ R5,*
	DJNZ R6,*
	DJNZ R7,*
; Fx

	MOV A,@R0
	MOV A,@R1
	JB7 *
	; 3
	CALL $7FF
	SEL MB1
	JC *
	RLC A
	MOV A,R0
	MOV A,R1
	MOV A,R2
	MOV A,R3
	MOV A,R4
	MOV A,R5
	MOV A,R6
	MOV A,R7

	end
