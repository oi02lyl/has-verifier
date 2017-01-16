/*
 * LivenessProperty.cpp
 *
 *  Created on: Jan 12, 2017
 *      Author: oi02l
 */

#include "LivenessProperty.h"

namespace std {

string LivenessProperty::generate_hltlfo() {
	// root task only
	return "G (p -> F q)\n\nPropositions:\np: " + form1->to_string() + "\nq: " + form2->to_string();
}

void LivenessProperty::generate_atms(vector<Automaton>& atms) {
    // root task only
    Automaton atm(0);
    vector<AState> states(2, AState());

    states[0].id = 0;
    states[0].name = "S0";
    states[0].accept = false;

    states[1].id = 1;
    states[1].name = "S1";
    states[1].accept = true;

    // vector<vector<pair<vector<AProp*>, int> > > transition(2, vector<pair<vector<AProp*>, int> > >());
    // TODO
    // transition 1
    
    // transition 2
    

    // transition 3



    atm.num_states = 2;
    atm.states = states;
    // atm.transition = transition;
}

}
