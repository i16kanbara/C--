.L1
	STRING	"reloc: %04x -> %04x (%04x) | %04x\n"
.relocateSegment
	ENTRY	4
	LDC	0
	STL	1
	POP
.L2
	LDL	1
	LDA	2
	LT
	JF	.L3
	LDL	1
	ARG
	LDA	1
	ARG
	CALLF	2,.__AddrAdd
	STL	2
	POP
	LDL	2
	LDC	1
	LDW
	LDC	16383
	BAND
	LDC	4
	MUL
	STL	3
	POP
	LDL	3
	ARG
	LDA	3
	ARG
	CALLF	2,.__AddrAdd
	STL	4
	POP
	LDL	4
	LDC	1
	LDW
	LDA	4
	LDL	2
	LDC	0
	LDW
	STW
	POP
	LDA	4
	LDL	2
	LDC	0
	LDW
	LDW
	ARG
	LDL	3
	ARG
	LDL	4
	LDC	1
	LDW
	ARG
	LDL	2
	LDC	0
	LDW
	ARG
	LDC	.L1
	ARG
	CALLF	5,.printf
	POP
	LDL	1
	LDC	4
	ADD
	STL	1
	POP
	JMP	.L2
.L3
	LDC	0
	MREG
	RET
