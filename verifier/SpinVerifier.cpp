/*
 * SpinVerifier.cpp
 *
 *  Created on: Dec 8, 2016
 *      Author: lyl
 */

#include <queue>
#include <unordered_set>
#include "SpinVerifier.h"

namespace std {


SpinVerifier::~SpinVerifier() {
}


string SpinVerifier::promela_translate_eq(int expr1, int expr2) {
	string res = "(" + expr_name[expr1] + " == " + expr_name[expr2] + ")";

	if ((is_navi(expr1) || is_var(expr1)) && (is_navi(expr2) || is_var(expr2))) {
		vector<int> ch1, ch2;
		get_child_expr(expr1, ch1);
		get_child_expr(expr2, ch2);

		if (ch1.size() != ch2.size()) {
			printf("children sizes of %s and %s doesn't match!", expr_name[expr1].c_str(), expr_name[expr2].c_str());
			exit(0);
		}

        if (!ch1.empty() && !ch2.empty()) {
            int sz = ch1.size();
            for (int i = 0; i < sz; i++) {
                int c1 = ch1[i];
                int c2 = ch2[i];
                res += " && ";
                res += promela_translate_eq(c1, c2);
            }
        }
	}
	return "(" + res + ")";
}


string SpinVerifier::promela_translate_condition(int task_id, Formula* form) {
	if (form == NULL)
		return "true";

	if (form->is_const()) {
		ConstTerm* term = (ConstTerm*) form;
		if (term->value)
			return "true";
		else
			return "false";
	} else if (form->is_cmp()) {
		CmpTerm* term = (CmpTerm*) form;
		int expr1 = para_to_expr(term->p1, task_id);
		int expr2 = para_to_expr(term->p2, task_id);
        pair<int, int> p1(expr1, expr2);
        pair<int, int> p2(expr2, expr1);

        if (unique_sign_pairs.count(p1) > 0 || unique_sign_pairs.count(p2) > 0)
            return "(true)";
        else {
		    if (term->equal)
	    		return promela_translate_eq(expr1, expr2);
    		else
		    	return "(" + expr_name[expr1] + " != " + expr_name[expr2] + ")";
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

        int root_id = get_expr_id_var(task_id, term->paras[0].id);
		string res = "(" + expr_name[root_id] + " != CONST_NULL)";
		for (int i = 0; i < (int) lefts.size(); i++) {
			res += " && ";

            pair<int, int> p1(lefts[i], rights[i]);
            pair<int, int> p2(rights[i], lefts[i]);
            if (unique_sign_pairs.count(p1) > 0 || unique_sign_pairs.count(p2) > 0)
                res += "(true)";
            else
    			res += promela_translate_eq(lefts[i], rights[i]);
		}
        if (res == "")
            res = "true";

		if (term->negated)
			res = "!( " + res + " )";
		return res;
	} else if (form->is_internal()) {
		Internal* term = (Internal*) form;

		string left = promela_translate_condition(task_id, term->paras[0]);
		string right = promela_translate_condition(task_id, term->paras[1]);
		return "(" + left + " " + term->op +  " " + right + ")";
	}
	return "true";
}

string SpinVerifier::promela_get_assignment(int task_id, vector<int>& prop_vars) {
	string res = "";
	set<int> unchanged(prop_vars.begin(), prop_vars.end());
	unchanged.insert(input_vars[task_id].begin(), input_vars[task_id].end());

	for (int vid = 0; vid < art.tasks[task_id].num_var; vid++)
		if (unchanged.count(vid) == 0) {
			for (int expr : task_var_expr_ids[task_id][vid]) {
                vector<string> domain;
                get_expr_domain(expr, domain);
                res += " if ";
                
                for (string& e : domain) {
                    if (is_navi(expr) && e == "0")
                        continue;
                    res += ":: " + expr_name[expr] + " = " + e + "; ";
                }
                res += " fi;\n";
            }
		}
	return res;
}

void SpinVerifier::get_prop_exprs(int task_id, vector<int>& prop_vars, vector<int>& res) {
    res.clear();
	set<int> unchanged(prop_vars.begin(), prop_vars.end());
	unchanged.insert(input_vars[task_id].begin(), input_vars[task_id].end());

	for (int vid = 0; vid < art.tasks[task_id].num_var; vid++)
		if (unchanged.count(vid) == 0) 
			for (int expr : task_var_expr_ids[task_id][vid])
                res.push_back(expr);
}

string SpinVerifier::promela_is_current(int task_id) {
	return "(current == " + to_string(task_id) + ")";
}

string SpinVerifier::promela_is_active(int task_id) {
	return "running[" + to_string(task_id) + "]";
}

string SpinVerifier::promela_is_ready(int task_id) {
	return "ready[" + to_string(task_id) + "]";
}

string SpinVerifier::promela_all_child_inactive(int task_id) {
	string res = "";
	Task& task = art.tasks[task_id];
	res += "(";
	for (int child_id = 0; child_id < (int) task.children.size(); child_id++) {
		int child = task.children[child_id];
		if (child_id > 0)
			res += " && ";
		res += "!(" +promela_is_active(child) + ")";
	}
	res += ")";

    if (res == "()")
        res = "true";
	return res;
}


void SpinVerifier::get_minimal_assignment_sets(vector<tuple<int, int, bool> >& edges) {
    int max_const = 0;
    unordered_set<int> const_set;
    int num_expr = expr_name.size();
    vector<set<int> > values = vector<set<int> >(num_expr, set<int>());

    // initialize set of consts
    for (int expr = 0; expr < num_expr; expr++)
        if (is_const(expr)) {
            max_const = max(max_const, expr);
            const_set.insert(expr);
        }

    // build uneql graph
    vector<vector<int> > graph(num_expr, vector<int>());
    for (auto tp : edges) {
        if (!get<2>(tp)) {
            int u = get<0>(tp);
            int v = get<1>(tp);
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
    }

    // greedy algorithm to compute chromatic numbers
	int chromatic = max_const + 1;
	vector<int> colors(num_expr, -1);
    for (int expr : const_set)
        colors[expr] = expr;

	for (int u = 0; u < num_expr; u++) {
        if (colors[u] >= 0)
            continue;
		unordered_set<int> used;
		for (int v : graph[u])
			if (colors[v] >= 0)
				used.insert(colors[v]);
		bool found = false;
		for (int c = 0; c < chromatic; c++)
			if (used.count(c) == 0) {
				found = true;
				colors[u] = c;
				break;
			}
		if (!found) {
			colors[u] = chromatic++;
		}
	}
    
    for (int expr = 0; expr < num_expr; expr++)
        values[expr].insert(colors[expr]);

    // propagate
    bool found = true;
    while (found) {
        found = false;

        for (auto tp : edges) {
            int u = get<0>(tp);
            int v = get<1>(tp);
            if (!get<2>(tp) || (is_const(u) && is_const(v)))
                continue;

            if (is_const(u) && values[v].count(u) == 0) {
                found = true;
                values[v].insert(u);
            } else if (is_const(v) && values[u].count(v) == 0) {
                found = true;
                values[u].insert(v);
            } else if (!is_const(u) && !is_const(v) && values[u] != values[v]) {
                found = true;
                values[u].insert(values[v].begin(), values[v].end());
                values[v] = values[u];
            }
        }
    }

    // return
    assignment_sets = vector<vector<string> >(num_expr, vector<string>());
    for (int expr = 0; expr < num_expr; expr++)
        for (int val : values[expr]) {
            if (val == null_id && expr_to_node[expr]->type >= 0 && is_navi(expr))
                continue;

            if (val <= max_const)
                assignment_sets[expr].push_back(expr_name[val]);
            else
                assignment_sets[expr].push_back(to_string(val));
        }
}



void SpinVerifier::compute_expr_domains() {
    vector<tuple<int, int, bool> > eql_sets;
    get_eql_sets(eql_sets);
    get_eql_sets(property.task_id, property.form1, eql_sets);

    Internal* negated = new Internal("!");
    negated->paras[0] = property.form2->copy();
    get_eql_sets(property.task_id, negated, eql_sets);

    // remove the ones with unique sign
    vector<tuple<int, int, bool> > new_eql_sets;
    for (auto tp : eql_sets) {
        int e1 = get<0>(tp);
        int e2 = get<1>(tp);
        if (unique_sign_pairs.count(pair<int, int>(e1, e2)) == 0 && 
            unique_sign_pairs.count(pair<int, int>(e2, e1)) == 0)
            new_eql_sets.push_back(tp);
    }

    // NULLS
    for (int expr = 0; expr < (int) expr_to_node.size(); expr++)
        if (expr_to_node[expr]->type >= 0)
            new_eql_sets.push_back(tuple<int, int, bool>(expr, null_id, false));

    get_minimal_assignment_sets(new_eql_sets);
}

void SpinVerifier::get_expr_domain(int expr, vector<string>& res) {
    res = assignment_sets[expr];
}

string SpinVerifier::generate_promela() {
	// pre-processing
	preprocess();
	// rename_expressions
	for (string& name : expr_name) {
        if (name == "NULL")
            name = "CONST_NULL";
		int len = name.length();
		for (int i = 0; i < len; i++)
			if (name[i] == '.')
				name[i] = '_';
	}
    compute_expr_domains();

	int num_tasks = art.tasks.size();
	vector<int> parent_task(num_tasks, -1);
	for (int i = 0; i < num_tasks; i++)
		for (int child : art.tasks[i].children)
			parent_task[child] = i;

	string promela = "";

	// variables to indicate status of tasks
	promela += "byte current;\n";
	promela += "bool running[" + to_string(num_tasks) + "];\n";
	promela += "bool ready[" + to_string(num_tasks) + "];\n\n";


	// all expressions
	for (int expr = 0; expr < (int) expr_name.size(); expr++) {
		promela += "byte " + expr_name[expr] + ";\n";
	}

	promela += "init {\n";
    promela += "    atomic { ";
	promela += "    current = 255;\n";
	for (int i = 0; i < num_tasks; i++) {
		promela += "    ready[" + to_string(i) + "] = false;\n";
		if (i == 0)
			promela += "    running[" + to_string(i) + "] = true;\n";
		else
			promela += "    running[" + to_string(i) + "] = false;\n";
	}
	promela += "\n";
	for (int expr = 0; expr < (int) expr_name.size(); expr++) {
		if (is_const(expr))
            promela += "    " + expr_name[expr] + " = " + to_string(expr) + ";\n";
	}
	promela += "    } \n";
	promela += "  current = 0; \n\n\n";
	promela += "  do\n";

	for (int task_id = 0; task_id < num_tasks; task_id++) {
		Task& task = art.tasks[task_id];

		promela += "  :: " + promela_is_current(task_id) + " ->\n";
		promela += "  if\n";
		// services
		promela += "  :: atomic { " + promela_all_child_inactive(task_id) + " -> \n";
		promela += "    if\n";

		for (Service& ser : task.services) {
			// pre-condition
			promela += "    :: (" + promela_translate_condition(task_id, ser.pre_cond) + ") -> \n";
			// assignment and post-condition
            promela += "      current = 255; \n";
            
			promela += "      " + promela_get_assignment(task_id, ser.var_prop) + " \n";
            promela += "      if \n";
			promela += "      :: " + promela_translate_condition(task_id, ser.post_cond)+ " -> skip; \n";
			promela += "      fi;   \n ";
            
            promela += "      current = " + to_string(task_id) + "; \n";
		}
		promela += "  fi } \n";

		// opening child tasks
		for (int child : task.children) {
			promela += "  :: atomic { !" + promela_is_active(child) + " && (" + promela_translate_condition(task_id, art.tasks[child].open_cond) + ") ->\n";
			promela += "    current = " + to_string(child) + ";\n";
			promela += "    running[" + to_string(child) + "] = true;\n";
			// copy and initialize
			Task& child_task = art.tasks[child];
			set<int> input_var_set(input_vars[child].begin(), input_vars[child].end());

			for (int vid = 0; vid < child_task.num_var; vid++) {
				if (input_var_set.count(vid) == 0) {
					for (int expr : task_var_expr_ids[child][vid]) {
                        string init_value;
                        int type = expr_to_node[expr]->type;
                        if (type == -1)
                            init_value = expr_name[zero_id];
                        else if (type == -2)
                            init_value = expr_name[empty_id];
                        else
                            init_value = expr_name[null_id];
  						promela += "    " + expr_name[expr] + " = " + init_value + ";\n";
                    }
				} else {
					for (int expr : task_var_expr_ids[child][vid])
						promela += "    " + expr_name[expr] + " = " + expr_name[expr_rename_to_parent[expr]] + ";\n";
				}
			}
            promela += "} \n";
		}

		// closing child tasks
		for (int child : task.children) {
			promela += "  :: atomic { " + promela_is_ready(child) + " ->\n";
			promela += "    ready[" + to_string(child) + "] = false;\n";
			promela += "    running[" + to_string(child) + "] = false;\n";
			// copy
			for (int vid : return_vars[child]) {
				for (int expr : task_var_expr_ids[child][vid])
					promela += "    " + expr_name[expr_rename_to_parent[expr]] + " = " + expr_name[expr] + ";\n";
			}  
            promela += " } \n";
		}

		// task finishes
		if (task_id > 0) {
			promela += "  :: atomic { (" + promela_translate_condition(task_id, art.tasks[task_id].close_cond) + ") ->\n";
			promela += "    current = " + to_string(parent_task[task_id]) + ";\n";
			promela += "    ready[" + to_string(task_id) + "] = true;\n";
            promela += " } \n";
		}
		// promela += "  :: else skip;\n";
		promela += "  fi\n";
	}
	promela += "    od;\n";
    promela += "end:\n";
	promela += "}\n";

    string cond0 = "(current == " + to_string(property.task_id) + ")";
	string cond1 = "(" + cond0 + " && " + promela_translate_condition(property.task_id, property.form1) + ")";
	string cond2 = "(" + cond0 + " && " + promela_translate_condition(property.task_id, property.form2) + ")";
    
    promela += "never { /* (G (F r)) && !( G (p -> F q)) */\n";
    promela += "T0_init :    /* init */\n";
    promela += "    if\n";
    promela += "    :: (1) -> goto T0_init\n";
    promela += "    :: (!" + cond2 +" && " + cond1 + ") -> goto T0_S4\n";
    promela += "                            fi;\n";
    promela += "T0_S4 :    /* 1 */\n";
    promela += "    if\n";
    promela += "    :: (!" + cond2 + ") -> goto T0_S4\n";
    promela += "    :: (" + cond0 + " && !(" + cond2 + ")) -> goto accept_S4\n";
    promela += "    fi;\n";
    promela += "accept_S4 :    /* 2 */\n";
    promela += "    if\n";
    promela += "    :: (!" + cond2 + ") -> goto T0_S4\n";
    promela += "    :: (" + cond0 + " && !" + cond2 + ") -> goto accept_S4\n";
    promela += "    fi;\n";
    promela += "}\n";

	return promela;
}


} /* namespace std */
