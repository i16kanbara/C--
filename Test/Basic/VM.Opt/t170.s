.a
	WS	1
.b
	WS	1
.c
	WS	1
.d
	WS	1
.e
	WS	1
.f
	DW	10
.g
	DW	20
.h
	WS	1
.L1
	DW	1000
	DW	2000
	DW	3000
.i
	DW	.L1
.j
	WS	1
.k
	WS	1
.L2
	DW	4000
	DW	5000
.L3
	DW	6000
	DW	7000
	DW	8000
.L4
	DW	.L2
	DW	.L3
.l
	DW	.L4
.L5
	DW	1
	DW	2
.L6
	DW	3
	DW	4
	DW	5
.L7
	DW	6
.L8
	DW	7
	DW	8
.L9
	DW	9
.L10
	DW	.L5
	DW	.L6
.L11
	DW	.L7
	DW	.L8
	DW	.L9
.L12
	DW	.L10
	DW	.L11
.m
	DW	.L12
.L13
	STRING	"aaaa"
.n
	DW	.L13
.L14
	STRING	"abc"
.L15
	DB	97
	DB	98
	DB	0
.L16
	DW	.L14
	DW	.L15
.o
	DW	.L16
.L17
	DW	6
.L18
	DW	7
	DW	8
.L19
	DW	9
.L20
	WS	10
.L21
	DW	.L17
	DW	.L18
	DW	.L19
.L22
	DW	.L20
	DW	.L21
.p
	DW	.L22
.pq
	WS	1
.f1
	ENTRY	1
	LDC	1
	STL	1
	POP
	RET
.L23
	STRING	"ab"
.L24
	STRING	"cd"
.L25
	DW	.L23
	DW	.L24
.L26
	WS	3
.L27
	DW	.L25
	DW	.L26
.q
	DW	.L27
.L28
	STRING	"ABCDE"
.f2
	ENTRY	5
	LDC	10
	ARG
	CALLF	1,.malloc
	STL	3
	POP
	LDA	1
	STL	4
	POP
	LDA	1
	STL	5
	POP
	LDC	30
	STL	4
	POP
	LDC	.L28
	STL	5
	POP
	LDC	10
	STL	1
	POP
	LDL	1
	CHAR
	STG	.b
	POP
	LDG	.b
	STL	2
	POP
	LDL	2
	LDG	.i
	LDL	1
	STW
	POP
	LDC	10
	STL	2
	STL	1
	POP
	LDC	10
	STL	2
	LDG	.i
	LDL	1
	STW
	POP
	LDC	20
	STL	2
	LDG	.i
	LDL	2
	STW
	LDG	.i
	LDL	1
	STW
	POP
	LDG	.l
	LDC	1
	LDW
	LDC	2
	LDW
	STL	1
	POP
	LDG	.l
	LDG	.i
	LDC	5
	LDW
	LDW
	LDG	.i
	LDL	1
	LDW
	LDW
	STL	1
	POP
	LDC	20
	LDG	.i
	LDL	1
	STW
	LDG	.l
	LDG	.i
	LDC	5
	LDW
	LDW
	LDG	.i
	LDL	1
	LDW
	STW
	POP
	LDL	1
	MREG
	RET
.f3
	ENTRY	1
	LDC	1
	STL	1
	POP
	LDL	1
	MREG
	RET
.f4
	ENTRY	22
	LDG	.a
	LDG	.b
	BOR
	STL	1
	POP
	LDG	.a
	LDG	.b
	MUL
	LDC	2
	BXOR
	STL	2
	POP
	LDG	.a
	LDG	.b
	BAND
	STL	3
	POP
	LDG	.a
	LDG	.b
	EQ
	STL	4
	POP
	LDG	.a
	CHAR
	LDG	.b
	NE
	STL	5
	POP
	LDG	.a
	LDG	.b
	LT
	STL	6
	POP
	LDG	.a
	LDG	.b
	LE
	STL	7
	POP
	LDG	.a
	LDG	.b
	GT
	STL	8
	POP
	LDG	.a
	LDG	.b
	GE
	STL	9
	POP
	LDG	.a
	LDG	.b
	SHL
	STL	10
	POP
	LDG	.a
	LDG	.b
	SHR
	STL	11
	POP
	LDG	.a
	LDG	.b
	ADD
	STL	12
	POP
	LDG	.a
	LDG	.b
	SUB
	STL	13
	POP
	LDG	.a
	LDG	.b
	MUL
	STL	14
	POP
	LDG	.a
	LDG	.b
	DIV
	STL	15
	POP
	LDG	.a
	LDG	.b
	MOD
	STL	16
	POP
	LDG	.a
	NEG
	STL	17
	POP
	LDG	.a
	STL	18
	POP
	LDL	4
	NOT
	STL	19
	POP
	LDG	.a
	BNOT
	STL	20
	POP
	LDG	.a
	LDG	.b
	MUL
	LDC	6
	ADD
	LDG	.c
	LDG	.d
	MUL
	ADD
	STL	21
	POP
	LDL	4
	JF	.L30
	LDL	5
; BOOLAND .L30,.L-1,.L29
	JMP	.L29
.L30
	LDC	0
.L29
; ----
	STL	22
	POP
	LDC	2
	LDG	.pq
	LDC	2
	STW
	POP
	RET
