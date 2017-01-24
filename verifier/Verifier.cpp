/*
 * Verifier.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: lyl
 */

#include "Verifier.h"

#include <stack>
#include <queue>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <algorithm>

namespace std {

Node::Node() {
	expr_id = -1;
}

Node::~Node() {
}

bool Node::is_const() {
	return false;
}

bool Node::is_navi() {
	return false;
}

bool Node::is_var() {
	return false;
}

bool ConstNode::is_const() {
	return true;
}

bool VarNode::is_var() {
	return true;
}

bool NaviNode::is_navi() {
	return true;
}

// recursively build the full navigation set
void Verifier::build_tree(Node*& res, int taskid, int varid, int par_expr_id,
		int type, string prefix, int& num_expr) {
	DBSchema& schema = art.db;
	NaviNode* node = new NaviNode();
	res = node;
	node->expr_id = num_expr++;
    node->type = type;
	node->par_expr_id = par_expr_id;
	expr_name.push_back(prefix);

	expr_to_node.push_back(node);
	task_var_expr_ids[taskid][varid].insert(node->expr_id);

	if (type >= 0) {
		node->children = vector<Node*>(schema.relations[type].arity - 1, NULL);

		// printf("%d\n", current);
		for (int i = 0; i < schema.relations[type].arity - 1; i++) {
			char name[30];
			sprintf(name, "%s.%d", prefix.c_str(), i + 1);
			build_tree(node->children[i], taskid, varid, node->expr_id,
					schema.relations[type].types[i + 1], name, num_expr);
		}
	}
}

void Verifier::add_rename(Node* parent_node, Node* child_node, int task_id,
		int child_id) {
    // TODO handle the case when input and return variables overlap in the parent
	// rename to parent
	expr_rename_to_parent[child_node->expr_id] = parent_node->expr_id;
	// rename to children
	if (expr_rename_to_child[parent_node->expr_id].empty())
		expr_rename_to_child[parent_node->expr_id] = vector<int>(
				art.tasks[task_id].children.size());
	expr_rename_to_child[parent_node->expr_id][child_id] = child_node->expr_id;

	if (parent_node->is_navi()) {
		NaviNode* n1 = (NaviNode*) parent_node;
		NaviNode* n2 = (NaviNode*) child_node;
		if (n1->children.size() != n2->children.size()) {
			exit(0);
		}
		for (int i = 0; i < (int) n1->children.size(); i++)
			add_rename(n1->children[i], n2->children[i], task_id, child_id);
	} else if (parent_node->is_var()) {
		VarNode* n1 = (VarNode*) parent_node;
		VarNode* n2 = (VarNode*) child_node;
		if (n1->children.size() != n2->children.size()) {
			printf(
					"Bug add rename, id1 = %d, id2 = %d, task = %d, child_id = %d\n",
					n1->id, n2->id, task_id, child_id);
			exit(0);
		}
		for (int i = 0; i < (int) n1->children.size(); i++)
			add_rename(n1->children[i], n2->children[i], task_id, child_id);
	}
}

void Verifier::form_to_dnf_negdown(Formula* form, int task_id,
		vector<Conjunct>& conjuncts) {
	pushdown_neg(form, false);
	vector<Conjunct> res;
	form_to_dnf(form, task_id, res);

	// remove duplicates
	for (Conjunct dj : res) {
		sort(dj.eqs.begin(), dj.eqs.end());
		sort(dj.uneqs.begin(), dj.uneqs.end());
	}

	sort(res.begin(), res.end());

	conjuncts.clear();
	int N = res.size();
	for (int i = 0; i < N; i++) {
		conjuncts.push_back(res[i]);
		while (i + 1 < N && res[i] == res[i + 1])
			i++;
	}
}

void Verifier::form_to_dnf(Formula* form, int task_id, vector<Conjunct>& res) {
	// printf("%s\n", form->to_string().c_str());
	if (form == NULL)
		return;

	if (form->is_const()) {
		ConstTerm* term = (ConstTerm*) form;
		Conjunct d;
		if (!term->value)
			d.uneqs.push_back(pair<int, int>(0, 0));
		res.push_back(d);
	}
	if (form->is_cmp()) {
		CmpTerm* term = (CmpTerm*) form;
		Conjunct d;
		int expr1 = para_to_expr(term->p1, task_id);
		int expr2 = para_to_expr(term->p2, task_id);
        
        pair<int, int> pair1(expr1, expr2);
        pair<int, int> pair2(expr2, expr1);

		if (expr1 >= 0 && expr2 >= 0) {
            if (unique_sign_pairs.count(pair1) == 0 && unique_sign_pairs.count(pair2) == 0) {
    			if (term->equal)
	    			d.eqs.push_back(pair<int, int>(expr1, expr2));
		    	else
			    	d.uneqs.push_back(pair<int, int>(expr1, expr2));
		    }
		    res.push_back(d);
        }
	} else if (form->is_relation()) {
		RelationTerm* term = (RelationTerm*) form;

		vector<int> lefts, rights;
		for (int i = 1; i < (int) term->paras.size(); i++) {
			int expr_right = para_to_expr(term->paras[i], task_id);
			if (expr_right >= 0) {
				lefts.push_back(
						get_expr_id_navi(task_id, term->paras[0].id, i - 1));
				rights.push_back(expr_right);
			}
		}

		if (term->negated) {
			for (int i = 0; i < (int) lefts.size(); i++) {
				Conjunct d;

				pair<int, int> p1(lefts[i], rights[i]);
				pair<int, int> p2(rights[i], lefts[i]);
                if (unique_sign_pairs.count(p1) == 0 && unique_sign_pairs.count(p2) == 0)
    				d.uneqs.push_back(pair<int, int>(lefts[i], rights[i]));
	    		res.push_back(d);
			}
		} else {
			Conjunct d;
			for (int i = 0; i < (int) lefts.size(); i++) {
				pair<int, int> p1(lefts[i], rights[i]);
				pair<int, int> p2(rights[i], lefts[i]);
                if (unique_sign_pairs.count(p1) == 0 && unique_sign_pairs.count(p2) == 0)
				    d.eqs.push_back(pair<int, int>(lefts[i], rights[i])); 
            }

    		res.push_back(d);
		}
	} else if (form->is_internal()) {
		Internal* term = (Internal*) form;

		if (term->op == "&&") {
			vector<Conjunct> left, right;
			State tmp;

			form_to_dnf(term->paras[0], task_id, left);
			form_to_dnf(term->paras[1], task_id, right);
			for (Conjunct& dl : left)
				for (Conjunct& dr : right) {
					Conjunct d = dl;
					d.eqs.insert(d.eqs.end(), dr.eqs.begin(), dr.eqs.end());
					d.uneqs.insert(d.uneqs.end(), dr.uneqs.begin(),
							dr.uneqs.end());
					// remove formulas that are not satisfiable
					set<int> exprs;
					for (auto& pp : d.eqs) {
						exprs.insert(pp.first);
						exprs.insert(pp.second);
					}

					for (auto& pp : d.uneqs) {
						exprs.insert(pp.first);
						exprs.insert(pp.second);
					}

					tmp.exprs = vector<int>(exprs.begin(), exprs.end());
					if (convert_eqls_to_state(d, tmp)) {
						res.push_back(d);
					}
				}
		} else {
			form_to_dnf(term->paras[0], task_id, res);
			form_to_dnf(term->paras[1], task_id, res);
		}
	}
}

void Verifier::preprocess_atms() {
	if (!atm_var_id.empty())
		return;

	atm_var_id = vector<int>(atms.size(), -1);
	atm_task = vector<vector<int> >(art.tasks.size(), vector<int>());

	for (int atm_id = 0; atm_id < (int) atms.size(); atm_id++){
		Automaton& atm = atms[atm_id];
		Task& task = art.tasks[atm.taskid];
		task.var_types.push_back(-2);
		int varid = task.num_var;
		task.vars.push_back(varid);
		task.num_var++;
		atm_var_id[atm_id] = varid;
		atm_task[atm.taskid].push_back(atm_id);

		if (atm.num_states > (int) art.str_consts.size()) {
			while (atm.num_states > (int) art.str_consts.size()) {
				int sid = (int) art.str_consts.size();
				char str[30];
				sprintf(str, "S%d", sid);
				art.str_consts.push_back(str);
			}
		}
	}
}

// build the full navigation set
void Verifier::preprocess() {
	int num_tasks = art.tasks.size();

    // preprocess atms
    preprocess_atms();

	// profile
	profile_vstate_set = vector<set<VASSState> >(num_tasks, set<VASSState>());
	profile_cyclomatic = vector<int>(num_tasks, 0);

	// reach_map
	reach_map = vector<map<VASSState, vector<tuple<State, State, vector<int> > >*> >(num_tasks,
			map<VASSState, vector<tuple<State, State, vector<int> > >*>());
	empty_states = vector<vector<VASSState> >(num_tasks, vector<VASSState>());
	empty_states_tries = vector<TrieNode*>(num_tasks, NULL);
	for (int i = 0; i < num_tasks; i++)
		empty_states_tries[i] = new TrieNode();

	// modify each task to add set variables
	for (int i = 0; i < num_tasks; i++)
		art.tasks[i].add_set_vars();

	// compute input, return and unchanged variables
	input_vars = vector<vector<int> >(num_tasks, vector<int>());
	passed_vars = vector<vector<int> >(num_tasks, vector<int>());
	return_vars = vector<vector<int> >(num_tasks, vector<int>());
	returned_vars = vector<vector<int> >(num_tasks, vector<int>());
	unchanged_vars = vector<vector<int> >(num_tasks, vector<int>());

	for (int i = 0; i < num_tasks; i++) {
		Task& task = art.tasks[i];
		for (auto& pp : task.input_vars) {
			passed_vars[i].push_back(pp.first);
			input_vars[i].push_back(pp.second);
		}
		for (auto& pp : task.return_vars) {
			return_vars[i].push_back(pp.first);
			returned_vars[i].push_back(pp.second);
		}

		int num_child = task.children.size();
		for (int j = 0; j < num_child; j++) {
			Task& child = art.tasks[task.children[j]];
			set<int> changed;
			for (auto& pp : child.return_vars)
				changed.insert(pp.second);
			for (int vid : task.vars)
				if (changed.count(vid) == 0)
					unchanged_vars[task.children[j]].push_back(vid);
		}
	}

	// clean up
	expr_name.clear();
	expr_to_node.clear();
	task_var_expr_ids = vector<vector<set<int> > >(num_tasks,
			vector<set<int> >());
	task_var_expr = vector<vector<int> >(num_tasks, vector<int>());

	for (int i = 0; i < num_tasks; i++) {
		task_var_expr_ids[i] = vector<set<int> >(art.tasks[i].num_var,
				set<int>());
		task_var_expr[i] = vector<int>(art.tasks[i].num_var, -1);
	}

	int num_expr = 0;
	// numeric consts
	for (int i = 0; i < (int) art.num_consts.size(); i++) {
		ConstNode* node = new ConstNode();
		node->id = i;
		node->expr_id = num_expr++;
		node->type = -1;
		char name[10];
		sprintf(name, "N%d", i);
		expr_name.push_back(name);
		expr_to_node.push_back(node);
	}

	// string consts
	for (int i = 0; i < (int) art.str_consts.size(); i++) {
		ConstNode* node = new ConstNode();
		node->id = i;
		node->expr_id = num_expr++;
		node->type = -2;
		char name[10];
		sprintf(name, "S%d", i);
		expr_name.push_back(name);
		expr_to_node.push_back(node);
	}
	// null const
	{
		ConstNode* node = new ConstNode();
		node->id = -1;
		node->expr_id = num_expr++;
		node->type = -3;
		expr_name.push_back("NULL");
		expr_to_node.push_back(node);
	}

	// variables
	for (int taskid = 0; taskid < num_tasks; taskid++) {
		Task& task = art.tasks[taskid];
		for (int idx = 0; idx < (int) task.vars.size(); idx++) {
			VarNode* node = new VarNode();

			node->id = task.vars[idx];
            node->type = task.var_types[idx];
			node->taskid = taskid;
			node->expr_id = num_expr++;

			task_var_expr_ids[taskid][idx].insert(node->expr_id);
			task_var_expr[taskid][idx] = node->expr_id;

			char name[10];
			sprintf(name, "T%d.X%d", taskid, idx);
			expr_name.push_back(name);
			expr_to_node.push_back(node);

			if (task.var_types[idx] >= 0) {
				int rel_id = task.var_types[idx];
				int arity = art.db.relations[rel_id].arity;
				node->children = vector<Node*>(arity - 1, NULL);

				for (int i = 0; i < arity - 1; i++) {
					int type = art.db.relations[rel_id].types[i + 1];
					char child_name[15];
					sprintf(child_name, "%s.%d", name, i + 1);

					build_tree(node->children[i], taskid, node->id,
							node->expr_id, type, child_name, num_expr);
				}
			}
		}
	}

	// build indices for renaming variables
	expr_rename_to_parent = vector<int>(num_expr, -1);
	expr_rename_to_child = vector<vector<int> >(num_expr, vector<int>());
	for (int par_task_id = 0; par_task_id < num_tasks; par_task_id++) {
		Task& task = art.tasks[par_task_id];
		for (int child_id = 0; child_id < (int) task.children.size();
				child_id++) {
			int child_task_id = task.children[child_id];
            Task& child_task = art.tasks[child_task_id];

			for (auto& pp : child_task.input_vars) {
				int parent_expr = task_var_expr[par_task_id][pp.first];
				int child_expr = task_var_expr[child_task_id][pp.second];
				add_rename(expr_to_node[parent_expr], expr_to_node[child_expr],
						par_task_id, child_id);
			}
			for (auto& pp : child_task.return_vars) {
				int parent_expr = task_var_expr[par_task_id][pp.second];
				int child_expr = task_var_expr[child_task_id][pp.first];
				add_rename(expr_to_node[parent_expr], expr_to_node[child_expr],
						par_task_id, child_id);
			}
		}
	}

	// printf("Start pre-processing formulas\n");
    // static analysis for unique sign pairs
    get_unique_sign_pairs();

	// pre-process formulas
	open_conds = vector<vector<Conjunct> >(num_tasks, vector<Conjunct>());
	close_conds = vector<vector<Conjunct> >(num_tasks, vector<Conjunct>());
	pre_conds = vector<vector<vector<Conjunct> > >(num_tasks,
			vector<vector<Conjunct> >());
	post_conds = vector<vector<vector<Conjunct> > >(num_tasks,
			vector<vector<Conjunct> >());
	global_pre_conds.clear();
	form_to_dnf_negdown(art.global_pre_cond, 0, global_pre_conds);

	vector<int> parent_task(num_tasks, -1);
	for (int task_id = 0; task_id < num_tasks; task_id++)
		for (int ch : art.tasks[task_id].children)
			parent_task[ch] = task_id;
	parent_task[0] = 0;

	for (int task_id = 0; task_id < num_tasks; task_id++) {
		form_to_dnf_negdown(art.tasks[task_id].open_cond, parent_task[task_id],
				open_conds[task_id]);
		form_to_dnf_negdown(art.tasks[task_id].close_cond, task_id,
				close_conds[task_id]);

		int num_sers = art.tasks[task_id].services.size();
		pre_conds[task_id] = vector<vector<Conjunct> >(num_sers,
				vector<Conjunct>());
		post_conds[task_id] = vector<vector<Conjunct> >(num_sers,
				vector<Conjunct>());
		for (int ser_id = 0; ser_id < num_sers; ser_id++) {
			form_to_dnf_negdown(art.tasks[task_id].services[ser_id].pre_cond,
					task_id, pre_conds[task_id][ser_id]);
			form_to_dnf_negdown(art.tasks[task_id].services[ser_id].post_cond,
					task_id, post_conds[task_id][ser_id]);
		}
	}

	// find zero, empty string and null
	for (int i = 0; i < (int) art.num_consts.size(); i++)
		if (art.num_consts[i] == 0)
			zero_id = i;

	for (int i = 0; i < (int) art.str_consts.size(); i++)
		if (art.str_consts[i] == "\"\"")
			empty_id = i;

	zero_id = get_expr_id_const(-1, zero_id);
	empty_id = get_expr_id_const(-2, empty_id);
	null_id = get_expr_id_const(-3, 0);
}

int Verifier::para_to_expr(Parameter& para, int task_id) {
	if (para.is_wildcard)
		return -1;
	if (para.is_const)
		return get_expr_id_const(para.type, para.id);
	else
		return get_expr_id_var(task_id, para.id);
}

int Verifier::get_expr_id_const(int type, int id) {
	if (type == -1)
		return id;
	else if (type == -2)
		return id + art.num_consts.size();
	else
		return art.num_consts.size() + art.str_consts.size();
}

int Verifier::get_expr_id_var(int task_id, int vid) {
	return task_var_expr[task_id][vid];
}

int Verifier::get_expr_id_navi(int task_id, int vid, int attr) {
	return ((VarNode*) expr_to_node[task_var_expr[task_id][vid]])->children[attr]->expr_id;
}

bool Verifier::is_null(int expr) {
	if (expr_to_node[expr]->is_const()) {
		ConstNode* node = (ConstNode*) expr_to_node[expr];
		return node->type == -3;
	} else
		return false;
}

bool Verifier::is_const(int expr) {
	return expr_to_node[expr]->is_const();
}

bool Verifier::is_var(int expr) {
	return expr_to_node[expr]->is_var();
}

bool Verifier::is_navi(int expr) {
	return expr_to_node[expr]->is_navi();
}

int debug_cnt = 5;

// compute the projection of a state to a set of variables
void Verifier::project(State& state, int taskid, vector<int>& vars,
		State& res) {
	// printf("project\n");

	// initialize
	res.exprs.clear();
	res.eq_classes.clear();
	res.uneqs.clear();

	int num_expr = state.exprs.size();
	vector<bool> keep(num_expr, false);

	// the number of equivalence classes
	int num_eqc = 0;
	for (int i = 0; i < num_expr; i++)
		if (state.eq_classes[i] + 1 > num_eqc)
			num_eqc = state.eq_classes[i] + 1;
	vector<int> eq_cnt(num_eqc, 0);

	// compute the set of expressions to be kept
	for (int i = 0; i < num_expr; i++) {
		int expr = state.exprs[i];

		if (is_const(expr))
			keep[i] = true;
		else {
			for (int x : vars) {
				if (task_var_expr_ids[taskid][x].count(expr)) {
					keep[i] = true;
					break;
				}
			}
		}
		if (keep[i]) {
			int eqc = state.eq_classes[i];
			eq_cnt[eqc]++;
		}
	}

	for (auto& pp : state.uneqs)
		if (eq_cnt[pp.first] > 0 && eq_cnt[pp.second] > 0) {
			eq_cnt[pp.first]++;
			eq_cnt[pp.second]++;
		}

	vector<int> new_eq_idx(num_eqc, -1);
	int num_new_eq = 0;
	for (int i = 0; i < num_expr; i++)
		if (keep[i] && eq_cnt[state.eq_classes[i]] > 1) {
			res.exprs.push_back(state.exprs[i]);
			// set equivalence classes
			if (new_eq_idx[state.eq_classes[i]] < 0)
				new_eq_idx[state.eq_classes[i]] = num_new_eq++;
			res.eq_classes.push_back(new_eq_idx[state.eq_classes[i]]);
		}

	for (auto pp : state.uneqs) {
		if (new_eq_idx[pp.first] >= 0 && new_eq_idx[pp.second] >= 0) {
			int e1 = new_eq_idx[pp.first];
			int e2 = new_eq_idx[pp.second];
			if (e1 > e2)
				swap(e1, e2);
			res.uneqs.push_back(pair<int, int>(e1, e2));
		}
	}
	sort(res.uneqs.begin(), res.uneqs.end());
//
//	if (debug_cnt >= 0) {
//		for (int var : vars)
//			printf("X%d ", var);
//		printf("\nBefore:\n");
//		dump_state(state);
//		printf("After:\n");
//		dump_state(res);
//		printf("\n");
//		debug_cnt--;
//	}
}

void Verifier::insert_state_to_eqls(State& state, Conjunct& conjunct) {
	vector<pair<int, int> >& eqs = conjunct.eqs;
	vector<pair<int, int> >& uneqs = conjunct.uneqs;

	int num_expr = state.exprs.size();
	int num_eqc = 0;
	for (int i = 0; i < num_expr; i++)
		if (state.eq_classes[i] + 1 > num_eqc)
			num_eqc = state.eq_classes[i] + 1;

	vector<vector<int> > eq_sets(num_eqc, vector<int>());
	for (int i = 0; i < num_expr; i++)
		eq_sets[state.eq_classes[i]].push_back(state.exprs[i]);

	for (int i = 0; i < num_eqc; i++)
		for (int j = 1; j < (int) eq_sets[i].size(); j++)
			if (eq_sets[i][0] < eq_sets[i][j])
				eqs.push_back(pair<int, int>(eq_sets[i][0], eq_sets[i][j]));
			else
				eqs.push_back(pair<int, int>(eq_sets[i][j], eq_sets[i][0]));

	for (auto pp : state.uneqs) {
		int e1 = eq_sets[pp.first][0];
		int e2 = eq_sets[pp.second][0];
		if (e1 < e2)
			uneqs.push_back(pair<int, int>(e1, e2));
		else
			uneqs.push_back(pair<int, int>(e2, e1));
	}
}

bool pair_first_comp(pair<int, int> l, pair<int, int> r) {
	return l.first < r.first;
}

void Verifier::get_child_expr(int expr, vector<int>& res) {
	res.clear();
	Node* node = expr_to_node[expr];

	if (node->is_var()) {
		VarNode* vn = (VarNode*) node;

		for (Node* child : vn->children)
			if (child != NULL)
				res.push_back(child->expr_id);
	} else if (node->is_navi()) {
		NaviNode* vn = (NaviNode*) node;

		for (Node* child : vn->children)
			if (child != NULL)
				res.push_back(child->expr_id);
	}
}

void Verifier::rename_to_set(int task_id, int set_id, vector<int>& vars,
		State& state) {
	vector<int>& set_vars = art.tasks[task_id].set_var_ids[set_id];
	Conjunct dj;
	for (int i = 0; i < (int) vars.size(); i++) {
		int svid = set_vars[i];
		int expr1 = get_expr_id_var(task_id, vars[i]);
		int expr2 = get_expr_id_var(task_id, svid);
		dj.eqs.push_back(pair<int, int>(expr1, expr2));
	}
	State res;
	if (!intersect(state, dj, res)) {
		printf("bug in rename to set\n");
		dump_state(state);
		intersect(state, dj, res);
		exit(0);
	}
	project(res, task_id, set_vars, state);
}

void Verifier::rename_from_set(int task_id, int set_id, vector<int>& vars,
		State& state) {
	vector<int>& set_vars = art.tasks[task_id].set_var_ids[set_id];
	Conjunct dj;
	for (int i = 0; i < (int) vars.size(); i++) {
		int svid = set_vars[i];
		int expr1 = get_expr_id_var(task_id, vars[i]);
		int expr2 = get_expr_id_var(task_id, svid);
		dj.eqs.push_back(pair<int, int>(expr1, expr2));
	}
	State res;
	if (!intersect(state, dj, res)) {
		printf("bug in rename from set\n");
		exit(0);
	}
	project(res, task_id, vars, state);
}

int Verifier::union_find_set_find(map<int, int>& parents, int expr) {
	if (parents[expr] == expr)
		return expr;
	vector<int> path;
	path.push_back(expr);
	while (expr != parents[expr]) {
		expr = parents[expr];
		path.push_back(expr);
	}
	for (int ex : path)
		parents[ex] = expr;
	return expr;
}

void Verifier::union_find_set_union(map<int, int>& parents, int expr1,
		int expr2) {
	int p1 = union_find_set_find(parents, expr1);
	int p2 = union_find_set_find(parents, expr2);

	if (p1 == p2)
		return;
	if (p1 > p2)
		swap(p1, p2);
	parents[p2] = p1;

	vector<int> p1_child_expr, p2_child_expr;
	get_child_expr(p1, p1_child_expr);
	get_child_expr(p2, p2_child_expr);

	if (p1_child_expr.size() == p2_child_expr.size()) {
//		printf("BUG!\n");
//		printf("%s\n", expr_name[p1].c_str());
//		printf("%s\n", expr_name[p2].c_str());
//
//		exit(0);
		int sz = p1_child_expr.size();
		for (int i = 0; i < sz; i++) {
			int c1 = p1_child_expr[i];
			int c2 = p2_child_expr[i];

			if (parents.count(c1) == 0 && parents.count(c2) == 0)
				continue;
			if (parents.count(c1) == 0)
				parents[c1] = c1;
			if (parents.count(c2) == 0)
				parents[c2] = c2;
			union_find_set_union(parents, c1, c2);
		}
	}
}

void Verifier::get_initial_state(int task_id, State& res) {
	Conjunct conjunct;

	set<int> input_vars_set(input_vars[task_id].begin(),
			input_vars[task_id].end());
	set<int> expr_set;
	int num_vars = art.tasks[task_id].vars.size();
    
    unordered_set<int> atm_state_exprs;
    for (int atm_id : atm_task[task_id])
        atm_state_exprs.insert(get_expr_id_var(task_id, atm_var_id[atm_id]));

	for (int vid = 0; vid < num_vars; vid++)
		if (input_vars_set.count(vid) == 0) {
			int type = art.tasks[task_id].var_types[vid];
			int expr = task_var_expr[task_id][vid];
			// printf("%d %d %d\n", vid, type, expr);
			if (type == -1) {
				conjunct.eqs.push_back(pair<int, int>(expr, zero_id));
				expr_set.insert(zero_id);
			} else if (type == -2) {
                if (atm_state_exprs.count(expr) == 0) {
    				conjunct.eqs.push_back(pair<int, int>(expr, empty_id));
	    			expr_set.insert(empty_id);
                } else {
                    // also initialize atm state
                    int state_expr = get_expr_id_const(-2, 0);
                    conjunct.eqs.push_back(pair<int, int>(expr, state_expr));
                    expr_set.insert(state_expr);
                }
			} else {
				conjunct.eqs.push_back(pair<int, int>(expr, null_id));
				expr_set.insert(null_id);
			}
			expr_set.insert(expr);
		}
	res.exprs = vector<int>(expr_set.begin(), expr_set.end());
	convert_eqls_to_state(conjunct, res);
}

// return true if eqs and uneqs can be coverted to a state
// assuming the list of expressions are set in state
bool Verifier::convert_eqls_to_state(Conjunct& conjunct, State& state) {
	// printf("convert_eqls_to_state\n");
	vector<pair<int, int> >& eqs = conjunct.eqs;
	vector<pair<int, int> >& uneqs = conjunct.uneqs;
	
	sort(eqs.begin(), eqs.end());
	sort(uneqs.begin(), uneqs.end());

	int num_expr = state.exprs.size();
	map<int, int> eqc_map;
	set<int> const_set;

	for (int expr : state.exprs) {
		eqc_map[expr] = expr;
		if (is_const(expr))
			const_set.insert(expr);
	}

	for (auto& eq : eqs)
		union_find_set_union(eqc_map, eq.first, eq.second);

	// early pruning
	for (auto& pp : uneqs) {
		if (eqc_map[pp.first] == eqc_map[pp.second])
			return false;
	}

	for (int expr : state.exprs)
		union_find_set_find(eqc_map, expr);

	// handle NULLs
	int null_par = -1;
	for (int expr : state.exprs)
		if (is_null(expr))
			null_par = eqc_map[expr];
	if (null_par >= 0) {
		// no navigation can equal to NULL
		// if a variable equals NULL, then it has no navigation
		for (int expr : state.exprs) {
			if (eqc_map[expr] == null_par) {
				if (!is_var(expr) && !is_null(expr))
					return false;
				vector<int> children;
				get_child_expr(expr, children);
				for (int ch : children)
					if (eqc_map.count(ch) > 0)
						return false;
			}
		}
	}

	int num_eqc = 0;
	state.exprs.clear();
	for (auto& kv : eqc_map)
		state.exprs.push_back(kv.first);

	for (int expr : state.exprs) {
		int parent = eqc_map[expr];
		if (expr == parent)
			eqc_map[expr] = num_eqc++;
		else
			eqc_map[expr] = eqc_map[parent];
	}

	unordered_set<int> const_eqc_set;
	unordered_set<int> non_null_set;
	int null_eqc = -1;

	// check inconsistency and set uneqs and remove sets with only one element
	for (int const_expr : const_set) {
		int eqc = eqc_map[const_expr];
		if (const_eqc_set.count(eqc) > 0)
			return false;
		const_eqc_set.insert(eqc);
	}

	for (int expr : state.exprs) {
		if (is_null(expr))
			null_eqc = eqc_map[expr];
		else {
			if (is_navi(expr) || is_const(expr))
				non_null_set.insert(eqc_map[expr]);
			vector<int> children;
			get_child_expr(expr, children);
			bool has_child = false;
			for (int ch : children)
				if (eqc_map.count(ch) > 0) {
					has_child = true;
					break;
				}
			if (has_child)
				non_null_set.insert(eqc_map[expr]);
		}
	}

	set<pair<int, int> > uneq_set;
	for (auto& pp : uneqs) {
		int eq1 = eqc_map[pp.first];
		int eq2 = eqc_map[pp.second];
		if (eq1 > eq2)
			swap(eq1, eq2);

		if (eq1 == eq2)
			return false;

		if (((const_eqc_set.count(eq1) == 0 || const_eqc_set.count(eq2) == 0))
				&& !(eq1 == null_eqc && non_null_set.count(eq2) > 0)
				&& !(eq2 == null_eqc && non_null_set.count(eq1) > 0))
			uneq_set.insert(pair<int, int>(eq1, eq2));
	}
	// state.uneqs = vector<pair<int, int> >(uneq_set.begin(), uneq_set.end());

	// printf("here\n");
	vector<int> single(num_eqc, 0);
	vector<int> unique_parent(num_eqc, -1); // -1: unknown parent -2: not unique
	for (auto& pp : eqc_map) {
		int eqc = pp.second;
		int expr = pp.first;
		single[eqc]++;

		if (unique_parent[eqc] != -2) {
			if (is_const(expr) || is_var(expr))
				unique_parent[eqc] = -2;
			else {
				int par_expr_id = ((NaviNode*) expr_to_node[expr])->par_expr_id;
				if (eqc_map.count(par_expr_id) == 0)
					unique_parent[eqc] = -2;
				else {
					int par_eqc = eqc_map[par_expr_id];

					if (unique_parent[eqc] == -1)
						unique_parent[eqc] = par_eqc;
					else if (unique_parent[eqc] != par_eqc)
						unique_parent[eqc] = -2;
				}
			}
		}
	}

	for (auto& pp : uneq_set) {
		int eqc1 = pp.first;
		int eqc2 = pp.second;
		if (single[eqc1] == 0 || single[eqc2] == 0)
			continue;
		single[eqc1]++;
		single[eqc2]++;
		unique_parent[eqc1] = -2;
		unique_parent[eqc2] = -2;
	}

	// reconstruct the list of expressions
	state.exprs.clear();
	state.eq_classes.clear();
	state.uneqs.clear();
	for (auto& kv : eqc_map) {
		if (single[kv.second] == 1 || unique_parent[kv.second] >= 0)
			continue;

		state.exprs.push_back(kv.first);
		state.eq_classes.push_back(kv.second);
	}
	num_expr = state.exprs.size();

	// rename eqc's
	int eqc_idx = 0;
	vector<int> rename(num_eqc, -1);

	for (int i = 0; i < num_expr; i++) {
		if (rename[state.eq_classes[i]] < 0)
			rename[state.eq_classes[i]] = eqc_idx++;
		state.eq_classes[i] = rename[state.eq_classes[i]];
	}

	for (auto uneq : uneq_set) {
		int e1 = rename[uneq.first];
		int e2 = rename[uneq.second];

		if (e1 >= 0 && e2 >= 0) {
			if (e1 > e2)
				swap(e1, e2);
			state.uneqs.push_back(pair<int, int>(e1, e2));
		}
	}
	sort(state.uneqs.begin(), state.uneqs.end());

	/*
	 for (int i = 0; i < (int) state.uneqs.size(); i++) {
	 state.uneqs[i].first = rename[state.uneqs[i].first];
	 state.uneqs[i].second = rename[state.uneqs[i].second];
	 if (state.uneqs[i].first < 0 || state.uneqs[i].second < 0) {
	 printf("4: %d %d\n", state.uneqs[i].first, state.uneqs[i].second);
	 exit(0);
	 }
	 }

	 if (!state.validate()) {
	 printf("At merging:\n");
	 dump_state(state);
	 exit(0);
	 }
	 */

	return true;
}

// intersect two states. Return true if the intersection is not empty
bool Verifier::intersect(State& s1, State& s2, State& res) {
	// printf("intersect\n");
	set<int> expr_set(s1.exprs.begin(), s1.exprs.end());
	expr_set.insert(s2.exprs.begin(), s2.exprs.end());
	res.exprs = vector<int>(expr_set.begin(), expr_set.end());

	Conjunct combined;

	insert_state_to_eqls(s1, combined);
	insert_state_to_eqls(s2, combined);

	bool flag = convert_eqls_to_state(combined, res);

//	if (debug_cnt >= 0) {
//		printf("S1:\n");
//		// printf("%d %d\n", (int) s1.exprs.size(), (int) flag);
//		dump_state(s1);
//		printf("S2:\n");
//		dump_state(s2);
//		if (flag) {
//			printf("Res:\n");
//			dump_state(res);
//		} else {
//			printf("Res: Empty\n");
//		}
//		printf("\n");
//		debug_cnt--;
//	}

	return flag;
}

bool Verifier::intersect(State& s, Conjunct& conjunct, State& res) {
	set<int> expr_set(s.exprs.begin(), s.exprs.end());
	for (auto& pp : conjunct.eqs) {
		expr_set.insert(pp.first);
		expr_set.insert(pp.second);
	}
	for (auto& pp : conjunct.uneqs) {
		expr_set.insert(pp.first);
		expr_set.insert(pp.second);
	}
	Conjunct combined = conjunct;
	insert_state_to_eqls(s, combined);
	res.exprs = vector<int>(expr_set.begin(), expr_set.end());

	bool flag = convert_eqls_to_state(combined, res);
	return flag;
}

bool Verifier::intersect(State& s, vector<Conjunct>& conjuncts,
		vector<State>& res) {
    set<State> res_set;

	for (Conjunct dj : conjuncts) {
		State state;
		if (intersect(s, dj, state)) {
            res_set.insert(state);
//			res.push_back(state);
        }
	}

    res.insert(res.end(), res_set.begin(), res_set.end());
	return !res.empty();
}

void Verifier::rename_to_parent(State& state) {
	int num_expr = state.exprs.size();
	for (int i = 0; i < num_expr; i++)
		if (!is_const(state.exprs[i]))
			state.exprs[i] = expr_rename_to_parent[state.exprs[i]];
}

void Verifier::rename_to_child(int child_id, State& state) {
	int num_expr = state.exprs.size();
	for (int i = 0; i < num_expr; i++)
		if (!is_const(state.exprs[i]))
			state.exprs[i] = expr_rename_to_child[state.exprs[i]][child_id];
}

void Verifier::dump_state(State& state) {
	printf("Expressions+EQC:");
	for (int i = 0; i < (int) state.exprs.size(); i++)
		printf(" %s:%d", expr_name[state.exprs[i]].c_str(),
				state.eq_classes[i]);
	printf("\nUNEQ:");
	for (int i = 0; i < (int) state.uneqs.size(); i++)
		printf(" %d:%d", state.uneqs[i].first, state.uneqs[i].second);
	printf("\n");
}

void Verifier::dump_vass_state(VASSState& state) {
	printf("State:\n");
	dump_state(state.state);
	for (auto& pp : state.returns) {
		printf("child%d:\n", pp.first);
		dump_state(pp.second);
	}

	int cc = 0;
	for (auto& pp : state.counters) {
		printf("counter%d: cnt = %d\n", cc++, pp.second);
		dump_state(pp.first);
	}
}

bool Verifier::satisfy() {
    vector<tuple<State, State, vector<int> > > res;
    reachable_root(res);

    for (auto tp : res)
        for (int atm_id : get<2>(tp))
            if (atm_id == 0)
                return true;
    return false;
}

void Verifier::reachable_root(vector<tuple<State, State, vector<int> > >& results) {
	for (Conjunct& d : global_pre_conds) {
		State start;

		set<int> exprs;
		for (auto& pp : d.eqs) {
			exprs.insert(pp.first);
			exprs.insert(pp.second);
		}
		for (auto& pp : d.uneqs) {
			exprs.insert(pp.first);
			exprs.insert(pp.second);
		}
		start.exprs = vector<int>(exprs.begin(), exprs.end());

		if (convert_eqls_to_state(d, start)) {
			vector<tuple<State, State, vector<int> > >* vec = reachable(0, start);
			results.insert(results.end(), vec->begin(), vec->end());
		}
	}
	sort(results.begin(), results.end());
}

void Verifier::update_counter(vector<pair<State, int> >& counter, State& state,
		int delta) {
	vector<pair<State, int> > res;
	bool found = false;
	for (auto& cnt : counter) {
		if (cnt.first == state) {
			found = true;
			if (cnt.second >= 0) {
				int val = cnt.second + delta;
				if (val > 0)
					res.push_back(pair<State, int>(cnt.first, val));
			} else
				res.push_back(cnt);
		} else
			res.push_back(cnt);
	}

	if (!found && delta > 0)
		res.push_back(pair<State, int>(state, delta));
	sort(res.begin(), res.end());
	counter = res;
}

void Verifier::insert_to_set(int task_id, SetUpdate& su, State& cur_state,
		vector<pair<State, int> >& counters) {
	State state_insert;
	vector<int> inst_vars = su.vars;
	inst_vars.insert(inst_vars.end(), input_vars[task_id].begin(),
			input_vars[task_id].end());
	project(cur_state, task_id, inst_vars, state_insert);
	rename_to_set(task_id, su.setid, su.vars, state_insert);
	update_counter(counters, state_insert, 1);
}

void Verifier::retrieve_from_set(int task_id, SetUpdate& su, VASSState* vstate,
		vector<VASSState*>& next_vstates) {
	for (auto& pp : vstate->counters) {
		VASSState* next = new VASSState();
		State state_retrieved = pp.first;
		rename_from_set(task_id, su.setid, su.vars, state_retrieved);

		if (intersect(vstate->state, state_retrieved, next->state)) {
			next->returns.clear();
			next->counters = vstate->counters;
			update_counter(next->counters, state_retrieved, -1);
			next_vstates.push_back(next);
		} else
			delete next;
	}
}

void Verifier::get_atm_states(int task_id, State& state, vector<pair<int, int> >& results) {

	for (int atm_id : atm_task[task_id]) {
		int var_id = atm_var_id[atm_id];
		int expr = get_expr_id_var(task_id, var_id);
		int eqc = -1;
		int sid = -1;

		for (int i = 0; i < (int) state.exprs.size(); i++)
			if (state.exprs[i] == expr) {
				eqc = state.eq_classes[i];
				break;
			}

		if (eqc >= 0) {
			for (int i = 0; i < (int) state.exprs.size(); i++)
				if (state.eq_classes[i] == eqc) {
					int e = state.exprs[i];
					if (is_const(e)) {
						ConstNode* const_node = (ConstNode*) expr_to_node[e];
						if (const_node->type == -2)
							sid = const_node->id;
					}
				}
			if (sid >= 0)
				results.push_back(pair<int, int>(atm_id, sid));
		}
	}
}

void Verifier::make_atm_transition(int task_id, VASSState* now,
		vector<pair<int, int> >& prev_atm_states, int ser_id, bool is_open,
		int child_id, vector<int>& accepted_child_atms, vector<VASSState*>& results) {

	results.clear();
	results.push_back(now);

	for (pair<int, int>& atm_state : prev_atm_states) {
		int atm_id = atm_state.first;
		int state_id = atm_state.second;

		vector<VASSState*> new_results;
		for (VASSState* vstate : results) {
			for (pair<vector<AProp*>, int>& trans : atms[atm_id].transition[state_id]) {
				vector<VASSState*> next_set;
                VASSState* new_vstate = new VASSState();
                *new_vstate = *vstate;
				next_set.push_back(new_vstate);

				bool valid = true;
				for (AProp* prop : trans.first) {
					if (prop->is_fo()) {
						vector<Conjunct> conjuncts;
						get_atm_form_conjunct((APropFO*) prop, task_id,
								conjuncts);
						vector<VASSState*> new_next_set;

						for (Conjunct& dis : conjuncts) {
							for (VASSState* next : next_set) {
								State tmp;
								if (intersect(next->state, dis, tmp)) {
									VASSState* new_vstate = new VASSState();
									*new_vstate = *next;
									new_vstate->state = tmp;
									new_next_set.push_back(new_vstate);
								}
							}
						}

						for (VASSState* next : next_set)
							delete next;
						next_set = new_next_set;

						if (next_set.empty()) {
							valid = false;
							break;
						}
					} else if (prop->is_child()) {
						APropChild* apc = (APropChild*) prop;
						bool flag = (ser_id < 0 && apc->child_id == child_id
								&& apc->is_open == is_open);
						if (flag == prop->negated) {
							valid = false;
							break;
						}
					} else if (prop->is_service()) {
						APropService* aps = (APropService*) prop;
						bool flag = (ser_id >= 0 && aps->ser_id == ser_id);
						if (flag == prop->negated) {
							valid = false;
							break;
						}
					} else {
						APropSubForm* apsf = (APropSubForm*) prop;
						bool flag = (ser_id >= 0 && is_open
								&& apsf->child_id == child_id);

						if (flag) {
							bool accept = false;
							for (int accepted_atm : accepted_child_atms) {
								if (accepted_atm == apsf->auto_id) {
									accept = true;
									break;
								}
							}
							if (!accept)
								flag = false;
						}
						if (flag != prop->negated) {
							valid = false;
							break;
						}
					}
				}

				if (valid) {
					for (VASSState* next : next_set) {
						Conjunct dis;
						int expr1 = get_expr_id_const(-2, trans.second);
						int expr2 = get_expr_id_var(task_id, atm_var_id[atm_id]);
						dis.eqs.push_back(pair<int, int>(expr1, expr2));
						State tmp;
						if (intersect(next->state, dis, tmp)) {
							next->state = tmp;
							new_results.push_back(next);
						}
					}
				} else {
					for (VASSState* next : next_set)
						delete next;
				}
			}
		}
		for (VASSState* vstate : results)
			delete vstate;

        results.clear();
        set<VASSState> vstate_sets;
        for (VASSState* vstate : new_results) {
            if (vstate_sets.count(*vstate) == 0) {
                results.push_back(vstate);
                vstate_sets.insert(*vstate);
            } else {
                delete vstate;
            }
        }
	}
}

void Verifier::get_atm_form_conjunct(APropFO* prop, int task_id,
		vector<Conjunct>& result) {
	if (atm_form_dis_map.count(prop) > 0)
		result = atm_form_dis_map[prop];

	if (prop->negated) {
		Internal* new_form = new Internal();
		new_form->op = "!";
		new_form->paras.push_back(prop->fo->copy());
		prop->fo = new_form;
	}

	form_to_dnf_negdown(prop->fo, task_id, result);
	atm_form_dis_map[prop] = result;
}

void Verifier::get_next_states(int task_id, VASSState* vstate,
		vector<VASSState*>& results) {
	clock_t start = clock();

	results.clear();
	Task& task = art.tasks[task_id];
	int num_child = task.children.size();
	int num_sers = task.services.size();
	bool no_active = vstate->returns.empty();

	unordered_set<int> active_child;
	for (auto& pp : vstate->returns)
		active_child.insert(pp.first);

	vector<pair<int, int> > prev_atm_states;
	get_atm_states(task_id, vstate->state, prev_atm_states);

	VASSState* next = NULL;
	vector<State> vec_tmp;

	// opening child task
	for (int i = 0; i < num_child; i++) {
		if (active_child.count(i) == 0
				&& intersect(vstate->state, open_conds[task.children[i]],
						vec_tmp)) {
			for (State& after_precond : vec_tmp) {
				State input;
				project(after_precond, task_id, passed_vars[task.children[i]],
						input);
				rename_to_child(i, input);

				vector<tuple<State, State, vector<int> > >* outputs;
				time_nextstate += clock() - start;
				outputs = reachable(task.children[i], input);
				start = clock();
				for (tuple<State, State, vector<int> >& out : *outputs) {
					next = new VASSState();
					if (!intersect(after_precond, get<0>(out), next->state))
						continue;
					next->returns = vstate->returns;
					next->returns.push_back(pair<int, State>(i, get<1>(out)));
					sort(next->returns.begin(), next->returns.end());
					next->counters = vstate->counters;

					vector<VASSState*> next_set;
					make_atm_transition(task_id, next, prev_atm_states, -1,
							true, i, get<2>(out), next_set);
					results.insert(results.end(), next_set.begin(),
							next_set.end());
					// results.push_back(next);
				}
			}
		}
	}

	// closing child task
	for (auto& ret : vstate->returns) {
		int i = ret.first;
		State tmp;
		project(vstate->state, task_id, unchanged_vars[task.children[i]], tmp);
		next = new VASSState();
		if (intersect(tmp, ret.second, next->state)) {
			for (auto& pp : vstate->returns)
				if (pp.first != i)
					next->returns.push_back(pp);
			next->counters = vstate->counters;
			vector<VASSState*> next_set;
			vector<int> tmp;
			make_atm_transition(task_id, next, prev_atm_states, -1,
					false, i, tmp, next_set);
			results.insert(results.end(), next_set.begin(),
					next_set.end());
		}
	}

	// applying internal service
	if (no_active) {
		for (int i = 0; i < num_sers; i++) {
			// get insert and retrieve info
			vector<SetUpdate> retrieve;
			vector<SetUpdate> insert_before;
			vector<SetUpdate> insert_after;

			// handle insert before and insert after
			// handle multiple insert and retrieve
			for (SetUpdate& su : task.services[i].set_update) {
				if (su.type == InsertBefore)
					insert_before.push_back(su);
				else if (su.type == InsertAfter)
					insert_after.push_back(su);
				else if (su.type == Retrieve)
					retrieve.push_back(su);
			}

			// intersect with pre-condition
			intersect(vstate->state, pre_conds[task_id][i], vec_tmp);

			State projected;
			vector<State> after_postconds;
			for (State& after_precond : vec_tmp) {
				// insertion
				vector<pair<State, int> > after_insert = vstate->counters;
				for (SetUpdate& su : insert_before)
					insert_to_set(task_id, su, after_precond, after_insert);

				// project to preserving variables
				project(after_precond, task_id, task.services[i].var_prop,
						projected);

				// intersect with post-condition
				intersect(projected, post_conds[task_id][i], after_postconds);
				for (State& after_postcond : after_postconds) {
					VASSState* vstate = new VASSState();
					vstate->state = after_postcond;
					vstate->returns.clear();
					vstate->counters = after_insert;
					vector<VASSState*> candidates;
					candidates.push_back(vstate);

					int ptr_head = 0, ptr_end = 1;
					for (SetUpdate& su : retrieve) {
						for (int i = ptr_head; i < ptr_end; i++) {
							vector<VASSState*> new_vstates;
							retrieve_from_set(task_id, su, candidates[i],
									new_vstates);
							candidates.insert(candidates.end(),
									new_vstates.begin(), new_vstates.end());
						}
						ptr_head = ptr_end;
						ptr_end = candidates.size();
					}

					for (VASSState* cand : candidates) {
						vector<VASSState*> next_set;
						vector<int> tmp;
						make_atm_transition(task_id, cand, prev_atm_states, -1,
								false, i, tmp, next_set);

						for (VASSState* next : next_set) {
							for (SetUpdate& su : insert_after)
								insert_to_set(task_id, su, next->state,
										next->counters);
							results.push_back(next);
						}
					}
				}

			}
		}
	}
    
    // remove duplicate
    vector<VASSState*> new_results;
    set<VASSState> vstate_set;
    for (VASSState* vstate : results) {
        if (vstate_set.count(*vstate) == 0) {
            vstate_set.insert(*vstate);
            new_results.push_back(vstate);
        } else
            delete vstate;
    }
    results = new_results;

	time_nextstate += clock() - start;
}

int global_counter = 0;

void Verifier::descendants(vector<vector<int> >& edges, int v,
		vector<int>& desc) {
	queue<int> que;
	unordered_set<int> visited;
	que.push(v);
	visited.insert(v);

	while (!que.empty()) {
		int now = que.front();
		que.pop();

		for (int next : edges[now])
			if (visited.count(next) == 0) {
				que.push(next);
				visited.insert(next);
			}
	}
	desc = vector<int>(visited.begin(), visited.end());
}

vector<tuple<State, State, vector<int> > >* Verifier::reachable(int taskid,
		State& input_state) {
	VASSState* init = new VASSState();

	// intersect with initialization
	State init_state;
	get_initial_state(taskid, init_state);
	bool flag = intersect(input_state, init_state, init->state);
	// dump_state(input_state);
	// dump_state(init_state);

	if (!flag) {
		printf("Bug in initial state! Check overlapping between inputs / returns variables in task %d\n", taskid);
	    flag = intersect(input_state, init_state, init->state);
		exit(0);
	}
	//init->state = input_state;

	init->returns.clear();
	init->counters.clear();

	num_reach_map_tests++;
	auto map_ptr = reach_map[taskid].find(*init);
	if (map_ptr != reach_map[taskid].end()) {
		num_reach_map_hits++;
		delete init;
		return map_ptr->second;
	}

	// Karp-Miller Tree
	vector<VASSState*> que;
	vector<vector<int> > backward_edges;
	vector<vector<int> > forward_edges;
	vector<bool> pruned;

	// set of output states each VASSState can reach
	vector<set<tuple<State, State, vector<int> > > > output_sets;

	VASSStateStore visited(que, naive);

	// add initial states
	que.push_back(init);
	backward_edges.push_back(vector<int>());
	forward_edges.push_back(vector<int>());
	pruned.push_back(false);
	output_sets.push_back(set<tuple<State, State, vector<int> > >());
	visited.insert(0);

	// profiling
	clock_t start_time = 0;

	int ptr_idx = 0;
	while (ptr_idx < (int) que.size()) {
		if ((float) (clock() - timer) / CLOCKS_PER_SEC >= 1800)
			break;

		global_counter++;

		VASSState* now = que[ptr_idx];
		int now_idx = ptr_idx;
		ptr_idx++;

		if (debug && global_counter % 1000 == 0) {
			printf("At task %d, cnt = %d\n", taskid, global_counter);
			for (int i = 0; i < (int) art.tasks.size(); i++)
				printf("Task %d : reach_map.size() = %d\n", i,
						(int) reach_map[i].size());
			dump_vass_state(*now);
			printf("\n");
		}

		if (pruned[now_idx])
			continue;

		// check whether there exists empty superstate
		vector<int> cand_ids;
		empty_states_tries[taskid]->query(*now, cand_ids);
		bool has_empty_superstate = false;
		for (int idx : cand_ids) {
			if ((!naive && now->is_substate_of(empty_states[taskid][idx]))
					|| (naive
							&& now->is_substate_of_naive(
									empty_states[taskid][idx], naive))) {
				has_empty_superstate = true;
				break;
			}
		}
		if (has_empty_superstate)
			continue;

		// check previously reached
		auto reach_map_ptr = reach_map[taskid].find(*now);
		num_reach_map_tests++;

		if (reach_map_ptr != reach_map[taskid].end()) {
			num_reach_map_hits++;
			output_sets[now_idx].insert(reach_map_ptr->second->begin(),
					reach_map_ptr->second->end());
			continue;
		}

		// collect ancestors
		vector<int> ancestors;
		descendants(backward_edges, now_idx, ancestors);
		unordered_set<int> anc_set(ancestors.begin(), ancestors.end());
		// printf("%d\n", (int) ancestors.size());

		// compute next states
		vector<VASSState*> next_states;
		get_next_states(taskid, now, next_states);

		for (VASSState* next : next_states) {
			// prune the search if a superstate is visited
			// if equals to the superstate, add forward/backward edges
			num_superstate_tests++;
			start_time = clock();
			int equal_vstate_idx = -1;
			if (visited.superstate(next, pruned, equal_vstate_idx)) {
				time_superstate += clock() - start_time;
				num_superstate_hits++;
				// add edges only if equal
				if (equal_vstate_idx >= 0) {
					forward_edges[now_idx].push_back(equal_vstate_idx);
					backward_edges[equal_vstate_idx].push_back(now_idx);
					delete next;
				} else {
					// add edges and set pruned to be true
					int next_idx = (int) que.size();
					que.push_back(next);
					backward_edges.push_back(vector<int>());
					forward_edges.push_back(vector<int>());
					backward_edges[next_idx].push_back(now_idx);
					forward_edges[now_idx].push_back(next_idx);
					pruned.push_back(true);
					output_sets.push_back(set<tuple<State, State, vector<int> > >());
				}
				continue;
			}
			time_superstate += clock() - start_time;

			// omega operation
			start_time = clock();
			TrieNode* counter_trie = next->get_counter_trie();
			bool changed = true;
			while (changed) {
				changed = false;
				for (int anc_idx : ancestors) {
					//			if (!pruned[anc_idx]) {
					if (!naive)
						que[anc_idx]->is_substate_of(*next, counter_trie, true,
								changed);
					else {
						que[anc_idx]->is_substate_of_naive(*next, true, changed,
								naive);
					}
					//			}
				}
			}
			delete counter_trie;

			// remove counters that are substate of an omega counter
			if (!naive) {
				vector<pair<State, int> > new_counters;
				for (pair<State, int>& cnt : next->counters) {
					bool found = false;
					for (pair<State, int>& omega_cnt : next->counters)
						if (omega_cnt.second < 0 && cnt.first != omega_cnt.first
								&& cnt.first.is_substate_of(omega_cnt.first)) {
							found = true;
							break;
						}
					if (!found)
						new_counters.push_back(cnt);
				}
				next->counters = new_counters;
			}
			time_omega += clock() - start_time;

			// pruned substates and their descendants
			vector<int> substate_indices;
			num_substate_tests++;
			start_time = clock();
			visited.substates(next, substate_indices);
			time_substate += clock() - start_time;
			num_substate_hits += (!substate_indices.empty());

			for (int ss_idx : substate_indices) {
				if (anc_set.count(ss_idx) == 0 || !pruned[ss_idx]) {
					vector<int> desc;
					descendants(forward_edges, ss_idx, desc);
					for (int d : desc)
						pruned[d] = true;
				}
			}

			// add edges
			int next_idx = (int) que.size();
			que.push_back(next);
			backward_edges.push_back(vector<int>());
			forward_edges.push_back(vector<int>());
			backward_edges[next_idx].push_back(now_idx);
			forward_edges[now_idx].push_back(next_idx);
			pruned.push_back(false);
			output_sets.push_back(set<tuple<State, State, vector<int> > >());

			visited.insert(next_idx);
		}
	}
	
	// profiling: get cyclomatic complexity
	profile_cyclomatic[taskid] = max(profile_cyclomatic[taskid],
									 profile_get_cyclomatic(taskid, que, forward_edges));

    int que_size = que.size();
    // perform repeated reachability test
	vector<bool> repeated;

    // TODO: ignore repeated reachability test when there is no atm
    if (atm_task[taskid].empty())
        repeated = vector<bool>(que_size, false);
    else
    	repeated_reachable(taskid, que, visited, forward_edges, pruned, repeated);

	// collect final states
	for (int idx = que_size - 1; idx >= 0; idx--) {
		VASSState* now = que[idx];
		if (!pruned[idx] && reach_map[taskid].count(*now) == 0) {
			if (now->returns.empty()) {
				vector<State> last_states;
                // finite local runs
				intersect(now->state, close_conds[taskid], last_states);
                // infinite local runs
                if (repeated[idx])
                    last_states.push_back(now->state);
                int num_last_states = last_states.size();

				for (int last_idx = 0; last_idx < num_last_states; last_idx++) {
                    State& last = last_states[last_idx];
					tuple<State, State, vector<int> > out;

                    if (last_idx < num_last_states - 1 || !repeated[idx]) {
                        // finite
                        project(last, taskid, input_vars[taskid], get<0>(out));
                        project(last, taskid, return_vars[taskid], get<1>(out));
                        rename_to_parent(get<0>(out));
                        rename_to_parent(get<1>(out));
                    } else {
                        // infinite
                        project(last, taskid, input_vars[taskid], get<0>(out));
                        rename_to_parent(get<0>(out));
                        // write something invalid to get<1>(out)
                        State invalid;
                        invalid.exprs.push_back(0);
                        invalid.eq_classes.push_back(0);
                        invalid.uneqs.push_back(pair<int, int>(0, 0));
                        get<1>(out) = invalid;
                    }
                    
					vector<pair<int, int> > atm_states;
					get_atm_states(taskid, last, atm_states);
					for (pair<int, int>& atm_state : atm_states) {
						int atm_id = atm_state.first;
						int state_id = atm_state.second;
						if (atms[atm_id].states[state_id].accept) {
                            if (repeated[idx] || taskid != 0 )
    							get<2>(out).push_back(atm_id);
                        }
					}

					output_sets[idx].insert(out);
				}
			}

//			reach_map[taskid][*now] = new vector<pair<State, State> >(
//					output_sets[idx].begin(), output_sets[idx].end());
//
//			// build index for empty states
//			if (output_sets[idx].empty()) {
//				int state_id = (int) empty_states[taskid].size();
//				empty_states[taskid].push_back(*now);
//				empty_states_tries[taskid]->insert(state_id, empty_states[taskid][state_id]);
//			}
		}

		for (int prev_idx : backward_edges[idx]) {
			output_sets[prev_idx].insert(output_sets[idx].begin(),
					output_sets[idx].end());
		}
	}

	reach_map[taskid][*init] = new vector<tuple<State, State, vector<int> > >(
			output_sets[0].begin(), output_sets[0].end());

	// build index for empty states
	if (output_sets[0].empty()) {
		int state_id = (int) empty_states[taskid].size();
		empty_states[taskid].push_back(*init);
		empty_states_tries[taskid]->insert(state_id,
				empty_states[taskid][state_id]);
	}

	vector<tuple<State, State, vector<int> > >* res = reach_map[taskid][*init];

	// collect all vstates for profiling
	for (VASSState* vstate : que)
		profile_vstate_set[taskid].insert(*vstate);

	// delete all vass_states
	for (VASSState* vstate : que)
		delete vstate;
	return res;
}

void Verifier::repeated_reachable(int taskid, vector<VASSState*>& que,
		VASSStateStore& visited, vector<vector<int> >& forward_edges,
		vector<bool>& pruned, vector<bool>& result) {
	// profiling
	clock_t start = clock();

	int num_nodes = que.size();
	result = vector<bool>(num_nodes, false);

	// mark omega nodes
	for (int i = 0; i < num_nodes; i++) {
		bool has_omega = false;
		for (auto& counter : que[i]->counters)
			if (counter.second == -1) {
				has_omega = true;
				break;
			}
		if (has_omega)
			result[i] = true;
	}

	// Compute the strongly connected components
	vector<int> color(num_nodes, 0);
	vector<bool> in_stack(num_nodes, false);
	vector<int> visit_idx(num_nodes, -1);
	vector<int> low_idx(num_nodes, -1);

	stack<int> dfs_stk, scc_stk;
	dfs_stk.push(0);
	// color[0] = 1;
	int current_idx = 0;
	VASSStateStore visited_new(que, naive);

	//visited_new.insert(0);
	// int cnt = 0;
	while (!dfs_stk.empty()) {
		if ((float) (clock() - timer) / CLOCKS_PER_SEC >= 1800)
			break;

		int u = dfs_stk.top();
		if (color[u] == 0) {
			int eq_idx = -1;
			if (visited_new.superstate(que[u], eq_idx)) {
				color[u] = 2;
				dfs_stk.pop();
			} else {
				visited_new.insert(u);
				visit_idx[u] = current_idx;
				low_idx[u] = current_idx++;
				color[u] = 1;
				scc_stk.push(u);
				in_stack[u] = true;

				// add forward edges if missing
				int vid = -1, tmp = -1;
				if (forward_edges[u].empty()) {
					vector<VASSState*> next_states;

					// time_scc += clock() - start;
					get_next_states(taskid, que[u], next_states);
					// start = clock();

					for (VASSState* vstate : next_states) {
						bool added = false;

						bool covered = visited.superstate(vstate, tmp);
						//if (!covered)
						//	printf("bug!\n");

						if (covered
								&& !visited.superstate_strict(vstate, tmp)) {
							if (!visited_new.superstate(vstate, vid)) {
								vid = (int) que.size();
								que.push_back(vstate);
								// visited_new.insert(vid);
								color.push_back(0);
								in_stack.push_back(false);
								visit_idx.push_back(-1);
								low_idx.push_back(-1);
								forward_edges.push_back(vector<int>());
								added = true;
								forward_edges[u].push_back(vid);
								if (color[vid] == 0) {
									// color[vid] = 1;
									dfs_stk.push(vid);
								} else if (in_stack[vid] && visit_idx[vid] >= 0)
									low_idx[u] = min(low_idx[u],
											visit_idx[vid]);
							}
						}
						if (!added)
							delete vstate;
					}
				} else {
					for (int v : forward_edges[u])
						if (!result[v] && visited.superstate(que[v], tmp)
								&& !visited.superstate_strict(que[v], tmp)
								&& !(visited_new.superstate(que[v], vid))) {
							if (color[v] == 0) {
								// visited_new.insert(v);
								// color[v] = 1;
								dfs_stk.push(v);
							} else if (in_stack[v] && visit_idx[v] >= 0)
								low_idx[u] = min(low_idx[u], visit_idx[v]);
						}
				}
			}
		} else if (color[u] == 1) {
			color[u] = 2;
			dfs_stk.pop();
			for (int v : forward_edges[u])
				if (low_idx[v] >= 0)
					low_idx[u] = min(low_idx[u], low_idx[v]);

			if (low_idx[u] == visit_idx[u]) {
				vector<int> scc;
				while (true) {
					int v = scc_stk.top();
					scc_stk.pop();
					scc.push_back(v);
					in_stack[v] = false;

					if (v == u)
						break;
				}
				if (scc.size() > 1) {
					for (int v : scc)
						if (v < num_nodes)
							result[v] = true;
				} else {
                    bool self_loop = false;
                    for (int v : forward_edges[u])
                        if (u == v) {
                            self_loop = true;
                            break;
                        }
                    if (self_loop && u < num_nodes)
                        result[u] = true;
                }
            }
		} else {
			dfs_stk.pop();
		}
	}
    
    // mark all self-loops
    for (int u = 0; u < num_nodes; u++) {
        if (result[u])
            continue;
        bool self_loop = false;
        for (int v : forward_edges[u])
            if (u == v) {
                self_loop = true;
                break;
            }
        if (self_loop)
            result[u] = true;
    }

	time_scc += clock() - start;
}

void Verifier::get_eql_sets(vector<tuple<int, int, bool> >& eql_sets) {
	eql_sets.clear();

    // pre-, post-, open-, close-conditions
    for (int task_id = 0; task_id < (int) art.tasks.size(); task_id++) {
        Task& task = art.tasks[task_id];

        get_eql_sets(task_id, task.close_cond, eql_sets);
        for (int child_task_id : task.children)
            get_eql_sets(task_id, art.tasks[child_task_id].open_cond, eql_sets);

        for (Service& ser : task.services) {
            get_eql_sets(task_id, ser.pre_cond, eql_sets);
            get_eql_sets(task_id, ser.post_cond, eql_sets);
        }
    }

    // eqls for opening/closing tasks
    for (int task_id = 0; task_id < (int) art.tasks.size(); task_id++) {
    	Task& task = art.tasks[task_id];
		// opening
		for (int child : task.children) {
			Task& child_task = art.tasks[child];
			set<int> input_var_set(input_vars[child].begin(), input_vars[child].end());

			for (int vid = 0; vid < child_task.num_var; vid++)
				if (input_var_set.count(vid) > 0) {
					for (int expr : task_var_expr_ids[child][vid]) {
                        int e = expr_rename_to_parent[expr];
                        eql_sets.push_back(tuple<int, int, bool>(expr, e, true));
                    }
				}
		}
		// closing
		for (int child : task.children)
			for (int vid : return_vars[child]) {
				for (int expr : task_var_expr_ids[child][vid]) {
                    int e = expr_rename_to_parent[expr];
                    eql_sets.push_back(tuple<int, int, bool>(expr, e, true));
                }
			}
		// initialization
		for (int vid = 0; vid < task.num_var; vid++) {
			int expr = get_expr_id_var(task_id, vid);
			if (task.var_types[vid] == -1)
				eql_sets.push_back(tuple<int, int, bool>(expr, zero_id, true));
			else if (task.var_types[vid] == -2)
				eql_sets.push_back(tuple<int, int, bool>(expr, empty_id, true));
			else
				eql_sets.push_back(tuple<int, int, bool>(expr, null_id, true));
		}
    }

    // eqls in property
    for (Automaton& atm : atms) {
        for (int sid = 0; sid < atm.num_states; sid++)
            for (auto& tran : atm.transition[sid])
                for (AProp* prop : tran.first)
                    if (prop->is_fo()) {
                        APropFO* p = (APropFO*) prop;
                        get_eql_sets(atm.taskid, p->fo, eql_sets);
                    }
    }
}

void Verifier::get_eql_sets(int expr1, int expr2, vector<tuple<int, int, bool> >& res) {
    res.push_back(tuple<int, int, bool>(expr1, expr2, true));

	if ((is_navi(expr1) || is_var(expr1)) && (is_navi(expr2) || is_var(expr2))) {
		vector<int> ch1, ch2;
		get_child_expr(expr1, ch1);
		get_child_expr(expr2, ch2);

		if (ch1.size() != ch2.size()) {
			printf("children sizes of %s and %s doesn't match!", expr_name[expr1].c_str(), expr_name[expr2].c_str());
			exit(0);
		}

		int sz = ch1.size();
		for (int i = 0; i < sz; i++) {
			int c1 = ch1[i];
			int c2 = ch2[i];
            get_eql_sets(c1, c2, res);
		}
	}
}


void Verifier::get_eql_sets(int task_id, Formula* form, vector<tuple<int, int, bool> >& res, bool pushed_down) {
    if (!pushed_down)
        pushdown_neg(form, false);

	if (form == NULL || form->is_const())
		return;
	else if (form->is_cmp()) {
		CmpTerm* term = (CmpTerm*) form;
		int expr1 = para_to_expr(term->p1, task_id);
		int expr2 = para_to_expr(term->p2, task_id);

		if (term->equal)
			get_eql_sets(expr1, expr2, res);
		else
            res.push_back(tuple<int, int, bool>(expr1, expr2, false));
	} else if (form->is_relation()) {
		RelationTerm* term = (RelationTerm*) form;

		vector<int> lefts, rights;
		for (int i = 1; i < (int) term->paras.size(); i++) {
			int expr_right = para_to_expr(term->paras[i], task_id);
			if (expr_right >= 0) {
				lefts.push_back(
						get_expr_id_navi(task_id, term->paras[0].id, i - 1));
				rights.push_back(expr_right);
			}
		}

        vector<tuple<int, int, bool> > tmp;
		for (int i = 0; i < (int) lefts.size(); i++)
            get_eql_sets(lefts[i], rights[i], tmp);

		if (term->negated) {
			for (auto& tp : tmp)
                get<2>(tp) = false;
        }
		res.insert(res.end(), tmp.begin(), tmp.end());
	} else if (form->is_internal()) {
		Internal* term = (Internal*) form;
        get_eql_sets(task_id, term->paras[0], res, true);
        get_eql_sets(task_id, term->paras[1], res, true);
	}

}

void Verifier::get_unique_sign_pairs() {
	if (!unique_sign_pairs.empty())
		return;

    vector<tuple<int, int, bool> > eql_sets;
    get_eql_sets(eql_sets);

    // bfs
    int N = expr_name.size();
    vector<vector<pair<int, bool> > > edges(N, vector<pair<int, bool> >());
    for (auto& tp : eql_sets) {
        int u = get<0>(tp);
        int v = get<1>(tp);
        bool flag = get<2>(tp);
        edges[u].push_back(pair<int, bool>(v, flag));
        edges[v].push_back(pair<int, bool>(u, flag));
    }

    vector<int> expr_group_id = vector<int>(N, -1);
    int group_id = 0;

    for (int expr = 0; expr < N; expr++) {
        if (expr_group_id[expr] >= 0 || is_const(expr))
            continue;

        unordered_set<int> const_exprs;
        queue<int> que;
        que.push(expr);
        expr_group_id[expr] = group_id;
        set<tuple<int, int, bool> > edge_set;
        int uneql_cnt = 0;
        int eql_cnt = 0;
        Conjunct conj;

        while (!que.empty()) {
            int u = que.front();
            que.pop();

            for (auto pp : edges[u]) {
            	// add edge
				int e1 = u;
				int e2 = pp.first;
				if (e1 > e2)
					swap(e1, e2);
                if (pp.second)
                    conj.eqs.push_back(pair<int, int>(e1, e2));
                else
                    conj.uneqs.push_back(pair<int, int>(e1, e2));

				edge_set.insert(tuple<int, int, bool>(e1, e2, pp.second));
                if (!is_const(e1) || !is_const(e2)) {                    
                    if (pp.second)
                        eql_cnt++;
                    else
                        uneql_cnt++;
                }

                if (expr_group_id[pp.first] == -1) {
                    if (is_const(pp.first))
                        const_exprs.insert(pp.first);
                    else {
                    	expr_group_id[pp.first] = group_id;
                    	que.push(pp.first);
                    }
                }
            }
        }

        if ((const_exprs.size() <= 1 && uneql_cnt == 0) || eql_cnt == 0) {
            for (auto& tp : edge_set)
                unique_sign_pairs.insert(pair<int, int>(get<0>(tp), get<1>(tp)));
        } else {
            set<pair<int, int> > eqs_set(conj.eqs.begin(), conj.eqs.end());
            conj.eqs = vector<pair<int, int> >(eqs_set.begin(), eqs_set.end());
            set<pair<int, int> > uneqs_set(conj.uneqs.begin(), conj.uneqs.end());
            conj.uneqs = vector<pair<int, int> >(uneqs_set.begin(), uneqs_set.end());
            
            set<int> expr_set;
            for (pair<int, int> pp : conj.eqs) {
                expr_set.insert(pp.first);
                expr_set.insert(pp.second);
            }
            for (pair<int, int> pp : conj.uneqs) {
                expr_set.insert(pp.first);
                expr_set.insert(pp.second);
            }

            State tmp;
            tmp.exprs = vector<int>(expr_set.begin(), expr_set.end());
            if (convert_eqls_to_state(conj, tmp)) {
                for (auto& tp : edge_set)
                    unique_sign_pairs.insert(pair<int, int>(get<0>(tp), get<1>(tp)));
            } else {
                // add unequalities that does not violate any equalties
                Conjunct eq_only;
                // eq_only.eqs = conj.eqs;
                tmp.exprs = vector<int>(expr_set.begin(), expr_set.end());
                vector<pair<int, int> > const_eqs;
                for (pair<int, int> eq : conj.eqs)
                    if (is_const(eq.first) || is_const(eq.second)) {
                        if (eq.first > eq.second)
                            swap(eq.first, eq.second);
                        const_eqs.push_back(eq);
                    } else
                        eq_only.eqs.push_back(eq);

                sort(const_eqs.begin(), const_eqs.end());
                int num_const_eqs = const_eqs.size();
                for (int i = 0; i < num_const_eqs; i++) {
                    vector<int> exprs(1, const_eqs[i].second);
                    while (i + 1 < num_const_eqs && const_eqs[i + 1].first == const_eqs[i].first) {
                        i++;
                        exprs.push_back(const_eqs[i].first);
                    }
                    for (int j = 0; j + 1 < (int) exprs.size(); j++)
                        eq_only.eqs.push_back(pair<int, int>(exprs[j], exprs[j + 1]));
                }

                if (convert_eqls_to_state(eq_only, tmp)) {
                    map<int, int> eqc;
                    for (int idx = 0; idx < (int) tmp.exprs.size(); idx++)
                        eqc[tmp.exprs[idx]] = tmp.eq_classes[idx];
                    for (auto& tp : edge_set) {
                        int e1 = get<0>(tp);
                        int e2 = get<1>(tp);
                        if (!get<2>(tp) && !(eqc.count(e1) > 0 && eqc.count(e2) > 0 && eqc[e1] != eqc[e2]))
                            unique_sign_pairs.insert(pair<int, int>(e1, e2));
                    }
                }

                // add equalities that does not violate any unequalities
                Conjunct uneq_only;
                uneq_only.uneqs = conj.uneqs;
                set<int> const_set;
                for (pair<int, int>& pp : conj.eqs)
                    if (is_const(pp.first) || is_const(pp.second)) {
                        uneq_only.eqs.push_back(pp);
                        if (is_const(pp.first))
                            const_set.insert(pp.first);
                        if (is_const(pp.second))
                            const_set.insert(pp.second);
                    }
                if (const_set.size() <= 1 && convert_eqls_to_state(uneq_only, tmp)) {
                    map<int, int> eqc;
                    for (int idx = 0; idx < (int) tmp.exprs.size(); idx++)
                        eqc[tmp.exprs[idx]] = tmp.eq_classes[idx];
                    set<pair<int, int> > uneql_set(tmp.uneqs.begin(), tmp.uneqs.end());

                    for (auto& tp : edge_set) {
                        if (get<2>(tp)) {
                            int e1 = get<0>(tp);
                            int e2 = get<1>(tp);
                            if (eqc.count(e1) == 0 || eqc.count(e2) == 0)
                                unique_sign_pairs.insert(pair<int, int>(e1, e2));
                            else {
                                int eqc1 = eqc[e1];
                                int eqc2 = eqc[e2];
                                if (eqc1 > eqc2)
                                    swap(eqc1, eqc2);
                                if (uneql_set.count(pair<int, int>(eqc1, eqc2)) == 0)
                                    unique_sign_pairs.insert(pair<int, int>(e1, e2));
                            }
                        }
                    }
                }

                tmp.exprs = vector<int>(expr_set.begin(), expr_set.end());
            }
        }
        group_id++;
    }
    
    // printf("%d\n", (int) unique_sign_pairs.size());
    // node with degree 1
    for (int expr = 0; expr < N; expr++) {
        set<pair<int, bool> > edge_set(edges[expr].begin(), edges[expr].end());
        if (edge_set.size() == 1) {
            int e1 = expr;
            int e2 = edges[expr][0].first;
            if (e1 > e2)
                swap(e1, e2);

            if (!edges[expr][0].second || (!is_const(e1) && !is_const(e2)))
                unique_sign_pairs.insert(pair<int, int>(e1, e2));
        }
    }
    
    // printf("%d\n", (int) unique_sign_pairs.size());
}

void Verifier::profile_get_num_counters(vector<int>& results) {
	int num_tasks = art.tasks.size();

	results = vector<int>(num_tasks, 0);
	for (int i = 0; i < num_tasks; i++) {
		set<State> state_set;
		for (auto& vs : profile_vstate_set[i])
			for (auto& cnt : vs.counters)
				state_set.insert(cnt.first);
		results[i] = state_set.size();
	}
}

void Verifier::profile_get_num_states(vector<int>& results) {
	int num_tasks = art.tasks.size();

	results = vector<int>(num_tasks, 0);
	for (int i = 0; i < num_tasks; i++) {
		set<State> state_set;
		for (auto& vs : profile_vstate_set[i])
			state_set.insert(vs.state);
		results[i] = state_set.size();
	}
}

int Verifier::profile_get_max_active_counters() {
	int result = 0;
	int num_tasks = art.tasks.size();
	for (int i = 0; i < num_tasks; i++)
		for (auto& vs : profile_vstate_set[i])
			result = max(result, (int) vs.counters.size());
	return result;
}

int Verifier::profile_get_cyclomatic(int task_id, vector<VASSState*>& que, vector<vector<int> >& forward_edges) {
	int N = que.size();
	vector<int> projected;
	map<State, int> proj_map;
	vector<int> nonid_vars;
	Task& task = art.tasks[task_id];

	for (int vid = 0; vid < task.num_var; vid++) {
		if (task.var_types[vid] < 0)
			nonid_vars.push_back(vid);
	}

	for (int i = 0; i < N; i++) {
		State p;

        /*
        if (task_id == 0) {
            printf("State %d:\n", i);
            dump_state(que[i]->state);
            printf("Edges: ");
            for (int v : forward_edges[i])
                printf("%d ", v);
            printf("\n\n");
        }
        */

		project(que[i]->state, task_id, nonid_vars, p);
		if (proj_map.count(p) == 0)
			proj_map[p] = (int) proj_map.size();
		projected.push_back(proj_map[p]);
	}

	set<pair<int, int> > edge_set;
	for (int u = 0; u < N; u++) {
		int new_u = projected[u];

		for (int v : forward_edges[u]) {
			int new_v = projected[v];
			edge_set.insert(pair<int, int>(new_u, new_v));
		}
	}

	return (int) edge_set.size() - (int) proj_map.size() + 2;
}


}
