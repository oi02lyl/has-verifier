/*
 * VASSState.h
 *
 *  Created on: Jul 18, 2016
 *      Author: lyl
 */

#ifndef VASSSTATE_H_
#define VASSSTATE_H_

#include <map>
#include <unordered_map>
#include <unordered_set>

#include "Verifier.h"
#include "Utility.h"
namespace std {

typedef unordered_map<term_t, vector<int>*, key_hash, key_equal> inverted_list_t;
typedef unordered_set<term_t, key_hash, key_equal> term_set_t;

class TrieNode;

struct State {
	vector<int> exprs;
	vector<int> eq_classes;
	vector<pair<int, int> > uneqs;

	State();

	~State();

	// convert the state to disjunct
	// result will be stored in variable disjunct
	void get_disjunct(vector<term_t>& res);

	// return the set of the terms in disjunct
	void get_term_set(term_set_t& res);

	// return true if the current state is a substate of s
	bool is_substate_of(State& s);

	// return false if there are redundant information stored in the state
	bool validate();

	friend bool operator==(const State& l, const State& r) {
		return (l.exprs == r.exprs) && (l.eq_classes == r.eq_classes)
				&& (l.uneqs == r.uneqs);
	}

	friend bool operator!=(const State& l, const State& r) {
		return !(l == r);
	}

	friend bool operator<(const State& l, const State& r) {
		if (l.exprs != r.exprs)
			return l.exprs < r.exprs;
		if (l.eq_classes != r.eq_classes)
			return l.eq_classes < r.eq_classes;
		return l.uneqs < r.uneqs;
	}
};

// VASS state
struct VASSState {
	// local state
	State state;

	// VASS counter
	vector<pair<State, int> > counters;

	// NULL means the child task is inactive
	// otherwise the guessed return isomorphism type
	vector<pair<int, State> > returns;

	VASSState();

	~VASSState();

	// return true if the current state is a substate of s
	bool is_substate_of(VASSState& s);

	// substate funciton given the Trie of the counters
	bool is_substate_of(VASSState& s, TrieNode* counter_trie, bool omega, bool& changed);

	// substate funciton given the Trie of the counters
	bool is_substate_of_strict(VASSState& s, TrieNode* counter_trie);

	// naive version of testing substate relation
	bool is_substate_of_naive(VASSState& s, int naive);

	// naive version of testing substate relation
	bool is_substate_of_naive(VASSState& s, bool omega, bool& changed, int naive);

	// return the trie of the counters
	TrieNode* get_counter_trie();

	// return the lists of terms, state -> returns -> counter
	void get_term_lists(vector<pair<int, vector<term_t> > >& res);

	// return the sets of terms, same order above
	void get_term_sets(vector<pair<int, term_set_t> >& res);

	friend bool operator==(const VASSState& l, const VASSState& r) {
		return l.state == r.state && l.returns == r.returns && l.counters == r.counters;
	}

	friend bool operator<(const VASSState& l, const VASSState& r) {
		if (l.state != r.state)
			return l.state < r.state;
		if (l.returns != r.returns)
			return l.returns < r.returns;
		if (l.counters != r.counters)
			return l.counters < r.counters;
		return false;
	}
};

class TrieNode {
public:
	unordered_map<term_t, TrieNode*, key_hash, key_equal> child;

	// -1 for state, -2 for counter, otherwise child task
	unordered_map<int, TrieNode*> next_level;

	// list of state_id
	vector<int> state_ids;

	TrieNode();

	~TrieNode();

	void insert(int state_id, VASSState& vstate);

	void insert(int state_id, vector<pair<int, vector<term_t> > >& terms, int level,
			int idx);

	void query(VASSState& vstate, vector<int>& results);

	void query(vector<pair<int, term_set_t> >& terms, int level, vector<int>& results);
};


class VASSStateStore {
public:
	vector<VASSState*>& states;

	vector<TrieNode*> counter_tries;

	TrieNode* trie;

	// use naive substate or not
	int naive;

	// left_expr, right_expr, eq/uneq
	inverted_list_t state_index;

	// child_id, left_expr, right_expr, eq/uneq
	unordered_map<child_term_t, vector<int>*, key_hash2, key_equal2> return_index;

	// left_expr, right_expr, eq/uneq, cnt
	inverted_list_t counter_index;

	VASSStateStore(vector<VASSState*>& _states): states(_states), naive(false) { trie = new TrieNode(); }

	VASSStateStore(vector<VASSState*>& _states, int _naive): states(_states), naive(_naive) { trie = new TrieNode(); }

	~VASSStateStore();

	void insert(int vstate_id);

	bool superstate(VASSState* vstate, int& equal_vstate_id);

	bool superstate_strict(VASSState* vstate, int& equal_vstate_id);

	bool superstate(VASSState* vstate, vector<bool>& ignored, int& equal_vstate_id);

	void substates(VASSState* vstate, vector<int>& results);
};

bool list_length_cmp(vector<int>* l1, vector<int>* l2);

}

#endif /* VASSSTATE_H_ */
