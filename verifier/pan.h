#ifndef PAN_H
#define PAN_H

#define SpinVersion	"Spin Version 6.4.5 -- 1 January 2016"
#define PanSource	"output.pml"

#define G_long	8
#define G_int	4

#define ulong	unsigned long
#define ushort	unsigned short
#ifdef WIN64
	#define ONE_L	(1L)
/*	#define long	long long */
#else
	#define ONE_L	(1L)
#endif

#ifdef BFS_PAR
	#define NRUNS	0
	#ifndef BFS
		#define BFS
	#endif
	#ifndef PUTPID
		#define PUTPID
	#endif

	#if !defined(USE_TDH) && !defined(NO_TDH)
		#define USE_TDH
	#endif
	#if defined(USE_TDH) && !defined(NO_HC)
		#define HC /* default for USE_TDH */
	#endif
	#ifndef BFS_MAXPROCS
		#define BFS_MAXPROCS	64	/* max nr of cores to use */
	#endif
	#define BFS_GLOB	0	/* global lock */
	#define BFS_ORD		1	/* used with -DCOLLAPSE */
	#define BFS_MEM		2	/* malloc from shared heap */
	#define BFS_PRINT	3	/* protect printfs */
	#define BFS_STATE	4	/* hashtable */

	#define BFS_INQ 	2	/* state is in q */

	#ifdef BFS_FIFO
	  #define BFS_ID(a,b)	(BFS_STATE + (int) ((a)*BFS_MAXPROCS+(b)))
	  #define BFS_MAXLOCKS	(BFS_STATE + (BFS_MAXPROCS*BFS_MAXPROCS))
	#else
	  #ifndef BFS_W
		#define BFS_W	10
	  #endif
	  #define BFS_MASK	((1<<BFS_W) - 1)
	  #define BFS_ID	(BFS_STATE + (int) (j1_spin & (BFS_MASK)))
	  #define BFS_MAXLOCKS	(BFS_STATE + (1<<BFS_W))
	#endif
	#undef NCORE
	extern int Cores, who_am_i;
	#ifndef SAFETY
	  #if !defined(BFS_STAGGER) && !defined(BFS_DISK)
		#define BFS_STAGGER	64 /* randomizer, was 16 */
	  #endif
	  #ifndef L_BOUND
		#define L_BOUND 	10 /* default */
	  #endif
	  extern int L_bound;
	#endif
	#if defined(BFS_DISK) && defined(BFS_STAGGER)
		#error BFS_DISK and BFS_STAGGER are not compatible
	#endif
#endif

#if defined(BFS)
	#ifndef SAFETY
		#define SAFETY
	#endif
	#ifndef XUSAFE
		#define XUSAFE
	#endif
#endif
#ifndef uchar
	#define uchar	unsigned char
#endif
#ifndef uint
	#define uint	unsigned int
#endif
#define DELTA	500
#ifdef MA
	#if NCORE>1 && !defined(SEP_STATE)
		#define SEP_STATE
	#endif
	#if MA==1
		#undef MA
		#define MA	100
	#endif
#endif
#ifdef W_XPT
	#if W_XPT==1
		#undef W_XPT
		#define W_XPT 1000000
	#endif
#endif
#ifndef NFAIR
	#define NFAIR	2	/* must be >= 2 */
#endif
#define HAS_CODE	1
#if defined(RANDSTORE) && !defined(RANDSTOR)
	#define RANDSTOR	RANDSTORE
#endif
#define MERGED	1
#if !defined(HAS_LAST) && defined(BCS)
	#define HAS_LAST	1 /* use it, but */
	#ifndef STORE_LAST
		#define NO_LAST	1 /* dont store it */
	#endif
#endif
#if defined(BCS) && defined(BITSTATE)
	#ifndef NO_CTX
		#define STORE_CTX	1
	#endif
#endif
#ifdef NP
	#define HAS_NP	2
	#define VERI	26	/* np_ */
#endif
#ifndef NOCLAIM
	#define NCLAIMS	1
	#ifndef NP
		#define VERI	25
	#endif
#endif

typedef struct S_F_MAP {
	char *fnm;
	int from;
	int upto;
} S_F_MAP;

#define nstates25	20	/* never_0 */
#define minseq25	3112
#define maxseq25	3130
#define endstate25	19

#define nstates24	2771	/* :init: */
#define minseq24	342
#define maxseq24	3111
#define endstate24	2770

#define nstates23	12	/* get_val_1_14 */
#define minseq23	331
#define maxseq23	341
#define endstate23	11

#define nstates22	12	/* get_val_1_13 */
#define minseq22	320
#define maxseq22	330
#define endstate22	11

#define nstates21	12	/* get_val_1_12 */
#define minseq21	309
#define maxseq21	319
#define endstate21	11

#define nstates20	12	/* get_val_1_11 */
#define minseq20	298
#define maxseq20	308
#define endstate20	11

#define nstates19	12	/* get_val_1_10 */
#define minseq19	287
#define maxseq19	297
#define endstate19	11

#define nstates18	11	/* get_val_1_9 */
#define minseq18	277
#define maxseq18	286
#define endstate18	10

#define nstates17	19	/* get_val_1_8 */
#define minseq17	259
#define maxseq17	276
#define endstate17	18

#define nstates16	19	/* get_val_1_7 */
#define minseq16	241
#define maxseq16	258
#define endstate16	18

#define nstates15	19	/* get_val_1_6 */
#define minseq15	223
#define maxseq15	240
#define endstate15	18

#define nstates14	19	/* get_val_1_5 */
#define minseq14	205
#define maxseq14	222
#define endstate14	18

#define nstates13	19	/* get_val_1_4 */
#define minseq13	187
#define maxseq13	204
#define endstate13	18

#define nstates12	19	/* get_val_1_3 */
#define minseq12	169
#define maxseq12	186
#define endstate12	18

#define nstates11	12	/* get_val_1_2 */
#define minseq11	158
#define maxseq11	168
#define endstate11	11

#define nstates10	14	/* get_val_1_1 */
#define minseq10	145
#define maxseq10	157
#define endstate10	13

#define nstates9	11	/* get_val_1_0 */
#define minseq9	135
#define maxseq9	144
#define endstate9	10

#define nstates8	12	/* get_val_0_8 */
#define minseq8	124
#define maxseq8	134
#define endstate8	11

#define nstates7	19	/* get_val_0_7 */
#define minseq7	106
#define maxseq7	123
#define endstate7	18

#define nstates6	19	/* get_val_0_6 */
#define minseq6	88
#define maxseq6	105
#define endstate6	18

#define nstates5	19	/* get_val_0_5 */
#define minseq5	70
#define maxseq5	87
#define endstate5	18

#define nstates4	19	/* get_val_0_4 */
#define minseq4	52
#define maxseq4	69
#define endstate4	18

#define nstates3	19	/* get_val_0_3 */
#define minseq3	34
#define maxseq3	51
#define endstate3	18

#define nstates2	12	/* get_val_0_2 */
#define minseq2	23
#define maxseq2	33
#define endstate2	11

#define nstates1	14	/* get_val_0_1 */
#define minseq1	10
#define maxseq1	22
#define endstate1	13

#define nstates0	11	/* get_val_0_0 */
#define minseq0	0
#define maxseq0	9
#define endstate0	10

extern short src_ln25[];
extern short src_ln24[];
extern short src_ln23[];
extern short src_ln22[];
extern short src_ln21[];
extern short src_ln20[];
extern short src_ln19[];
extern short src_ln18[];
extern short src_ln17[];
extern short src_ln16[];
extern short src_ln15[];
extern short src_ln14[];
extern short src_ln13[];
extern short src_ln12[];
extern short src_ln11[];
extern short src_ln10[];
extern short src_ln9[];
extern short src_ln8[];
extern short src_ln7[];
extern short src_ln6[];
extern short src_ln5[];
extern short src_ln4[];
extern short src_ln3[];
extern short src_ln2[];
extern short src_ln1[];
extern short src_ln0[];
extern S_F_MAP src_file25[];
extern S_F_MAP src_file24[];
extern S_F_MAP src_file23[];
extern S_F_MAP src_file22[];
extern S_F_MAP src_file21[];
extern S_F_MAP src_file20[];
extern S_F_MAP src_file19[];
extern S_F_MAP src_file18[];
extern S_F_MAP src_file17[];
extern S_F_MAP src_file16[];
extern S_F_MAP src_file15[];
extern S_F_MAP src_file14[];
extern S_F_MAP src_file13[];
extern S_F_MAP src_file12[];
extern S_F_MAP src_file11[];
extern S_F_MAP src_file10[];
extern S_F_MAP src_file9[];
extern S_F_MAP src_file8[];
extern S_F_MAP src_file7[];
extern S_F_MAP src_file6[];
extern S_F_MAP src_file5[];
extern S_F_MAP src_file4[];
extern S_F_MAP src_file3[];
extern S_F_MAP src_file2[];
extern S_F_MAP src_file1[];
extern S_F_MAP src_file0[];

#define T_ID	unsigned short
#define _T5	965
#define _T2	966
#define WS		8 /* word size in bytes */
#define SYNC	24
#define ASYNC	0

#ifndef NCORE
	#ifdef DUAL_CORE
		#define NCORE	2
	#elif QUAD_CORE
		#define NCORE	4
	#else
		#define NCORE	1
	#endif
#endif

typedef struct P25 { /* never_0 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
} P25;
#define Air25	(sizeof(P25) - 4)

#define Pinit	((P24 *)this)
typedef struct P24 { /* :init: */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
} P24;
#define Air24	(sizeof(P24) - 4)

#define Pget_val_1_14	((P23 *)this)
typedef struct P23 { /* get_val_1_14 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P23;
#define Air23	(sizeof(P23) - Offsetof(P23, _msg) - 1*sizeof(uchar))

#define Pget_val_1_13	((P22 *)this)
typedef struct P22 { /* get_val_1_13 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P22;
#define Air22	(sizeof(P22) - Offsetof(P22, _msg) - 1*sizeof(uchar))

#define Pget_val_1_12	((P21 *)this)
typedef struct P21 { /* get_val_1_12 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P21;
#define Air21	(sizeof(P21) - Offsetof(P21, _msg) - 1*sizeof(uchar))

#define Pget_val_1_11	((P20 *)this)
typedef struct P20 { /* get_val_1_11 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P20;
#define Air20	(sizeof(P20) - Offsetof(P20, _msg) - 1*sizeof(uchar))

#define Pget_val_1_10	((P19 *)this)
typedef struct P19 { /* get_val_1_10 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P19;
#define Air19	(sizeof(P19) - Offsetof(P19, _msg) - 1*sizeof(uchar))

#define Pget_val_1_9	((P18 *)this)
typedef struct P18 { /* get_val_1_9 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P18;
#define Air18	(sizeof(P18) - Offsetof(P18, _msg) - 1*sizeof(uchar))

#define Pget_val_1_8	((P17 *)this)
typedef struct P17 { /* get_val_1_8 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P17;
#define Air17	(sizeof(P17) - Offsetof(P17, _msg) - 1*sizeof(uchar))

#define Pget_val_1_7	((P16 *)this)
typedef struct P16 { /* get_val_1_7 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P16;
#define Air16	(sizeof(P16) - Offsetof(P16, _msg) - 1*sizeof(uchar))

#define Pget_val_1_6	((P15 *)this)
typedef struct P15 { /* get_val_1_6 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P15;
#define Air15	(sizeof(P15) - Offsetof(P15, _msg) - 1*sizeof(uchar))

#define Pget_val_1_5	((P14 *)this)
typedef struct P14 { /* get_val_1_5 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P14;
#define Air14	(sizeof(P14) - Offsetof(P14, _msg) - 1*sizeof(uchar))

#define Pget_val_1_4	((P13 *)this)
typedef struct P13 { /* get_val_1_4 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P13;
#define Air13	(sizeof(P13) - Offsetof(P13, _msg) - 1*sizeof(uchar))

#define Pget_val_1_3	((P12 *)this)
typedef struct P12 { /* get_val_1_3 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P12;
#define Air12	(sizeof(P12) - Offsetof(P12, _msg) - 1*sizeof(uchar))

#define Pget_val_1_2	((P11 *)this)
typedef struct P11 { /* get_val_1_2 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P11;
#define Air11	(sizeof(P11) - Offsetof(P11, _msg) - 1*sizeof(uchar))

#define Pget_val_1_1	((P10 *)this)
typedef struct P10 { /* get_val_1_1 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P10;
#define Air10	(sizeof(P10) - Offsetof(P10, _msg) - 1*sizeof(uchar))

#define Pget_val_1_0	((P9 *)this)
typedef struct P9 { /* get_val_1_0 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P9;
#define Air9	(sizeof(P9) - Offsetof(P9, _msg) - 1*sizeof(uchar))

#define Pget_val_0_8	((P8 *)this)
typedef struct P8 { /* get_val_0_8 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P8;
#define Air8	(sizeof(P8) - Offsetof(P8, _msg) - 1*sizeof(uchar))

#define Pget_val_0_7	((P7 *)this)
typedef struct P7 { /* get_val_0_7 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P7;
#define Air7	(sizeof(P7) - Offsetof(P7, _msg) - 1*sizeof(uchar))

#define Pget_val_0_6	((P6 *)this)
typedef struct P6 { /* get_val_0_6 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P6;
#define Air6	(sizeof(P6) - Offsetof(P6, _msg) - 1*sizeof(uchar))

#define Pget_val_0_5	((P5 *)this)
typedef struct P5 { /* get_val_0_5 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P5;
#define Air5	(sizeof(P5) - Offsetof(P5, _msg) - 1*sizeof(uchar))

#define Pget_val_0_4	((P4 *)this)
typedef struct P4 { /* get_val_0_4 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P4;
#define Air4	(sizeof(P4) - Offsetof(P4, _msg) - 1*sizeof(uchar))

#define Pget_val_0_3	((P3 *)this)
typedef struct P3 { /* get_val_0_3 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P3;
#define Air3	(sizeof(P3) - Offsetof(P3, _msg) - 1*sizeof(uchar))

#define Pget_val_0_2	((P2 *)this)
typedef struct P2 { /* get_val_0_2 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P2;
#define Air2	(sizeof(P2) - Offsetof(P2, _msg) - 1*sizeof(uchar))

#define Pget_val_0_1	((P1 *)this)
typedef struct P1 { /* get_val_0_1 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P1;
#define Air1	(sizeof(P1) - Offsetof(P1, _msg) - 1*sizeof(uchar))

#define Pget_val_0_0	((P0 *)this)
typedef struct P0 { /* get_val_0_0 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	uchar _msg;
} P0;
#define Air0	(sizeof(P0) - Offsetof(P0, _msg) - 1*sizeof(uchar))

typedef struct P26 { /* np_ */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 6; /* proctype */
	unsigned _p   : 13; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
} P26;
#define Air26	(sizeof(P26) - 4)

#define Pclaim	P0
#ifndef NCLAIMS
	#define NCLAIMS 1
#endif
#if defined(BFS) && defined(REACH)
	#undef REACH
#endif
#ifdef VERI
	#define BASE	1
#else
	#define BASE	0
#endif
typedef struct Trans {
	short atom;	/* if &2 = atomic trans; if &8 local */
#ifdef HAS_UNLESS
	short escp[HAS_UNLESS];	/* lists the escape states */
	short e_trans;	/* if set, this is an escp-trans */
#endif
	short tpe[2];	/* class of operation (for reduction) */
	short qu[6];	/* for conditional selections: qid's  */
	uchar ty[6];	/* ditto: type's */
#ifdef NIBIS
	short om;	/* completion status of preselects */
#endif
	char *tp;	/* src txt of statement */
	int st;		/* the nextstate */
	int t_id;	/* transition id, unique within proc */
	int forw;	/* index forward transition */
	int back;	/* index return  transition */
	struct Trans *nxt;
} Trans;

#ifdef TRIX
	#define qptr(x)	(channels[x]->body)
	#define pptr(x)	(processes[x]->body)
#else
	#define qptr(x)	(((uchar *)&now)+(int)q_offset[x])
	#define pptr(x)	(((uchar *)&now)+(int)proc_offset[x])
#endif
extern uchar *Pptr(int);
extern uchar *Qptr(int);
#define q_sz(x)	(((Q0 *)qptr(x))->Qlen)

#ifdef TRIX
	#ifdef VECTORSZ
		#undef VECTORSZ
	#endif
	#if WS==4
		#define VECTORSZ	2056	/* ((MAXPROC+MAXQ+4)*sizeof(uchar *)) */
	#else
		#define VECTORSZ	4112	/* the formula causes probs in preprocessing */
	#endif
#else
	#ifndef VECTORSZ
		#define VECTORSZ	1024	/* sv size in bytes */
	#endif
#endif

#define MAXQ   	255
#define MAXPROC	255

#ifdef VERBOSE
	#ifndef CHECK
		#define CHECK
	#endif
	#ifndef DEBUG
		#define DEBUG
	#endif
#endif
#ifdef SAFETY
	#ifndef NOFAIR
		#define NOFAIR
	#endif
#endif
#ifdef NOREDUCE
	#ifndef XUSAFE
		#define XUSAFE
	#endif
	#if !defined(SAFETY) && !defined(MA)
		#define FULLSTACK
	#endif
#else
	#ifdef BITSTATE
		#if defined(SAFETY) && WS<=4
			#define CNTRSTACK
		#else
			#define FULLSTACK
		#endif
	#else
		#define FULLSTACK
	#endif
#endif
#ifdef BITSTATE
	#ifndef NOCOMP
		#define NOCOMP
	#endif
	#if !defined(LC) && defined(SC)
		#define LC
	#endif
#endif
#if defined(COLLAPSE2) || defined(COLLAPSE3) || defined(COLLAPSE4)
	/* accept the above for backward compatibility */
	#define COLLAPSE
#endif
#ifdef HC
	#undef HC
	#define HC4
#endif
#if defined(HC0) || defined(HC1) || defined(HC2) || defined(HC3) || defined(HC4)
	#define HC	4
#endif

typedef struct _Stack  {	 /* for queues and processes */
#if VECTORSZ>32000
	int o_delta;
	#ifndef TRIX
		int o_offset;
		int o_skip;
	#endif
	int o_delqs;
#else
	short o_delta;
	#ifndef TRIX
		short o_offset;
		short o_skip;
	#endif
	short o_delqs;
#endif
	short o_boq;
#ifdef TRIX
	short parent;
	char *b_ptr;
#else
	char *body;
#endif
#ifndef XUSAFE
	char *o_name;
#endif
	struct _Stack *nxt;
	struct _Stack *lst;
} _Stack;

typedef struct Svtack { /* for complete state vector */
#if VECTORSZ>32000
	int o_delta;
	int m_delta;
#else
	short o_delta;	 /* current size of frame */
	short m_delta;	 /* maximum size of frame */
#endif
#if SYNC
	short o_boq;
#endif
#define StackSize	(WS)
	char *body;
	struct Svtack *nxt;
	struct Svtack *lst;
} Svtack;

typedef struct State {
	uchar _nr_pr;
	uchar _nr_qs;
	uchar   _a_t;	/* cycle detection */
#ifndef NOFAIR
	uchar   _cnt[NFAIR];	/* counters, weak fairness */
#endif
#ifndef NOVSZ
#if VECTORSZ<65536
	unsigned short _vsz;
#else
	ulong  _vsz;
#endif
#endif
#ifdef HAS_LAST
	uchar  _last;	/* pid executed in last step */
#endif
#if defined(BITSTATE) && defined(BCS) && defined(STORE_CTX)
	uchar  _ctx;	/* nr of context switches so far */
#endif
#if defined(BFS_PAR) && defined(L_BOUND)
	uchar  _l_bnd;	/* bounded liveness */
	uchar *_l_sds;	/* seed state */
#endif
#ifdef EVENT_TRACE
	#if nstates_event<256
		uchar _event;
	#else
		unsigned short _event;
	#endif
#endif
	uchar running[2];
	uchar ready[2];
	uchar current;
	uchar S0;
	uchar S1;
	uchar S2;
	uchar S3;
	uchar S4;
	uchar S5;
	uchar S6;
	uchar S7;
	uchar CONST_NULL;
	uchar T0_X0;
	uchar T0_X1;
	uchar T0_X2;
	uchar T0_X3;
	uchar T0_X3_1;
	uchar T0_X3_2;
	uchar T0_X4;
	uchar T0_X4_1;
	uchar T0_X4_2;
	uchar T0_X5;
	uchar T0_X5_1;
	uchar T0_X5_2;
	uchar T0_X6;
	uchar T0_X6_1;
	uchar T0_X6_2;
	uchar T0_X7;
	uchar T0_X7_1;
	uchar T0_X7_2;
	uchar T0_X8;
	uchar T1_X0;
	uchar T1_X1;
	uchar T1_X2;
	uchar T1_X3;
	uchar T1_X3_1;
	uchar T1_X3_2;
	uchar T1_X4;
	uchar T1_X4_1;
	uchar T1_X4_2;
	uchar T1_X5;
	uchar T1_X5_1;
	uchar T1_X5_2;
	uchar T1_X6;
	uchar T1_X6_1;
	uchar T1_X6_2;
	uchar T1_X7;
	uchar T1_X7_1;
	uchar T1_X7_2;
	uchar T1_X8;
	uchar T1_X8_1;
	uchar T1_X8_2;
	uchar T1_X9;
	uchar T1_X10;
	uchar T1_X11;
	uchar T1_X12;
	uchar T1_X13;
	uchar T1_X14;
	uchar msg;
	uchar val_0_0;
	uchar val_0_1;
	uchar val_0_2;
	uchar val_0_3;
	uchar val_0_4;
	uchar val_0_5;
	uchar val_0_6;
	uchar val_0_7;
	uchar val_0_8;
	uchar val_1_0;
	uchar val_1_1;
	uchar val_1_2;
	uchar val_1_3;
	uchar val_1_4;
	uchar val_1_5;
	uchar val_1_6;
	uchar val_1_7;
	uchar val_1_8;
	uchar val_1_9;
	uchar val_1_10;
	uchar val_1_11;
	uchar val_1_12;
	uchar val_1_13;
	uchar val_1_14;
#ifdef TRIX
	/* room for 512 proc+chan ptrs, + safety margin */
	char *_ids_[MAXPROC+MAXQ+4];
#else
	uchar sv[VECTORSZ];
#endif
} State;

#ifdef TRIX
typedef struct TRIX_v6 {
	uchar *body; /* aligned */
#ifndef BFS
	short modified;
#endif
	short psize;
	short parent_pid;
	struct TRIX_v6 *nxt;
} TRIX_v6;
#endif

#define HAS_TRACK	0
/* hidden variable: */	uchar N0;
#define FORWARD_MOVES	"pan.m"
#define BACKWARD_MOVES	"pan.b"
#define TRANSITIONS	"pan.t"
#define _NP_	26
#define nstates26	3 /* np_ */
#define endstate26	2 /* np_ */

#define start26	0 /* np_ */
#define start25	5
#define start24	16
#define start23	8
#define start22	8
#define start21	8
#define start20	8
#define start19	8
#define start18	7
#define start17	15
#define start16	15
#define start15	15
#define start14	15
#define start13	15
#define start12	15
#define start11	8
#define start10	10
#define start9	7
#define start8	8
#define start7	15
#define start6	15
#define start5	15
#define start4	15
#define start3	15
#define start2	8
#define start1	10
#define start0	7
#ifdef NP
	#define ACCEPT_LAB	1 /* at least 1 in np_ */
#else
	#define ACCEPT_LAB	1 /* user-defined accept labels */
#endif
#ifdef MEMCNT
	#ifdef MEMLIM
		#warning -DMEMLIM takes precedence over -DMEMCNT
		#undef MEMCNT
	#else
		#if MEMCNT<20
			#warning using minimal value -DMEMCNT=20 (=1MB)
			#define MEMLIM	(1)
			#undef MEMCNT
		#else
			#if MEMCNT==20
				#define MEMLIM	(1)
				#undef MEMCNT
			#else
			 #if MEMCNT>=50
				#error excessive value for MEMCNT
			 #else
				#define MEMLIM	(1<<(MEMCNT-20))
			 #endif
			#endif
		#endif
	#endif
#endif
#if NCORE>1 && !defined(MEMLIM)
	#define MEMLIM	(2048)	/* need a default, using 2 GB */
#endif
#define PROG_LAB	0 /* progress labels */
#define NQS	24
typedef struct Q24 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q24;
typedef struct Q23 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q23;
typedef struct Q22 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q22;
typedef struct Q21 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q21;
typedef struct Q20 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q20;
typedef struct Q19 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q19;
typedef struct Q18 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q18;
typedef struct Q17 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q17;
typedef struct Q16 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q16;
typedef struct Q15 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q15;
typedef struct Q14 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q14;
typedef struct Q13 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q13;
typedef struct Q12 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q12;
typedef struct Q11 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q11;
typedef struct Q10 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q10;
typedef struct Q9 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q9;
typedef struct Q8 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q8;
typedef struct Q7 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q7;
typedef struct Q6 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q6;
typedef struct Q5 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q5;
typedef struct Q4 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q4;
typedef struct Q3 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q3;
typedef struct Q2 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q2;
typedef struct Q1 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		uchar fld0;
	} contents[1];
} Q1;
typedef struct Q0 {	/* generic q */
	uchar Qlen;	/* q_size */
	uchar _t;
} Q0;

/** function prototypes **/
char *emalloc(ulong);
char *Malloc(ulong);
int Boundcheck(int, int, int, int, Trans *);
int addqueue(int, int, int);
/* int atoi(char *); */
/* int abort(void); */
int close(int);
int delproc(int, int);
int endstate(void);
int find_claim(char *);
int h_store(char *, int);
int pan_rand(void);
int q_cond(short, Trans *);
int q_full(int);
int q_len(int);
int q_zero(int);
int qrecv(int, int, int, int);
int unsend(int);
/* void *sbrk(int); */
void spin_assert(int, char *, int, int, Trans *);
#ifdef BFS_PAR
void bfs_printf(const char *, ...);
volatile uchar *sh_pre_malloc(ulong);
#endif
void c_chandump(int);
void c_globals(void);
void c_locals(int, int);
void checkcycles(void);
void crack(int, int, Trans *, short *);
void d_sfh(uchar *, int);
void d_hash(uchar *, int);
void m_hash(uchar *, int);
void s_hash(uchar *, int);
void delq(int);
void dot_crack(int, int, Trans *);
void do_reach(void);
void pan_exit(int);
void exit(int);
#ifdef BFS_PAR
	void bfs_setup_mem(void);
#endif
#ifdef BITSTATE
	void sinit(void);
#else
	void hinit(void);
#endif
void imed(Trans *, int, int, int);
void new_state(void);
void p_restor(int);
void putpeg(int, int);
void putrail(void);
void q_restor(void);
void retrans(int, int, int, short *, uchar *, uchar *);
void select_claim(int);
void settable(void);
void setq_claim(int, int, char *, int, char *);
void sv_restor(void);
void sv_save(void);
void tagtable(int, int, int, short *, uchar *);
void do_dfs(int, int, int, short *, uchar *, uchar *);
void unrecv(int, int, int, int, int);
void usage(FILE *);
void wrap_stats(void);

#ifdef MA
	int g_store(char *, int, uchar);
#endif
#if defined(FULLSTACK) && defined(BITSTATE)
	int  onstack_now(void);
	void onstack_init(void);
	void onstack_put(void);
	void onstack_zap(void);
#endif
#ifndef XUSAFE
	int q_S_check(int, int);
	int q_R_check(int, int);
	uchar q_claim[MAXQ+1];
	char *q_name[MAXQ+1];
	char *p_name[MAXPROC+1];
#endif

#ifndef NO_V_PROVISO
	#define V_PROVISO
#endif
#if !defined(NO_RESIZE) && !defined(AUTO_RESIZE) && !defined(BITSTATE) && !defined(SPACE) && !defined(USE_TDH) && NCORE==1
	#define AUTO_RESIZE
#endif

typedef struct Trail Trail;
typedef struct H_el  H_el;

struct H_el {
	H_el *nxt;
	#ifdef FULLSTACK
		unsigned int tagged;
		#if defined(BITSTATE) && !defined(NOREDUCE) && !defined(SAFETY)
			unsigned int proviso;
		#endif
	#endif
	#if defined(CHECK) || (defined(COLLAPSE) && !defined(FULLSTACK))
		ulong st_id;
	#endif
	#if !defined(SAFETY) || defined(REACH)
		uint D;
	#endif
	#ifdef BCS
		#ifndef CONSERVATIVE
			#define CONSERVATIVE	1 /* good for up to 8 processes */
		#endif
		#ifdef CONSERVATIVE
			#if CONSERVATIVE <= 0 || CONSERVATIVE>32
			#error sensible values for CONSERVATIVE are 1..32 (256/8 = 32)
			#endif
			uchar ctx_pid[CONSERVATIVE];
		#endif
		uchar ctx_low;
	#endif
	#if NCORE>1
		/* could cost 1 extra word: 4 bytes if 32-bit and 8 bytes if 64-bit */
		#ifdef V_PROVISO
			uchar	cpu_id;		/* id of cpu that created the state */
		#endif
	#endif
	#ifdef COLLAPSE
		#if VECTORSZ<65536
			ushort ln;
		#else
			ulong ln;
		#endif
	#endif
	#if defined(AUTO_RESIZE) && !defined(BITSTATE)
		ulong m_K1;
	#endif
	ulong state;
};

#ifdef BFS_PAR
typedef struct BFS_Trail BFS_Trail;
struct BFS_Trail {
	H_el *ostate;
	int   st;
	int   o_tt;
	T_ID  t_id;
	uchar pr;
	uchar o_pm;
	uchar tau;
};
	#if SYNC>0
		#undef BFS_NOTRAIL
	#endif
#endif

#ifdef RHASH
	#ifndef PERMUTED
	#define PERMUTED
	#endif
#endif

struct Trail {
	int   st;	/* current state */
	int   o_tt;
#ifdef PERMUTED
	uint  seed;
	uchar oII;
#endif
	uchar pr;	/* process id */
	uchar tau;	/* 8 bit-flags */
	uchar o_pm;	/* 8 more bit-flags */

	#if 0
	Meaning of bit-flags on tau and o_pm:
	tau&1   -> timeout enabled
	tau&2   -> request to enable timeout 1 level up (in claim)
	tau&4   -> current transition is a  claim move
	tau&8   -> current transition is an atomic move
	tau&16  -> last move was truncated on stack
	tau&32  -> current transition is a preselected move
	tau&64  -> at least one next state is not on the stack
	tau&128 -> current transition is a stutter move
	o_pm&1	-> the current pid moved -- implements else
	o_pm&2	-> this is an acceptance state
	o_pm&4	-> this is a  progress state
	o_pm&8	-> fairness alg rule 1 undo mark
	o_pm&16	-> fairness alg rule 3 undo mark
	o_pm&32	-> fairness alg rule 2 undo mark
	o_pm&64 -> the current proc applied rule2
	o_pm&128 -> a fairness, dummy move - all procs blocked
	#endif

	#ifdef NSUCC
	 uchar n_succ;	/* nr of successor states */
	#endif
	#if defined(FULLSTACK) && defined(MA) && !defined(BFS)
	 uchar proviso;
	#endif
	#ifndef BFS
	 uchar  o_n, o_ot;	/* to save locals */
	#endif
	uchar  o_m;
	#ifdef EVENT_TRACE
		#if nstates_event<256
		 uchar o_event;
		#else
		 unsigned short o_event;
		#endif
	#endif
	#ifndef BFS
		short o_To;
		#if defined(T_RAND) || defined(RANDOMIZE)
		 short oo_i;
		#endif
	#endif
	#if defined(HAS_UNLESS) && !defined(BFS)
	 int e_state;	/* if escape trans - state of origin */
	#endif
	#if (defined(FULLSTACK) && !defined(MA)) || defined(BFS) || (NCORE>1)
	 H_el *ostate;	/* pointer to stored state */
	#endif
	#if defined(CNTRSTACK) && !defined(BFS)
	 long	j6, j7;
	#endif
	Trans *o_t;
	#if !defined(BFS) && !defined(TRIX_ORIG)
	 char *p_bup;
	 char *q_bup;
	#endif
	#ifdef BCS
	 unsigned short sched_limit;
	 unsigned char  bcs; /* phase 1 or 2, or forced 4 */
	 unsigned char  b_pno; /* preferred pid */
	#endif
	#ifdef P_RAND
	 unsigned char p_left;	/* nr of procs left to explore */
	 short p_skip;	/* to find starting point in list */
	#endif
	#ifdef HAS_SORTED
	 short ipt;
	#endif
	#ifdef HAS_PRIORITY
	 short o_priority;
	#endif
	union {
	 int oval;
	 int *ovals;
	} bup;
}; /* end of struct Trail */

#ifdef BFS
	#define Q_PROVISO
	#ifndef INLINE_REV
		#define INLINE_REV
	#endif

typedef struct SV_Hold {
	State *sv;
 #ifndef BFS_PAR
	int  sz;
 #endif
	struct SV_Hold *nxt;
} SV_Hold;
#if !defined(BFS_PAR) || NRUNS>0
	typedef struct EV_Hold {
	 #if !defined(BFS_PAR) || (!defined(NOCOMP) && !defined(HC) && NRUNS>0)
		char *sv;	/* Mask */
	 #endif
	 #if VECTORSZ<65536
		ushort sz;	/* vsize */
	 #else
		ulong  sz;
	 #endif
	 #ifdef BFS_PAR
		uchar owner;
	 #endif
		uchar nrpr;
		uchar nrqs;
		#if !defined(BFS_PAR) || (!defined(TRIX) && NRUNS>0)
			char *po, *qo;
			char *ps, *qs;
		#endif
		struct EV_Hold *nxt;
	} EV_Hold;
#endif
typedef struct BFS_State {
 #ifdef BFS_PAR
	BFS_Trail *t_info;
	State	*osv;
 #else
	Trail	*frame;
	SV_Hold *onow;
 #endif
 #if !defined(BFS_PAR) || NRUNS>0
	EV_Hold *omask;
 #endif
 #if defined(Q_PROVISO) && !defined(NOREDUCE)
	H_el *lstate;
 #endif
 #if !defined(BFS_PAR) || SYNC>0
	short boq;
 #endif
 #ifdef VERBOSE
	ulong nr;
 #endif
 #ifndef BFS_PAR
	struct BFS_State *nxt;
 #endif
} BFS_State;
#endif

void qsend(int, int, int, int);

#define Addproc(x,y)	addproc(256, y, x)
#define LOCAL	1
#define Q_FULL_F	2
#define Q_EMPT_F	3
#define Q_EMPT_T	4
#define Q_FULL_T	5
#define TIMEOUT_F	6
#define GLOBAL	7
#define BAD	8
#define ALPHA_F	9
#define NTRANS	967
unsigned char Is_Recv[3131];
#if defined(BFS_PAR) || NCORE>1
	void e_critical(int);
	void x_critical(int);
	#ifdef BFS_PAR
		void bfs_main(int, int);
		void bfs_report_mem(void);
	#endif
#endif


/* end of PAN_H */
#endif
