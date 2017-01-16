/*
 * Automaton.h
 *
 *  Created on: Oct 26, 2016
 *      Author: lyl
 */

#ifndef AUTOMATON_H_
#define AUTOMATON_H_

#include <string>
#include <vector>
#include "Formula.h"
#include "Verifier.h"

namespace std {

class AState;
class AProp;

class Automaton {
public:
	int taskid;
	int num_states;
	vector<AState> states;
	vector<vector<pair<vector<AProp*>, int> > > transition;

	Automaton(int _taskid) : taskid(_taskid), num_states(0) { }
	virtual ~Automaton();

    void dump();
};

void generate_atms(Artifact& art, int num_atms, int num_states, int num_trans, vector<Automaton>& results);

class AProp {
public:
	bool negated;
	virtual bool is_fo() const;
	virtual bool is_service() const;
	virtual bool is_child() const;
	virtual bool is_subform() const;
    virtual string to_string() const;

	AProp() : negated(false) { }
	virtual ~AProp() { }

};

class APropFO : public AProp {
public:
	Formula* fo;

	bool is_fo() const;
    string to_string() const;

	APropFO(Formula* _fo) : fo(_fo) { }

	virtual ~APropFO() { }
};

class APropService : public AProp {
public:
	int ser_id;

	bool is_service() const;
    string to_string() const;

	APropService(int _ser_id) : ser_id(_ser_id) { }

	virtual ~APropService() { }
};

class APropChild : public AProp {
public:
	int child_id;
	bool is_open;

	bool is_child() const;
    string to_string() const;

	APropChild(int _child_id, bool _is_open) : child_id(_child_id), is_open(_is_open) { }
	virtual ~APropChild() { }
};

class APropSubForm : public AProp {
public:
	int child_id;
	int auto_id;

	bool is_subform() const;
    string to_string() const;

	APropSubForm(int _child_id, int _auto_id) : child_id(_child_id), auto_id(_auto_id) { }
	virtual ~APropSubForm() { }
};

struct AState {
	int id;
	string name;
	bool accept;

    string to_string() const;
};



}

#endif /* AUTOMATON_H_ */
