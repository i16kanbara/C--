.L1
	WS	2
.L2
	WS	2
.L3
	DW	.L1
	DW	.L2
.a
	DW	.L3
.L4
	WS	2
.b
	DW	.L4
.t
	WS	1
.main
	ENTRY	0
	LDC	2
	LDG	.a
	LDC	1
	LDW
	LDC	2
	STW
	POP
	LDC	2
	LDG	.t
	LDC	1
	LDW
	LDC	1
	STW
	POP
	LDC	2
	LDG	.t
	LDC	2
	LDW
	LDC	2
	LDW
	LDC	0
	STW
	POP
	LDC	1
	CALLF	0,.f
	LDC	2
	STW
	POP
	LDC	2
	CALLF	0,.f
	STG	.b
	LDC	4
	STW
	POP
	RET
