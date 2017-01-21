/*
 * Verifier.h
 *
 *  Created on: Feb 3, 2016
 *      Author: lyl
 */

#ifndef VERIFIER_H_
#define VERIFIER_H_

#include <vector>
#include <string>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <ctime>
#include "Artifact.h"
#include "VASSState.h"
#include "Automaton.h"

namespace std {

struct Node;
struct State;
struct VASSState;
struct TrieNode;
class Automaton;
class APropFO;

class VASSStateStore;

struct Disjunct;

class Verifier {
public:
	Verifier(Artifact& a, vector<Automaton>& _atms, int _naive = false, bool _debug = false) :
			art(a), atms(_atms), naive(_naive), debug(_debug) {
		time_superstate = 0;
		time_substate = 0;
		time_nextstate = 0;
		time_omega = 0;
		time_scc = 0;
		num_reach_map_tests = 0;
		num_reach_map_hits = 0;
		num_superstate_tests = 0;
		num_superstate_hits = 0;
		num_substate_tests = 0;
		num_substate_hits = 0;
		zero_id = 0;
		empty_id = 0;
		null_id = 0;
		timer = clock();
	}

    bool satisfy();

	void reachable_root(vector<tuple<State, State, vector<int> > >& results);

	// given an input isomorphism type,
	// return a list of pairs of new input type and output type
	vector<tuple<State, State, vector<int> > >* reachable(int taskid, State& input_state);

	// given a KM-tree, mark nodes that are repeatedly reachable
	// by computing the strongly connected components
	void repeated_reachable(int taskid, vector<VASSState*>& que,
			VASSStateStore& visited, vector<vector<int> >& forward_edges,
			vector<bool>& pruned, vector<bool>& result);

	// check whether initial can be reached repeatedly
	bool repeated_reachable(int taskid, VASSState& initial,
			VASSStateStore& original_visited);

	void build_tree(Node*& res, int taskid, int varid, int par_expr_id,
			int type, string prefix, int& num_expr);

	void add_rename(Node* parent_node, Node* child_node, int task_id,
			int child_id);

	// convert a formula to conjunctive normal form
	// assuming that negations have been pushed down to leaves
	void form_to_cnf(Formula* form, int task_id, vector<Disjunct>& disjuncts);

	// convert a formula to conjunctive normal form
	// negations have not been pushed down to leaves
	void form_to_cnf_negdown(Formula* form, int task_id,
			vector<Disjunct>& disjuncts);

	// add extra variables for atm states
	void preprocess_atms();

	void preprocess();

	int para_to_expr(Parameter& p, int task_id);

	int get_expr_id_const(int type, int id);

	int get_expr_id_var(int task_id, int id);

	int get_expr_id_navi(int task_id, int id, int attr);

	bool is_const(int expr);

	bool is_null(int expr);

	bool is_var(int expr);

	bool is_navi(int expr);

	void get_child_expr(int expr, vector<int>& res);

	void rename_to_set(int task_id, int set_id, vector<int>& vars,
			State& state);

	void rename_from_set(int task_id, int set_id, vector<int>& vars,
			State& state);

	void union_find_set_union(map<int, int>& parents, int expr1, int expr2);

	int union_find_set_find(map<int, int>& parents, int expr);

	void get_initial_state(int task_id, State& state);

	void rename_to_parent(State& state);

	void rename_to_child(int child_id, State& state);

	void project(State& state, int taskid, vector<int>& vars, State& res);

	bool intersect(State& s1, State& s2, State& res);

	bool intersect(State& s, vector<Disjunct>& disjuncts, vector<State>& res);

	bool intersect(State& s, Disjunct& disjunct, State& res);

	void insert_state_to_eqls(State& state, Disjunct& disjunct);

	bool convert_eqls_to_state(Disjunct& disjunct, State& state);

	void get_next_states(int task_id, VASSState* vstate,
			vector<VASSState*>& results);

	void insert_to_set(int task_id, SetUpdate& su, State& cur_state,
			vector<pair<State, int> >& counters);

	void retrieve_from_set(int task_id, SetUpdate& su, VASSState* vstate,
			vector<VASSState*>& next_vstates);

	void update_counter(vector<pair<State, int> >& counter, State& state,
			int delta);

	// From a search state, extract the pairs of (atm_id, state_id) where
	// atm_id is the automaton id and state_id is the current state
	void get_atm_states(int task_id, State& state, vector<pair<int, int> >& results);

	// Make automata transitions. Return a list of possible next states
	void make_atm_transition(int task_id, VASSState* vstate,
			vector<pair<int, int> >& prev_atm_states, int ser_id, bool is_open,
			int child_id, vector<int>& accepted_child_atms, vector<VASSState*>& results);

	void get_atm_form_disjunct(APropFO* prop, int task_id, vector<Disjunct>& result);

	void dump_state(State& state);

	void dump_vass_state(VASSState& state);

	void descendants(vector<vector<int> >& edges, int v, vector<int>& desc);

	// the artifact system
	Artifact& art;

	// the property that needs to be verified. The first one is the root property
	vector<Automaton> atms;

	// whether use naive pruning
	// 0 - with all prunings, 1 - without MaxFlow pruning for counters
	// 2 - without substates pruning
	int naive;

	// whether use debug mode
	bool debug;

	// the set of expressions for each variable & task
	vector<vector<set<int> > > task_var_expr_ids;

	// the expression of var & task
	vector<vector<int> > task_var_expr;

	// map from expr_ids to Node*
	vector<Node*> expr_to_node;

	// names of expressions
	vector<string> expr_name;

	// Input variables of each task
	vector<vector<int> > input_vars;

	// The variables passed from parent task to task i
	vector<vector<int> > passed_vars;

	// return variables of each task
	vector<vector<int> > return_vars;

	// return variables of each task after returned to the parent task
	vector<vector<int> > returned_vars;

	// unchanged variables. task_unchanged_vars[i] the list of unchanged variables of the parent of task i when returns
	vector<vector<int> > unchanged_vars;

	// renaming expr to the parent task
	vector<int> expr_rename_to_parent;

	// rename expr to child task, given child task id
	vector<vector<int> > expr_rename_to_child;

	// transformed global pre-condition
	vector<Disjunct> global_pre_conds;

	// transformed opening conditions
	vector<vector<Disjunct> > open_conds;

	// transformed closing conditions
	vector<vector<Disjunct> > close_conds;

	// transformed pre-conditions
	vector<vector<vector<Disjunct> > > pre_conds;

	// transformed post-conditions
	vector<vector<vector<Disjunct> > > post_conds;

	// memorize the reachable results
	vector<map<VASSState, vector<tuple<State, State, vector<int> > >*> > reach_map;

	// the set of VASSStates that cannot reach any returning state
	vector<vector<VASSState> > empty_states;

	// Trie index for empty states
	vector<TrieNode*> empty_states_tries;

	// automata formulas to vector of disjuncts
	unordered_map<APropFO*, vector<Disjunct> > atm_form_dis_map;

	// the variable id for each atm
	vector<int> atm_var_id;

	// the atms of each tasks
	vector<vector<int> > atm_task;

	// id's for zero, empty string and null
	int zero_id;
	int empty_id;
	int null_id;

	// Optimization with static checking
	void get_eql_sets(vector<tuple<int, int, bool> >& res);

    void get_eql_sets(int expr1, int expr2, vector<tuple<int, int, bool> >& result);

    void get_eql_sets(int task_id, Formula* form, vector<tuple<int, int, bool> >& result, bool pushed_down = false);

    void get_unique_sign_pairs();

    set<pair<int, int> > unique_sign_pairs;

	// profiling code
	void profile_get_num_states(vector<int>& results);

	void profile_get_num_counters(vector<int>& results);

	int profile_get_cyclomatic(int task_id, vector<VASSState*>& que, vector<vector<int> >& forward_edges);

	int profile_get_max_active_counters();

	clock_t timer;

	clock_t time_superstate;

	clock_t time_substate;

	clock_t time_nextstate;

	clock_t time_omega;

	clock_t time_scc;

	vector<set<VASSState> > profile_vstate_set;

	vector<int> profile_cyclomatic;

	int num_reach_map_tests;

	int num_reach_map_hits;

	int num_superstate_tests;

	int num_superstate_hits;

	int num_substate_tests;

	int num_substate_hits;
};

struct Node {
	int expr_id;

	virtual bool is_const();
	virtual bool is_navi();
	virtual bool is_var();

	Node();
	virtual ~Node();
};

struct ConstNode: public Node {
	int type;
	int id;

	bool is_const();
};

struct VarNode: public Node {
	int taskid;
	int id;
	vector<Node*> children;

	bool is_var();
};

struct NaviNode: public Node {
	int par_expr_id;
	vector<Node*> children;

	bool is_navi();
};

struct Disjunct {
	vector<pair<int, int> > eqs;
	vector<pair<int, int> > uneqs;

	friend bool operator<(const Disjunct& l, const Disjunct& r) {
		if (l.eqs != r.eqs)
			return l.eqs < r.eqs;
		return l.uneqs < r.uneqs;
	}

	friend bool operator==(const Disjunct& l, const Disjunct& r) {
		return (l.eqs == r.eqs && l.uneqs == r.uneqs);
	}
};

}

#endif /* VERIFIER_H_ */
