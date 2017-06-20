/*
 * SpinVerifier.cpp
 *
 *  Created on: Dec 8, 2016
 *      Author: lyl
 */

#include <queue>
#include <unordered_set>
#include <iostream>
#include "SpinVerifier.h"

namespace std {


SpinVerifier::~SpinVerifier() {
}


string SpinVerifier::promela_translate_eq(int expr1, int expr2) {
	string res = "(" + expr_name[expr1] + " == " + expr_name[expr2] + ")";

	if (naive != 2 && (is_navi(expr1) || is_var(expr1)) && (is_navi(expr2) || is_var(expr2))) {
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

        //if (naive != 2 && (unique_sign_pairs.count(p1) > 0 || unique_sign_pairs.count(p2) > 0))
        //    return "(true)";
        //else {
		    if (term->equal)
	    		return promela_translate_eq(expr1, expr2);
    		else
		    	return "(" + expr_name[expr1] + " != " + expr_name[expr2] + ")";
        //}
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
            //if (naive != 2 && (unique_sign_pairs.count(p1) > 0 || unique_sign_pairs.count(p2) > 0))
            //    res += "(true)";
            //else
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

string SpinVerifier::promela_get_assignment(int task_id, int vid) {
    char chan_name[50], func_name[50];
    sprintf(chan_name, "val_%d_%d", task_id, vid);
    string res = string("chan ") + chan_name + " = [0] of { byte }\n";
    
    sprintf(func_name, "active proctype get_val_%d_%d() {\n", task_id, vid);
    res += func_name;

    res += "  byte _msg;\n";
    res += "  do\n";
    res += "  ::\n";
    res += string("  ") + chan_name + "?_msg;\n";
    for (int expr : task_var_expr_ids[task_id][vid]) {
        vector<string> domain;
        get_expr_domain(expr, domain);
        if (domain.size() <= 1)
            continue;

        res += "  if ";
        
        for (string& e : domain) {
            if (is_navi(expr) && e == "0")
                continue;
            res += ":: " + expr_name[expr] + " = " + e + "; ";
        }
        res += "fi;\n";
    }
    res += string("  ") + chan_name + "!1;\n";
    res += "  od\n";
    res += "}\n\n";
    return res;
}

string SpinVerifier::promela_get_assignment(int task_id, vector<int>& prop_vars) {
	string res = "";
	set<int> unchanged(prop_vars.begin(), prop_vars.end());
	unchanged.insert(input_vars[task_id].begin(), input_vars[task_id].end());

	for (int vid = 0; vid < art.tasks[task_id].num_var; vid++)
		if (unchanged.count(vid) == 0) {
            char chan_name[50];
            sprintf(chan_name, "val_%d_%d", task_id, vid);
            res += string(chan_name) + "!1;\n";
            res += string(chan_name) + "?msg;\n";
            // res += promela_get_assignment(task_id, vid);
		}

    if (naive == 2) {
        for (int vid = 0; vid < art.tasks[task_id].num_var; vid++) 
            // if (unchanged.count(vid) == 0) {
                for (int e1 : task_var_expr_ids[task_id][vid]) {
                    if (expr_to_node[e1]->type < 0)
                        continue;
                    for (int e2 = 0; e2 < (int) expr_name.size(); e2++)
                        if (expr_to_node[e1]->type == expr_to_node[e2]->type) {
                            vector<int> child1;
                            vector<int> child2;
                            get_child_expr(e1, child1);
                            get_child_expr(e2, child2);

                            if (!child1.empty() && child1.size() == child2.size()) {
                                int sz = child1.size();
                                string cond = (expr_name[child1[0]] + " == " + expr_name[child2[0]]);
                                for (int i = 1; i < sz; i++) {
                                    cond += " && " + expr_name[child1[i]] + 
                                            " == " + expr_name[child2[i]];
                                }
                                res += "  if\n";
                                res += "  :: (" + expr_name[e1] + " == " + expr_name[e2] + 
                                       ") && (" + cond + ") -> skip ";
                                res += "  fi;\n";
                            }
                        }
                }
            // }
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
    
    vector<vector<int> > eq_graph(num_expr, vector<int>());
    vector<pair<int, int> > uneqls;
    for (auto tp : edges) {
        int u = get<0>(tp);
        int v = get<1>(tp);
        
        if (get<2>(tp)) {
            eq_graph[u].push_back(v);
            eq_graph[v].push_back(u);
        } else
            uneqls.push_back(pair<int, int>(u, v));
    }

    // compute connected components of =-edges
    int current_color = max_const + 1;
    for (int expr = 0; expr < num_expr; expr++) {
        if (is_const(expr)) {
            values[expr].insert(expr);
            continue;
        }
        
        queue<int> q;
        unordered_set<int> visited;
        q.push(expr);
        visited.insert(expr);
        vector<int> const_exprs;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : eq_graph[u]) 
                if (visited.count(v) == 0) {
                    visited.insert(v);
                    if (is_const(v))
                        const_exprs.push_back(v);
                    else
                        q.push(v);
                }
        }
        
        int num_uneqls = 0;
        for (pair<int, int> edge : uneqls)
            if (!is_const(edge.first) && !is_const(edge.second) && 
                visited.count(edge.first) > 0 && visited.count(edge.second) > 0)
                num_uneqls++;

        int chromatic = 1;
        while (chromatic * (chromatic - 1) < num_uneqls * 2)
            chromatic++;
        
        set<int> value_set(const_exprs.begin(), const_exprs.end());
        while ((int) value_set.size() < chromatic)
            value_set.insert(current_color++);

        for (int u : visited)
            if (!is_const(u))
                values[u] = value_set;
    }
    // handle corner case where x != y and A(x) = A(y) = {v}
    for (pair<int, int> edge : uneqls) {
        int u = edge.first;
        int v = edge.second;
        if (values[u] == values[v] && values[u].size() == 1) {
            queue<int> q;
            unordered_set<int> visited;

            if (!is_const(u)) {
                q.push(u);
                visited.insert(u);
            }
            
            if (!is_const(v)) {
                q.push(v);
                visited.insert(v);
            }

            while (!q.empty()) {
                int u = q.front();
                values[u].insert(current_color);

                q.pop();
                for (int v : eq_graph[u]) 
                    if (visited.count(v) == 0 && !is_const(v)) {
                        visited.insert(v);
                        q.push(v);
                    }
            }
            current_color++;
        }
    }

    // return
    assignment_sets = vector<vector<string> >(num_expr, vector<string>());
    int total = 0, total2 = 0, cnt = 0;
    for (int expr = 0; expr < num_expr; expr++) {
        
        if (!is_const(expr)) {
            cnt++;
            total += (int) values[expr].size();

            for (int i = 0; i < num_expr; i++)
                if (expr_to_node[i]->type == expr_to_node[expr]->type)
                    total2++;
        }
        for (int val : values[expr]) {
            if (val == null_id && expr_to_node[expr]->type >= 0 && is_navi(expr))
                continue;

            if (val <= max_const)
                assignment_sets[expr].push_back(expr_name[val]);
            else
                assignment_sets[expr].push_back(to_string(val));
        }
    }
    printf("avg_as_size = %lf\n", (double) total / cnt);
    printf("avg_as_naive = %lf\n", (double) total2 / cnt);
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
        //if (naive == 2 || (unique_sign_pairs.count(pair<int, int>(e1, e2)) == 0 && 
        //    unique_sign_pairs.count(pair<int, int>(e2, e1)) == 0))
        new_eql_sets.push_back(tp);
    }

    // NULLS
    for (int expr = 0; expr < (int) expr_to_node.size(); expr++)
        if (expr_to_node[expr]->type >= 0)
            new_eql_sets.push_back(tuple<int, int, bool>(expr, null_id, false));

    if (naive == 2) {
        int N = expr_name.size();
        for (int e1 = 0; e1 < N; e1++) {
            if (expr_to_node[e1]->type < 0)
                continue;
            for (int e2 = 0; e2 < (int) expr_name.size(); e2++)
                if (expr_to_node[e1]->type == expr_to_node[e2]->type) {
                    vector<int> child1;
                    vector<int> child2;
                    get_child_expr(e1, child1);
                    get_child_expr(e2, child2);

                    if (!child1.empty() && child1.size() == child2.size()) {
                        int sz = child1.size();
                        for (int i = 0; i < sz; i++) {
                            new_eql_sets.push_back(
                                    tuple<int, int, bool>(child1[i], child2[i], true));
                        }
                        new_eql_sets.push_back(tuple<int, int, bool>(e1, e2, true));
                    }
                }
        }
    }

    get_minimal_assignment_sets(new_eql_sets);
}

void SpinVerifier::get_expr_domain(int expr, vector<string>& res) {
    if (naive != 1)
        res = assignment_sets[expr];
    else {
        res.clear();
        int N = expr_name.size();
        for (int i = 0; i < N; i++)
            if (expr_to_node[i]->type == expr_to_node[expr]->type) {
                res.push_back(expr_name[i]);
                //if (res.size() >= 4)
                //    break;
            }
    }
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

    // variable assignments
    promela += "byte msg;\n";
    for (int task_id = 0; task_id < num_tasks; task_id++) {
        for (int vid = 0; vid < art.tasks[task_id].num_var; vid++)
            promela += promela_get_assignment(task_id, vid);
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
        // TODO need to be checked
        // promela += "    :: break;\n";
        // int ser_id = -1;
		for (Service& ser : task.services) {
            /*
            ser_id++;
            if (pre_conds[task_id][ser_id].empty() || 
                post_conds[task_id][ser_id].empty() ||
                open_conds[task_id].empty() ||
                close_conds[task_id].empty())
                continue;
                */


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
            if (open_conds[child].empty() || close_conds[child].empty())
                continue;

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
            if (open_conds[child].empty() || close_conds[child].empty())
                continue;
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
