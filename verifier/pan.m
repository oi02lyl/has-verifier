#define rand	pan_rand
#define pthread_equal(a,b)	((a)==(b))
#if defined(HAS_CODE) && defined(VERBOSE)
	#ifdef BFS_PAR
		bfs_printf("Pr: %d Tr: %d\n", II, t->forw);
	#else
		cpu_printf("Pr: %d Tr: %d\n", II, t->forw);
	#endif
#endif
	switch (t->forw) {
	default: Uerror("bad forward move");
	case 0:	/* if without executable clauses */
		continue;
	case 1: /* generic 'goto' or 'skip' */
		IfNotBlocked
		_m = 3; goto P999;
	case 2: /* generic 'else' */
		IfNotBlocked
		if (trpt->o_pm&1) continue;
		_m = 3; goto P999;

		 /* CLAIM never_0 */
	case 3: // STATE 3 - output.pml:247 - [(((!((T0_X4==S6))&&(T0_X4==S3))&&(current==0)))] (0:0:0 - 1)
		
#if defined(VERI) && !defined(NP)
#if NCLAIMS>1
		{	static int reported3 = 0;
			if (verbose && !reported3)
			{	int nn = (int) ((Pclaim *)pptr(0))->_n;
				printf("depth %ld: Claim %s (%d), state %d (line %d)\n",
					depth, procname[spin_c_typ[nn]], nn, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported3 = 1;
				fflush(stdout);
		}	}
#else
		{	static int reported3 = 0;
			if (verbose && !reported3)
			{	printf("depth %d: Claim, state %d (line %d)\n",
					(int) depth, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported3 = 1;
				fflush(stdout);
		}	}
#endif
#endif
		reached[1][3] = 1;
		if (!((( !((((int)now.T0_X4)==((int)now.S6)))&&(((int)now.T0_X4)==((int)now.S3)))&&(((int)now.current)==0))))
			continue;
		_m = 3; goto P999; /* 0 */
	case 4: // STATE 7 - output.pml:251 - [(!((T0_X4==S6)))] (0:0:0 - 1)
		
#if defined(VERI) && !defined(NP)
#if NCLAIMS>1
		{	static int reported7 = 0;
			if (verbose && !reported7)
			{	int nn = (int) ((Pclaim *)pptr(0))->_n;
				printf("depth %ld: Claim %s (%d), state %d (line %d)\n",
					depth, procname[spin_c_typ[nn]], nn, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported7 = 1;
				fflush(stdout);
		}	}
#else
		{	static int reported7 = 0;
			if (verbose && !reported7)
			{	printf("depth %d: Claim, state %d (line %d)\n",
					(int) depth, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported7 = 1;
				fflush(stdout);
		}	}
#endif
#endif
		reached[1][7] = 1;
		if (!( !((((int)now.T0_X4)==((int)now.S6)))))
			continue;
		_m = 3; goto P999; /* 0 */
	case 5: // STATE 9 - output.pml:252 - [(((T0_X4==S6)||!(running[0])))] (0:0:0 - 1)
		
#if defined(VERI) && !defined(NP)
#if NCLAIMS>1
		{	static int reported9 = 0;
			if (verbose && !reported9)
			{	int nn = (int) ((Pclaim *)pptr(0))->_n;
				printf("depth %ld: Claim %s (%d), state %d (line %d)\n",
					depth, procname[spin_c_typ[nn]], nn, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported9 = 1;
				fflush(stdout);
		}	}
#else
		{	static int reported9 = 0;
			if (verbose && !reported9)
			{	printf("depth %d: Claim, state %d (line %d)\n",
					(int) depth, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported9 = 1;
				fflush(stdout);
		}	}
#endif
#endif
		reached[1][9] = 1;
		if (!(((((int)now.T0_X4)==((int)now.S6))|| !(((int)now.running[0])))))
			continue;
		_m = 3; goto P999; /* 0 */
	case 6: // STATE 13 - output.pml:254 - [-end-] (0:0:0 - 1)
		
#if defined(VERI) && !defined(NP)
#if NCLAIMS>1
		{	static int reported13 = 0;
			if (verbose && !reported13)
			{	int nn = (int) ((Pclaim *)pptr(0))->_n;
				printf("depth %ld: Claim %s (%d), state %d (line %d)\n",
					depth, procname[spin_c_typ[nn]], nn, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported13 = 1;
				fflush(stdout);
		}	}
#else
		{	static int reported13 = 0;
			if (verbose && !reported13)
			{	printf("depth %d: Claim, state %d (line %d)\n",
					(int) depth, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported13 = 1;
				fflush(stdout);
		}	}
#endif
#endif
		reached[1][13] = 1;
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC :init: */
	case 7: // STATE 1 - output.pml:50 - [current = 255] (0:55:53 - 1)
		IfNotBlocked
		reached[0][1] = 1;
		(trpt+1)->bup.ovals = grab_ints(53);
		(trpt+1)->bup.ovals[0] = ((int)now.current);
		now.current = 255;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: ready[0] = 0(55, 2, 55) */
		reached[0][2] = 1;
		(trpt+1)->bup.ovals[1] = ((int)now.ready[0]);
		now.ready[0] = 0;
#ifdef VAR_RANGES
		logval("ready[0]", ((int)now.ready[0]));
#endif
		;
		/* merge: running[0] = 1(55, 3, 55) */
		reached[0][3] = 1;
		(trpt+1)->bup.ovals[2] = ((int)now.running[0]);
		now.running[0] = 1;
#ifdef VAR_RANGES
		logval("running[0]", ((int)now.running[0]));
#endif
		;
		/* merge: ready[1] = 0(55, 4, 55) */
		reached[0][4] = 1;
		(trpt+1)->bup.ovals[3] = ((int)now.ready[1]);
		now.ready[1] = 0;
#ifdef VAR_RANGES
		logval("ready[1]", ((int)now.ready[1]));
#endif
		;
		/* merge: running[1] = 0(55, 5, 55) */
		reached[0][5] = 1;
		(trpt+1)->bup.ovals[4] = ((int)now.running[1]);
		now.running[1] = 0;
#ifdef VAR_RANGES
		logval("running[1]", ((int)now.running[1]));
#endif
		;
		/* merge: ready[2] = 0(55, 6, 55) */
		reached[0][6] = 1;
		(trpt+1)->bup.ovals[5] = ((int)now.ready[2]);
		now.ready[2] = 0;
#ifdef VAR_RANGES
		logval("ready[2]", ((int)now.ready[2]));
#endif
		;
		/* merge: running[2] = 0(55, 7, 55) */
		reached[0][7] = 1;
		(trpt+1)->bup.ovals[6] = ((int)now.running[2]);
		now.running[2] = 0;
#ifdef VAR_RANGES
		logval("running[2]", ((int)now.running[2]));
#endif
		;
		/* merge: ready[3] = 0(55, 8, 55) */
		reached[0][8] = 1;
		(trpt+1)->bup.ovals[7] = ((int)now.ready[3]);
		now.ready[3] = 0;
#ifdef VAR_RANGES
		logval("ready[3]", ((int)now.ready[3]));
#endif
		;
		/* merge: running[3] = 0(55, 9, 55) */
		reached[0][9] = 1;
		(trpt+1)->bup.ovals[8] = ((int)now.running[3]);
		now.running[3] = 0;
#ifdef VAR_RANGES
		logval("running[3]", ((int)now.running[3]));
#endif
		;
		/* merge: N0 = 0(55, 10, 55) */
		reached[0][10] = 1;
		(trpt+1)->bup.ovals[9] = ((int)now.N0);
		now.N0 = 0;
#ifdef VAR_RANGES
		logval("N0", ((int)now.N0));
#endif
		;
		/* merge: S0 = 1(55, 11, 55) */
		reached[0][11] = 1;
		(trpt+1)->bup.ovals[10] = ((int)now.S0);
		now.S0 = 1;
#ifdef VAR_RANGES
		logval("S0", ((int)now.S0));
#endif
		;
		/* merge: S1 = 2(55, 12, 55) */
		reached[0][12] = 1;
		(trpt+1)->bup.ovals[11] = ((int)now.S1);
		now.S1 = 2;
#ifdef VAR_RANGES
		logval("S1", ((int)now.S1));
#endif
		;
		/* merge: S2 = 3(55, 13, 55) */
		reached[0][13] = 1;
		(trpt+1)->bup.ovals[12] = ((int)now.S2);
		now.S2 = 3;
#ifdef VAR_RANGES
		logval("S2", ((int)now.S2));
#endif
		;
		/* merge: S3 = 4(55, 14, 55) */
		reached[0][14] = 1;
		(trpt+1)->bup.ovals[13] = ((int)now.S3);
		now.S3 = 4;
#ifdef VAR_RANGES
		logval("S3", ((int)now.S3));
#endif
		;
		/* merge: S4 = 5(55, 15, 55) */
		reached[0][15] = 1;
		(trpt+1)->bup.ovals[14] = ((int)now.S4);
		now.S4 = 5;
#ifdef VAR_RANGES
		logval("S4", ((int)now.S4));
#endif
		;
		/* merge: S5 = 6(55, 16, 55) */
		reached[0][16] = 1;
		(trpt+1)->bup.ovals[15] = ((int)now.S5);
		now.S5 = 6;
#ifdef VAR_RANGES
		logval("S5", ((int)now.S5));
#endif
		;
		/* merge: S6 = 7(55, 17, 55) */
		reached[0][17] = 1;
		(trpt+1)->bup.ovals[16] = ((int)now.S6);
		now.S6 = 7;
#ifdef VAR_RANGES
		logval("S6", ((int)now.S6));
#endif
		;
		/* merge: S7 = 0(55, 18, 55) */
		reached[0][18] = 1;
		(trpt+1)->bup.ovals[17] = ((int)now.S7);
		now.S7 = 0;
#ifdef VAR_RANGES
		logval("S7", ((int)now.S7));
#endif
		;
		/* merge: CONST_NULL = 0(55, 19, 55) */
		reached[0][19] = 1;
		(trpt+1)->bup.ovals[18] = ((int)now.CONST_NULL);
		now.CONST_NULL = 0;
#ifdef VAR_RANGES
		logval("CONST_NULL", ((int)now.CONST_NULL));
#endif
		;
		/* merge: T0_X0 = 0(55, 20, 55) */
		reached[0][20] = 1;
		(trpt+1)->bup.ovals[19] = ((int)now.T0_X0);
		now.T0_X0 = 0;
#ifdef VAR_RANGES
		logval("T0_X0", ((int)now.T0_X0));
#endif
		;
		/* merge: T0_X0_1 = 0(55, 21, 55) */
		reached[0][21] = 1;
		(trpt+1)->bup.ovals[20] = ((int)now.T0_X0_1);
		now.T0_X0_1 = 0;
#ifdef VAR_RANGES
		logval("T0_X0_1", ((int)now.T0_X0_1));
#endif
		;
		/* merge: T0_X0_2 = 0(55, 22, 55) */
		reached[0][22] = 1;
		(trpt+1)->bup.ovals[21] = ((int)now.T0_X0_2);
		now.T0_X0_2 = 0;
#ifdef VAR_RANGES
		logval("T0_X0_2", ((int)now.T0_X0_2));
#endif
		;
		/* merge: T0_X0_2_1 = 0(55, 23, 55) */
		reached[0][23] = 1;
		(trpt+1)->bup.ovals[22] = ((int)now.T0_X0_2_1);
		now.T0_X0_2_1 = 0;
#ifdef VAR_RANGES
		logval("T0_X0_2_1", ((int)now.T0_X0_2_1));
#endif
		;
		/* merge: T0_X0_2_2 = 0(55, 24, 55) */
		reached[0][24] = 1;
		(trpt+1)->bup.ovals[23] = ((int)now.T0_X0_2_2);
		now.T0_X0_2_2 = 0;
#ifdef VAR_RANGES
		logval("T0_X0_2_2", ((int)now.T0_X0_2_2));
#endif
		;
		/* merge: T0_X0_2_3 = 0(55, 25, 55) */
		reached[0][25] = 1;
		(trpt+1)->bup.ovals[24] = ((int)now.T0_X0_2_3);
		now.T0_X0_2_3 = 0;
#ifdef VAR_RANGES
		logval("T0_X0_2_3", ((int)now.T0_X0_2_3));
#endif
		;
		/* merge: T0_X1 = 0(55, 26, 55) */
		reached[0][26] = 1;
		(trpt+1)->bup.ovals[25] = ((int)now.T0_X1);
		now.T0_X1 = 0;
#ifdef VAR_RANGES
		logval("T0_X1", ((int)now.T0_X1));
#endif
		;
		/* merge: T0_X2 = 0(55, 27, 55) */
		reached[0][27] = 1;
		(trpt+1)->bup.ovals[26] = ((int)now.T0_X2);
		now.T0_X2 = 0;
#ifdef VAR_RANGES
		logval("T0_X2", ((int)now.T0_X2));
#endif
		;
		/* merge: T0_X3 = 0(55, 28, 55) */
		reached[0][28] = 1;
		(trpt+1)->bup.ovals[27] = ((int)now.T0_X3);
		now.T0_X3 = 0;
#ifdef VAR_RANGES
		logval("T0_X3", ((int)now.T0_X3));
#endif
		;
		/* merge: T0_X3_1 = 0(55, 29, 55) */
		reached[0][29] = 1;
		(trpt+1)->bup.ovals[28] = ((int)now.T0_X3_1);
		now.T0_X3_1 = 0;
#ifdef VAR_RANGES
		logval("T0_X3_1", ((int)now.T0_X3_1));
#endif
		;
		/* merge: T0_X3_2 = 0(55, 30, 55) */
		reached[0][30] = 1;
		(trpt+1)->bup.ovals[29] = ((int)now.T0_X3_2);
		now.T0_X3_2 = 0;
#ifdef VAR_RANGES
		logval("T0_X3_2", ((int)now.T0_X3_2));
#endif
		;
		/* merge: T0_X3_3 = 0(55, 31, 55) */
		reached[0][31] = 1;
		(trpt+1)->bup.ovals[30] = ((int)now.T0_X3_3);
		now.T0_X3_3 = 0;
#ifdef VAR_RANGES
		logval("T0_X3_3", ((int)now.T0_X3_3));
#endif
		;
		/* merge: T0_X4 = 0(55, 32, 55) */
		reached[0][32] = 1;
		(trpt+1)->bup.ovals[31] = ((int)now.T0_X4);
		now.T0_X4 = 0;
#ifdef VAR_RANGES
		logval("T0_X4", ((int)now.T0_X4));
#endif
		;
		/* merge: T1_X0 = 0(55, 33, 55) */
		reached[0][33] = 1;
		(trpt+1)->bup.ovals[32] = ((int)now.T1_X0);
		now.T1_X0 = 0;
#ifdef VAR_RANGES
		logval("T1_X0", ((int)now.T1_X0));
#endif
		;
		/* merge: T1_X0_1 = 0(55, 34, 55) */
		reached[0][34] = 1;
		(trpt+1)->bup.ovals[33] = ((int)T1_X0_1);
		T1_X0_1 = 0;
#ifdef VAR_RANGES
		logval("T1_X0_1", ((int)T1_X0_1));
#endif
		;
		/* merge: T1_X0_2 = 0(55, 35, 55) */
		reached[0][35] = 1;
		(trpt+1)->bup.ovals[34] = ((int)T1_X0_2);
		T1_X0_2 = 0;
#ifdef VAR_RANGES
		logval("T1_X0_2", ((int)T1_X0_2));
#endif
		;
		/* merge: T1_X0_2_1 = 0(55, 36, 55) */
		reached[0][36] = 1;
		(trpt+1)->bup.ovals[35] = ((int)T1_X0_2_1);
		T1_X0_2_1 = 0;
#ifdef VAR_RANGES
		logval("T1_X0_2_1", ((int)T1_X0_2_1));
#endif
		;
		/* merge: T1_X0_2_2 = 0(55, 37, 55) */
		reached[0][37] = 1;
		(trpt+1)->bup.ovals[36] = ((int)T1_X0_2_2);
		T1_X0_2_2 = 0;
#ifdef VAR_RANGES
		logval("T1_X0_2_2", ((int)T1_X0_2_2));
#endif
		;
		/* merge: T1_X0_2_3 = 0(55, 38, 55) */
		reached[0][38] = 1;
		(trpt+1)->bup.ovals[37] = ((int)T1_X0_2_3);
		T1_X0_2_3 = 0;
#ifdef VAR_RANGES
		logval("T1_X0_2_3", ((int)T1_X0_2_3));
#endif
		;
		/* merge: T1_X1 = 0(55, 39, 55) */
		reached[0][39] = 1;
		(trpt+1)->bup.ovals[38] = ((int)now.T1_X1);
		now.T1_X1 = 0;
#ifdef VAR_RANGES
		logval("T1_X1", ((int)now.T1_X1));
#endif
		;
		/* merge: T1_X2 = 0(55, 40, 55) */
		reached[0][40] = 1;
		(trpt+1)->bup.ovals[39] = ((int)now.T1_X2);
		now.T1_X2 = 0;
#ifdef VAR_RANGES
		logval("T1_X2", ((int)now.T1_X2));
#endif
		;
		/* merge: T1_X3 = 0(55, 41, 55) */
		reached[0][41] = 1;
		(trpt+1)->bup.ovals[40] = ((int)now.T1_X3);
		now.T1_X3 = 0;
#ifdef VAR_RANGES
		logval("T1_X3", ((int)now.T1_X3));
#endif
		;
		/* merge: T2_X0 = 0(55, 42, 55) */
		reached[0][42] = 1;
		(trpt+1)->bup.ovals[41] = ((int)now.T2_X0);
		now.T2_X0 = 0;
#ifdef VAR_RANGES
		logval("T2_X0", ((int)now.T2_X0));
#endif
		;
		/* merge: T2_X1 = 0(55, 43, 55) */
		reached[0][43] = 1;
		(trpt+1)->bup.ovals[42] = ((int)now.T2_X1);
		now.T2_X1 = 0;
#ifdef VAR_RANGES
		logval("T2_X1", ((int)now.T2_X1));
#endif
		;
		/* merge: T2_X2 = 0(55, 44, 55) */
		reached[0][44] = 1;
		(trpt+1)->bup.ovals[43] = ((int)now.T2_X2);
		now.T2_X2 = 0;
#ifdef VAR_RANGES
		logval("T2_X2", ((int)now.T2_X2));
#endif
		;
		/* merge: T2_X3 = 0(55, 45, 55) */
		reached[0][45] = 1;
		(trpt+1)->bup.ovals[44] = ((int)now.T2_X3);
		now.T2_X3 = 0;
#ifdef VAR_RANGES
		logval("T2_X3", ((int)now.T2_X3));
#endif
		;
		/* merge: T2_X3_1 = 0(55, 46, 55) */
		reached[0][46] = 1;
		(trpt+1)->bup.ovals[45] = ((int)T2_X3_1);
		T2_X3_1 = 0;
#ifdef VAR_RANGES
		logval("T2_X3_1", ((int)T2_X3_1));
#endif
		;
		/* merge: T2_X3_2 = 0(55, 47, 55) */
		reached[0][47] = 1;
		(trpt+1)->bup.ovals[46] = ((int)T2_X3_2);
		T2_X3_2 = 0;
#ifdef VAR_RANGES
		logval("T2_X3_2", ((int)T2_X3_2));
#endif
		;
		/* merge: T2_X3_3 = 0(55, 48, 55) */
		reached[0][48] = 1;
		(trpt+1)->bup.ovals[47] = ((int)T2_X3_3);
		T2_X3_3 = 0;
#ifdef VAR_RANGES
		logval("T2_X3_3", ((int)T2_X3_3));
#endif
		;
		/* merge: T2_X4 = 0(55, 49, 55) */
		reached[0][49] = 1;
		(trpt+1)->bup.ovals[48] = ((int)now.T2_X4);
		now.T2_X4 = 0;
#ifdef VAR_RANGES
		logval("T2_X4", ((int)now.T2_X4));
#endif
		;
		/* merge: T2_X5 = 0(55, 50, 55) */
		reached[0][50] = 1;
		(trpt+1)->bup.ovals[49] = ((int)now.T2_X5);
		now.T2_X5 = 0;
#ifdef VAR_RANGES
		logval("T2_X5", ((int)now.T2_X5));
#endif
		;
		/* merge: T2_X6 = 0(55, 51, 55) */
		reached[0][51] = 1;
		(trpt+1)->bup.ovals[50] = ((int)T2_X6);
		T2_X6 = 0;
#ifdef VAR_RANGES
		logval("T2_X6", ((int)T2_X6));
#endif
		;
		/* merge: T3_X0 = 0(55, 52, 55) */
		reached[0][52] = 1;
		(trpt+1)->bup.ovals[51] = ((int)now.T3_X0);
		now.T3_X0 = 0;
#ifdef VAR_RANGES
		logval("T3_X0", ((int)now.T3_X0));
#endif
		;
		/* merge: T3_X1 = 0(55, 53, 55) */
		reached[0][53] = 1;
		(trpt+1)->bup.ovals[52] = ((int)now.T3_X1);
		now.T3_X1 = 0;
#ifdef VAR_RANGES
		logval("T3_X1", ((int)now.T3_X1));
#endif
		;
		_m = 3; goto P999; /* 52 */
	case 8: // STATE 55 - output.pml:105 - [current = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[0][55] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 0;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 9: // STATE 56 - output.pml:109 - [(running[0])] (0:0:0 - 1)
		IfNotBlocked
		reached[0][56] = 1;
		if (!(((int)now.running[0])))
			continue;
		_m = 3; goto P999; /* 0 */
	case 10: // STATE 57 - output.pml:111 - [(((!(running[1])&&!(running[2]))&&!(running[3])))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][57] = 1;
		if (!((( !(((int)now.running[1]))&& !(((int)now.running[2])))&& !(((int)now.running[3])))))
			continue;
		_m = 3; goto P999; /* 0 */
	case 11: // STATE 58 - output.pml:113 - [(1)] (61:0:1 - 1)
		IfNotBlocked
		reached[0][58] = 1;
		if (!(1))
			continue;
		/* merge: current = 255(0, 59, 61) */
		reached[0][59] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 255;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 12: // STATE 60 - output.pml:115 - [T0_X0 = CONST_NULL] (0:0:1 - 1)
		IfNotBlocked
		reached[0][60] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X0);
		now.T0_X0 = ((int)now.CONST_NULL);
#ifdef VAR_RANGES
		logval("T0_X0", ((int)now.T0_X0));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 13: // STATE 63 - output.pml:116 - [T0_X0_1 = 11] (0:0:1 - 1)
		IfNotBlocked
		reached[0][63] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X0_1);
		now.T0_X0_1 = 11;
#ifdef VAR_RANGES
		logval("T0_X0_1", ((int)now.T0_X0_1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 14: // STATE 66 - output.pml:117 - [T0_X0_2 = 11] (0:0:1 - 1)
		IfNotBlocked
		reached[0][66] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X0_2);
		now.T0_X0_2 = 11;
#ifdef VAR_RANGES
		logval("T0_X0_2", ((int)now.T0_X0_2));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 15: // STATE 69 - output.pml:118 - [T0_X0_2_1 = 11] (0:0:1 - 1)
		IfNotBlocked
		reached[0][69] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X0_2_1);
		now.T0_X0_2_1 = 11;
#ifdef VAR_RANGES
		logval("T0_X0_2_1", ((int)now.T0_X0_2_1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 16: // STATE 72 - output.pml:119 - [T0_X0_2_2 = 11] (0:0:1 - 1)
		IfNotBlocked
		reached[0][72] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X0_2_2);
		now.T0_X0_2_2 = 11;
#ifdef VAR_RANGES
		logval("T0_X0_2_2", ((int)now.T0_X0_2_2));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 17: // STATE 75 - output.pml:120 - [T0_X0_2_3 = 11] (0:0:1 - 1)
		IfNotBlocked
		reached[0][75] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X0_2_3);
		now.T0_X0_2_3 = 11;
#ifdef VAR_RANGES
		logval("T0_X0_2_3", ((int)now.T0_X0_2_3));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 18: // STATE 78 - output.pml:121 - [T0_X1 = S6] (0:0:1 - 1)
		IfNotBlocked
		reached[0][78] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X1);
		now.T0_X1 = ((int)now.S6);
#ifdef VAR_RANGES
		logval("T0_X1", ((int)now.T0_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 19: // STATE 79 - output.pml:121 - [T0_X1 = S5] (0:0:1 - 1)
		IfNotBlocked
		reached[0][79] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X1);
		now.T0_X1 = ((int)now.S5);
#ifdef VAR_RANGES
		logval("T0_X1", ((int)now.T0_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 20: // STATE 80 - output.pml:121 - [T0_X1 = S4] (0:0:1 - 1)
		IfNotBlocked
		reached[0][80] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X1);
		now.T0_X1 = ((int)now.S4);
#ifdef VAR_RANGES
		logval("T0_X1", ((int)now.T0_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 21: // STATE 81 - output.pml:121 - [T0_X1 = S1] (0:0:1 - 1)
		IfNotBlocked
		reached[0][81] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X1);
		now.T0_X1 = ((int)now.S1);
#ifdef VAR_RANGES
		logval("T0_X1", ((int)now.T0_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 22: // STATE 82 - output.pml:121 - [T0_X1 = S0] (0:0:1 - 1)
		IfNotBlocked
		reached[0][82] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X1);
		now.T0_X1 = ((int)now.S0);
#ifdef VAR_RANGES
		logval("T0_X1", ((int)now.T0_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 23: // STATE 83 - output.pml:121 - [T0_X1 = S2] (0:0:1 - 1)
		IfNotBlocked
		reached[0][83] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X1);
		now.T0_X1 = ((int)now.S2);
#ifdef VAR_RANGES
		logval("T0_X1", ((int)now.T0_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 24: // STATE 84 - output.pml:121 - [T0_X1 = S7] (0:0:1 - 1)
		IfNotBlocked
		reached[0][84] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X1);
		now.T0_X1 = ((int)now.S7);
#ifdef VAR_RANGES
		logval("T0_X1", ((int)now.T0_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 25: // STATE 85 - output.pml:121 - [T0_X1 = S3] (0:0:1 - 1)
		IfNotBlocked
		reached[0][85] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X1);
		now.T0_X1 = ((int)now.S3);
#ifdef VAR_RANGES
		logval("T0_X1", ((int)now.T0_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 26: // STATE 88 - output.pml:122 - [T0_X2 = CONST_NULL] (0:0:1 - 1)
		IfNotBlocked
		reached[0][88] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X2);
		now.T0_X2 = ((int)now.CONST_NULL);
#ifdef VAR_RANGES
		logval("T0_X2", ((int)now.T0_X2));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 27: // STATE 89 - output.pml:122 - [T0_X2 = N0] (0:0:1 - 1)
		IfNotBlocked
		reached[0][89] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X2);
		now.T0_X2 = ((int)now.N0);
#ifdef VAR_RANGES
		logval("T0_X2", ((int)now.T0_X2));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 28: // STATE 92 - output.pml:123 - [T0_X3 = CONST_NULL] (0:0:1 - 1)
		IfNotBlocked
		reached[0][92] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X3);
		now.T0_X3 = ((int)now.CONST_NULL);
#ifdef VAR_RANGES
		logval("T0_X3", ((int)now.T0_X3));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 29: // STATE 95 - output.pml:124 - [T0_X3_1 = 11] (0:0:1 - 1)
		IfNotBlocked
		reached[0][95] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X3_1);
		now.T0_X3_1 = 11;
#ifdef VAR_RANGES
		logval("T0_X3_1", ((int)now.T0_X3_1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 30: // STATE 98 - output.pml:125 - [T0_X3_2 = 11] (0:0:1 - 1)
		IfNotBlocked
		reached[0][98] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X3_2);
		now.T0_X3_2 = 11;
#ifdef VAR_RANGES
		logval("T0_X3_2", ((int)now.T0_X3_2));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 31: // STATE 101 - output.pml:126 - [T0_X3_3 = 11] (0:0:1 - 1)
		IfNotBlocked
		reached[0][101] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X3_3);
		now.T0_X3_3 = 11;
#ifdef VAR_RANGES
		logval("T0_X3_3", ((int)now.T0_X3_3));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 32: // STATE 104 - output.pml:127 - [T0_X4 = S6] (0:0:1 - 1)
		IfNotBlocked
		reached[0][104] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X4);
		now.T0_X4 = ((int)now.S6);
#ifdef VAR_RANGES
		logval("T0_X4", ((int)now.T0_X4));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 33: // STATE 105 - output.pml:127 - [T0_X4 = S7] (0:0:1 - 1)
		IfNotBlocked
		reached[0][105] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X4);
		now.T0_X4 = ((int)now.S7);
#ifdef VAR_RANGES
		logval("T0_X4", ((int)now.T0_X4));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 34: // STATE 106 - output.pml:127 - [T0_X4 = S3] (0:0:1 - 1)
		IfNotBlocked
		reached[0][106] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X4);
		now.T0_X4 = ((int)now.S3);
#ifdef VAR_RANGES
		logval("T0_X4", ((int)now.T0_X4));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 35: // STATE 107 - output.pml:127 - [T0_X4 = CONST_NULL] (0:0:1 - 1)
		IfNotBlocked
		reached[0][107] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X4);
		now.T0_X4 = ((int)now.CONST_NULL);
#ifdef VAR_RANGES
		logval("T0_X4", ((int)now.T0_X4));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 36: // STATE 108 - output.pml:127 - [T0_X4 = 11] (0:0:1 - 1)
		IfNotBlocked
		reached[0][108] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X4);
		now.T0_X4 = 11;
#ifdef VAR_RANGES
		logval("T0_X4", ((int)now.T0_X4));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 37: // STATE 109 - output.pml:127 - [T0_X4 = 12] (0:0:1 - 1)
		IfNotBlocked
		reached[0][109] = 1;
		(trpt+1)->bup.oval = ((int)now.T0_X4);
		now.T0_X4 = 12;
#ifdef VAR_RANGES
		logval("T0_X4", ((int)now.T0_X4));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 38: // STATE 112 - output.pml:130 - [((((((T0_X0!=CONST_NULL)&&(T0_X2!=N0))&&(T0_X1==S0))&&(T0_X3!=CONST_NULL))&&(T0_X4!=CONST_NULL)))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][112] = 1;
		if (!((((((((int)now.T0_X0)!=((int)now.CONST_NULL))&&(((int)now.T0_X2)!=((int)now.N0)))&&(((int)now.T0_X1)==((int)now.S0)))&&(((int)now.T0_X3)!=((int)now.CONST_NULL)))&&(((int)now.T0_X4)!=((int)now.CONST_NULL)))))
			continue;
		_m = 3; goto P999; /* 0 */
	case 39: // STATE 113 - output.pml:130 - [(1)] (262:0:1 - 1)
		IfNotBlocked
		reached[0][113] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(262, 115, 262) */
		reached[0][115] = 1;
		;
		/* merge: current = 0(262, 116, 262) */
		reached[0][116] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 0;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: .(goto)(262, 118, 262) */
		reached[0][118] = 1;
		;
		/* merge: .(goto)(0, 169, 262) */
		reached[0][169] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 5 */
	case 40: // STATE 116 - output.pml:132 - [current = 0] (0:262:1 - 2)
		IfNotBlocked
		reached[0][116] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 0;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: .(goto)(262, 118, 262) */
		reached[0][118] = 1;
		;
		/* merge: .(goto)(0, 169, 262) */
		reached[0][169] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 41: // STATE 120 - output.pml:134 - [((!(running[1])&&1))] (262:0:11 - 1)
		IfNotBlocked
		reached[0][120] = 1;
		if (!(( !(((int)now.running[1]))&&1)))
			continue;
		/* merge: current = 1(262, 121, 262) */
		reached[0][121] = 1;
		(trpt+1)->bup.ovals = grab_ints(11);
		(trpt+1)->bup.ovals[0] = ((int)now.current);
		now.current = 1;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: running[1] = 1(262, 122, 262) */
		reached[0][122] = 1;
		(trpt+1)->bup.ovals[1] = ((int)now.running[1]);
		now.running[1] = 1;
#ifdef VAR_RANGES
		logval("running[1]", ((int)now.running[1]));
#endif
		;
		/* merge: T1_X0 = T0_X0(262, 123, 262) */
		reached[0][123] = 1;
		(trpt+1)->bup.ovals[2] = ((int)now.T1_X0);
		now.T1_X0 = ((int)now.T0_X0);
#ifdef VAR_RANGES
		logval("T1_X0", ((int)now.T1_X0));
#endif
		;
		/* merge: T1_X0_1 = T0_X0_1(262, 124, 262) */
		reached[0][124] = 1;
		(trpt+1)->bup.ovals[3] = ((int)T1_X0_1);
		T1_X0_1 = ((int)now.T0_X0_1);
#ifdef VAR_RANGES
		logval("T1_X0_1", ((int)T1_X0_1));
#endif
		;
		/* merge: T1_X0_2 = T0_X0_2(262, 125, 262) */
		reached[0][125] = 1;
		(trpt+1)->bup.ovals[4] = ((int)T1_X0_2);
		T1_X0_2 = ((int)now.T0_X0_2);
#ifdef VAR_RANGES
		logval("T1_X0_2", ((int)T1_X0_2));
#endif
		;
		/* merge: T1_X0_2_1 = T0_X0_2_1(262, 126, 262) */
		reached[0][126] = 1;
		(trpt+1)->bup.ovals[5] = ((int)T1_X0_2_1);
		T1_X0_2_1 = ((int)now.T0_X0_2_1);
#ifdef VAR_RANGES
		logval("T1_X0_2_1", ((int)T1_X0_2_1));
#endif
		;
		/* merge: T1_X0_2_2 = T0_X0_2_2(262, 127, 262) */
		reached[0][127] = 1;
		(trpt+1)->bup.ovals[6] = ((int)T1_X0_2_2);
		T1_X0_2_2 = ((int)now.T0_X0_2_2);
#ifdef VAR_RANGES
		logval("T1_X0_2_2", ((int)T1_X0_2_2));
#endif
		;
		/* merge: T1_X0_2_3 = T0_X0_2_3(262, 128, 262) */
		reached[0][128] = 1;
		(trpt+1)->bup.ovals[7] = ((int)T1_X0_2_3);
		T1_X0_2_3 = ((int)now.T0_X0_2_3);
#ifdef VAR_RANGES
		logval("T1_X0_2_3", ((int)T1_X0_2_3));
#endif
		;
		/* merge: T1_X1 = 0(262, 129, 262) */
		reached[0][129] = 1;
		(trpt+1)->bup.ovals[8] = ((int)now.T1_X1);
		now.T1_X1 = 0;
#ifdef VAR_RANGES
		logval("T1_X1", ((int)now.T1_X1));
#endif
		;
		/* merge: T1_X2 = T0_X2(262, 130, 262) */
		reached[0][130] = 1;
		(trpt+1)->bup.ovals[9] = ((int)now.T1_X2);
		now.T1_X2 = ((int)now.T0_X2);
#ifdef VAR_RANGES
		logval("T1_X2", ((int)now.T1_X2));
#endif
		;
		/* merge: T1_X3 = T0_X4(262, 131, 262) */
		reached[0][131] = 1;
		(trpt+1)->bup.ovals[10] = ((int)now.T1_X3);
		now.T1_X3 = ((int)now.T0_X4);
#ifdef VAR_RANGES
		logval("T1_X3", ((int)now.T1_X3));
#endif
		;
		/* merge: .(goto)(0, 169, 262) */
		reached[0][169] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 13 */
	case 42: // STATE 133 - output.pml:147 - [((!(running[2])&&((T0_X1==S0)||(T0_X1==S2))))] (262:0:12 - 1)
		IfNotBlocked
		reached[0][133] = 1;
		if (!(( !(((int)now.running[2]))&&((((int)now.T0_X1)==((int)now.S0))||(((int)now.T0_X1)==((int)now.S2))))))
			continue;
		/* merge: current = 2(262, 134, 262) */
		reached[0][134] = 1;
		(trpt+1)->bup.ovals = grab_ints(12);
		(trpt+1)->bup.ovals[0] = ((int)now.current);
		now.current = 2;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: running[2] = 1(262, 135, 262) */
		reached[0][135] = 1;
		(trpt+1)->bup.ovals[1] = ((int)now.running[2]);
		now.running[2] = 1;
#ifdef VAR_RANGES
		logval("running[2]", ((int)now.running[2]));
#endif
		;
		/* merge: T2_X0 = 0(262, 136, 262) */
		reached[0][136] = 1;
		(trpt+1)->bup.ovals[2] = ((int)now.T2_X0);
		now.T2_X0 = 0;
#ifdef VAR_RANGES
		logval("T2_X0", ((int)now.T2_X0));
#endif
		;
		/* merge: T2_X1 = T0_X2(262, 137, 262) */
		reached[0][137] = 1;
		(trpt+1)->bup.ovals[3] = ((int)now.T2_X1);
		now.T2_X1 = ((int)now.T0_X2);
#ifdef VAR_RANGES
		logval("T2_X1", ((int)now.T2_X1));
#endif
		;
		/* merge: T2_X2 = 0(262, 138, 262) */
		reached[0][138] = 1;
		(trpt+1)->bup.ovals[4] = ((int)now.T2_X2);
		now.T2_X2 = 0;
#ifdef VAR_RANGES
		logval("T2_X2", ((int)now.T2_X2));
#endif
		;
		/* merge: T2_X3 = T0_X3(262, 139, 262) */
		reached[0][139] = 1;
		(trpt+1)->bup.ovals[5] = ((int)now.T2_X3);
		now.T2_X3 = ((int)now.T0_X3);
#ifdef VAR_RANGES
		logval("T2_X3", ((int)now.T2_X3));
#endif
		;
		/* merge: T2_X3_1 = T0_X3_1(262, 140, 262) */
		reached[0][140] = 1;
		(trpt+1)->bup.ovals[6] = ((int)T2_X3_1);
		T2_X3_1 = ((int)now.T0_X3_1);
#ifdef VAR_RANGES
		logval("T2_X3_1", ((int)T2_X3_1));
#endif
		;
		/* merge: T2_X3_2 = T0_X3_2(262, 141, 262) */
		reached[0][141] = 1;
		(trpt+1)->bup.ovals[7] = ((int)T2_X3_2);
		T2_X3_2 = ((int)now.T0_X3_2);
#ifdef VAR_RANGES
		logval("T2_X3_2", ((int)T2_X3_2));
#endif
		;
		/* merge: T2_X3_3 = T0_X3_3(262, 142, 262) */
		reached[0][142] = 1;
		(trpt+1)->bup.ovals[8] = ((int)T2_X3_3);
		T2_X3_3 = ((int)now.T0_X3_3);
#ifdef VAR_RANGES
		logval("T2_X3_3", ((int)T2_X3_3));
#endif
		;
		/* merge: T2_X4 = 0(262, 143, 262) */
		reached[0][143] = 1;
		(trpt+1)->bup.ovals[9] = ((int)now.T2_X4);
		now.T2_X4 = 0;
#ifdef VAR_RANGES
		logval("T2_X4", ((int)now.T2_X4));
#endif
		;
		/* merge: T2_X5 = T0_X4(262, 144, 262) */
		reached[0][144] = 1;
		(trpt+1)->bup.ovals[10] = ((int)now.T2_X5);
		now.T2_X5 = ((int)now.T0_X4);
#ifdef VAR_RANGES
		logval("T2_X5", ((int)now.T2_X5));
#endif
		;
		/* merge: T2_X6 = 0(262, 145, 262) */
		reached[0][145] = 1;
		(trpt+1)->bup.ovals[11] = ((int)T2_X6);
		T2_X6 = 0;
#ifdef VAR_RANGES
		logval("T2_X6", ((int)T2_X6));
#endif
		;
		/* merge: .(goto)(0, 169, 262) */
		reached[0][169] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 14 */
	case 43: // STATE 147 - output.pml:161 - [((!(running[3])&&(T0_X1==S3)))] (262:0:4 - 1)
		IfNotBlocked
		reached[0][147] = 1;
		if (!(( !(((int)now.running[3]))&&(((int)now.T0_X1)==((int)now.S3)))))
			continue;
		/* merge: current = 3(262, 148, 262) */
		reached[0][148] = 1;
		(trpt+1)->bup.ovals = grab_ints(4);
		(trpt+1)->bup.ovals[0] = ((int)now.current);
		now.current = 3;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: running[3] = 1(262, 149, 262) */
		reached[0][149] = 1;
		(trpt+1)->bup.ovals[1] = ((int)now.running[3]);
		now.running[3] = 1;
#ifdef VAR_RANGES
		logval("running[3]", ((int)now.running[3]));
#endif
		;
		/* merge: T3_X0 = 0(262, 150, 262) */
		reached[0][150] = 1;
		(trpt+1)->bup.ovals[2] = ((int)now.T3_X0);
		now.T3_X0 = 0;
#ifdef VAR_RANGES
		logval("T3_X0", ((int)now.T3_X0));
#endif
		;
		/* merge: T3_X1 = 0(262, 151, 262) */
		reached[0][151] = 1;
		(trpt+1)->bup.ovals[3] = ((int)now.T3_X1);
		now.T3_X1 = 0;
#ifdef VAR_RANGES
		logval("T3_X1", ((int)now.T3_X1));
#endif
		;
		/* merge: .(goto)(0, 169, 262) */
		reached[0][169] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 6 */
	case 44: // STATE 153 - output.pml:167 - [(ready[1])] (262:0:3 - 1)
		IfNotBlocked
		reached[0][153] = 1;
		if (!(((int)now.ready[1])))
			continue;
		/* merge: ready[1] = 0(262, 154, 262) */
		reached[0][154] = 1;
		(trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((int)now.ready[1]);
		now.ready[1] = 0;
#ifdef VAR_RANGES
		logval("ready[1]", ((int)now.ready[1]));
#endif
		;
		/* merge: running[1] = 0(262, 155, 262) */
		reached[0][155] = 1;
		(trpt+1)->bup.ovals[1] = ((int)now.running[1]);
		now.running[1] = 0;
#ifdef VAR_RANGES
		logval("running[1]", ((int)now.running[1]));
#endif
		;
		/* merge: T0_X1 = T1_X1(262, 156, 262) */
		reached[0][156] = 1;
		(trpt+1)->bup.ovals[2] = ((int)now.T0_X1);
		now.T0_X1 = ((int)now.T1_X1);
#ifdef VAR_RANGES
		logval("T0_X1", ((int)now.T0_X1));
#endif
		;
		/* merge: .(goto)(0, 169, 262) */
		reached[0][169] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 5 */
	case 45: // STATE 158 - output.pml:172 - [(ready[2])] (262:0:3 - 1)
		IfNotBlocked
		reached[0][158] = 1;
		if (!(((int)now.ready[2])))
			continue;
		/* merge: ready[2] = 0(262, 159, 262) */
		reached[0][159] = 1;
		(trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((int)now.ready[2]);
		now.ready[2] = 0;
#ifdef VAR_RANGES
		logval("ready[2]", ((int)now.ready[2]));
#endif
		;
		/* merge: running[2] = 0(262, 160, 262) */
		reached[0][160] = 1;
		(trpt+1)->bup.ovals[1] = ((int)now.running[2]);
		now.running[2] = 0;
#ifdef VAR_RANGES
		logval("running[2]", ((int)now.running[2]));
#endif
		;
		/* merge: T0_X1 = T2_X0(262, 161, 262) */
		reached[0][161] = 1;
		(trpt+1)->bup.ovals[2] = ((int)now.T0_X1);
		now.T0_X1 = ((int)now.T2_X0);
#ifdef VAR_RANGES
		logval("T0_X1", ((int)now.T0_X1));
#endif
		;
		/* merge: .(goto)(0, 169, 262) */
		reached[0][169] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 5 */
	case 46: // STATE 163 - output.pml:177 - [(ready[3])] (262:0:3 - 1)
		IfNotBlocked
		reached[0][163] = 1;
		if (!(((int)now.ready[3])))
			continue;
		/* merge: ready[3] = 0(262, 164, 262) */
		reached[0][164] = 1;
		(trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((int)now.ready[3]);
		now.ready[3] = 0;
#ifdef VAR_RANGES
		logval("ready[3]", ((int)now.ready[3]));
#endif
		;
		/* merge: running[3] = 0(262, 165, 262) */
		reached[0][165] = 1;
		(trpt+1)->bup.ovals[1] = ((int)now.running[3]);
		now.running[3] = 0;
#ifdef VAR_RANGES
		logval("running[3]", ((int)now.running[3]));
#endif
		;
		/* merge: T0_X1 = T3_X0(262, 166, 262) */
		reached[0][166] = 1;
		(trpt+1)->bup.ovals[2] = ((int)now.T0_X1);
		now.T0_X1 = ((int)now.T3_X0);
#ifdef VAR_RANGES
		logval("T0_X1", ((int)now.T0_X1));
#endif
		;
		/* merge: .(goto)(0, 169, 262) */
		reached[0][169] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 5 */
	case 47: // STATE 170 - output.pml:183 - [(running[1])] (0:0:0 - 1)
		IfNotBlocked
		reached[0][170] = 1;
		if (!(((int)now.running[1])))
			continue;
		_m = 3; goto P999; /* 0 */
	case 48: // STATE 172 - output.pml:187 - [(((T1_X0!=CONST_NULL)&&(T1_X1==S0)))] (182:0:1 - 1)
		IfNotBlocked
		reached[0][172] = 1;
		if (!(((((int)now.T1_X0)!=((int)now.CONST_NULL))&&(((int)now.T1_X1)==((int)now.S0)))))
			continue;
		/* merge: current = 255(0, 173, 182) */
		reached[0][173] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 255;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 49: // STATE 174 - output.pml:189 - [T1_X1 = S6] (0:0:1 - 1)
		IfNotBlocked
		reached[0][174] = 1;
		(trpt+1)->bup.oval = ((int)now.T1_X1);
		now.T1_X1 = ((int)now.S6);
#ifdef VAR_RANGES
		logval("T1_X1", ((int)now.T1_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 50: // STATE 175 - output.pml:189 - [T1_X1 = S5] (0:0:1 - 1)
		IfNotBlocked
		reached[0][175] = 1;
		(trpt+1)->bup.oval = ((int)now.T1_X1);
		now.T1_X1 = ((int)now.S5);
#ifdef VAR_RANGES
		logval("T1_X1", ((int)now.T1_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 51: // STATE 176 - output.pml:189 - [T1_X1 = S4] (0:0:1 - 1)
		IfNotBlocked
		reached[0][176] = 1;
		(trpt+1)->bup.oval = ((int)now.T1_X1);
		now.T1_X1 = ((int)now.S4);
#ifdef VAR_RANGES
		logval("T1_X1", ((int)now.T1_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 52: // STATE 177 - output.pml:189 - [T1_X1 = S1] (0:0:1 - 1)
		IfNotBlocked
		reached[0][177] = 1;
		(trpt+1)->bup.oval = ((int)now.T1_X1);
		now.T1_X1 = ((int)now.S1);
#ifdef VAR_RANGES
		logval("T1_X1", ((int)now.T1_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 53: // STATE 178 - output.pml:189 - [T1_X1 = S0] (0:0:1 - 1)
		IfNotBlocked
		reached[0][178] = 1;
		(trpt+1)->bup.oval = ((int)now.T1_X1);
		now.T1_X1 = ((int)now.S0);
#ifdef VAR_RANGES
		logval("T1_X1", ((int)now.T1_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 54: // STATE 179 - output.pml:189 - [T1_X1 = S2] (0:0:1 - 1)
		IfNotBlocked
		reached[0][179] = 1;
		(trpt+1)->bup.oval = ((int)now.T1_X1);
		now.T1_X1 = ((int)now.S2);
#ifdef VAR_RANGES
		logval("T1_X1", ((int)now.T1_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 55: // STATE 180 - output.pml:189 - [T1_X1 = S7] (0:0:1 - 1)
		IfNotBlocked
		reached[0][180] = 1;
		(trpt+1)->bup.oval = ((int)now.T1_X1);
		now.T1_X1 = ((int)now.S7);
#ifdef VAR_RANGES
		logval("T1_X1", ((int)now.T1_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 56: // STATE 181 - output.pml:189 - [T1_X1 = S3] (0:0:1 - 1)
		IfNotBlocked
		reached[0][181] = 1;
		(trpt+1)->bup.oval = ((int)now.T1_X1);
		now.T1_X1 = ((int)now.S3);
#ifdef VAR_RANGES
		logval("T1_X1", ((int)now.T1_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 57: // STATE 184 - output.pml:192 - [((((T1_X1==S2)&&(T1_X2!=CONST_NULL))&&(T1_X3!=CONST_NULL)))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][184] = 1;
		if (!((((((int)now.T1_X1)==((int)now.S2))&&(((int)now.T1_X2)!=((int)now.CONST_NULL)))&&(((int)now.T1_X3)!=((int)now.CONST_NULL)))))
			continue;
		_m = 3; goto P999; /* 0 */
	case 58: // STATE 185 - output.pml:192 - [(1)] (262:0:1 - 1)
		IfNotBlocked
		reached[0][185] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(262, 187, 262) */
		reached[0][187] = 1;
		;
		/* merge: current = 1(262, 188, 262) */
		reached[0][188] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 1;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: .(goto)(262, 190, 262) */
		reached[0][190] = 1;
		;
		/* merge: .(goto)(0, 197, 262) */
		reached[0][197] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 5 */
	case 59: // STATE 188 - output.pml:194 - [current = 1] (0:262:1 - 2)
		IfNotBlocked
		reached[0][188] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 1;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: .(goto)(262, 190, 262) */
		reached[0][190] = 1;
		;
		/* merge: .(goto)(0, 197, 262) */
		reached[0][197] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 60: // STATE 192 - output.pml:196 - [((T1_X1==S1))] (262:0:2 - 1)
		IfNotBlocked
		reached[0][192] = 1;
		if (!((((int)now.T1_X1)==((int)now.S1))))
			continue;
		/* merge: current = 0(262, 193, 262) */
		reached[0][193] = 1;
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((int)now.current);
		now.current = 0;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: ready[1] = 1(262, 194, 262) */
		reached[0][194] = 1;
		(trpt+1)->bup.ovals[1] = ((int)now.ready[1]);
		now.ready[1] = 1;
#ifdef VAR_RANGES
		logval("ready[1]", ((int)now.ready[1]));
#endif
		;
		/* merge: .(goto)(0, 197, 262) */
		reached[0][197] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 4 */
	case 61: // STATE 198 - output.pml:201 - [(running[2])] (0:0:0 - 1)
		IfNotBlocked
		reached[0][198] = 1;
		if (!(((int)now.running[2])))
			continue;
		_m = 3; goto P999; /* 0 */
	case 62: // STATE 200 - output.pml:205 - [(((T2_X0==S0)||(T2_X0==S2)))] (208:0:1 - 1)
		IfNotBlocked
		reached[0][200] = 1;
		if (!(((((int)now.T2_X0)==((int)now.S0))||(((int)now.T2_X0)==((int)now.S2)))))
			continue;
		/* merge: current = 255(0, 201, 208) */
		reached[0][201] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 255;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 63: // STATE 202 - output.pml:207 - [T2_X2 = S6] (0:0:1 - 1)
		IfNotBlocked
		reached[0][202] = 1;
		(trpt+1)->bup.oval = ((int)now.T2_X2);
		now.T2_X2 = ((int)now.S6);
#ifdef VAR_RANGES
		logval("T2_X2", ((int)now.T2_X2));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 64: // STATE 203 - output.pml:207 - [T2_X2 = S7] (0:0:1 - 1)
		IfNotBlocked
		reached[0][203] = 1;
		(trpt+1)->bup.oval = ((int)now.T2_X2);
		now.T2_X2 = ((int)now.S7);
#ifdef VAR_RANGES
		logval("T2_X2", ((int)now.T2_X2));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 65: // STATE 204 - output.pml:207 - [T2_X2 = S3] (0:0:1 - 1)
		IfNotBlocked
		reached[0][204] = 1;
		(trpt+1)->bup.oval = ((int)now.T2_X2);
		now.T2_X2 = ((int)now.S3);
#ifdef VAR_RANGES
		logval("T2_X2", ((int)now.T2_X2));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 66: // STATE 205 - output.pml:207 - [T2_X2 = CONST_NULL] (0:0:1 - 1)
		IfNotBlocked
		reached[0][205] = 1;
		(trpt+1)->bup.oval = ((int)now.T2_X2);
		now.T2_X2 = ((int)now.CONST_NULL);
#ifdef VAR_RANGES
		logval("T2_X2", ((int)now.T2_X2));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 67: // STATE 206 - output.pml:207 - [T2_X2 = 11] (0:0:1 - 1)
		IfNotBlocked
		reached[0][206] = 1;
		(trpt+1)->bup.oval = ((int)now.T2_X2);
		now.T2_X2 = 11;
#ifdef VAR_RANGES
		logval("T2_X2", ((int)now.T2_X2));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 68: // STATE 207 - output.pml:207 - [T2_X2 = 12] (0:0:1 - 1)
		IfNotBlocked
		reached[0][207] = 1;
		(trpt+1)->bup.oval = ((int)now.T2_X2);
		now.T2_X2 = 12;
#ifdef VAR_RANGES
		logval("T2_X2", ((int)now.T2_X2));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 69: // STATE 210 - output.pml:208 - [T2_X4 = CONST_NULL] (0:0:1 - 1)
		IfNotBlocked
		reached[0][210] = 1;
		(trpt+1)->bup.oval = ((int)now.T2_X4);
		now.T2_X4 = ((int)now.CONST_NULL);
#ifdef VAR_RANGES
		logval("T2_X4", ((int)now.T2_X4));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 70: // STATE 211 - output.pml:208 - [T2_X4 = N0] (0:0:1 - 1)
		IfNotBlocked
		reached[0][211] = 1;
		(trpt+1)->bup.oval = ((int)now.T2_X4);
		now.T2_X4 = ((int)now.N0);
#ifdef VAR_RANGES
		logval("T2_X4", ((int)now.T2_X4));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 71: // STATE 214 - output.pml:209 - [T2_X6 = N0] (0:0:1 - 1)
		IfNotBlocked
		reached[0][214] = 1;
		(trpt+1)->bup.oval = ((int)T2_X6);
		T2_X6 = ((int)now.N0);
#ifdef VAR_RANGES
		logval("T2_X6", ((int)T2_X6));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 72: // STATE 217 - output.pml:212 - [(((((((T2_X2!=T2_X5)||(T2_X3==CONST_NULL))||((T2_X4==T2_X1)&&((T2_X4==CONST_NULL)||((T2_X4!=CONST_NULL)&&(T2_X1!=CONST_NULL)))))||(T2_X0==S3))&&((((T2_X2!=T2_X5)||(T2_X3==CONST_NULL))||((T2_X4==T2_X1)&&((T2_X4==CONST_NULL)||((T2_X4!=CONST_NULL)&&(T2_X1!=CONST_NULL)))))||(T2_X0==S4)))&&((T2_X0!=S5)||((T2_X4==T2_X1)&&((T2_X4==CONST_NULL)||((T2_X4!=CONST_NULL)&&(T2_X1!=CONST_NULL)))))))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][217] = 1;
		if (!(((((((((int)now.T2_X2)!=((int)now.T2_X5))||(((int)now.T2_X3)==((int)now.CONST_NULL)))||((((int)now.T2_X4)==((int)now.T2_X1))&&((((int)now.T2_X4)==((int)now.CONST_NULL))||((((int)now.T2_X4)!=((int)now.CONST_NULL))&&(((int)now.T2_X1)!=((int)now.CONST_NULL))))))||(((int)now.T2_X0)==((int)now.S3)))&&((((((int)now.T2_X2)!=((int)now.T2_X5))||(((int)now.T2_X3)==((int)now.CONST_NULL)))||((((int)now.T2_X4)==((int)now.T2_X1))&&((((int)now.T2_X4)==((int)now.CONST_NULL))||((((int)now.T2_X4)!=((int)now.CONST_NULL))&&(((int)now.T2_X1)!=((int)now.CONST_NULL))))))||(((int)now.T2_X0)==((int)now.S4))))&&((((int)now.T2_X0)!=((int)now.S5))||((((int)now.T2_X4)==((int)now.T2_X1))&&((((int)now.T2_X4)==((int)now.CONST_NULL))||((((int)now.T2_X4)!=((int)now.CONST_NULL))&&(((int)now.T2_X1)!=((int)now.CONST_NULL)))))))))
			continue;
		_m = 3; goto P999; /* 0 */
	case 73: // STATE 218 - output.pml:212 - [(1)] (262:0:1 - 1)
		IfNotBlocked
		reached[0][218] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(262, 220, 262) */
		reached[0][220] = 1;
		;
		/* merge: current = 2(262, 221, 262) */
		reached[0][221] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 2;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: .(goto)(262, 223, 262) */
		reached[0][223] = 1;
		;
		/* merge: .(goto)(0, 230, 262) */
		reached[0][230] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 5 */
	case 74: // STATE 221 - output.pml:214 - [current = 2] (0:262:1 - 2)
		IfNotBlocked
		reached[0][221] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 2;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: .(goto)(262, 223, 262) */
		reached[0][223] = 1;
		;
		/* merge: .(goto)(0, 230, 262) */
		reached[0][230] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 75: // STATE 225 - output.pml:216 - [(((T2_X0==S3)||(T2_X0==S4)))] (262:0:2 - 1)
		IfNotBlocked
		reached[0][225] = 1;
		if (!(((((int)now.T2_X0)==((int)now.S3))||(((int)now.T2_X0)==((int)now.S4)))))
			continue;
		/* merge: current = 0(262, 226, 262) */
		reached[0][226] = 1;
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((int)now.current);
		now.current = 0;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: ready[2] = 1(262, 227, 262) */
		reached[0][227] = 1;
		(trpt+1)->bup.ovals[1] = ((int)now.ready[2]);
		now.ready[2] = 1;
#ifdef VAR_RANGES
		logval("ready[2]", ((int)now.ready[2]));
#endif
		;
		/* merge: .(goto)(0, 230, 262) */
		reached[0][230] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 4 */
	case 76: // STATE 231 - output.pml:221 - [(running[3])] (0:0:0 - 1)
		IfNotBlocked
		reached[0][231] = 1;
		if (!(((int)now.running[3])))
			continue;
		_m = 3; goto P999; /* 0 */
	case 77: // STATE 233 - output.pml:225 - [(((T3_X0==S3)&&(T3_X1!=S7)))] (243:0:1 - 1)
		IfNotBlocked
		reached[0][233] = 1;
		if (!(((((int)now.T3_X0)==((int)now.S3))&&(((int)now.T3_X1)!=((int)now.S7)))))
			continue;
		/* merge: current = 255(0, 234, 243) */
		reached[0][234] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 255;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 78: // STATE 235 - output.pml:227 - [T3_X0 = S6] (0:0:1 - 1)
		IfNotBlocked
		reached[0][235] = 1;
		(trpt+1)->bup.oval = ((int)now.T3_X0);
		now.T3_X0 = ((int)now.S6);
#ifdef VAR_RANGES
		logval("T3_X0", ((int)now.T3_X0));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 79: // STATE 236 - output.pml:227 - [T3_X0 = S5] (0:0:1 - 1)
		IfNotBlocked
		reached[0][236] = 1;
		(trpt+1)->bup.oval = ((int)now.T3_X0);
		now.T3_X0 = ((int)now.S5);
#ifdef VAR_RANGES
		logval("T3_X0", ((int)now.T3_X0));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 80: // STATE 237 - output.pml:227 - [T3_X0 = S4] (0:0:1 - 1)
		IfNotBlocked
		reached[0][237] = 1;
		(trpt+1)->bup.oval = ((int)now.T3_X0);
		now.T3_X0 = ((int)now.S4);
#ifdef VAR_RANGES
		logval("T3_X0", ((int)now.T3_X0));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 81: // STATE 238 - output.pml:227 - [T3_X0 = S1] (0:0:1 - 1)
		IfNotBlocked
		reached[0][238] = 1;
		(trpt+1)->bup.oval = ((int)now.T3_X0);
		now.T3_X0 = ((int)now.S1);
#ifdef VAR_RANGES
		logval("T3_X0", ((int)now.T3_X0));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 82: // STATE 239 - output.pml:227 - [T3_X0 = S0] (0:0:1 - 1)
		IfNotBlocked
		reached[0][239] = 1;
		(trpt+1)->bup.oval = ((int)now.T3_X0);
		now.T3_X0 = ((int)now.S0);
#ifdef VAR_RANGES
		logval("T3_X0", ((int)now.T3_X0));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 83: // STATE 240 - output.pml:227 - [T3_X0 = S2] (0:0:1 - 1)
		IfNotBlocked
		reached[0][240] = 1;
		(trpt+1)->bup.oval = ((int)now.T3_X0);
		now.T3_X0 = ((int)now.S2);
#ifdef VAR_RANGES
		logval("T3_X0", ((int)now.T3_X0));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 84: // STATE 241 - output.pml:227 - [T3_X0 = S7] (0:0:1 - 1)
		IfNotBlocked
		reached[0][241] = 1;
		(trpt+1)->bup.oval = ((int)now.T3_X0);
		now.T3_X0 = ((int)now.S7);
#ifdef VAR_RANGES
		logval("T3_X0", ((int)now.T3_X0));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 85: // STATE 242 - output.pml:227 - [T3_X0 = S3] (0:0:1 - 1)
		IfNotBlocked
		reached[0][242] = 1;
		(trpt+1)->bup.oval = ((int)now.T3_X0);
		now.T3_X0 = ((int)now.S3);
#ifdef VAR_RANGES
		logval("T3_X0", ((int)now.T3_X0));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 86: // STATE 245 - output.pml:228 - [T3_X1 = S7] (0:0:1 - 1)
		IfNotBlocked
		reached[0][245] = 1;
		(trpt+1)->bup.oval = ((int)now.T3_X1);
		now.T3_X1 = ((int)now.S7);
#ifdef VAR_RANGES
		logval("T3_X1", ((int)now.T3_X1));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 87: // STATE 248 - output.pml:231 - [((T3_X0==S6))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][248] = 1;
		if (!((((int)now.T3_X0)==((int)now.S6))))
			continue;
		_m = 3; goto P999; /* 0 */
	case 88: // STATE 249 - output.pml:231 - [(1)] (262:0:1 - 1)
		IfNotBlocked
		reached[0][249] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(262, 251, 262) */
		reached[0][251] = 1;
		;
		/* merge: current = 3(262, 252, 262) */
		reached[0][252] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 3;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: .(goto)(262, 254, 262) */
		reached[0][254] = 1;
		;
		/* merge: .(goto)(0, 261, 262) */
		reached[0][261] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 5 */
	case 89: // STATE 252 - output.pml:233 - [current = 3] (0:262:1 - 2)
		IfNotBlocked
		reached[0][252] = 1;
		(trpt+1)->bup.oval = ((int)now.current);
		now.current = 3;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: .(goto)(262, 254, 262) */
		reached[0][254] = 1;
		;
		/* merge: .(goto)(0, 261, 262) */
		reached[0][261] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 90: // STATE 256 - output.pml:235 - [((T3_X0==S6))] (262:0:2 - 1)
		IfNotBlocked
		reached[0][256] = 1;
		if (!((((int)now.T3_X0)==((int)now.S6))))
			continue;
		/* merge: current = 0(262, 257, 262) */
		reached[0][257] = 1;
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((int)now.current);
		now.current = 0;
#ifdef VAR_RANGES
		logval("current", ((int)now.current));
#endif
		;
		/* merge: ready[3] = 1(262, 258, 262) */
		reached[0][258] = 1;
		(trpt+1)->bup.ovals[1] = ((int)now.ready[3]);
		now.ready[3] = 1;
#ifdef VAR_RANGES
		logval("ready[3]", ((int)now.ready[3]));
#endif
		;
		/* merge: .(goto)(0, 261, 262) */
		reached[0][261] = 1;
		;
		/* merge: .(goto)(0, 263, 262) */
		reached[0][263] = 1;
		;
		_m = 3; goto P999; /* 4 */
	case 91: // STATE 266 - output.pml:242 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[0][266] = 1;
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */
	case  _T5:	/* np_ */
		if (!((!(trpt->o_pm&4) && !(trpt->tau&128))))
			continue;
		/* else fall through */
	case  _T2:	/* true */
		_m = 3; goto P999;
#undef rand
	}

