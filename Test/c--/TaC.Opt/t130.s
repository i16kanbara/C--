.x	DW	0
.y	WS	1
.f	PUSH	FP
	LD	FP,SP
	PUSH	G3
	PUSH	G4
	PUSH	G5
	CALL	__stkChk
	CMP	G3,G4
	LD	G0,#1
	JZ	.T0
	LD	G0,#0
.T0
	LD	G1,.x
	CMP	G1,.y
	LD	G1,#1
	JZ	.T1
	LD	G1,#0
.T1
	CMP	G0,G1
	JNZ	.L1
	LD	G0,G3
	AND	G0,#0x00ff
	JMP	.L2
.L1
	CMP	G5,G5
	JNZ	.L3
	LD	G0,G3
	AND	G0,#0x00ff
	JMP	.L2
.L3
	LD	G0,G3
	SHLA	G0,#1
	LD	G4,G0
	LD	G3,G0
	LD	G3,#0
	LD	G4,#0
.L4
	CMP	G3,#10
	JGE	.L5
	LD	G0,G4
	SHLA	G0,#1
	LD	G4,G0
	LD	G0,G3
	ADD	G0,#1
	LD	G3,G0
	JMP	.L4
.L5
	LD	G4,#0
	LD	G3,G4
.L6
	CMP	G3,#10
	JGE	.L7
	LD	G0,G4
	SHLA	G0,#1
	LD	G4,G0
	LD	G0,G3
	ADD	G0,#1
	LD	G3,G0
	JMP	.L6
.L7
	LD	G0,G3
	AND	G0,#0x00ff
.L2
	POP	G5
	POP	G4
	POP	G3
	POP	FP
	RET
