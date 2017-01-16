#ifdef PEG
struct T_SRC {
	char *fl; int ln;
} T_SRC[NTRANS];

void
tr_2_src(int m, char *file, int ln)
{	T_SRC[m].fl = file;
	T_SRC[m].ln = ln;
}

void
putpeg(int n, int m)
{	printf("%5d	trans %4d ", m, n);
	printf("%s:%d\n",
		T_SRC[n].fl, T_SRC[n].ln);
}
#endif

void
settable(void)
{	Trans *T;
	Trans *settr(int, int, int, int, int, char *, int, int, int);

	trans = (Trans ***) emalloc(3*sizeof(Trans **));

	/* proctype 1: never_0 */

	trans[1] = (Trans **) emalloc(14*sizeof(Trans *));

	T = trans[1][5] = settr(270,0,0,0,0,"IF", 0, 2, 0);
	T = T->nxt	= settr(270,0,1,0,0,"IF", 0, 2, 0);
	    T->nxt	= settr(270,0,3,0,0,"IF", 0, 2, 0);
	trans[1][1]	= settr(266,0,5,1,0,"(1)", 0, 2, 0);
	trans[1][2]	= settr(267,0,5,1,0,"goto T0_init", 0, 2, 0);
	trans[1][6]	= settr(271,0,11,1,0,".(goto)", 0, 2, 0);
	trans[1][3]	= settr(268,0,11,3,0,"(((!((T0_X4==S6))&&(T0_X4==S3))&&(current==0)))", 1, 2, 0);
	trans[1][4]	= settr(269,0,11,1,0,"goto accept_S2", 0, 2, 0);
	T = trans[1][11] = settr(276,0,0,0,0,"IF", 0, 2, 0);
	T = T->nxt	= settr(276,0,7,0,0,"IF", 0, 2, 0);
	    T->nxt	= settr(276,0,9,0,0,"IF", 0, 2, 0);
	trans[1][7]	= settr(272,0,11,4,0,"(!((T0_X4==S6)))", 1, 2, 0);
	trans[1][8]	= settr(273,0,11,1,0,"goto accept_S2", 0, 2, 0);
	trans[1][12]	= settr(277,0,13,1,0,".(goto)", 0, 2, 0);
	trans[1][9]	= settr(274,0,5,5,0,"(((T0_X4==S6)||!(running[0])))", 1, 2, 0);
	trans[1][10]	= settr(275,0,5,1,0,"goto T0_init", 0, 2, 0);
	trans[1][13]	= settr(278,0,0,6,6,"-end-", 0, 3500, 0);

	/* proctype 0: :init: */

	trans[0] = (Trans **) emalloc(267*sizeof(Trans *));

	T = trans[ 0][54] = settr(53,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(53,2,1,0,0,"ATOMIC", 1, 2, 0);
	trans[0][1]	= settr(0,4,55,7,7,"current = 255", 1, 2, 0); /* m: 2 -> 0,55 */
	reached0[2] = 1;
	trans[0][2]	= settr(0,0,0,0,0,"ready[0] = 0",0,0,0);
	trans[0][3]	= settr(0,0,0,0,0,"running[0] = 1",0,0,0);
	trans[0][4]	= settr(0,0,0,0,0,"ready[1] = 0",0,0,0);
	trans[0][5]	= settr(0,0,0,0,0,"running[1] = 0",0,0,0);
	trans[0][6]	= settr(0,0,0,0,0,"ready[2] = 0",0,0,0);
	trans[0][7]	= settr(0,0,0,0,0,"running[2] = 0",0,0,0);
	trans[0][8]	= settr(0,0,0,0,0,"ready[3] = 0",0,0,0);
	trans[0][9]	= settr(0,0,0,0,0,"running[3] = 0",0,0,0);
	trans[0][10]	= settr(0,0,0,0,0,"N0 = 0",0,0,0);
	trans[0][11]	= settr(0,0,0,0,0,"S0 = 1",0,0,0);
	trans[0][12]	= settr(0,0,0,0,0,"S1 = 2",0,0,0);
	trans[0][13]	= settr(0,0,0,0,0,"S2 = 3",0,0,0);
	trans[0][14]	= settr(0,0,0,0,0,"S3 = 4",0,0,0);
	trans[0][15]	= settr(0,0,0,0,0,"S4 = 5",0,0,0);
	trans[0][16]	= settr(0,0,0,0,0,"S5 = 6",0,0,0);
	trans[0][17]	= settr(0,0,0,0,0,"S6 = 7",0,0,0);
	trans[0][18]	= settr(0,0,0,0,0,"S7 = 0",0,0,0);
	trans[0][19]	= settr(0,0,0,0,0,"CONST_NULL = 0",0,0,0);
	trans[0][20]	= settr(0,0,0,0,0,"T0_X0 = 0",0,0,0);
	trans[0][21]	= settr(0,0,0,0,0,"T0_X0_1 = 0",0,0,0);
	trans[0][22]	= settr(0,0,0,0,0,"T0_X0_2 = 0",0,0,0);
	trans[0][23]	= settr(0,0,0,0,0,"T0_X0_2_1 = 0",0,0,0);
	trans[0][24]	= settr(0,0,0,0,0,"T0_X0_2_2 = 0",0,0,0);
	trans[0][25]	= settr(0,0,0,0,0,"T0_X0_2_3 = 0",0,0,0);
	trans[0][26]	= settr(0,0,0,0,0,"T0_X1 = 0",0,0,0);
	trans[0][27]	= settr(0,0,0,0,0,"T0_X2 = 0",0,0,0);
	trans[0][28]	= settr(0,0,0,0,0,"T0_X3 = 0",0,0,0);
	trans[0][29]	= settr(0,0,0,0,0,"T0_X3_1 = 0",0,0,0);
	trans[0][30]	= settr(0,0,0,0,0,"T0_X3_2 = 0",0,0,0);
	trans[0][31]	= settr(0,0,0,0,0,"T0_X3_3 = 0",0,0,0);
	trans[0][32]	= settr(0,0,0,0,0,"T0_X4 = 0",0,0,0);
	trans[0][33]	= settr(0,0,0,0,0,"T1_X0 = 0",0,0,0);
	trans[0][34]	= settr(0,0,0,0,0,"T1_X0_1 = 0",0,0,0);
	trans[0][35]	= settr(0,0,0,0,0,"T1_X0_2 = 0",0,0,0);
	trans[0][36]	= settr(0,0,0,0,0,"T1_X0_2_1 = 0",0,0,0);
	trans[0][37]	= settr(0,0,0,0,0,"T1_X0_2_2 = 0",0,0,0);
	trans[0][38]	= settr(0,0,0,0,0,"T1_X0_2_3 = 0",0,0,0);
	trans[0][39]	= settr(0,0,0,0,0,"T1_X1 = 0",0,0,0);
	trans[0][40]	= settr(0,0,0,0,0,"T1_X2 = 0",0,0,0);
	trans[0][41]	= settr(0,0,0,0,0,"T1_X3 = 0",0,0,0);
	trans[0][42]	= settr(0,0,0,0,0,"T2_X0 = 0",0,0,0);
	trans[0][43]	= settr(0,0,0,0,0,"T2_X1 = 0",0,0,0);
	trans[0][44]	= settr(0,0,0,0,0,"T2_X2 = 0",0,0,0);
	trans[0][45]	= settr(0,0,0,0,0,"T2_X3 = 0",0,0,0);
	trans[0][46]	= settr(0,0,0,0,0,"T2_X3_1 = 0",0,0,0);
	trans[0][47]	= settr(0,0,0,0,0,"T2_X3_2 = 0",0,0,0);
	trans[0][48]	= settr(0,0,0,0,0,"T2_X3_3 = 0",0,0,0);
	trans[0][49]	= settr(0,0,0,0,0,"T2_X4 = 0",0,0,0);
	trans[0][50]	= settr(0,0,0,0,0,"T2_X5 = 0",0,0,0);
	trans[0][51]	= settr(0,0,0,0,0,"T2_X6 = 0",0,0,0);
	trans[0][52]	= settr(0,0,0,0,0,"T3_X0 = 0",0,0,0);
	trans[0][53]	= settr(0,0,0,0,0,"T3_X1 = 0",0,0,0);
	trans[0][55]	= settr(54,0,262,8,8,"current = 0", 1, 2, 0);
	trans[0][263]	= settr(262,0,262,1,0,".(goto)", 0, 2, 0);
	T = trans[0][262] = settr(261,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(261,0,56,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(261,0,170,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(261,0,198,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(261,0,231,0,0,"DO", 0, 2, 0);
	trans[0][56]	= settr(55,0,168,9,0,"(running[0])", 1, 2, 0);
	T = trans[0][168] = settr(167,0,0,0,0,"IF", 0, 2, 0);
	T = T->nxt	= settr(167,0,119,0,0,"IF", 0, 2, 0);
	T = T->nxt	= settr(167,0,132,0,0,"IF", 0, 2, 0);
	T = T->nxt	= settr(167,0,146,0,0,"IF", 0, 2, 0);
	T = T->nxt	= settr(167,0,152,0,0,"IF", 0, 2, 0);
	T = T->nxt	= settr(167,0,157,0,0,"IF", 0, 2, 0);
	T = T->nxt	= settr(167,0,162,0,0,"IF", 0, 2, 0);
	    T->nxt	= settr(167,0,167,0,0,"IF", 0, 2, 0);
	T = trans[ 0][119] = settr(118,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(118,2,57,0,0,"ATOMIC", 1, 2, 0);
	trans[0][57]	= settr(56,2,117,10,0,"(((!(running[1])&&!(running[2]))&&!(running[3])))", 1, 2, 0);
	T = trans[0][117] = settr(116,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(116,2,58,0,0,"IF", 1, 2, 0);
	trans[0][58]	= settr(57,2,61,11,11,"(1)", 1, 2, 0); /* m: 59 -> 61,0 */
	reached0[59] = 1;
	trans[0][59]	= settr(0,0,0,0,0,"current = 255",0,0,0);
	T = trans[0][61] = settr(60,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(60,2,60,0,0,"IF", 1, 2, 0);
	trans[0][60]	= settr(59,2,64,12,12,"T0_X0 = CONST_NULL", 1, 2, 0);
	trans[0][62]	= settr(61,2,64,1,0,".(goto)", 1, 2, 0);
	T = trans[0][64] = settr(63,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(63,2,63,0,0,"IF", 1, 2, 0);
	trans[0][63]	= settr(62,2,67,13,13,"T0_X0_1 = 11", 1, 2, 0);
	trans[0][65]	= settr(64,2,67,1,0,".(goto)", 1, 2, 0);
	T = trans[0][67] = settr(66,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(66,2,66,0,0,"IF", 1, 2, 0);
	trans[0][66]	= settr(65,2,70,14,14,"T0_X0_2 = 11", 1, 2, 0);
	trans[0][68]	= settr(67,2,70,1,0,".(goto)", 1, 2, 0);
	T = trans[0][70] = settr(69,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(69,2,69,0,0,"IF", 1, 2, 0);
	trans[0][69]	= settr(68,2,73,15,15,"T0_X0_2_1 = 11", 1, 2, 0);
	trans[0][71]	= settr(70,2,73,1,0,".(goto)", 1, 2, 0);
	T = trans[0][73] = settr(72,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(72,2,72,0,0,"IF", 1, 2, 0);
	trans[0][72]	= settr(71,2,76,16,16,"T0_X0_2_2 = 11", 1, 2, 0);
	trans[0][74]	= settr(73,2,76,1,0,".(goto)", 1, 2, 0);
	T = trans[0][76] = settr(75,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(75,2,75,0,0,"IF", 1, 2, 0);
	trans[0][75]	= settr(74,2,86,17,17,"T0_X0_2_3 = 11", 1, 2, 0);
	trans[0][77]	= settr(76,2,86,1,0,".(goto)", 1, 2, 0);
	T = trans[0][86] = settr(85,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(85,2,78,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(85,2,79,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(85,2,80,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(85,2,81,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(85,2,82,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(85,2,83,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(85,2,84,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(85,2,85,0,0,"IF", 1, 2, 0);
	trans[0][78]	= settr(77,2,90,18,18,"T0_X1 = S6", 1, 2, 0);
	trans[0][87]	= settr(86,2,90,1,0,".(goto)", 1, 2, 0);
	trans[0][79]	= settr(78,2,90,19,19,"T0_X1 = S5", 1, 2, 0);
	trans[0][80]	= settr(79,2,90,20,20,"T0_X1 = S4", 1, 2, 0);
	trans[0][81]	= settr(80,2,90,21,21,"T0_X1 = S1", 1, 2, 0);
	trans[0][82]	= settr(81,2,90,22,22,"T0_X1 = S0", 1, 2, 0);
	trans[0][83]	= settr(82,2,90,23,23,"T0_X1 = S2", 1, 2, 0);
	trans[0][84]	= settr(83,2,90,24,24,"T0_X1 = S7", 1, 2, 0);
	trans[0][85]	= settr(84,2,90,25,25,"T0_X1 = S3", 1, 2, 0);
	T = trans[0][90] = settr(89,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(89,2,88,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(89,2,89,0,0,"IF", 1, 2, 0);
	trans[0][88]	= settr(87,2,93,26,26,"T0_X2 = CONST_NULL", 1, 2, 0);
	trans[0][91]	= settr(90,2,93,1,0,".(goto)", 1, 2, 0);
	trans[0][89]	= settr(88,2,93,27,27,"T0_X2 = N0", 1, 2, 0);
	T = trans[0][93] = settr(92,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(92,2,92,0,0,"IF", 1, 2, 0);
	trans[0][92]	= settr(91,2,96,28,28,"T0_X3 = CONST_NULL", 1, 2, 0);
	trans[0][94]	= settr(93,2,96,1,0,".(goto)", 1, 2, 0);
	T = trans[0][96] = settr(95,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(95,2,95,0,0,"IF", 1, 2, 0);
	trans[0][95]	= settr(94,2,99,29,29,"T0_X3_1 = 11", 1, 2, 0);
	trans[0][97]	= settr(96,2,99,1,0,".(goto)", 1, 2, 0);
	T = trans[0][99] = settr(98,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(98,2,98,0,0,"IF", 1, 2, 0);
	trans[0][98]	= settr(97,2,102,30,30,"T0_X3_2 = 11", 1, 2, 0);
	trans[0][100]	= settr(99,2,102,1,0,".(goto)", 1, 2, 0);
	T = trans[0][102] = settr(101,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(101,2,101,0,0,"IF", 1, 2, 0);
	trans[0][101]	= settr(100,2,110,31,31,"T0_X3_3 = 11", 1, 2, 0);
	trans[0][103]	= settr(102,2,110,1,0,".(goto)", 1, 2, 0);
	T = trans[0][110] = settr(109,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(109,2,104,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(109,2,105,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(109,2,106,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(109,2,107,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(109,2,108,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(109,2,109,0,0,"IF", 1, 2, 0);
	trans[0][104]	= settr(103,2,114,32,32,"T0_X4 = S6", 1, 2, 0);
	trans[0][111]	= settr(110,2,114,1,0,".(goto)", 1, 2, 0);
	trans[0][105]	= settr(104,2,114,33,33,"T0_X4 = S7", 1, 2, 0);
	trans[0][106]	= settr(105,2,114,34,34,"T0_X4 = S3", 1, 2, 0);
	trans[0][107]	= settr(106,2,114,35,35,"T0_X4 = CONST_NULL", 1, 2, 0);
	trans[0][108]	= settr(107,2,114,36,36,"T0_X4 = 11", 1, 2, 0);
	trans[0][109]	= settr(108,2,114,37,37,"T0_X4 = 12", 1, 2, 0);
	T = trans[0][114] = settr(113,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(113,2,112,0,0,"IF", 1, 2, 0);
	trans[0][112]	= settr(111,2,113,38,0,"((((((T0_X0!=CONST_NULL)&&(T0_X2!=N0))&&(T0_X1==S0))&&(T0_X3!=CONST_NULL))&&(T0_X4!=CONST_NULL)))", 1, 2, 0);
	trans[0][113]	= settr(112,0,262,39,39,"(1)", 1, 2, 0); /* m: 116 -> 262,0 */
	reached0[116] = 1;
	trans[0][115]	= settr(114,2,116,1,0,".(goto)", 1, 2, 0); /* m: 116 -> 0,262 */
	reached0[116] = 1;
	trans[0][116]	= settr(115,0,262,40,40,"current = 0", 1, 2, 0); /* m: 118 -> 0,262 */
	reached0[118] = 1;
	trans[0][118]	= settr(117,0,262,1,0,".(goto)", 1, 2, 0);
	trans[0][169]	= settr(168,0,262,1,0,".(goto)", 0, 2, 0);
	T = trans[ 0][132] = settr(131,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(131,2,120,0,0,"ATOMIC", 1, 2, 0);
	trans[0][120]	= settr(119,0,262,41,41,"((!(running[1])&&1))", 1, 2, 0); /* m: 121 -> 262,0 */
	reached0[121] = 1;
	trans[0][121]	= settr(0,0,0,0,0,"current = 1",0,0,0);
	trans[0][122]	= settr(0,0,0,0,0,"running[1] = 1",0,0,0);
	trans[0][123]	= settr(0,0,0,0,0,"T1_X0 = T0_X0",0,0,0);
	trans[0][124]	= settr(0,0,0,0,0,"T1_X0_1 = T0_X0_1",0,0,0);
	trans[0][125]	= settr(0,0,0,0,0,"T1_X0_2 = T0_X0_2",0,0,0);
	trans[0][126]	= settr(0,0,0,0,0,"T1_X0_2_1 = T0_X0_2_1",0,0,0);
	trans[0][127]	= settr(0,0,0,0,0,"T1_X0_2_2 = T0_X0_2_2",0,0,0);
	trans[0][128]	= settr(0,0,0,0,0,"T1_X0_2_3 = T0_X0_2_3",0,0,0);
	trans[0][129]	= settr(0,0,0,0,0,"T1_X1 = 0",0,0,0);
	trans[0][130]	= settr(0,0,0,0,0,"T1_X2 = T0_X2",0,0,0);
	trans[0][131]	= settr(0,0,0,0,0,"T1_X3 = T0_X4",0,0,0);
	T = trans[ 0][146] = settr(145,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(145,2,133,0,0,"ATOMIC", 1, 2, 0);
	trans[0][133]	= settr(132,0,262,42,42,"((!(running[2])&&((T0_X1==S0)||(T0_X1==S2))))", 1, 2, 0); /* m: 134 -> 262,0 */
	reached0[134] = 1;
	trans[0][134]	= settr(0,0,0,0,0,"current = 2",0,0,0);
	trans[0][135]	= settr(0,0,0,0,0,"running[2] = 1",0,0,0);
	trans[0][136]	= settr(0,0,0,0,0,"T2_X0 = 0",0,0,0);
	trans[0][137]	= settr(0,0,0,0,0,"T2_X1 = T0_X2",0,0,0);
	trans[0][138]	= settr(0,0,0,0,0,"T2_X2 = 0",0,0,0);
	trans[0][139]	= settr(0,0,0,0,0,"T2_X3 = T0_X3",0,0,0);
	trans[0][140]	= settr(0,0,0,0,0,"T2_X3_1 = T0_X3_1",0,0,0);
	trans[0][141]	= settr(0,0,0,0,0,"T2_X3_2 = T0_X3_2",0,0,0);
	trans[0][142]	= settr(0,0,0,0,0,"T2_X3_3 = T0_X3_3",0,0,0);
	trans[0][143]	= settr(0,0,0,0,0,"T2_X4 = 0",0,0,0);
	trans[0][144]	= settr(0,0,0,0,0,"T2_X5 = T0_X4",0,0,0);
	trans[0][145]	= settr(0,0,0,0,0,"T2_X6 = 0",0,0,0);
	T = trans[ 0][152] = settr(151,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(151,2,147,0,0,"ATOMIC", 1, 2, 0);
	trans[0][147]	= settr(146,0,262,43,43,"((!(running[3])&&(T0_X1==S3)))", 1, 2, 0); /* m: 148 -> 262,0 */
	reached0[148] = 1;
	trans[0][148]	= settr(0,0,0,0,0,"current = 3",0,0,0);
	trans[0][149]	= settr(0,0,0,0,0,"running[3] = 1",0,0,0);
	trans[0][150]	= settr(0,0,0,0,0,"T3_X0 = 0",0,0,0);
	trans[0][151]	= settr(0,0,0,0,0,"T3_X1 = 0",0,0,0);
	T = trans[ 0][157] = settr(156,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(156,2,153,0,0,"ATOMIC", 1, 2, 0);
	trans[0][153]	= settr(152,0,262,44,44,"(ready[1])", 1, 2, 0); /* m: 154 -> 262,0 */
	reached0[154] = 1;
	trans[0][154]	= settr(0,0,0,0,0,"ready[1] = 0",0,0,0);
	trans[0][155]	= settr(0,0,0,0,0,"running[1] = 0",0,0,0);
	trans[0][156]	= settr(0,0,0,0,0,"T0_X1 = T1_X1",0,0,0);
	T = trans[ 0][162] = settr(161,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(161,2,158,0,0,"ATOMIC", 1, 2, 0);
	trans[0][158]	= settr(157,0,262,45,45,"(ready[2])", 1, 2, 0); /* m: 159 -> 262,0 */
	reached0[159] = 1;
	trans[0][159]	= settr(0,0,0,0,0,"ready[2] = 0",0,0,0);
	trans[0][160]	= settr(0,0,0,0,0,"running[2] = 0",0,0,0);
	trans[0][161]	= settr(0,0,0,0,0,"T0_X1 = T2_X0",0,0,0);
	T = trans[ 0][167] = settr(166,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(166,2,163,0,0,"ATOMIC", 1, 2, 0);
	trans[0][163]	= settr(162,0,262,46,46,"(ready[3])", 1, 2, 0); /* m: 164 -> 262,0 */
	reached0[164] = 1;
	trans[0][164]	= settr(0,0,0,0,0,"ready[3] = 0",0,0,0);
	trans[0][165]	= settr(0,0,0,0,0,"running[3] = 0",0,0,0);
	trans[0][166]	= settr(0,0,0,0,0,"T0_X1 = T3_X0",0,0,0);
	trans[0][170]	= settr(169,0,196,47,0,"(running[1])", 1, 2, 0);
	T = trans[0][196] = settr(195,0,0,0,0,"IF", 0, 2, 0);
	T = T->nxt	= settr(195,0,191,0,0,"IF", 0, 2, 0);
	    T->nxt	= settr(195,0,195,0,0,"IF", 0, 2, 0);
	T = trans[ 0][191] = settr(190,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(190,2,171,0,0,"ATOMIC", 1, 2, 0);
	trans[0][171]	= settr(170,2,189,1,0,"(1)", 1, 2, 0);
	T = trans[0][189] = settr(188,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(188,2,172,0,0,"IF", 1, 2, 0);
	trans[0][172]	= settr(171,2,182,48,48,"(((T1_X0!=CONST_NULL)&&(T1_X1==S0)))", 1, 2, 0); /* m: 173 -> 182,0 */
	reached0[173] = 1;
	trans[0][173]	= settr(0,0,0,0,0,"current = 255",0,0,0);
	T = trans[0][182] = settr(181,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(181,2,174,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(181,2,175,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(181,2,176,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(181,2,177,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(181,2,178,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(181,2,179,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(181,2,180,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(181,2,181,0,0,"IF", 1, 2, 0);
	trans[0][174]	= settr(173,2,186,49,49,"T1_X1 = S6", 1, 2, 0);
	trans[0][183]	= settr(182,2,186,1,0,".(goto)", 1, 2, 0);
	trans[0][175]	= settr(174,2,186,50,50,"T1_X1 = S5", 1, 2, 0);
	trans[0][176]	= settr(175,2,186,51,51,"T1_X1 = S4", 1, 2, 0);
	trans[0][177]	= settr(176,2,186,52,52,"T1_X1 = S1", 1, 2, 0);
	trans[0][178]	= settr(177,2,186,53,53,"T1_X1 = S0", 1, 2, 0);
	trans[0][179]	= settr(178,2,186,54,54,"T1_X1 = S2", 1, 2, 0);
	trans[0][180]	= settr(179,2,186,55,55,"T1_X1 = S7", 1, 2, 0);
	trans[0][181]	= settr(180,2,186,56,56,"T1_X1 = S3", 1, 2, 0);
	T = trans[0][186] = settr(185,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(185,2,184,0,0,"IF", 1, 2, 0);
	trans[0][184]	= settr(183,2,185,57,0,"((((T1_X1==S2)&&(T1_X2!=CONST_NULL))&&(T1_X3!=CONST_NULL)))", 1, 2, 0);
	trans[0][185]	= settr(184,0,262,58,58,"(1)", 1, 2, 0); /* m: 188 -> 262,0 */
	reached0[188] = 1;
	trans[0][187]	= settr(186,2,188,1,0,".(goto)", 1, 2, 0); /* m: 188 -> 0,262 */
	reached0[188] = 1;
	trans[0][188]	= settr(187,0,262,59,59,"current = 1", 1, 2, 0); /* m: 190 -> 0,262 */
	reached0[190] = 1;
	trans[0][190]	= settr(189,0,262,1,0,".(goto)", 1, 2, 0);
	trans[0][197]	= settr(196,0,262,1,0,".(goto)", 0, 2, 0);
	T = trans[ 0][195] = settr(194,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(194,2,192,0,0,"ATOMIC", 1, 2, 0);
	trans[0][192]	= settr(191,0,262,60,60,"((T1_X1==S1))", 1, 2, 0); /* m: 193 -> 262,0 */
	reached0[193] = 1;
	trans[0][193]	= settr(0,0,0,0,0,"current = 0",0,0,0);
	trans[0][194]	= settr(0,0,0,0,0,"ready[1] = 1",0,0,0);
	trans[0][198]	= settr(197,0,229,61,0,"(running[2])", 1, 2, 0);
	T = trans[0][229] = settr(228,0,0,0,0,"IF", 0, 2, 0);
	T = T->nxt	= settr(228,0,224,0,0,"IF", 0, 2, 0);
	    T->nxt	= settr(228,0,228,0,0,"IF", 0, 2, 0);
	T = trans[ 0][224] = settr(223,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(223,2,199,0,0,"ATOMIC", 1, 2, 0);
	trans[0][199]	= settr(198,2,222,1,0,"(1)", 1, 2, 0);
	T = trans[0][222] = settr(221,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(221,2,200,0,0,"IF", 1, 2, 0);
	trans[0][200]	= settr(199,2,208,62,62,"(((T2_X0==S0)||(T2_X0==S2)))", 1, 2, 0); /* m: 201 -> 208,0 */
	reached0[201] = 1;
	trans[0][201]	= settr(0,0,0,0,0,"current = 255",0,0,0);
	T = trans[0][208] = settr(207,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(207,2,202,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(207,2,203,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(207,2,204,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(207,2,205,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(207,2,206,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(207,2,207,0,0,"IF", 1, 2, 0);
	trans[0][202]	= settr(201,2,212,63,63,"T2_X2 = S6", 1, 2, 0);
	trans[0][209]	= settr(208,2,212,1,0,".(goto)", 1, 2, 0);
	trans[0][203]	= settr(202,2,212,64,64,"T2_X2 = S7", 1, 2, 0);
	trans[0][204]	= settr(203,2,212,65,65,"T2_X2 = S3", 1, 2, 0);
	trans[0][205]	= settr(204,2,212,66,66,"T2_X2 = CONST_NULL", 1, 2, 0);
	trans[0][206]	= settr(205,2,212,67,67,"T2_X2 = 11", 1, 2, 0);
	trans[0][207]	= settr(206,2,212,68,68,"T2_X2 = 12", 1, 2, 0);
	T = trans[0][212] = settr(211,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(211,2,210,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(211,2,211,0,0,"IF", 1, 2, 0);
	trans[0][210]	= settr(209,2,215,69,69,"T2_X4 = CONST_NULL", 1, 2, 0);
	trans[0][213]	= settr(212,2,215,1,0,".(goto)", 1, 2, 0);
	trans[0][211]	= settr(210,2,215,70,70,"T2_X4 = N0", 1, 2, 0);
	T = trans[0][215] = settr(214,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(214,2,214,0,0,"IF", 1, 2, 0);
	trans[0][214]	= settr(213,2,219,71,71,"T2_X6 = N0", 1, 2, 0);
	trans[0][216]	= settr(215,2,219,1,0,".(goto)", 1, 2, 0);
	T = trans[0][219] = settr(218,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(218,2,217,0,0,"IF", 1, 2, 0);
	trans[0][217]	= settr(216,2,218,72,0,"(((((((T2_X2!=T2_X5)||(T2_X3==CONST_NULL))||((T2_X4==T2_X1)&&((T2_X4==CONST_NULL)||((T2_X4!=CONST_NULL)&&(T2_X1!=CONST_NULL)))))||(T2_X0==S3))&&((((T2_X2!=T2_X5)||(T2_X3==CONST_NULL))||((T2_X4==T2_X1)&&((T2_X4==CONST_NULL)||((T2_X4!=CONST_NULL)&&(T2_X1!=CONST_NULL)))))||(T2_X0==S4)))&&((T2_X0!=S5)||((T2_X4==T2_X1)&&((T2_X4==CONST_NULL)||((T2_X4!=CONST_NULL)&&(T2_X1!=CONST_NULL)))))))", 1, 2, 0);
	trans[0][218]	= settr(217,0,262,73,73,"(1)", 1, 2, 0); /* m: 221 -> 262,0 */
	reached0[221] = 1;
	trans[0][220]	= settr(219,2,221,1,0,".(goto)", 1, 2, 0); /* m: 221 -> 0,262 */
	reached0[221] = 1;
	trans[0][221]	= settr(220,0,262,74,74,"current = 2", 1, 2, 0); /* m: 223 -> 0,262 */
	reached0[223] = 1;
	trans[0][223]	= settr(222,0,262,1,0,".(goto)", 1, 2, 0);
	trans[0][230]	= settr(229,0,262,1,0,".(goto)", 0, 2, 0);
	T = trans[ 0][228] = settr(227,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(227,2,225,0,0,"ATOMIC", 1, 2, 0);
	trans[0][225]	= settr(224,0,262,75,75,"(((T2_X0==S3)||(T2_X0==S4)))", 1, 2, 0); /* m: 226 -> 262,0 */
	reached0[226] = 1;
	trans[0][226]	= settr(0,0,0,0,0,"current = 0",0,0,0);
	trans[0][227]	= settr(0,0,0,0,0,"ready[2] = 1",0,0,0);
	trans[0][231]	= settr(230,0,260,76,0,"(running[3])", 1, 2, 0);
	T = trans[0][260] = settr(259,0,0,0,0,"IF", 0, 2, 0);
	T = T->nxt	= settr(259,0,255,0,0,"IF", 0, 2, 0);
	    T->nxt	= settr(259,0,259,0,0,"IF", 0, 2, 0);
	T = trans[ 0][255] = settr(254,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(254,2,232,0,0,"ATOMIC", 1, 2, 0);
	trans[0][232]	= settr(231,2,253,1,0,"(1)", 1, 2, 0);
	T = trans[0][253] = settr(252,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(252,2,233,0,0,"IF", 1, 2, 0);
	trans[0][233]	= settr(232,2,243,77,77,"(((T3_X0==S3)&&(T3_X1!=S7)))", 1, 2, 0); /* m: 234 -> 243,0 */
	reached0[234] = 1;
	trans[0][234]	= settr(0,0,0,0,0,"current = 255",0,0,0);
	T = trans[0][243] = settr(242,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(242,2,235,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(242,2,236,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(242,2,237,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(242,2,238,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(242,2,239,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(242,2,240,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(242,2,241,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(242,2,242,0,0,"IF", 1, 2, 0);
	trans[0][235]	= settr(234,2,246,78,78,"T3_X0 = S6", 1, 2, 0);
	trans[0][244]	= settr(243,2,246,1,0,".(goto)", 1, 2, 0);
	trans[0][236]	= settr(235,2,246,79,79,"T3_X0 = S5", 1, 2, 0);
	trans[0][237]	= settr(236,2,246,80,80,"T3_X0 = S4", 1, 2, 0);
	trans[0][238]	= settr(237,2,246,81,81,"T3_X0 = S1", 1, 2, 0);
	trans[0][239]	= settr(238,2,246,82,82,"T3_X0 = S0", 1, 2, 0);
	trans[0][240]	= settr(239,2,246,83,83,"T3_X0 = S2", 1, 2, 0);
	trans[0][241]	= settr(240,2,246,84,84,"T3_X0 = S7", 1, 2, 0);
	trans[0][242]	= settr(241,2,246,85,85,"T3_X0 = S3", 1, 2, 0);
	T = trans[0][246] = settr(245,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(245,2,245,0,0,"IF", 1, 2, 0);
	trans[0][245]	= settr(244,2,250,86,86,"T3_X1 = S7", 1, 2, 0);
	trans[0][247]	= settr(246,2,250,1,0,".(goto)", 1, 2, 0);
	T = trans[0][250] = settr(249,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(249,2,248,0,0,"IF", 1, 2, 0);
	trans[0][248]	= settr(247,2,249,87,0,"((T3_X0==S6))", 1, 2, 0);
	trans[0][249]	= settr(248,0,262,88,88,"(1)", 1, 2, 0); /* m: 252 -> 262,0 */
	reached0[252] = 1;
	trans[0][251]	= settr(250,2,252,1,0,".(goto)", 1, 2, 0); /* m: 252 -> 0,262 */
	reached0[252] = 1;
	trans[0][252]	= settr(251,0,262,89,89,"current = 3", 1, 2, 0); /* m: 254 -> 0,262 */
	reached0[254] = 1;
	trans[0][254]	= settr(253,0,262,1,0,".(goto)", 1, 2, 0);
	trans[0][261]	= settr(260,0,262,1,0,".(goto)", 0, 2, 0);
	T = trans[ 0][259] = settr(258,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(258,2,256,0,0,"ATOMIC", 1, 2, 0);
	trans[0][256]	= settr(255,0,262,90,90,"((T3_X0==S6))", 1, 2, 0); /* m: 257 -> 262,0 */
	reached0[257] = 1;
	trans[0][257]	= settr(0,0,0,0,0,"current = 0",0,0,0);
	trans[0][258]	= settr(0,0,0,0,0,"ready[3] = 1",0,0,0);
	trans[0][264]	= settr(263,0,265,1,0,"break", 0, 2, 0);
	trans[0][265]	= settr(264,0,266,1,0,"(1)", 0, 2, 0);
	trans[0][266]	= settr(265,0,0,91,91,"-end-", 0, 3500, 0);
	/* np_ demon: */
	trans[_NP_] = (Trans **) emalloc(2*sizeof(Trans *));
	T = trans[_NP_][0] = settr(9997,0,1,_T5,0,"(np_)", 1,2,0);
	    T->nxt	  = settr(9998,0,0,_T2,0,"(1)",   0,2,0);
	T = trans[_NP_][1] = settr(9999,0,1,_T5,0,"(np_)", 1,2,0);
}

Trans *
settr(	int t_id, int a, int b, int c, int d,
	char *t, int g, int tpe0, int tpe1)
{	Trans *tmp = (Trans *) emalloc(sizeof(Trans));

	tmp->atom  = a&(6|32);	/* only (2|8|32) have meaning */
	if (!g) tmp->atom |= 8;	/* no global references */
	tmp->st    = b;
	tmp->tpe[0] = tpe0;
	tmp->tpe[1] = tpe1;
	tmp->tp    = t;
	tmp->t_id  = t_id;
	tmp->forw  = c;
	tmp->back  = d;
	return tmp;
}

Trans *
cpytr(Trans *a)
{	Trans *tmp = (Trans *) emalloc(sizeof(Trans));

	int i;
	tmp->atom  = a->atom;
	tmp->st    = a->st;
#ifdef HAS_UNLESS
	tmp->e_trans = a->e_trans;
	for (i = 0; i < HAS_UNLESS; i++)
		tmp->escp[i] = a->escp[i];
#endif
	tmp->tpe[0] = a->tpe[0];
	tmp->tpe[1] = a->tpe[1];
	for (i = 0; i < 6; i++)
	{	tmp->qu[i] = a->qu[i];
		tmp->ty[i] = a->ty[i];
	}
	tmp->tp    = (char *) emalloc(strlen(a->tp)+1);
	strcpy(tmp->tp, a->tp);
	tmp->t_id  = a->t_id;
	tmp->forw  = a->forw;
	tmp->back  = a->back;
	return tmp;
}

#ifndef NOREDUCE
int
srinc_set(int n)
{	if (n <= 2) return LOCAL;
	if (n <= 2+  DELTA) return Q_FULL_F; /* 's' or nfull  */
	if (n <= 2+2*DELTA) return Q_EMPT_F; /* 'r' or nempty */
	if (n <= 2+3*DELTA) return Q_EMPT_T; /* empty */
	if (n <= 2+4*DELTA) return Q_FULL_T; /* full  */
	if (n ==   5*DELTA) return GLOBAL;
	if (n ==   6*DELTA) return TIMEOUT_F;
	if (n ==   7*DELTA) return ALPHA_F;
	Uerror("cannot happen srinc_class");
	return BAD;
}
int
srunc(int n, int m)
{	switch(m) {
	case Q_FULL_F: return n-2;
	case Q_EMPT_F: return n-2-DELTA;
	case Q_EMPT_T: return n-2-2*DELTA;
	case Q_FULL_T: return n-2-3*DELTA;
	case ALPHA_F:
	case TIMEOUT_F: return 257; /* non-zero, and > MAXQ */
	}
	Uerror("cannot happen srunc");
	return 0;
}
#endif
int cnt;
#ifdef HAS_UNLESS
int
isthere(Trans *a, int b)
{	Trans *t;
	for (t = a; t; t = t->nxt)
		if (t->t_id == b)
			return 1;
	return 0;
}
#endif
#ifndef NOREDUCE
int
mark_safety(Trans *t) /* for conditional safety */
{	int g = 0, i, j, k;

	if (!t) return 0;
	if (t->qu[0])
		return (t->qu[1])?2:1;	/* marked */

	for (i = 0; i < 2; i++)
	{	j = srinc_set(t->tpe[i]);
		if (j >= GLOBAL && j != ALPHA_F)
			return -1;
		if (j != LOCAL)
		{	k = srunc(t->tpe[i], j);
			if (g == 0
			||  t->qu[0] != k
			||  t->ty[0] != j)
			{	t->qu[g] = k;
				t->ty[g] = j;
				g++;
	}	}	}
	return g;
}
#endif
void
retrans(int n, int m, int is, short srcln[], uchar reach[], uchar lpstate[])
	/* process n, with m states, is=initial state */
{	Trans *T0, *T1, *T2, *T3;
	Trans *T4, *T5; /* t_reverse or has_unless */
	int i;
#if defined(HAS_UNLESS) || !defined(NOREDUCE)
	int k;
#endif
#ifndef NOREDUCE
	int g, h, j, aa;
#endif
#ifdef HAS_UNLESS
	int p;
#endif
	if (state_tables >= 4)
	{	printf("STEP 1 %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			crack(n, i, T0, srcln);
		return;
	}
	do {
		for (i = 1, cnt = 0; i < m; i++)
		{	T2 = trans[n][i];
			T1 = T2?T2->nxt:(Trans *)0;
/* prescan: */		for (T0 = T1; T0; T0 = T0->nxt)
/* choice in choice */	{	if (T0->st && trans[n][T0->st]
				&&  trans[n][T0->st]->nxt)
					break;
			}
#if 0
		if (T0)
		printf("\tstate %d / %d: choice in choice\n",
		i, T0->st);
#endif
			if (T0)
			for (T0 = T1; T0; T0 = T0->nxt)
			{	T3 = trans[n][T0->st];
				if (!T3->nxt)
				{	T2->nxt = cpytr(T0);
					T2 = T2->nxt;
					imed(T2, T0->st, n, i);
					continue;
				}
				do {	T3 = T3->nxt;
					T2->nxt = cpytr(T3);
					T2 = T2->nxt;
					imed(T2, T0->st, n, i);
				} while (T3->nxt);
				cnt++;
			}
		}
	} while (cnt);
	if (state_tables >= 3)
	{	printf("STEP 2 %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			crack(n, i, T0, srcln);
		return;
	}
	for (i = 1; i < m; i++)
	{	if (trans[n][i] && trans[n][i]->nxt) /* optimize */
		{	T1 = trans[n][i]->nxt;
#if 0
			printf("\t\tpull %d (%d) to %d\n",
			T1->st, T1->forw, i);
#endif
			srcln[i] = srcln[T1->st];	/* Oyvind Teig, 5.2.0 */

			if (!trans[n][T1->st]) continue;
			T0 = cpytr(trans[n][T1->st]);
			trans[n][i] = T0;
			reach[T1->st] = 1;
			imed(T0, T1->st, n, i);
			for (T1 = T1->nxt; T1; T1 = T1->nxt)
			{
#if 0
			printf("\t\tpull %d (%d) to %d\n",
				T1->st, T1->forw, i);
#endif
		/*		srcln[i] = srcln[T1->st];  gh: not useful */
				if (!trans[n][T1->st]) continue;
				T0->nxt = cpytr(trans[n][T1->st]);
				T0 = T0->nxt;
				reach[T1->st] = 1;
				imed(T0, T1->st, n, i);
	}	}	}
	if (state_tables >= 2)
	{	printf("STEP 3 %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			crack(n, i, T0, srcln);
		return;
	}
#ifdef HAS_UNLESS
	for (i = 1; i < m; i++)
	{	if (!trans[n][i]) continue;
		/* check for each state i if an
		 * escape to some state p is defined
		 * if so, copy and mark p's transitions
		 * and prepend them to the transition-
		 * list of state i
		 */
	 if (!like_java) /* the default */
	 {	for (T0 = trans[n][i]; T0; T0 = T0->nxt)
		for (k = HAS_UNLESS-1; k >= 0; k--)
		{	if (p = T0->escp[k])
			for (T1 = trans[n][p]; T1; T1 = T1->nxt)
			{	if (isthere(trans[n][i], T1->t_id))
					continue;
				T2 = cpytr(T1);
				T2->e_trans = p;
				T2->nxt = trans[n][i];
				trans[n][i] = T2;
		}	}
	 } else /* outermost unless checked first */
	 {	T4 = T3 = (Trans *) 0;
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
		for (k = HAS_UNLESS-1; k >= 0; k--)
		{	if (p = T0->escp[k])
			for (T1 = trans[n][p]; T1; T1 = T1->nxt)
			{	if (isthere(trans[n][i], T1->t_id))
					continue;
				T2 = cpytr(T1);
				T2->nxt = (Trans *) 0;
				T2->e_trans = p;
				if (T3)	T3->nxt = T2;
				else	T4 = T2;
				T3 = T2;
		}	}
		if (T4)
		{	T3->nxt = trans[n][i];
			trans[n][i] = T4;
		}
	 }
	}
#endif
#ifndef NOREDUCE
	for (i = 1; i < m; i++)
	{	if (a_cycles)
		{ /* moves through these states are visible */
	#if PROG_LAB>0 && defined(HAS_NP)
			if (progstate[n][i])
				goto degrade;
			for (T1 = trans[n][i]; T1; T1 = T1->nxt)
				if (progstate[n][T1->st])
					goto degrade;
	#endif
			if (accpstate[n][i] || visstate[n][i])
				goto degrade;
			for (T1 = trans[n][i]; T1; T1 = T1->nxt)
				if (accpstate[n][T1->st])
					goto degrade;
		}
		T1 = trans[n][i];
		if (!T1) continue;
		g = mark_safety(T1);	/* V3.3.1 */
		if (g < 0) goto degrade; /* global */
		/* check if mixing of guards preserves reduction */
		if (T1->nxt)
		{	k = 0;
			for (T0 = T1; T0; T0 = T0->nxt)
			{	if (!(T0->atom&8))
					goto degrade;
				for (aa = 0; aa < 2; aa++)
				{	j = srinc_set(T0->tpe[aa]);
					if (j >= GLOBAL && j != ALPHA_F)
						goto degrade;
					if (T0->tpe[aa]
					&&  T0->tpe[aa]
					!=  T1->tpe[0])
						k = 1;
			}	}
			/* g = 0;	V3.3.1 */
			if (k)	/* non-uniform selection */
			for (T0 = T1; T0; T0 = T0->nxt)
			for (aa = 0; aa < 2; aa++)
			{	j = srinc_set(T0->tpe[aa]);
				if (j != LOCAL)
				{	k = srunc(T0->tpe[aa], j);
					for (h = 0; h < 6; h++)
						if (T1->qu[h] == k
						&&  T1->ty[h] == j)
							break;
					if (h >= 6)
					{	T1->qu[g%6] = k;
						T1->ty[g%6] = j;
						g++;
			}	}	}
			if (g > 6)
			{	T1->qu[0] = 0;	/* turn it off */
				printf("pan: warning, line %d, ",
					srcln[i]);
			 	printf("too many stmnt types (%d)",
					g);
			  	printf(" in selection\n");
			  goto degrade;
			}
		}
		/* mark all options global if >=1 is global */
		for (T1 = trans[n][i]; T1; T1 = T1->nxt)
			if (!(T1->atom&8)) break;
		if (T1)
degrade:	for (T1 = trans[n][i]; T1; T1 = T1->nxt)
			T1->atom &= ~8;	/* mark as unsafe */
		/* can only mix 'r's or 's's if on same chan */
		/* and not mixed with other local operations */
		T1 = trans[n][i];
		if (!T1 || T1->qu[0]) continue;
		j = T1->tpe[0];
		if (T1->nxt && T1->atom&8)
		{ if (j == 5*DELTA)
		  {	printf("warning: line %d ", srcln[i]);
			printf("mixed condition ");
			printf("(defeats reduction)\n");
			goto degrade;
		  }
		  for (T0 = T1; T0; T0 = T0->nxt)
		  for (aa = 0; aa < 2; aa++)
		  if  (T0->tpe[aa] && T0->tpe[aa] != j)
		  {	printf("warning: line %d ", srcln[i]);
			printf("[%d-%d] mixed %stion ",
				T0->tpe[aa], j, 
				(j==5*DELTA)?"condi":"selec");
			printf("(defeats reduction)\n");
			printf("	'%s' <-> '%s'\n",
				T1->tp, T0->tp);
			goto degrade;
		} }
	}
#endif
	for (i = 1; i < m; i++)
	{	T2 = trans[n][i];
		if (!T2
		||  T2->nxt
		||  strncmp(T2->tp, ".(goto)", 7)
		||  !stopstate[n][i])
			continue;
		stopstate[n][T2->st] = 1;
	}
	if (state_tables && !verbose)
	{	if (dodot)
		{	char buf[256], *q = buf, *p = procname[n];
			while (*p != '\0')
			{	if (*p != ':')
				{	*q++ = *p;
				}
				p++;
			}
			*q = '\0';
			printf("digraph ");
			switch (Btypes[n]) {
			case I_PROC:  printf("init {\n"); break;
			case N_CLAIM: printf("claim_%s {\n", buf); break;
			case E_TRACE: printf("notrace {\n"); break;
			case N_TRACE: printf("trace {\n"); break;
			default:      printf("p_%s {\n", buf); break;
			}
			printf("size=\"8,10\";\n");
			printf("  GT [shape=box,style=dotted,label=\"%s\"];\n", buf);
			printf("  GT -> S%d;\n", is);
		} else
		{	switch (Btypes[n]) {
			case I_PROC:  printf("init\n"); break;
			case N_CLAIM: printf("claim %s\n", procname[n]); break;
			case E_TRACE: printf("notrace assertion\n"); break;
			case N_TRACE: printf("trace assertion\n"); break;
			default:      printf("proctype %s\n", procname[n]); break;
		}	}
		for (i = 1; i < m; i++)
		{	reach[i] = 1;
		}
		tagtable(n, m, is, srcln, reach);
		if (dodot) printf("}\n");
	} else
	for (i = 1; i < m; i++)
	{	int nrelse;
		if (Btypes[n] != N_CLAIM)
		{	for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			{	if (T0->st == i
				&& strcmp(T0->tp, "(1)") == 0)
				{	printf("error: proctype '%s' ",
						procname[n]);
		  			printf("line %d, state %d: has un",
						srcln[i], i);
					printf("conditional self-loop\n");
					pan_exit(1);
		}	}	}
		nrelse = 0;
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
		{	if (strcmp(T0->tp, "else") == 0)
				nrelse++;
		}
		if (nrelse > 1)
		{	printf("error: proctype '%s' state",
				procname[n]);
		  	printf(" %d, inherits %d", i, nrelse);
		  	printf(" 'else' stmnts\n");
			pan_exit(1);
	}	}
#if !defined(LOOPSTATE) && !defined(BFS_PAR)
	if (state_tables)
#endif
	do_dfs(n, m, is, srcln, reach, lpstate);

	if (!t_reverse)
	{	return;
	}
	/* process n, with m states, is=initial state -- reverse list */
	if (!state_tables && Btypes[n] != N_CLAIM)
	{	for (i = 1; i < m; i++)
		{	Trans *Tx = (Trans *) 0; /* list of escapes */
			Trans *Ty = (Trans *) 0; /* its tail element */
			T1 = (Trans *) 0; /* reversed list */
			T2 = (Trans *) 0; /* its tail */
			T3 = (Trans *) 0; /* remembers possible 'else' */

			/* find unless-escapes, they should go first */
			T4 = T5 = T0 = trans[n][i];
	#ifdef HAS_UNLESS
			while (T4 && T4->e_trans) /* escapes are first in orig list */
			{	T5 = T4;	  /* remember predecessor */
				T4 = T4->nxt;
			}
	#endif
			/* T4 points to first non-escape, T5 to its parent, T0 to original list */
			if (T4 != T0)		 /* there was at least one escape */
			{	T3 = T5->nxt;		 /* start of non-escapes */
				T5->nxt = (Trans *) 0;	 /* separate */
				Tx = T0;		 /* start of the escapes */
				Ty = T5;		 /* its tail */
				T0 = T3;		 /* the rest, to be reversed */
			}
			/* T0 points to first non-escape, Tx to the list of escapes, Ty to its tail */

			/* first tail-add non-escape transitions, reversed */
			T3 = (Trans *) 0;
			for (T5 = T0; T5; T5 = T4)
			{	T4 = T5->nxt;
	#ifdef HAS_UNLESS
				if (T5->e_trans)
				{	printf("error: cannot happen!\n");
					continue;
				}
	#endif
				if (strcmp(T5->tp, "else") == 0)
				{	T3 = T5;
					T5->nxt = (Trans *) 0;
				} else
				{	T5->nxt = T1;
					if (!T1) { T2 = T5; }
					T1 = T5;
			}	}
			/* T3 points to a possible else, which is removed from the list */
			/* T1 points to the reversed list so far (without escapes) */
			/* T2 points to the tail element -- where the else should go */
			if (T2 && T3)
			{	T2->nxt = T3;	/* add else */
			} else
			{	if (T3) /* there was an else, but there's no tail */
				{	if (!T1)	/* and no reversed list */
					{	T1 = T3; /* odd, but possible */
					} else		/* even stranger */
					{	T1->nxt = T3;
			}	}	}

			/* add in the escapes, to that they appear at the front */
			if (Tx && Ty) { Ty->nxt = T1; T1 = Tx; }

			trans[n][i] = T1;
			/* reversed, with escapes first and else last */
	}	}
	if (state_tables && verbose)
	{	printf("FINAL proctype %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			crack(n, i, T0, srcln);
	}
}
void
imed(Trans *T, int v, int n, int j)	/* set intermediate state */
{	progstate[n][T->st] |= progstate[n][v];
	accpstate[n][T->st] |= accpstate[n][v];
	stopstate[n][T->st] |= stopstate[n][v];
	mapstate[n][j] = T->st;
}
void
tagtable(int n, int m, int is, short srcln[], uchar reach[])
{	Trans *z;

	if (is >= m || !trans[n][is]
	||  is <= 0 || reach[is] == 0)
		return;
	reach[is] = 0;
	if (state_tables)
	for (z = trans[n][is]; z; z = z->nxt)
	{	if (dodot)
			dot_crack(n, is, z);
		else
			crack(n, is, z, srcln);
	}

	for (z = trans[n][is]; z; z = z->nxt)
	{
#ifdef HAS_UNLESS
		int i, j;
#endif
		tagtable(n, m, z->st, srcln, reach);
#ifdef HAS_UNLESS
		for (i = 0; i < HAS_UNLESS; i++)
		{	j = trans[n][is]->escp[i];
			if (!j) break;
			tagtable(n, m, j, srcln, reach);
		}
#endif
	}
}

extern Trans *t_id_lkup[];

void
dfs_table(int n, int m, int is, short srcln[], uchar reach[], uchar lpstate[])
{	Trans *z;

	if (is >= m || is <= 0 || !trans[n][is])
		return;
	if ((reach[is] & (4|8|16)) != 0)
	{	if ((reach[is] & (8|16)) == 16)	/* on stack, not yet recorded */
		{	lpstate[is] = 1;
			reach[is] |= 8; /* recorded */
			if (state_tables && verbose)
			{	printf("state %d line %d is a loopstate\n", is, srcln[is]);
		}	}
		return;
	}
	reach[is] |= (4|16);	/* visited | onstack */
	for (z = trans[n][is]; z; z = z->nxt)
	{	t_id_lkup[z->t_id] = z;
#ifdef HAS_UNLESS
		int i, j;
#endif
		dfs_table(n, m, z->st, srcln, reach, lpstate);
#ifdef HAS_UNLESS
		for (i = 0; i < HAS_UNLESS; i++)
		{	j = trans[n][is]->escp[i];
			if (!j) break;
			dfs_table(n, m, j, srcln, reach, lpstate);
		}
#endif
	}
	reach[is] &= ~16; /* no longer on stack */
}
void
do_dfs(int n, int m, int is, short srcln[], uchar reach[], uchar lpstate[])
{	int i;
	dfs_table(n, m, is, srcln, reach, lpstate);
	for (i = 0; i < m; i++)
		reach[i] &= ~(4|8|16);
}
void
crack(int n, int j, Trans *z, short srcln[])
{	int i;

	if (!z) return;
	printf("	state %3d -(tr %3d)-> state %3d  ",
		j, z->forw, z->st);
	printf("[id %3d tp %3d", z->t_id, z->tpe[0]);
	if (z->tpe[1]) printf(",%d", z->tpe[1]);
#ifdef HAS_UNLESS
	if (z->e_trans)
		printf(" org %3d", z->e_trans);
	else if (state_tables >= 2)
	for (i = 0; i < HAS_UNLESS; i++)
	{	if (!z->escp[i]) break;
		printf(" esc %d", z->escp[i]);
	}
#endif
	printf("]");
	printf(" [%s%s%s%s%s] %s:%d => ",
		z->atom&6?"A":z->atom&32?"D":"-",
		accpstate[n][j]?"a" :"-",
		stopstate[n][j]?"e" : "-",
		progstate[n][j]?"p" : "-",
		z->atom & 8 ?"L":"G",
		PanSource, srcln[j]);
	for (i = 0; z->tp[i]; i++)
		if (z->tp[i] == '\n')
			printf("\\n");
		else
			putchar(z->tp[i]);
	if (verbose && z->qu[0])
	{	printf("\t[");
		for (i = 0; i < 6; i++)
			if (z->qu[i])
				printf("(%d,%d)",
				z->qu[i], z->ty[i]);
		printf("]");
	}
	printf("\n");
	fflush(stdout);
}
/* spin -a m.pml; cc -o pan pan.c; ./pan -D | dot -Tps > foo.ps; ps2pdf foo.ps */
void
dot_crack(int n, int j, Trans *z)
{	int i;

	if (!z) return;
	printf("	S%d -> S%d  [color=black", j, z->st);

	if (z->atom&6) printf(",style=dashed");
	else if (z->atom&32) printf(",style=dotted");
	else if (z->atom&8) printf(",style=solid");
	else printf(",style=bold");

	printf(",label=\"");
	for (i = 0; z->tp[i]; i++)
	{	if (z->tp[i] == '\\'
		&&  z->tp[i+1] == 'n')
		{	i++; printf(" ");
		} else
		{	putchar(z->tp[i]);
	}	}
	printf("\"];\n");
	if (accpstate[n][j]) printf("  S%d [color=red,style=bold];\n", j);
	else if (progstate[n][j]) printf("  S%d [color=green,style=bold];\n", j);
	if (stopstate[n][j]) printf("  S%d [color=blue,style=bold,shape=box];\n", j);
}

#ifdef VAR_RANGES
#define BYTESIZE	32	/* 2^8 : 2^3 = 256:8 = 32 */

typedef struct Vr_Ptr {
	char	*nm;
	uchar	vals[BYTESIZE];
	struct Vr_Ptr *nxt;
} Vr_Ptr;
Vr_Ptr *ranges = (Vr_Ptr *) 0;

void
logval(char *s, int v)
{	Vr_Ptr *tmp;

	if (v<0 || v > 255) return;
	for (tmp = ranges; tmp; tmp = tmp->nxt)
		if (!strcmp(tmp->nm, s))
			goto found;
	tmp = (Vr_Ptr *) emalloc(sizeof(Vr_Ptr));
	tmp->nxt = ranges;
	ranges = tmp;
	tmp->nm = s;
found:
	tmp->vals[(v)/8] |= 1<<((v)%8);
}

void
dumpval(uchar X[], int range)
{	int w, x, i, j = -1;

	for (w = i = 0; w < range; w++)
	for (x = 0; x < 8; x++, i++)
	{
from:		if ((X[w] & (1<<x)))
		{	printf("%d", i);
			j = i;
			goto upto;
	}	}
	return;
	for (w = 0; w < range; w++)
	for (x = 0; x < 8; x++, i++)
	{
upto:		if (!(X[w] & (1<<x)))
		{	if (i-1 == j)
				printf(", ");
			else
				printf("-%d, ", i-1);
			goto from;
	}	}
	if (j >= 0 && j != 255)
		printf("-255");
}

void
dumpranges(void)
{	Vr_Ptr *tmp;
	printf("\nValues assigned within ");
	printf("interval [0..255]:\n");
	for (tmp = ranges; tmp; tmp = tmp->nxt)
	{	printf("\t%s\t: ", tmp->nm);
		dumpval(tmp->vals, BYTESIZE);
		printf("\n");
	}
}
#endif
