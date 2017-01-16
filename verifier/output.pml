byte current;
bool running[4];
bool ready[4];

byte N0;
byte S0;
byte S1;
byte S2;
byte S3;
byte S4;
byte S5;
byte S6;
byte S7;
byte CONST_NULL;
byte T0_X0;
byte T0_X0_1;
byte T0_X0_2;
byte T0_X0_2_1;
byte T0_X0_2_2;
byte T0_X0_2_3;
byte T0_X1;
byte T0_X2;
byte T0_X3;
byte T0_X3_1;
byte T0_X3_2;
byte T0_X3_3;
byte T0_X4;
byte T1_X0;
byte T1_X0_1;
byte T1_X0_2;
byte T1_X0_2_1;
byte T1_X0_2_2;
byte T1_X0_2_3;
byte T1_X1;
byte T1_X2;
byte T1_X3;
byte T2_X0;
byte T2_X1;
byte T2_X2;
byte T2_X3;
byte T2_X3_1;
byte T2_X3_2;
byte T2_X3_3;
byte T2_X4;
byte T2_X5;
byte T2_X6;
byte T3_X0;
byte T3_X1;
init {
    atomic {     current = 255;
    ready[0] = false;
    running[0] = true;
    ready[1] = false;
    running[1] = false;
    ready[2] = false;
    running[2] = false;
    ready[3] = false;
    running[3] = false;

    N0 = 0;
    S0 = 1;
    S1 = 2;
    S2 = 3;
    S3 = 4;
    S4 = 5;
    S5 = 6;
    S6 = 7;
    S7 = 0;
    CONST_NULL = 0;
    T0_X0 = 0;
    T0_X0_1 = 0;
    T0_X0_2 = 0;
    T0_X0_2_1 = 0;
    T0_X0_2_2 = 0;
    T0_X0_2_3 = 0;
    T0_X1 = 0;
    T0_X2 = 0;
    T0_X3 = 0;
    T0_X3_1 = 0;
    T0_X3_2 = 0;
    T0_X3_3 = 0;
    T0_X4 = 0;
    T1_X0 = 0;
    T1_X0_1 = 0;
    T1_X0_2 = 0;
    T1_X0_2_1 = 0;
    T1_X0_2_2 = 0;
    T1_X0_2_3 = 0;
    T1_X1 = 0;
    T1_X2 = 0;
    T1_X3 = 0;
    T2_X0 = 0;
    T2_X1 = 0;
    T2_X2 = 0;
    T2_X3 = 0;
    T2_X3_1 = 0;
    T2_X3_2 = 0;
    T2_X3_3 = 0;
    T2_X4 = 0;
    T2_X5 = 0;
    T2_X6 = 0;
    T3_X0 = 0;
    T3_X1 = 0;
    } 
  current = 0; 


  do
  :: running[0] ->
  if
  :: atomic { (!(running[1]) && !(running[2]) && !(running[3])) -> 
    if
    :: (true) -> 
      current = 255; 
       if :: T0_X0 = CONST_NULL;  fi;
 if :: T0_X0_1 = 11;  fi;
 if :: T0_X0_2 = 11;  fi;
 if :: T0_X0_2_1 = 11;  fi;
 if :: T0_X0_2_2 = 11;  fi;
 if :: T0_X0_2_3 = 11;  fi;
 if :: T0_X1 = S6; :: T0_X1 = S5; :: T0_X1 = S4; :: T0_X1 = S1; :: T0_X1 = S0; :: T0_X1 = S2; :: T0_X1 = S7; :: T0_X1 = S3;  fi;
 if :: T0_X2 = CONST_NULL; :: T0_X2 = N0;  fi;
 if :: T0_X3 = CONST_NULL;  fi;
 if :: T0_X3_1 = 11;  fi;
 if :: T0_X3_2 = 11;  fi;
 if :: T0_X3_3 = 11;  fi;
 if :: T0_X4 = S6; :: T0_X4 = S7; :: T0_X4 = S3; :: T0_X4 = CONST_NULL; :: T0_X4 = 11; :: T0_X4 = 12;  fi;
 
      if 
      :: (((((T0_X0 != CONST_NULL) && (T0_X2 != N0)) && (T0_X1 == S0)) && (T0_X3 != CONST_NULL)) && (T0_X4 != CONST_NULL)) -> skip; 
      fi;   
       current = 0; 
  fi } 
  :: atomic { !running[1] && (true) ->
    current = 1;
    running[1] = true;
    T1_X0 = T0_X0;
    T1_X0_1 = T0_X0_1;
    T1_X0_2 = T0_X0_2;
    T1_X0_2_1 = T0_X0_2_1;
    T1_X0_2_2 = T0_X0_2_2;
    T1_X0_2_3 = T0_X0_2_3;
    T1_X1 = 0;
    T1_X2 = T0_X2;
    T1_X3 = T0_X4;
} 
  :: atomic { !running[2] && (((T0_X1 == S0) || (T0_X1 == S2))) ->
    current = 2;
    running[2] = true;
    T2_X0 = 0;
    T2_X1 = T0_X2;
    T2_X2 = 0;
    T2_X3 = T0_X3;
    T2_X3_1 = T0_X3_1;
    T2_X3_2 = T0_X3_2;
    T2_X3_3 = T0_X3_3;
    T2_X4 = 0;
    T2_X5 = T0_X4;
    T2_X6 = 0;
} 
  :: atomic { !running[3] && ((T0_X1 == S3)) ->
    current = 3;
    running[3] = true;
    T3_X0 = 0;
    T3_X1 = 0;
} 
  :: atomic { ready[1] ->
    ready[1] = false;
    running[1] = false;
    T0_X1 = T1_X1;
 } 
  :: atomic { ready[2] ->
    ready[2] = false;
    running[2] = false;
    T0_X1 = T2_X0;
 } 
  :: atomic { ready[3] ->
    ready[3] = false;
    running[3] = false;
    T0_X1 = T3_X0;
 } 
  fi
  :: running[1] ->
  if
  :: atomic { true -> 
    if
    :: (((T1_X0 != CONST_NULL) && (T1_X1 == S0))) -> 
      current = 255; 
       if :: T1_X1 = S6; :: T1_X1 = S5; :: T1_X1 = S4; :: T1_X1 = S1; :: T1_X1 = S0; :: T1_X1 = S2; :: T1_X1 = S7; :: T1_X1 = S3;  fi;
 
      if 
      :: (((T1_X1 == S2) && (T1_X2 != CONST_NULL)) && (T1_X3 != CONST_NULL)) -> skip; 
      fi;   
       current = 1; 
  fi } 
  :: atomic { ((T1_X1 == S1)) ->
    current = 0;
    ready[1] = true;
 } 
  fi
  :: running[2] ->
  if
  :: atomic { true -> 
    if
    :: (((T2_X0 == S0) || (T2_X0 == S2))) -> 
      current = 255; 
       if :: T2_X2 = S6; :: T2_X2 = S7; :: T2_X2 = S3; :: T2_X2 = CONST_NULL; :: T2_X2 = 11; :: T2_X2 = 12;  fi;
 if :: T2_X4 = CONST_NULL; :: T2_X4 = N0;  fi;
 if :: T2_X6 = N0;  fi;
 
      if 
      :: ((((((T2_X2 != T2_X5) || (T2_X3 == CONST_NULL)) || ((T2_X4 == T2_X1) && ( (T2_X4 == CONST_NULL) || ((T2_X4 != CONST_NULL && T2_X1 != CONST_NULL))))) || (T2_X0 == S3)) && ((((T2_X2 != T2_X5) || (T2_X3 == CONST_NULL)) || ((T2_X4 == T2_X1) && ( (T2_X4 == CONST_NULL) || ((T2_X4 != CONST_NULL && T2_X1 != CONST_NULL))))) || (T2_X0 == S4))) && ((T2_X0 != S5) || ((T2_X4 == T2_X1) && ( (T2_X4 == CONST_NULL) || ((T2_X4 != CONST_NULL && T2_X1 != CONST_NULL)))))) -> skip; 
      fi;   
       current = 2; 
  fi } 
  :: atomic { (((T2_X0 == S3) || (T2_X0 == S4))) ->
    current = 0;
    ready[2] = true;
 } 
  fi
  :: running[3] ->
  if
  :: atomic { true -> 
    if
    :: (((T3_X0 == S3) && (T3_X1 != S7))) -> 
      current = 255; 
       if :: T3_X0 = S6; :: T3_X0 = S5; :: T3_X0 = S4; :: T3_X0 = S1; :: T3_X0 = S0; :: T3_X0 = S2; :: T3_X0 = S7; :: T3_X0 = S3;  fi;
 if :: T3_X1 = S7;  fi;
 
      if 
      :: (T3_X0 == S6) -> skip; 
      fi;   
       current = 3; 
  fi } 
  :: atomic { ((T3_X0 == S6)) ->
    current = 0;
    ready[3] = true;
 } 
  fi
    od;
end:
}
never { /* !(G (p -> F q)) */
T0_init :    /* init */
	if
	:: (1) -> goto T0_init
	:: (!((T0_X4 == S6))  && ((T0_X4 == S3)) && (current == 0)) -> goto accept_S2
	fi;
accept_S2 :    /* 1 */
	if
	:: (!((T0_X4 == S6))) -> goto accept_S2
	:: (((T0_X4 == S6)) || !running[0]) -> goto T0_init
	fi;
}

