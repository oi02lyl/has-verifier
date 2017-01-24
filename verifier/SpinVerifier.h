/*
 * SpinVerifier.h
 *
 *  Created on: Dec 8, 2016
 *      Author: lyl
 */

#ifndef SPINVERIFIER_H_
#define SPINVERIFIER_H_

#include "Verifier.h"
#include "LivenessProperty.h"

namespace std {

class SpinVerifier : public Verifier {
public:
	SpinVerifier(Artifact& a, vector<Automaton>& _atms, LivenessProperty& p, int _naive = 0) : Verifier(a, _atms, false, false), property(p), max_type_count(0), naive(_naive) { }

	// generate the promela code to verifier the target safety property
	string generate_promela();

	string promela_translate_eq(int expr1, int expr2);

	string promela_translate_condition(int task_id, Formula* form);

	string promela_get_assignment(int task_id, vector<int>& prop_vars);

    void get_prop_exprs(int task_id, vector<int>& prop_vars, vector<int>& res);

	string promela_is_current(int task_id);

	string promela_is_active(int task_id);

	string promela_is_ready(int task_id);
	
    string promela_resolve();

	string promela_all_child_inactive(int task_id);

	int chromatic_number(vector<vector<int> >& graph);
	
    void get_minimal_assignment_sets(vector<tuple<int, int, bool> >& edges);

    void get_type_groups();

    void get_type_group(int expr, vector<string>& result);

	virtual ~SpinVerifier();

	LivenessProperty& property;

    map<int, int> type_count;
    int max_type_count;

    map<int, vector<int> > type_groups;

    vector<int> expr_types;

    // new
    /*
    vector<int> expr_group_id;
    vector<vector<int> > group_const_exprs;
    vector<int> group_uneql_cnt;
    */
    vector<vector<string> > assignment_sets;



    int naive;
};

} /* namespace std */

#endif /* SPINVERIFIER_H_ */
