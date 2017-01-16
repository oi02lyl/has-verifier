/*
 * VASSState.cpp
 *
 *  Created on: Jul 18, 2016
 *      Author: lyl
 */

#include "VASSState.h"
#include <set>
#include <algorithm>
// Boost
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>
#include <boost/graph/read_dimacs.hpp>
#include <boost/graph/graph_utility.hpp>

namespace std {

void State::get_disjunct(vector<term_t>& disjunct) {
	disjunct.clear();

	int num_expr = exprs.size();
	int num_eqc = 0;
	for (int i = 0; i < num_expr; i++)
		if (eq_classes[i] + 1 > num_eqc)
			num_eqc = eq_classes[i] + 1;

	vector<vector<int> > eq_sets(num_eqc, vector<int>());
	for (int i = 0; i < num_expr; i++)
		eq_sets[eq_classes[i]].push_back(exprs[i]);

	for (int i = 0; i < num_eqc; i++) {
		int sz = eq_sets[i].size();

		for (int j = 0; j < sz; j++)
			for (int k = j + 1; k < sz; k++)
				if (eq_sets[i][j] < eq_sets[i][k])
					disjunct.push_back(term_t(eq_sets[i][j], eq_sets[i][k], true));
				else
					disjunct.push_back(term_t(eq_sets[i][k], eq_sets[i][j], true));
	}

	for (auto& pp : uneqs) {
		int sz1 = eq_sets[pp.first].size();
		int sz2 = eq_sets[pp.second].size();

		for (int i = 0; i < sz1; i++)
			for (int j = 0; j < sz2; j++) {
				int e1 = eq_sets[pp.first][i];
				int e2 = eq_sets[pp.second][j];
				if (e1 < e2)
					disjunct.push_back(term_t(e1, e2, false));
				else
					disjunct.push_back(term_t(e2, e1, false));
			}
	}
	sort(disjunct.begin(), disjunct.end());
}

void State::get_term_set(term_set_t& term_set) {
	vector<term_t> dj;
	get_disjunct(dj);
	term_set = term_set_t(dj.begin(), dj.end());
}

bool State::is_substate_of(State& s) {
	// return true if d1 is a superset of d2
//	vector<term_t> d1;
//	get_disjunct(d1);
//	vector<term_t> d2;
//	s.get_disjunct(d2);
//	int i1 = 0, i2 = 0, size1 = d1.size(), size2 = d2.size();
//	while (i1 < size1 && i2 < size2) {
//		if (d1[i1] < d2[i2])
//			i1++;
//		else if (d1[i1] > d2[i2])
//			return false;
//		else {
//			i1++;
//			i2++;
//		}
//	}
//	return (i2 == size2);

	int num_expr = exprs.size();
	unordered_map<int, int> eqc;
	for (int i = 0; i < num_expr; i++)
		eqc[exprs[i]] = eq_classes[i];
	set<pair<int, int> > uneqs_set(uneqs.begin(), uneqs.end());

	// check equalities
	unordered_map<int, int> s_first_expr;

	int s_num_expr = s.exprs.size();
	for (int i = 0; i < s_num_expr; i++) {
//		if (eqc.count(s.exprs[i]) == 0)
//			return false;
		if (s_first_expr.count(s.eq_classes[i]) == 0)
			s_first_expr[s.eq_classes[i]] = s.exprs[i];
		else {
			int expr1 = s.exprs[i];
			int expr2 = s_first_expr[s.eq_classes[i]];

			if (eqc.count(expr1) == 0 || eqc.count(expr2) == 0)
				return false; 

			if (eqc[expr1] != eqc[expr2])
				return false;
		}
	}

	// check unequalities
	for (auto uneq : s.uneqs) {
		int eqc1 = eqc[s_first_expr[uneq.first]];
		int eqc2 = eqc[s_first_expr[uneq.second]];
		if (eqc1 > eqc2)
			swap(eqc1, eqc2);
		if (uneqs_set.count(pair<int, int>(eqc1, eqc2)) == 0)
			return false;
	}
	return true;
}

bool State::validate() {
	unordered_map<int, int> eqc_cnt;
	int num_expr = exprs.size();
	for (int i = 0; i < num_expr; i++)
		if (eqc_cnt.count(eq_classes[i]) == 0)
			eqc_cnt[eq_classes[i]] = 1;
		else
			eqc_cnt[eq_classes[i]]++;

	for (auto uneq : uneqs) {
		eqc_cnt[uneq.first]++;
		eqc_cnt[uneq.second]++;
	}

	for (auto pp : eqc_cnt)
		if (pp.second <= 1)
			return false;
	return true;
}

State::State() {
}

State::~State() {
}

TrieNode* VASSState::get_counter_trie() {
	TrieNode* counter_trie = new TrieNode();
	for (int i = 0; i < (int) counters.size(); i++) {
		vector<term_t> dj;
		counters[i].first.get_disjunct(dj);
		vector<pair<int, vector<term_t> > > terms;
		terms.push_back(pair<int, vector<term_t> >(-1, dj));
		counter_trie->insert(i, terms, 0, -1);
	}

	return counter_trie;
}

void VASSState::get_term_lists(
		vector<pair<int, vector<term_t> > >& term_lists) {
	// state
	vector<term_t> state_dj;
	state.get_disjunct(state_dj);
	term_lists.push_back(pair<int, vector<term_t> >(-1, state_dj));

	// returns
	for (auto& pp : returns) {
		vector<term_t> ret_dj;
		pp.second.get_disjunct(ret_dj);
		term_lists.push_back(pair<int, vector<term_t> >(pp.first, ret_dj));
	}

	// counters
	set<term_t> counter_set;
	for (auto& kv : counters) {
		vector<term_t> cnt_dj;
		kv.first.get_disjunct(cnt_dj);
		counter_set.insert(cnt_dj.begin(), cnt_dj.end());
	}
	term_lists.push_back(
			pair<int, vector<term_t> >(-2,
					vector<term_t>(counter_set.begin(), counter_set.end())));
}

void VASSState::get_term_sets(vector<pair<int, term_set_t> >& term_sets) {
	term_set_t state_term_set;
	state.get_term_set(state_term_set);
	term_sets.push_back((pair<int, term_set_t>(-1, state_term_set)));

	// returns
	for (auto& pp : returns) {
		term_set_t ret_ts;
		pp.second.get_term_set(ret_ts);
		term_sets.push_back(pair<int, term_set_t>(pp.first, ret_ts));
	}

	// counters
	term_set_t counter_set;
	for (auto& kv : counters) {
		vector<term_t> cnt_dj;
		kv.first.get_disjunct(cnt_dj);
		counter_set.insert(cnt_dj.begin(), cnt_dj.end());
	}
	term_sets.push_back(pair<int, term_set_t>(-2, counter_set));
}

bool VASSState::is_substate_of(VASSState& s, TrieNode* counter_trie, bool omega, bool& changed) {
	if (returns.size() != s.returns.size())
		return false;

	if (!state.is_substate_of(s.state))
		return false;

	int r_size = returns.size();
	for (int i = 0; i < r_size; i++) {
		if (returns[i].first != s.returns[i].first)
			return false;
		if (!returns[i].second.is_substate_of(s.returns[i].second))
			return false;
	}

	if (counters.empty() && s.counters.empty())
		return true;

	// counter, need MaxFlow
	int n = counters.size();
	int m = s.counters.size();
	int max_cap = 0, total_cap = 0;
	vector<tuple<int, int, int> > edges;
	vector<vector<int> > back_edges(m, vector<int>());
	vector<vector<int> > fwd_edges(n, vector<int>());

	for (int i = 0; i < n; i++)
		if (counters[i].second >= 0)
			total_cap += counters[i].second;
	max_cap = total_cap + 1;

	for (int i = 0; i < n; i++) {
		// find matches
		vector<pair<int, term_set_t> > term_set;
		term_set_t ts;
		counters[i].first.get_term_set(ts);
		term_set.push_back(pair<int, term_set_t>(-1, ts));

		vector<int> results;
		counter_trie->query(term_set, -1, results);

		if (counters[i].second >= 0) {
			for (int j : results) {
				edges.push_back(tuple<int, int, int>(i, j + n, max_cap));
				fwd_edges[i].push_back(j);
				back_edges[j].push_back(i);
			}
		} else {
			bool no_inf = true;
			for (int j : results)
				if (s.counters[j].second < 0) {
					no_inf = false;
					break;
				}
			if (no_inf)
				return false;
		}
	}


	// add edges for sink and source
	for (int i = 0; i < n; i++)
		if (counters[i].second >= 0)
			edges.push_back(tuple<int, int, int>(n + m, i, counters[i].second));
	for (int i = 0; i < m; i++) {
		int cap = s.counters[i].second;
		if (cap < 0)
			cap = max_cap;
		edges.push_back(tuple<int, int, int>(i + n, n + m + 1, cap));
	}

	// boost max-flow
	using namespace boost;

	typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
	typedef adjacency_list<vecS, vecS, directedS,
						   property<vertex_name_t, std::string,
						   property<vertex_index_t, long,
						   property<vertex_color_t, boost::default_color_type,
						   property<vertex_distance_t, long,
						   property<vertex_predecessor_t,
						   Traits::edge_descriptor> > > > >,
						   property<edge_capacity_t, long,
						   property<edge_residual_capacity_t, long,
						   property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;

	Graph g;
	property_map<Graph, edge_capacity_t>::type capacity = get(edge_capacity, g);
	property_map<Graph, edge_residual_capacity_t>::type residual_capacity = get(
			edge_residual_capacity, g);
	property_map<Graph, edge_reverse_t>::type rev = get(edge_reverse, g);
	Traits::vertex_descriptor source, sink;
	std::vector<Traits::vertex_descriptor> verts;
	for (int i = 0; i < n + m + 2; i++)
		verts.push_back(add_vertex(g));
	source = verts[n + m];
	sink = verts[n + m + 1];
	for (auto pp : edges) {
		long head = get<1>(pp);
		long tail = get<0>(pp);
		Traits::edge_descriptor e1, e2;
		bool in1, in2;
		boost::tie(e1, in1) = add_edge(verts[tail], verts[head], g);
		boost::tie(e2, in2) = add_edge(verts[head], verts[tail], g);
		capacity[e1] = get<2>(pp);
		capacity[e2] = 0;
		rev[e1] = e2;
		rev[e2] = e1;
	}

	std::vector<default_color_type> color(num_vertices(g));
	std::vector<long> distance(num_vertices(g));
	long flow = boykov_kolmogorov_max_flow(g, source, sink);

	if (!omega)
		return (flow == total_cap);
	else {
		if (flow != total_cap)
			return false;

		vector<int> loose;
		for (int i = n; i < n + m; i++) {
			Traits::vertex_descriptor& v = verts[i];
			graph_traits<Graph>::out_edge_iterator ei, e_end;
			boost::tie(ei, e_end) = out_edges(v, g);
			if (residual_capacity[*ei] > 0) {
				loose.push_back(i - n);
			}
		}
		int ptr = 0;
		unordered_set<int> loose_set(loose.begin(), loose.end());

		while (ptr < (int) loose.size()) {
			int v = loose[ptr++];

			for (int u : back_edges[v])
				for (int vp : fwd_edges[u])
					if (loose_set.count(vp) == 0) {
						loose.push_back(vp);
						loose_set.insert(vp);
					}
		}
		for (int v : loose) {
			if (s.counters[v].second != -1) {
				changed = true;
				s.counters[v].second = -1;
			}
		}
		return true;
	}
}

bool VASSState::is_substate_of_strict(VASSState& s, TrieNode* counter_trie) {
	if (returns.size() != s.returns.size())
		return false;

	if (!state.is_substate_of(s.state))
		return false;

	int r_size = returns.size();
	for (int i = 0; i < r_size; i++) {
		if (returns[i].first != s.returns[i].first)
			return false;
		if (!returns[i].second.is_substate_of(s.returns[i].second))
			return false;
	}

	if (counters.empty() && s.counters.empty())
		return false;

	// counter, need MaxFlow
	int n = counters.size();
	int m = s.counters.size();
	int max_cap = 0, total_cap = 0;
	vector<tuple<int, int, int> > edges;
	vector<vector<int> > back_edges(m, vector<int>());
	vector<vector<int> > fwd_edges(n, vector<int>());

	for (int i = 0; i < n; i++)
		if (counters[i].second >= 0)
			total_cap += counters[i].second;
	max_cap = total_cap + 1;

	for (int i = 0; i < n; i++) {
		// find matches
		vector<pair<int, term_set_t> > term_set;
		term_set_t ts;
		counters[i].first.get_term_set(ts);
		term_set.push_back(pair<int, term_set_t>(-1, ts));

		vector<int> results;
		counter_trie->query(term_set, -1, results);

		if (counters[i].second >= 0) {
			for (int j : results) {
				edges.push_back(tuple<int, int, int>(i, j + n, max_cap));
				fwd_edges[i].push_back(j);
				back_edges[j].push_back(i);
			}
		} else {
			bool no_inf = true;
			for (int j : results)
				if (s.counters[j].second < 0) {
					no_inf = false;
					break;
				}
			if (no_inf)
				return false;
		}
	}

	// add edges for sink and source
	for (int i = 0; i < n; i++)
		if (counters[i].second >= 0)
			edges.push_back(tuple<int, int, int>(n + m, i, counters[i].second));
	for (int i = 0; i < m; i++) {
		int cap = s.counters[i].second;
		if (cap < 0)
			cap = max_cap;
		edges.push_back(tuple<int, int, int>(i + n, n + m + 1, cap));
	}

	// boost max-flow
	using namespace boost;

	typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
	typedef adjacency_list<vecS, vecS, directedS,
						   property<vertex_name_t, std::string,
						   property<vertex_index_t, long,
						   property<vertex_color_t, boost::default_color_type,
						   property<vertex_distance_t, long,
						   property<vertex_predecessor_t,
						   Traits::edge_descriptor> > > > >,
						   property<edge_capacity_t, long,
						   property<edge_residual_capacity_t, long,
						   property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;

	Graph g;
	property_map<Graph, edge_capacity_t>::type capacity = get(edge_capacity, g);
	property_map<Graph, edge_residual_capacity_t>::type residual_capacity = get(
			edge_residual_capacity, g);
	property_map<Graph, edge_reverse_t>::type rev = get(edge_reverse, g);
	Traits::vertex_descriptor source, sink;
	std::vector<Traits::vertex_descriptor> verts;
	for (int i = 0; i < n + m + 2; i++)
		verts.push_back(add_vertex(g));
	source = verts[n + m];
	sink = verts[n + m + 1];
	for (auto pp : edges) {
		long head = get<1>(pp);
		long tail = get<0>(pp);
		Traits::edge_descriptor e1, e2;
		bool in1, in2;
		boost::tie(e1, in1) = add_edge(verts[tail], verts[head], g);
		boost::tie(e2, in2) = add_edge(verts[head], verts[tail], g);
		capacity[e1] = get<2>(pp);
		capacity[e2] = 0;
		rev[e1] = e2;
		rev[e2] = e1;
	}

	std::vector<default_color_type> color(num_vertices(g));
	std::vector<long> distance(num_vertices(g));
	long flow = boykov_kolmogorov_max_flow(g, source, sink);

	if (flow != total_cap)
		return false;

	vector<int> loose;
	for (int i = n; i < n + m; i++) {
		Traits::vertex_descriptor& v = verts[i];
		graph_traits<Graph>::out_edge_iterator ei, e_end;
		boost::tie(ei, e_end) = out_edges(v, g);
		if (residual_capacity[*ei] > 0) {
			loose.push_back(i - n);
		}
	}
	return !loose.empty();
}

bool VASSState::is_substate_of(VASSState& s) {
	TrieNode* counter_trie = s.get_counter_trie();
	bool tmp = false;
	bool result = is_substate_of(s, counter_trie, false, tmp);
	delete counter_trie;
	return result;
}

bool VASSState::is_substate_of_naive(VASSState& s, int naive) {
	bool tmp = false;
	return is_substate_of_naive(s, false, tmp, naive);
}


bool VASSState::is_substate_of_naive(VASSState& s, bool omega, bool& changed, int naive) {
	if (naive == 2) {
		if (s.state != state || s.returns != returns)
			return false;
	} else {
		if (returns.size() != s.returns.size())
			return false;

		if (!state.is_substate_of(s.state))
			return false;

		int r_size = returns.size();
		for (int i = 0; i < r_size; i++) {
			if (returns[i].first != s.returns[i].first)
				return false;
			if (!returns[i].second.is_substate_of(s.returns[i].second))
				return false;
		}
	}

	if (counters.empty() && s.counters.empty())
		return true;

	vector<int> matches(counters.size(), -1);
	for (int i = 0; i < (int) counters.size(); i++) {
		bool found = false;
		for (int j = 0; j < (int) s.counters.size(); j++) {
			if (counters[i].first == s.counters[j].first) {
				if (counters[i].second < 0 && s.counters[j].second >= 0)
					return false;
				if (s.counters[j].second >= 0 && counters[i].second > s.counters[j].second)
					return false;

				matches[i] = j;
				found = true;
				break;
			}
		}
		if (!found)
			return false;
	}

	if (omega) {
		for (int i = 0; i < (int) matches.size(); i++) {
			if (counters[i].second < 0 || s.counters[matches[i]].second < 0)
				continue;
			if (counters[i].second < s.counters[matches[i]].second) {
				s.counters[matches[i]].second = -1;
				changed = true;
			}
		}
	}

	return true;
}


VASSState::VASSState() {
}

VASSState::~VASSState() {
}
//
//VASSStateStore::VASSStateStore(vector<VASSState>& st): states(st) {
//
//}

VASSStateStore::~VASSStateStore() {
	for (auto & trie : counter_tries)
		delete trie;
	for (auto& kv : state_index)
		delete kv.second;
	for (auto& kv : return_index)
		delete kv.second;
	for (auto& kv : counter_index)
		delete kv.second;
	delete trie;
}

void VASSStateStore::insert(int vstate_idx) {
	VASSState* vstate = states[vstate_idx];
	counter_tries.resize(states.size(), NULL);

	counter_tries[vstate_idx] = vstate->get_counter_trie();

	// state
	vector<term_t> state_dj;
	vstate->state.get_disjunct(state_dj);
	for (term_t& tp : state_dj) {
		if (state_index.count(tp) == 0)
			state_index[tp] = new vector<int>();
		state_index[tp]->push_back(vstate_idx);
	}

	// return
	for (auto ret : vstate->returns) {
		vector<term_t> ret_dj;
		ret.second.get_disjunct(ret_dj);
		for (auto& tp : ret_dj) {
			child_term_t new_tp = child_term_t(ret.first, get<0>(tp),
					get<1>(tp), get<2>(tp));
			if (return_index.count(new_tp) == 0)
				return_index[new_tp] = new vector<int>();
			return_index[new_tp]->push_back(vstate_idx);
		}
	}

	// counters
	term_set_t terms;
	for (pair<State, int>& ct : vstate->counters) {
		vector<term_t> cnt_dj;
		ct.first.get_disjunct(cnt_dj);
		terms.insert(cnt_dj.begin(), cnt_dj.end());
	}

	for (auto tp : terms) {
		if (counter_index.count(tp) == 0)
			counter_index[tp] = new vector<int>();
		counter_index[tp]->push_back(vstate_idx);
	}

	// trie
	vector<pair<int, vector<term_t> > > term_lists;
	vstate->get_term_lists(term_lists);
	trie->insert(vstate_idx, term_lists, 0, -1);
}

void VASSStateStore::substates(VASSState* vstate, vector<int>& results) {
	vector<vector<int>*> lists;

	// state
	vector<term_t> state_dj;
	vstate->state.get_disjunct(state_dj);
	for (auto& tp : state_dj) {
		if (state_index.count(tp) == 0)
			return;
		lists.push_back(state_index[tp]);
	}

	// return
	for (auto& ret : vstate->returns) {
		vector<term_t> dj;
		ret.second.get_disjunct(dj);
		for (auto& tp : dj) {
			child_term_t new_tp = child_term_t(ret.first, get<0>(tp),
					get<1>(tp), get<2>(tp));
			if (return_index.count(new_tp) == 0)
				return;
			lists.push_back(return_index[new_tp]);
		}
	}

	// counters
	term_set_t terms;
	for (pair<State, int>& ct : vstate->counters) {
		vector<term_t> dj;
		ct.first.get_disjunct(dj);
		terms.insert(dj.begin(), dj.end());
	}

	for (auto& tp : terms) {
		if (counter_index.count(tp) == 0)
			return;
		lists.push_back(counter_index[tp]);
	}

	if (lists.empty())
		return;

	// intersect lists
	sort(lists.begin(), lists.end(), list_length_cmp);
	vector<int> cands(lists[0]->begin(), lists[0]->end());
	for (int i = 1; i < (int) lists.size(); i++) {
		auto it = set_intersection(cands.begin(), cands.end(),
				lists[i]->begin(), lists[i]->end(), cands.begin());
		cands.resize(it - cands.begin());
	}

	// verify each candidate
	TrieNode* counter_trie = vstate->get_counter_trie();
	results.clear();
	for (int idx : cands) {
		bool tmp = false;
		if ((!naive && states[idx]->is_substate_of(*vstate, counter_trie, false, tmp)) ||
			(naive && states[idx]->is_substate_of_naive(*vstate, false, tmp, naive))	)
			results.push_back(idx);
	}
	delete counter_trie;
}

bool VASSStateStore::superstate(VASSState* vstate, vector<bool>& ignored, int& equal_vstate_id) {
	equal_vstate_id = -1;
	vector<pair<int, term_set_t> > term_sets;
	vstate->get_term_sets(term_sets);
	vector<int> results;
	trie->query(term_sets, -1, results);
	bool exists = false;
	bool tmp = false;

	for (int idx : results) {
		if (ignored[idx])
			continue;
		VASSState* cand = states[idx];
		TrieNode* counter_trie = counter_tries[idx];
		if (*vstate == *cand) {
			equal_vstate_id = idx;
			return true;
		} else if ((!naive && vstate->is_substate_of(*cand, counter_trie, false, tmp)) ||
				   (naive && vstate->is_substate_of_naive(*cand, false, tmp, naive)))
			exists = true;
	}
	return exists;
}

bool VASSStateStore::superstate(VASSState* vstate, int& equal_vstate_id) {
	equal_vstate_id = -1;
	vector<pair<int, term_set_t> > term_sets;
	vstate->get_term_sets(term_sets);
	vector<int> results;
	trie->query(term_sets, -1, results);
	bool exists = false;
	bool tmp = false;

	for (int idx : results) {
		VASSState* cand = states[idx];
		TrieNode* counter_trie = counter_tries[idx];
		if (*vstate == *cand) {
			equal_vstate_id = idx;
			return true;
		} else if ((!naive && vstate->is_substate_of(*cand, counter_trie, false, tmp)) ||
				   (naive && vstate->is_substate_of_naive(*cand, false, tmp, naive)))
			exists = true;
	}
	return exists;
}


bool VASSStateStore::superstate_strict(VASSState* vstate, int& equal_vstate_id) {
	equal_vstate_id = -1;
	vector<pair<int, term_set_t> > term_sets;
	vstate->get_term_sets(term_sets);
	vector<int> results;
	trie->query(term_sets, -1, results);
	bool exists = false;

	for (int idx : results) {
		VASSState* cand = states[idx];
		TrieNode* counter_trie = counter_tries[idx];
		if (*vstate == *cand) {
			equal_vstate_id = idx;
		} else if ((!naive && vstate->is_substate_of_strict(*cand, counter_trie)) ||
				   (naive && vstate->is_substate_of_naive(*cand, naive)))
			exists = true;
	}
	return exists;
}

bool list_length_cmp(vector<int>* l1, vector<int>* l2) {
	return l1->size() < l2->size();
}

TrieNode::TrieNode() {
}

TrieNode::~TrieNode() {
	for (auto& kv : next_level)
		delete kv.second;

	for (auto& kv : child)
		delete kv.second;
}

void TrieNode::insert(int state_id, VASSState& vstate) {
	vector<pair<int, vector<term_t> > > terms;
	vstate.get_term_lists(terms);
	insert(state_id, terms, 0, -1);
}

void TrieNode::insert(int state_id, vector<pair<int, vector<term_t> > >& terms,
		int level, int idx) {
	if (level >= (int) terms.size()) {
		state_ids.push_back(state_id);
	} else if (idx == -1) {
		int level_id = terms[level].first;
		if (next_level.count(level_id) == 0)
			next_level[level_id] = new TrieNode();
		if (idx + 1 < (int) terms[level].second.size())
			next_level[level_id]->insert(state_id, terms, level, idx + 1);
		else
			next_level[level_id]->insert(state_id, terms, level + 1, -1);
	} else {
		term_t term = terms[level].second[idx];
		if (child.count(term) == 0)
			child[term] = new TrieNode();

		if (idx + 1 < (int) terms[level].second.size())
			child[term]->insert(state_id, terms, level, idx + 1);
		else
			child[term]->insert(state_id, terms, level + 1, -1);
	}
}

void TrieNode::query(VASSState& vstate, vector<int>& results) {
	vector<pair<int, term_set_t> > terms;
	vstate.get_term_sets(terms);
	query(terms, -1, results);
}

void TrieNode::query(vector<pair<int, term_set_t> >& terms, int level,
		vector<int>& results) {
	if (!state_ids.empty())
		results.insert(results.end(), state_ids.begin(), state_ids.end());

	if (level + 1 < (int) terms.size()) {
		int nl = terms[level + 1].first;
		if (next_level.count(nl) > 0)
			next_level[nl]->query(terms, level + 1, results);
	}

	if (level >= 0 && level < (int) terms.size()) {
		for (auto& kv : child) {
			if (terms[level].second.count(kv.first) > 0)
				kv.second->query(terms, level, results);
		}
	}
}

}
