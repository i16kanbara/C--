.a	DW	1
_b	DW	2
.c	DW	3
.L1	BS	5
_x	DW	.L1
.L2	BS	5
.L3	BS	5
.L4	BS	5
.L5	BS	5
.L6	BS	5
.L7	DW	.L2
	DW	.L3
	DW	.L4
	DW	.L5
	DW	.L6
.y	DW	.L7
.f1	PUSH	FP
	LD	FP,SP
	CALL	__stkChk
	LD	G0,.y
	ADDS	G0,#1
	LD	G0,0,G0
	POP	FP
	RET
.f2	PUSH	FP
	LD	FP,SP
	CALL	__stkChk
	LD	G0,.y
	ADDS	G0,#1
	LD	G0,0,G0
	PUSH	G0
	CALL	.f1
	ADD	G0,#2
	LD	G0,@G0
	MUL	G0,.c
	LD	G1,.a
	MUL	G1,_b
	ADD	G0,G1
	POP	G1
	ADD	G1,G0
	LD	G1,@G1
	ST	G1,.c
	POP	FP
	RET
