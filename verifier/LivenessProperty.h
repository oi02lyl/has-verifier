/*
 * LivenessProperty.h
 *
 *  Created on: Jan 12, 2017
 *      Author: oi02l
 */

#ifndef LIVENESSPROPERTY_H_
#define LIVENESSPROPERTY_H_

#include "Artifact.h"
#include "Automaton.h"

namespace std {

class LivenessProperty {
public:
	LivenessProperty(Artifact& _art, int _task_id, Formula* _form1, Formula* _form2):
		art(_art), task_id(_task_id), form1(_form1), form2(_form2) { };

	string generate_hltlfo();

    void generate_atms(vector<Automaton>& atms);

	Artifact& art;
	int task_id;
	Formula* form1;
	Formula* form2;
};

class LtlfoProperty {
public:
    LtlfoProperty(string _ltl, vector<Formula*>& _forms):
        ltl(_ltl), forms(_forms) { };

    string ltl;
    vector<Formula*> forms;
};


void generate_random_atm(string ltl, Artifact& art, Automaton& atm);

}

#endif /* TEMPORALPROPERTY_H_ */
