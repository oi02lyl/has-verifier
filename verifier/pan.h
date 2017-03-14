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
	#define VERI	2	/* np_ */
#endif
#ifndef NOCLAIM
	#define NCLAIMS	1
	#ifndef NP
		#define VERI	1
	#endif
#endif

typedef struct S_F_MAP {
	char *fnm;
	int from;
	int upto;
} S_F_MAP;

#define nstates1	20	/* never_0 */
#define minseq1	34253
#define maxseq1	34271
#define endstate1	19

#define nstates0	34254	/* :init: */
#define minseq0	0
#define maxseq0	34252
#define endstate0	34253

extern short src_ln1[];
extern short src_ln0[];
extern S_F_MAP src_file1[];
extern S_F_MAP src_file0[];

#define T_ID	unsigned short
#define _T5	20108
#define _T2	20109
#define WS		8 /* word size in bytes */
#define SYNC	0
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

typedef struct P1 { /* never_0 */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 3; /* proctype */
	unsigned _p   : 17; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
} P1;
#define Air1	(sizeof(P1) - 4)

#define Pinit	((P0 *)this)
typedef struct P0 { /* :init: */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 3; /* proctype */
	unsigned _p   : 17; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
} P0;
#define Air0	(sizeof(P0) - 4)

typedef struct P2 { /* np_ */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 3; /* proctype */
	unsigned _p   : 17; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
} P2;
#define Air2	(sizeof(P2) - 4)

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
	uchar running[5];
	uchar ready[5];
	uchar current;
	uchar N0;
	uchar N1;
	uchar N2;
	uchar N3;
	uchar N4;
	uchar S0;
	uchar S1;
	uchar S2;
	uchar S3;
	uchar S4;
	uchar CONST_NULL;
	uchar T0_X0;
	uchar T0_X1;
	uchar T0_X2;
	uchar T0_X2_1;
	uchar T0_X2_2;
	uchar T0_X2_3;
	uchar T0_X2_4;
	uchar T0_X2_5;
	uchar T0_X2_5_1;
	uchar T0_X2_5_2;
	uchar T0_X2_5_3;
	uchar T0_X2_5_4;
	uchar T0_X2_6;
	uchar T0_X2_6_1;
	uchar T0_X2_6_2;
	uchar T0_X2_6_3;
	uchar T0_X2_6_4;
	uchar T0_X2_6_5;
	uchar T0_X2_6_5_1;
	uchar T0_X2_6_5_2;
	uchar T0_X2_6_5_3;
	uchar T0_X2_6_5_4;
	uchar T0_X2_7;
	uchar T0_X2_7_1;
	uchar T0_X2_7_2;
	uchar T0_X2_7_3;
	uchar T0_X2_7_4;
	uchar T0_X3;
	uchar T0_X3_1;
	uchar T0_X3_2;
	uchar T0_X3_3;
	uchar T0_X3_4;
	uchar T0_X4;
	uchar T0_X4_1;
	uchar T0_X4_2;
	uchar T0_X4_3;
	uchar T0_X4_4;
	uchar T0_X4_5;
	uchar T0_X4_5_1;
	uchar T0_X4_5_2;
	uchar T0_X4_5_3;
	uchar T0_X4_5_4;
	uchar T0_X5;
	uchar T0_X5_1;
	uchar T0_X5_2;
	uchar T0_X5_3;
	uchar T0_X5_4;
	uchar T0_X6;
	uchar T0_X6_1;
	uchar T0_X6_2;
	uchar T0_X6_3;
	uchar T0_X6_4;
	uchar T0_X7;
	uchar T0_X8;
	uchar T0_X9;
	uchar T0_X9_1;
	uchar T0_X9_2;
	uchar T0_X9_3;
	uchar T0_X9_4;
	uchar T0_X9_5;
	uchar T0_X9_5_1;
	uchar T0_X9_5_2;
	uchar T0_X9_5_3;
	uchar T0_X9_5_4;
	uchar T0_X9_6;
	uchar T0_X9_6_1;
	uchar T0_X9_6_2;
	uchar T0_X9_6_3;
	uchar T0_X9_6_4;
	uchar T0_X9_6_5;
	uchar T0_X9_6_5_1;
	uchar T0_X9_6_5_2;
	uchar T0_X9_6_5_3;
	uchar T0_X9_6_5_4;
	uchar T0_X9_7;
	uchar T0_X9_7_1;
	uchar T0_X9_7_2;
	uchar T0_X9_7_3;
	uchar T0_X9_7_4;
	uchar T0_X10;
	uchar T0_X10_1;
	uchar T0_X10_2;
	uchar T0_X10_3;
	uchar T0_X10_4;
	uchar T0_X11;
	uchar T0_X11_1;
	uchar T0_X11_2;
	uchar T0_X11_3;
	uchar T0_X11_4;
	uchar T0_X11_5;
	uchar T0_X11_5_1;
	uchar T0_X11_5_2;
	uchar T0_X11_5_3;
	uchar T0_X11_5_4;
	uchar T0_X12;
	uchar T0_X12_1;
	uchar T0_X12_2;
	uchar T0_X12_3;
	uchar T0_X12_4;
	uchar T0_X13;
	uchar T0_X13_1;
	uchar T0_X13_2;
	uchar T0_X13_3;
	uchar T0_X13_4;
	uchar T0_X14;
	uchar T0_X15;
	uchar T0_X16;
	uchar T0_X16_1;
	uchar T0_X16_2;
	uchar T0_X16_3;
	uchar T0_X16_4;
	uchar T0_X16_5;
	uchar T0_X16_5_1;
	uchar T0_X16_5_2;
	uchar T0_X16_5_3;
	uchar T0_X16_5_4;
	uchar T0_X16_6;
	uchar T0_X16_6_1;
	uchar T0_X16_6_2;
	uchar T0_X16_6_3;
	uchar T0_X16_6_4;
	uchar T0_X16_6_5;
	uchar T0_X16_6_5_1;
	uchar T0_X16_6_5_2;
	uchar T0_X16_6_5_3;
	uchar T0_X16_6_5_4;
	uchar T0_X16_7;
	uchar T0_X16_7_1;
	uchar T0_X16_7_2;
	uchar T0_X16_7_3;
	uchar T0_X16_7_4;
	uchar T0_X17;
	uchar T0_X17_1;
	uchar T0_X17_2;
	uchar T0_X17_3;
	uchar T0_X17_4;
	uchar T0_X18;
	uchar T0_X18_1;
	uchar T0_X18_2;
	uchar T0_X18_3;
	uchar T0_X18_4;
	uchar T0_X18_5;
	uchar T0_X18_5_1;
	uchar T0_X18_5_2;
	uchar T0_X18_5_3;
	uchar T0_X18_5_4;
	uchar T0_X19;
	uchar T0_X19_1;
	uchar T0_X19_2;
	uchar T0_X19_3;
	uchar T0_X19_4;
	uchar T1_X0;
	uchar T1_X1;
	uchar T1_X2;
	uchar T1_X2_1;
	uchar T1_X2_2;
	uchar T1_X2_3;
	uchar T1_X2_4;
	uchar T1_X2_5;
	uchar T1_X2_5_1;
	uchar T1_X2_5_2;
	uchar T1_X2_5_3;
	uchar T1_X2_5_4;
	uchar T1_X2_6;
	uchar T1_X2_6_1;
	uchar T1_X2_6_2;
	uchar T1_X2_6_3;
	uchar T1_X2_6_4;
	uchar T1_X2_6_5;
	uchar T1_X2_6_5_1;
	uchar T1_X2_6_5_2;
	uchar T1_X2_6_5_3;
	uchar T1_X2_6_5_4;
	uchar T1_X2_7;
	uchar T1_X2_7_1;
	uchar T1_X2_7_2;
	uchar T1_X2_7_3;
	uchar T1_X2_7_4;
	uchar T1_X3;
	uchar T1_X3_1;
	uchar T1_X3_2;
	uchar T1_X3_3;
	uchar T1_X3_4;
	uchar T1_X4;
	uchar T1_X4_1;
	uchar T1_X4_2;
	uchar T1_X4_3;
	uchar T1_X4_4;
	uchar T1_X4_5;
	uchar T1_X4_5_1;
	uchar T1_X4_5_2;
	uchar T1_X4_5_3;
	uchar T1_X4_5_4;
	uchar T1_X5;
	uchar T1_X5_1;
	uchar T1_X5_2;
	uchar T1_X5_3;
	uchar T1_X5_4;
	uchar T1_X6;
	uchar T1_X6_1;
	uchar T1_X6_2;
	uchar T1_X6_3;
	uchar T1_X6_4;
	uchar T1_X7;
	uchar T1_X8;
	uchar T1_X9;
	uchar T1_X9_1;
	uchar T1_X9_2;
	uchar T1_X9_3;
	uchar T1_X9_4;
	uchar T1_X9_5;
	uchar T1_X9_5_1;
	uchar T1_X9_5_2;
	uchar T1_X9_5_3;
	uchar T1_X9_5_4;
	uchar T1_X9_6;
	uchar T1_X9_6_1;
	uchar T1_X9_6_2;
	uchar T1_X9_6_3;
	uchar T1_X9_6_4;
	uchar T1_X9_6_5;
	uchar T1_X9_6_5_1;
	uchar T1_X9_6_5_2;
	uchar T1_X9_6_5_3;
	uchar T1_X9_6_5_4;
	uchar T1_X9_7;
	uchar T1_X9_7_1;
	uchar T1_X9_7_2;
	uchar T1_X9_7_3;
	uchar T1_X9_7_4;
	uchar T1_X10;
	uchar T1_X10_1;
	uchar T1_X10_2;
	uchar T1_X10_3;
	uchar T1_X10_4;
	uchar T1_X11;
	uchar T1_X11_1;
	uchar T1_X11_2;
	uchar T1_X11_3;
	uchar T1_X11_4;
	uchar T1_X11_5;
	uchar T1_X11_5_1;
	uchar T1_X11_5_2;
	uchar T1_X11_5_3;
	uchar T1_X11_5_4;
	uchar T1_X12;
	uchar T1_X12_1;
	uchar T1_X12_2;
	uchar T1_X12_3;
	uchar T1_X12_4;
	uchar T1_X13;
	uchar T1_X13_1;
	uchar T1_X13_2;
	uchar T1_X13_3;
	uchar T1_X13_4;
	uchar T1_X14;
	uchar T1_X15;
	uchar T1_X16;
	uchar T1_X16_1;
	uchar T1_X16_2;
	uchar T1_X16_3;
	uchar T1_X16_4;
	uchar T1_X16_5;
	uchar T1_X16_5_1;
	uchar T1_X16_5_2;
	uchar T1_X16_5_3;
	uchar T1_X16_5_4;
	uchar T1_X16_6;
	uchar T1_X16_6_1;
	uchar T1_X16_6_2;
	uchar T1_X16_6_3;
	uchar T1_X16_6_4;
	uchar T1_X16_6_5;
	uchar T1_X16_6_5_1;
	uchar T1_X16_6_5_2;
	uchar T1_X16_6_5_3;
	uchar T1_X16_6_5_4;
	uchar T1_X16_7;
	uchar T1_X16_7_1;
	uchar T1_X16_7_2;
	uchar T1_X16_7_3;
	uchar T1_X16_7_4;
	uchar T1_X17;
	uchar T1_X17_1;
	uchar T1_X17_2;
	uchar T1_X17_3;
	uchar T1_X17_4;
	uchar T1_X18;
	uchar T1_X18_1;
	uchar T1_X18_2;
	uchar T1_X18_3;
	uchar T1_X18_4;
	uchar T1_X18_5;
	uchar T1_X18_5_1;
	uchar T1_X18_5_2;
	uchar T1_X18_5_3;
	uchar T1_X18_5_4;
	uchar T1_X19;
	uchar T1_X19_1;
	uchar T1_X19_2;
	uchar T1_X19_3;
	uchar T1_X19_4;
	uchar T2_X0;
	uchar T2_X1;
	uchar T2_X2;
	uchar T2_X2_1;
	uchar T2_X2_2;
	uchar T2_X2_3;
	uchar T2_X2_4;
	uchar T2_X2_5;
	uchar T2_X2_5_1;
	uchar T2_X2_5_2;
	uchar T2_X2_5_3;
	uchar T2_X2_5_4;
	uchar T2_X2_6;
	uchar T2_X2_6_1;
	uchar T2_X2_6_2;
	uchar T2_X2_6_3;
	uchar T2_X2_6_4;
	uchar T2_X2_6_5;
	uchar T2_X2_6_5_1;
	uchar T2_X2_6_5_2;
	uchar T2_X2_6_5_3;
	uchar T2_X2_6_5_4;
	uchar T2_X2_7;
	uchar T2_X2_7_1;
	uchar T2_X2_7_2;
	uchar T2_X2_7_3;
	uchar T2_X2_7_4;
	uchar T2_X3;
	uchar T2_X3_1;
	uchar T2_X3_2;
	uchar T2_X3_3;
	uchar T2_X3_4;
	uchar T2_X4;
	uchar T2_X4_1;
	uchar T2_X4_2;
	uchar T2_X4_3;
	uchar T2_X4_4;
	uchar T2_X4_5;
	uchar T2_X4_5_1;
	uchar T2_X4_5_2;
	uchar T2_X4_5_3;
	uchar T2_X4_5_4;
	uchar T2_X5;
	uchar T2_X5_1;
	uchar T2_X5_2;
	uchar T2_X5_3;
	uchar T2_X5_4;
	uchar T2_X6;
	uchar T2_X6_1;
	uchar T2_X6_2;
	uchar T2_X6_3;
	uchar T2_X6_4;
	uchar T2_X7;
	uchar T2_X8;
	uchar T2_X9;
	uchar T2_X9_1;
	uchar T2_X9_2;
	uchar T2_X9_3;
	uchar T2_X9_4;
	uchar T2_X9_5;
	uchar T2_X9_5_1;
	uchar T2_X9_5_2;
	uchar T2_X9_5_3;
	uchar T2_X9_5_4;
	uchar T2_X9_6;
	uchar T2_X9_6_1;
	uchar T2_X9_6_2;
	uchar T2_X9_6_3;
	uchar T2_X9_6_4;
	uchar T2_X9_6_5;
	uchar T2_X9_6_5_1;
	uchar T2_X9_6_5_2;
	uchar T2_X9_6_5_3;
	uchar T2_X9_6_5_4;
	uchar T2_X9_7;
	uchar T2_X9_7_1;
	uchar T2_X9_7_2;
	uchar T2_X9_7_3;
	uchar T2_X9_7_4;
	uchar T2_X10;
	uchar T2_X10_1;
	uchar T2_X10_2;
	uchar T2_X10_3;
	uchar T2_X10_4;
	uchar T2_X11;
	uchar T2_X11_1;
	uchar T2_X11_2;
	uchar T2_X11_3;
	uchar T2_X11_4;
	uchar T2_X11_5;
	uchar T2_X11_5_1;
	uchar T2_X11_5_2;
	uchar T2_X11_5_3;
	uchar T2_X11_5_4;
	uchar T2_X12;
	uchar T2_X12_1;
	uchar T2_X12_2;
	uchar T2_X12_3;
	uchar T2_X12_4;
	uchar T2_X13;
	uchar T2_X13_1;
	uchar T2_X13_2;
	uchar T2_X13_3;
	uchar T2_X13_4;
	uchar T2_X14;
	uchar T2_X15;
	uchar T2_X16;
	uchar T2_X16_1;
	uchar T2_X16_2;
	uchar T2_X16_3;
	uchar T2_X16_4;
	uchar T2_X16_5;
	uchar T2_X16_5_1;
	uchar T2_X16_5_2;
	uchar T2_X16_5_3;
	uchar T2_X16_5_4;
	uchar T2_X16_6;
	uchar T2_X16_6_1;
	uchar T2_X16_6_2;
	uchar T2_X16_6_3;
	uchar T2_X16_6_4;
	uchar T2_X16_6_5;
	uchar T2_X16_6_5_1;
	uchar T2_X16_6_5_2;
	uchar T2_X16_6_5_3;
	uchar T2_X16_6_5_4;
	uchar T2_X16_7;
	uchar T2_X16_7_1;
	uchar T2_X16_7_2;
	uchar T2_X16_7_3;
	uchar T2_X16_7_4;
	uchar T2_X17;
	uchar T2_X17_1;
	uchar T2_X17_2;
	uchar T2_X17_3;
	uchar T2_X17_4;
	uchar T2_X18;
	uchar T2_X18_1;
	uchar T2_X18_2;
	uchar T2_X18_3;
	uchar T2_X18_4;
	uchar T2_X18_5;
	uchar T2_X18_5_1;
	uchar T2_X18_5_2;
	uchar T2_X18_5_3;
	uchar T2_X18_5_4;
	uchar T2_X19;
	uchar T2_X19_1;
	uchar T2_X19_2;
	uchar T2_X19_3;
	uchar T2_X19_4;
	uchar T3_X0;
	uchar T3_X1;
	uchar T3_X2;
	uchar T3_X2_1;
	uchar T3_X2_2;
	uchar T3_X2_3;
	uchar T3_X2_4;
	uchar T3_X2_5;
	uchar T3_X2_5_1;
	uchar T3_X2_5_2;
	uchar T3_X2_5_3;
	uchar T3_X2_5_4;
	uchar T3_X2_6;
	uchar T3_X2_6_1;
	uchar T3_X2_6_2;
	uchar T3_X2_6_3;
	uchar T3_X2_6_4;
	uchar T3_X2_6_5;
	uchar T3_X2_6_5_1;
	uchar T3_X2_6_5_2;
	uchar T3_X2_6_5_3;
	uchar T3_X2_6_5_4;
	uchar T3_X2_7;
	uchar T3_X2_7_1;
	uchar T3_X2_7_2;
	uchar T3_X2_7_3;
	uchar T3_X2_7_4;
	uchar T3_X3;
	uchar T3_X3_1;
	uchar T3_X3_2;
	uchar T3_X3_3;
	uchar T3_X3_4;
	uchar T3_X4;
	uchar T3_X4_1;
	uchar T3_X4_2;
	uchar T3_X4_3;
	uchar T3_X4_4;
	uchar T3_X4_5;
	uchar T3_X4_5_1;
	uchar T3_X4_5_2;
	uchar T3_X4_5_3;
	uchar T3_X4_5_4;
	uchar T3_X5;
	uchar T3_X5_1;
	uchar T3_X5_2;
	uchar T3_X5_3;
	uchar T3_X5_4;
	uchar T3_X6;
	uchar T3_X6_1;
	uchar T3_X6_2;
	uchar T3_X6_3;
	uchar T3_X6_4;
	uchar T3_X7;
	uchar T3_X8;
	uchar T3_X9;
	uchar T3_X9_1;
	uchar T3_X9_2;
	uchar T3_X9_3;
	uchar T3_X9_4;
	uchar T3_X9_5;
	uchar T3_X9_5_1;
	uchar T3_X9_5_2;
	uchar T3_X9_5_3;
	uchar T3_X9_5_4;
	uchar T3_X9_6;
	uchar T3_X9_6_1;
	uchar T3_X9_6_2;
	uchar T3_X9_6_3;
	uchar T3_X9_6_4;
	uchar T3_X9_6_5;
	uchar T3_X9_6_5_1;
	uchar T3_X9_6_5_2;
	uchar T3_X9_6_5_3;
	uchar T3_X9_6_5_4;
	uchar T3_X9_7;
	uchar T3_X9_7_1;
	uchar T3_X9_7_2;
	uchar T3_X9_7_3;
	uchar T3_X9_7_4;
	uchar T3_X10;
	uchar T3_X10_1;
	uchar T3_X10_2;
	uchar T3_X10_3;
	uchar T3_X10_4;
	uchar T3_X11;
	uchar T3_X11_1;
	uchar T3_X11_2;
	uchar T3_X11_3;
	uchar T3_X11_4;
	uchar T3_X11_5;
	uchar T3_X11_5_1;
	uchar T3_X11_5_2;
	uchar T3_X11_5_3;
	uchar T3_X11_5_4;
	uchar T3_X12;
	uchar T3_X12_1;
	uchar T3_X12_2;
	uchar T3_X12_3;
	uchar T3_X12_4;
	uchar T3_X13;
	uchar T3_X13_1;
	uchar T3_X13_2;
	uchar T3_X13_3;
	uchar T3_X13_4;
	uchar T3_X14;
	uchar T3_X15;
	uchar T3_X16;
	uchar T3_X16_1;
	uchar T3_X16_2;
	uchar T3_X16_3;
	uchar T3_X16_4;
	uchar T3_X16_5;
	uchar T3_X16_5_1;
	uchar T3_X16_5_2;
	uchar T3_X16_5_3;
	uchar T3_X16_5_4;
	uchar T3_X16_6;
	uchar T3_X16_6_1;
	uchar T3_X16_6_2;
	uchar T3_X16_6_3;
	uchar T3_X16_6_4;
	uchar T3_X16_6_5;
	uchar T3_X16_6_5_1;
	uchar T3_X16_6_5_2;
	uchar T3_X16_6_5_3;
	uchar T3_X16_6_5_4;
	uchar T3_X16_7;
	uchar T3_X16_7_1;
	uchar T3_X16_7_2;
	uchar T3_X16_7_3;
	uchar T3_X16_7_4;
	uchar T3_X17;
	uchar T3_X17_1;
	uchar T3_X17_2;
	uchar T3_X17_3;
	uchar T3_X17_4;
	uchar T3_X18;
	uchar T3_X18_1;
	uchar T3_X18_2;
	uchar T3_X18_3;
	uchar T3_X18_4;
	uchar T3_X18_5;
	uchar T3_X18_5_1;
	uchar T3_X18_5_2;
	uchar T3_X18_5_3;
	uchar T3_X18_5_4;
	uchar T3_X19;
	uchar T3_X19_1;
	uchar T3_X19_2;
	uchar T3_X19_3;
	uchar T3_X19_4;
	uchar T4_X0;
	uchar T4_X1;
	uchar T4_X2;
	uchar T4_X2_1;
	uchar T4_X2_2;
	uchar T4_X2_3;
	uchar T4_X2_4;
	uchar T4_X2_5;
	uchar T4_X2_5_1;
	uchar T4_X2_5_2;
	uchar T4_X2_5_3;
	uchar T4_X2_5_4;
	uchar T4_X2_6;
	uchar T4_X2_6_1;
	uchar T4_X2_6_2;
	uchar T4_X2_6_3;
	uchar T4_X2_6_4;
	uchar T4_X2_6_5;
	uchar T4_X2_6_5_1;
	uchar T4_X2_6_5_2;
	uchar T4_X2_6_5_3;
	uchar T4_X2_6_5_4;
	uchar T4_X2_7;
	uchar T4_X2_7_1;
	uchar T4_X2_7_2;
	uchar T4_X2_7_3;
	uchar T4_X2_7_4;
	uchar T4_X3;
	uchar T4_X3_1;
	uchar T4_X3_2;
	uchar T4_X3_3;
	uchar T4_X3_4;
	uchar T4_X4;
	uchar T4_X4_1;
	uchar T4_X4_2;
	uchar T4_X4_3;
	uchar T4_X4_4;
	uchar T4_X4_5;
	uchar T4_X4_5_1;
	uchar T4_X4_5_2;
	uchar T4_X4_5_3;
	uchar T4_X4_5_4;
	uchar T4_X5;
	uchar T4_X5_1;
	uchar T4_X5_2;
	uchar T4_X5_3;
	uchar T4_X5_4;
	uchar T4_X6;
	uchar T4_X6_1;
	uchar T4_X6_2;
	uchar T4_X6_3;
	uchar T4_X6_4;
	uchar T4_X7;
	uchar T4_X8;
	uchar T4_X9;
	uchar T4_X9_1;
	uchar T4_X9_2;
	uchar T4_X9_3;
	uchar T4_X9_4;
	uchar T4_X9_5;
	uchar T4_X9_5_1;
	uchar T4_X9_5_2;
	uchar T4_X9_5_3;
	uchar T4_X9_5_4;
	uchar T4_X9_6;
	uchar T4_X9_6_1;
	uchar T4_X9_6_2;
	uchar T4_X9_6_3;
	uchar T4_X9_6_4;
	uchar T4_X9_6_5;
	uchar T4_X9_6_5_1;
	uchar T4_X9_6_5_2;
	uchar T4_X9_6_5_3;
	uchar T4_X9_6_5_4;
	uchar T4_X9_7;
	uchar T4_X9_7_1;
	uchar T4_X9_7_2;
	uchar T4_X9_7_3;
	uchar T4_X9_7_4;
	uchar T4_X10;
	uchar T4_X10_1;
	uchar T4_X10_2;
	uchar T4_X10_3;
	uchar T4_X10_4;
	uchar T4_X11;
	uchar T4_X11_1;
	uchar T4_X11_2;
	uchar T4_X11_3;
	uchar T4_X11_4;
	uchar T4_X11_5;
	uchar T4_X11_5_1;
	uchar T4_X11_5_2;
	uchar T4_X11_5_3;
	uchar T4_X11_5_4;
	uchar T4_X12;
	uchar T4_X12_1;
	uchar T4_X12_2;
	uchar T4_X12_3;
	uchar T4_X12_4;
	uchar T4_X13;
	uchar T4_X13_1;
	uchar T4_X13_2;
	uchar T4_X13_3;
	uchar T4_X13_4;
	uchar T4_X14;
	uchar T4_X15;
	uchar T4_X16;
	uchar T4_X16_1;
	uchar T4_X16_2;
	uchar T4_X16_3;
	uchar T4_X16_4;
	uchar T4_X16_5;
	uchar T4_X16_5_1;
	uchar T4_X16_5_2;
	uchar T4_X16_5_3;
	uchar T4_X16_5_4;
	uchar T4_X16_6;
	uchar T4_X16_6_1;
	uchar T4_X16_6_2;
	uchar T4_X16_6_3;
	uchar T4_X16_6_4;
	uchar T4_X16_6_5;
	uchar T4_X16_6_5_1;
	uchar T4_X16_6_5_2;
	uchar T4_X16_6_5_3;
	uchar T4_X16_6_5_4;
	uchar T4_X16_7;
	uchar T4_X16_7_1;
	uchar T4_X16_7_2;
	uchar T4_X16_7_3;
	uchar T4_X16_7_4;
	uchar T4_X17;
	uchar T4_X17_1;
	uchar T4_X17_2;
	uchar T4_X17_3;
	uchar T4_X17_4;
	uchar T4_X18;
	uchar T4_X18_1;
	uchar T4_X18_2;
	uchar T4_X18_3;
	uchar T4_X18_4;
	uchar T4_X18_5;
	uchar T4_X18_5_1;
	uchar T4_X18_5_2;
	uchar T4_X18_5_3;
	uchar T4_X18_5_4;
	uchar T4_X19;
	uchar T4_X19_1;
	uchar T4_X19_2;
	uchar T4_X19_3;
	uchar T4_X19_4;
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
#define FORWARD_MOVES	"pan.m"
#define BACKWARD_MOVES	"pan.b"
#define TRANSITIONS	"pan.t"
#define _NP_	2
#define nstates2	3 /* np_ */
#define endstate2	2 /* np_ */

#define start2	0 /* np_ */
#define start1	5
#define start0	23
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
#define NQS	0
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

void qsend(int, int, int);

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
#define NTRANS	20110
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
