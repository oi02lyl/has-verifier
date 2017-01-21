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

    vector<vector<pair<vector<AProp*>, int> > > transition;
    vector<pair<vector<AProp*>, int> > tran1;
    vector<pair<vector<AProp*>, int> > tran2;
    
    // transition 1
    pair<vector<AProp*>, int> edge1(vector<AProp*>(), 0);
    tran1.push_back(edge1);
    
    // transition 2
    vector<AProp*> cond1;
    
    cond1.push_back(new APropFO(form1->copy()));
    Internal* negated = new Internal("!");
    negated->paras[0] = form2->copy();
    cond1.push_back(new APropFO(negated));
    pair<vector<AProp*>, int> edge2(cond1, 1);
    tran1.push_back(edge2);

    // transition 3
    vector<AProp*> cond2;
    cond2.push_back(new APropFO(negated->copy()));
    pair<vector<AProp*>, int> edge3(cond2, 1);
    tran2.push_back(edge3);

    transition.push_back(tran1);
    transition.push_back(tran2);

    atm.num_states = 2;
    atm.states = states;
    atm.transition = transition;
    atms.push_back(atm);
}

}
