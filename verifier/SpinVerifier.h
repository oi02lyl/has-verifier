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
	SpinVerifier(Artifact& a, vector<Automaton>& _atms, LivenessProperty& p, int _naive = 0) : Verifier(a, _atms, false, false), property(p), naive(_naive) { }

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

    void get_minimal_assignment_sets(vector<tuple<int, int, bool> >& edges);

    void compute_expr_domains();

    void get_expr_domain(int expr, vector<string>& result);

	virtual ~SpinVerifier();

    vector<vector<string> > assignment_sets;

	LivenessProperty& property;
    
    int naive;
};

} /* namespace std */

#endif /* SPINVERIFIER_H_ */
