__alcAddr	WS	1
_environ	WS	1
.a	WS	1
.b	DW	48
.d	WS	1
.e	WS	1
.f	DW	10
.g	DW	20
.h	WS	1
.L1	DW	1000
	DW	2000
	DW	3000
.i	DW	.L1
.j	WS	1
.k	WS	1
.L2	DW	4000
	DW	5000
.L3	DW	6000
	DW	7000
	DW	8000
.L4	DW	.L2
	DW	.L3
.l	DW	.L4
.L5	STRING	"aaaa"
.n	DW	.L5
.L6	STRING	"abc"
.L7	DB	97
	DB	98
	DB	0
.L8	DW	.L6
	DW	.L7
.o	DW	.L8
.L9	DW	6
.L10	DW	7
	DW	8
.L11	DW	9
.L12	WS	10
.L13	DW	.L9
	DW	.L10
	DW	.L11
.L14	DW	.L12
	DW	.L13
.p	DW	.L14
.L15	DW	6
.L16	DW	7
	DW	8
.L17	DW	9
.L18	DW	.L15
	DW	.L16
	DW	.L17
.L19	DW	.L18
.L20	DW	.L19
	DW	0
.L21	DW	.L20
	DW	0
.pq	DW	.L21
.L22	STRING	"abc"
.f0	PUSH	FP
	LD	FP,SP
	PUSH	G3
	PUSH	G4
	PUSH	G5
	CALL	__stkChk
	LD	G3,#1
	LD	G5,#.L22
	LD	G0,.h
	ADDS	G0,#1
	LD	G1,#1
	ST	G1,0,G0
	LD	G0,0,G4
	ADDS	G0,#1
	LD	G0,0,G0
	ADDS	G0,#1
	LD	G0,0,G0
	ADDS	G0,#3
	LD	G1,#2
	ST	G1,0,G0
	POP	G5
	POP	G4
	POP	G3
	POP	FP
	RET
.L23	STRING	"ab"
.L24	STRING	"cd"
.L25	DW	.L23
	DW	.L24
.L26	WS	3
.L27	DW	.L25
	DW	.L26
.q	DW	.L27
.L28	DW	1
	DW	2
.L29	DW	3
	DW	4
	DW	5
.L30	DW	6
.L31	DW	7
	DW	8
.L32	DW	9
.L33	DW	.L28
	DW	.L29
.L34	DW	.L30
	DW	.L31
	DW	.L32
.L35	DW	.L33
	DW	.L34
.r	DW	.L35
.L36	BS	3
.s	DW	.L36
.L37	BS	2
.L38	BS	2
.L39	BS	2
.L40	DW	.L37
	DW	.L38
	DW	.L39
.t	DW	.L40
.L41	BS	2
.L42	BS	2
.L43	BS	2
.L44	BS	2
.L45	DW	.L41
	DW	.L42
.L46	DW	.L43
	DW	.L44
.L47	DW	.L45
	DW	.L46
.u	DW	.L47
.L48	STRING	"ab"
.L49	STRING	"cd"
.L50	DW	.L48
	DW	.L49
.L51	BS	2
.L52	BS	2
.L53	DW	.L51
	DW	.L52
.L54	DW	.L50
	DW	.L53
.v	DW	.L54
.f1	PUSH	FP
	LD	FP,SP
	PUSH	G3
	PUSH	G4
	PUSH	G5
	PUSH	G6
	PUSH	G7
	CALL	__stkChk
	LD	G3,#1
	LD	G0,#20
	PUSH	G0
	CALL	_malloc
	ADD	SP,#2
	LD	G6,G0
	LD	G0,0,G6
	CMP	G0,#0
	JNZ	.L55
	LD	G0,G6
	ADDS	G0,#1
	LD	G1,#1
	ST	G1,0,G0
	JMP	.L56
.L55
	LD	G0,G6
	ADDS	G0,#1
	LD	G1,#0
	ST	G1,0,G0
.L56
	LD	G0,0,G6
	CMP	G0,#1
	JNZ	.L57
	LD	G0,G6
	ADDS	G0,#1
	LD	G1,#1
	ST	G1,0,G0
.L57
	CMP	G3,#1
	JNZ	.L58
	LD	G3,#2
.L58
	CMP	G3,#0
	JZ	.L59
	LD	G3,#3
	JMP	.L60
.L59
	LD	G3,#4
.L60
	CMP	G3,#1
	JZ	.L61
	LD	G3,#5
	LD	G4,#6
	JMP	.L62
.L61
	LD	G3,#7
	LD	G4,#8
.L62
.L63
	CMP	G3,#0
	JNZ	.L64
	LD	G7,#1
	LD	G0,G7
	ADD	G0,#1
	LD	G7,G0
	JMP	.L63
.L64
	LD	G3,#0
.L65
	CMP	G3,#3
	JGE	.L66
	LD	G0,G6
	ADDS	G0,G3
	ST	G3,0,G0
	LD	G0,G3
	ADD	G0,#1
	LD	G3,G0
	JMP	.L65
.L66
.L67
	CMP	G3,#2
	JZ	.L68
	CMP	G4,#2
	JZ	.L68
	CMP	G5,#2
	JNZ	.L69
.L68
	CMP	G5,#1
	JNZ	.L70
	JMP	.L67
.L70
.L71
	CMP	G4,#2
	JNZ	.L72
	CMP	G5,#3
	JNZ	.L73
	JMP	.L72
.L73
	JMP	.L71
.L72
	CMP	G5,#4
	JNZ	.L74
	JMP	.L69
.L74
	JMP	.L67
.L69
	PUSH	G6
	CALL	_free
	ADD	SP,#2
	LD	G0,G5
	POP	G7
	POP	G6
	POP	G5
	POP	G4
	POP	G3
	POP	FP
	RET
.L75	DB	1
	DB	2
.L76	DB	3
	DB	4
.L77	DW	.L75
	DW	.L76
.ab	DW	.L77
.f2	PUSH	FP
	LD	FP,SP
	PUSH	G3
	PUSH	G4
	CALL	__stkChk
	LD	G3,#1
	LD	G4,#2
	PUSH	G4
	CMP	G3,#1
	JNZ	.L79
	CMP	G4,#1
	LD	G0,#1
	JZ	.L78
.L79
	LD	G0,#0
.L78
	PUSH	G0
	CALL	.f1
	ADD	SP,#4
	LD	G0,.ab
	ADDS	G0,#1
	LD	G0,0,G0
	POP	G4
	POP	G3
	POP	FP
	RET
.f3	PUSH	FP
	LD	FP,SP
	CALL	__stkChk
.L80
	JMP	.L80
	POP	FP
	RET
.f4	PUSH	FP
	LD	FP,SP
	PUSH	G3
	PUSH	G4
	CALL	__stkChk
	LD	G3,#0
	LD	G4,#1
.L81
	CMP	G3,#10
	JGE	.L82
	LD	G0,G4
	ADD	G0,#1
	SHLA	G0,#1
	LD	G3,G0
	LD	G0,G3
	ADD	G0,#1
	LD	G3,G0
	JMP	.L81
.L82
	POP	G4
	POP	G3
	POP	FP
	RET
.L83	WS	5
.x	DW	.L83
.L84	WS	5
.L85	WS	5
.L86	WS	5
.L87	WS	5
.L88	WS	5
.L89	DW	.L84
	DW	.L85
	DW	.L86
	DW	.L87
	DW	.L88
.y	DW	.L89
.f5	PUSH	FP
	LD	FP,SP
	CALL	__stkChk
	LD	G0,.i
	ADDS	G0,#5
	LD	G1,#1
	ST	G1,0,G0
	LD	G0,.i
	ADDS	G0,4,FP
	ST	G1,0,G0
	LD	G0,.x
	ADDS	G0,#1
	LD	G2,.i
	ADDS	G2,0,G0
	LD	G2,0,G2
	ADD	G2,#1
	LD	G0,.x
	ADDS	G0,G2
	ST	G1,0,G0
	LD	G0,.y
	ADDS	G0,#1
	LD	G0,0,G0
	PUSH	G0
	CALL	.f2
	ADD	G0,#2
	LD	G1,4,FP
	MUL	G1,6,FP
	MUL	G1,@G0
	LD	G0,.x
	ADDS	G0,#3
	MUL	G1,0,G0
	POP	G0
	ADDS	G0,G1
	LD	G0,0,G0
	ST	G0,6,FP
	POP	FP
	RET
.z1	WS	1
.L90	DW	1
	DW	2
	DW	0
.z2	DW	.L90
.L91	DW	1
	DW	2
	DW	0
.L92	DW	3
	DW	4
	DW	0
.L93	DW	.L91
	DW	.L92
.z3	DW	.L93
.L94	STRING	"ab"
.L95	DW	1
	DW	2
	DW	.L94
.z4	DW	.L95
.z5	DW	0
.L96	STRING	"abc"
.L97	DW	1
	DW	2
	DW	.L96
.L98	DW	0
	DW	.L97
	DW	0
.z6	DW	.L98
.L99	WS	3
.z7	DW	.L99
.L100	WS	3
.L101	WS	3
.L102	DW	.L100
	DW	.L101
.z8	DW	.L102
.L103	WS	2
.z9	DW	.L103
.f6	PUSH	FP
	LD	FP,SP
	CALL	__stkChk
	LD	G0,.z2
	ST	G0,.z1
	LD	G0,#6
	PUSH	G0
	CALL	_malloc
	ADD	SP,#2
	ST	G0,.z1
	LD	G0,.z1
	PUSH	G0
	CALL	_free
	ADD	SP,#2
	LD	G0,.z3
	ADDS	G0,#3
	LD	G0,0,G0
	ST	G0,.z1
	LD	G0,.z3
	ADDS	G0,#3
	LD	G0,0,G0
	LD	G0,0,G0
	ST	G0,.a
	LD	G0,.z3
	ADDS	G0,#3
	LD	G0,0,G0
	ADDS	G0,#2
	LD	G0,0,G0
	ADDS	G0,#1
	LD	G0,0,G0
	ST	G0,.z1
	LD	G0,.z2
	ADDS	G0,#2
	LD	G0,0,G0
	ADDS	G0,#1
	LD	G0,0,G0
	ADDS	G0,#2
	LD	G0,0,G0
	ADDS	G0,#1
	LD	G0,0,G0
	LD	G0,0,G0
	ST	G0,.a
	LD	G0,#6
	PUSH	G0
	CALL	_malloc
	ADD	SP,#2
	LD	G1,.z8
	ADDS	G1,#1
	LD	G1,0,G1
	ADDS	G1,#1
	ST	G0,0,G1
	LD	G0,.z8
	ADDS	G0,#1
	LD	G0,0,G0
	ADDS	G0,#1
	LD	G0,0,G0
	LD	G1,#1
	ST	G1,0,G0
	LD	G0,.z8
	PUSH	G0
	CALL	_free
	ADD	SP,#2
	POP	FP
	RET
.L104	STRING	"aaa"
.L105	STRING	"aaa"
.L106	STRING	"bbb"
.f11	PUSH	FP
	LD	FP,SP
	PUSH	G3
	PUSH	G4
	PUSH	G5
	CALL	__stkChk
	LD	G0,#1
	PUSH	G0
	CALL	_f7
	ADD	SP,#2
	ST	G0,.z1
	LD	G0,#1
	PUSH	G0
	CALL	_f7
	ADD	SP,#2
	ADDS	G0,#1
	LD	G0,0,G0
	ST	G0,4,FP
	LD	G0,.z1
	PUSH	G0
	CALL	_f8
	ADD	SP,#2
	ADDS	G0,#1
	LD	G0,0,G0
	ST	G0,.z1
	LD	G0,.z3
	PUSH	G0
	CALL	_f9
	ADD	SP,#2
	ADDS	G0,#1
	LD	G0,0,G0
	ST	G0,.z1
	LD	G0,#0
	ST	G0,.z1
	LD	G0,#.L104
	PUSH	G0
	CALL	_f10
	ADD	SP,#2
	LD	G0,#.L106
	PUSH	G0
	LD	G0,#2
	PUSH	G0
	LD	G0,#.L105
	PUSH	G0
	CALL	_f10
	ADD	SP,#6
	PUSH	G3
	LD	G0,4,FP
	PUSH	G0
	PUSH	G3
	LD	G0,4,FP
	PUSH	G0
	PUSH	G3
	CALL	.f11
	ADD	SP,#10
	LD	G0,.z6
	ADDS	G0,#3
	LD	G1,.z6
	ADDS	G1,#2
	LD	G0,0,G0
	ST	G0,0,G1
	LD	G1,.z6
	ADDS	G1,#1
	ST	G0,0,G1
	LD	G0,.y
	ADDS	G0,#2
	LD	G1,.y
	ADDS	G1,#1
	LD	G0,0,G0
	ST	G0,0,G1
	ADDS	G0,#1
	LD	G1,#1
	ST	G1,0,G0
	LD	G4,#0
	LD	G5,#0
.L107
	CMP	G4,G5
	JNZ	.L108
	LD	G0,G5
	ADD	G0,#1
	LD	G5,G0
	LD	G0,G4
	ADD	G0,#1
	LD	G4,G0
	JMP	.L107
.L108
	POP	G5
	POP	G4
	POP	G3
	POP	FP
	RET
.L109	STRING	"aaa"
.ss	DW	.L109
.f12	PUSH	FP
	LD	FP,SP
	PUSH	G3
	PUSH	G4
	CALL	__stkChk
	LD	G0,#8
	PUSH	G0
	CALL	_malloc
	ADD	SP,#2
	LD	G3,G0
	PUSH	G3
	CALL	_free
	ADD	SP,#2
	LD	G4,#1
	POP	G4
	POP	G3
	POP	FP
	RET
.f13	PUSH	FP
	LD	FP,SP
	PUSH	G3
	PUSH	G4
	PUSH	G5
	PUSH	G6
	CALL	__stkChk
	LD	G4,#10
	LD	G5,#18
	LD	G6,#99
	LD	G3,.ss
	ST	G3,.z1
	POP	G6
	POP	G5
	POP	G4
	POP	G3
	POP	FP
	RET
