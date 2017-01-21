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
            string tmp = "(" + expr_name[expr1] + " != CONST_NULL && " + expr_name[expr2] + " != CONST_NULL)";
            int sz = ch1.size();
            for (int i = 0; i < sz; i++) {
                int c1 = ch1[i];
                int c2 = ch2[i];
                tmp += " && ";
                tmp += promela_translate_eq(c1, c2);
            }

            // handle NULL
            if (tmp != "") {
                if (is_var(expr1) && is_var(expr2))
                    res = "(" + res + " && ( (" + expr_name[expr1] + " == CONST_NULL) || (" + tmp + ")))";
                else
                    res = "(" + res + " && (" + tmp + "))";
            }
        }
	}
	return res;
}

int SpinVerifier::chromatic_number(vector<vector<int> >& graph) {
	int N = graph.size(), chromatic = 0;
	vector<int> colors(N, -1);

	for (int u = 0; u < N; u++) {
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

	return chromatic;
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

		string res = "";
		for (int i = 0; i < (int) lefts.size(); i++) {
			if (i > 0)
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
                vector<string> group;
                get_type_group(expr, group);
                res += " if ";
                
                for (string& e : group) {
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

void SpinVerifier::get_type_groups() {
    vector<tuple<int, int, bool> > eql_sets;
    get_eql_sets(eql_sets);
    get_eql_sets(property.task_id, property.form1, eql_sets);

    Internal* negated = new Internal("!");
    negated->paras[0] = property.form2->copy();
    get_eql_sets(property.task_id, negated, eql_sets);

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

    expr_group_id = vector<int>(N, -1);
    int group_id = 0;

    for (int expr = 0; expr < N; expr++) {
        if (expr_group_id[expr] >= 0 || is_const(expr))
            continue;
        
        unordered_set<int> const_exprs;
        unordered_set<int> uneql_consts;

        queue<int> que;
        que.push(expr);
        expr_group_id[expr] = group_id;
        set<pair<int, int> > uneql_set;
        unordered_map<int, int> rename;
        vector<vector<int> > uneql_graph;
        int num_node = 0;

        while (!que.empty()) {
            int u = que.front();
            que.pop();
            if (rename.count(u) == 0) {
                uneql_graph.push_back(vector<int>());
                rename[u] = num_node++;
            }

            for (auto pp : edges[u]) {
                if (!pp.second) {
                    int e1 = u;
                    int e2 = pp.first;
                    if (e1 > e2) 
                        swap(e1, e2);

                    if (!is_const(e1) && !is_const(e2))
                        uneql_set.insert(pair<int, int>(e1, e2));
                    else if (is_const(e1))
                        uneql_consts.insert(e1);
                    else if (is_const(e2))
                        uneql_consts.insert(e2);
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

        // process uneql_set to get the chromatic number
        for (auto& tp : uneql_set) {
        	int u = rename[get<0>(tp)];
        	int v = rename[get<1>(tp)];
        	uneql_graph[u].push_back(v);
        	uneql_graph[v].push_back(u);
        }

        // group_uneql_cnt.push_back(uneql_set.size());
        int uneql_cnt = uneql_consts.size();
        int chromatic = chromatic_number(uneql_graph);
        if (!const_exprs.empty() && uneql_cnt >= (int) const_exprs.size())
            chromatic++;

        group_uneql_cnt.push_back(chromatic);
        group_const_exprs.push_back(vector<int>(const_exprs.begin(), const_exprs.end()));

        group_id++;
    }
}

void SpinVerifier::get_type_group(int expr, vector<string>& res) {
    res.clear();
    
    int group_id = expr_group_id[expr];
    vector<int>& const_exprs = group_const_exprs[group_id];
    for (int expr : const_exprs)
        res.push_back(expr_name[expr]);

    int num_consts = const_exprs.size();
    int uneql_cnt = group_uneql_cnt[group_id] - (int) const_exprs.size();

    
    int num_num = art.num_consts.size();
    int num_str = art.str_consts.size();
    
    for (int i = 0; i < uneql_cnt; i++)
        res.push_back(to_string(num_num + num_str + i + 2));
}

string SpinVerifier::generate_promela() {
	// pre-processing
	preprocess();
    get_type_groups();
	// rename_expressions
	for (string& name : expr_name) {
        if (name == "NULL")
            name = "CONST_NULL";
		int len = name.length();
		for (int i = 0; i < len; i++)
			if (name[i] == '.')
				name[i] = '_';
	}

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
		if (is_const(expr)) {
			if (is_null(expr))
				promela += "    " + expr_name[expr] + " = 0;\n";
			else {
				ConstNode* node = (ConstNode*) expr_to_node[expr];
				int value = 0;

				if ((node->type == -1 && art.num_consts[node->id] == 0) ||
					(node->type == -2 && art.str_consts[node->id] == "\"\""))
					value = 0;
				else
					value = node->id + 1;

				promela += "    " + expr_name[expr] + " = " + to_string(value) + ";\n";
			}
		} else
			promela += "    " + expr_name[expr] + " = 0;\n";
	}
	promela += "    } \n";
	promela += "  current = 0; \n\n\n";
	promela += "  do\n";

	for (int task_id = 0; task_id < num_tasks; task_id++) {
		Task& task = art.tasks[task_id];

		promela += "  :: " + promela_is_active(task_id) + " ->\n";
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

        // padding for infinite loops
        /* 
		if (task_id == 0)
			promela += "    :: do :: current == 0 -> skip od\n";
        */

        // promela += "    :: else -> skip; \n";
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
					for (int expr : task_var_expr_ids[child][vid])
						promela += "    " + expr_name[expr] + " = 0;\n";
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

/*
	promela += "never { \n";
	promela += "T0_init :    \n";
	promela += "	if\n";
	promela += "	:: (1) -> goto T0_init\n";
	promela += "	:: (!(" + cond2 + ")  && (" + cond1 + ") && (current == " + to_string(property.task_id) + ") ) -> goto accept_S2\n";
	promela += "	fi;\n";
	promela += "accept_S2 :    \n";
	promela += "	if\n";
    promela += "    :: (current != " + to_string(property.task_id)+ ") -> goto accept_S2\n";
	promela += "	:: (!(" + cond2 + ") && (current == " + to_string(property.task_id)+ ")) -> goto accept_S2\n";
	promela += "	fi;\n";
	promela += "}\n";
    */
//	string target_cond = promela_translate_condition(taskid, target);
//    promela += "never { true; do :: (current == 0) && (" + target_cond + ") -> break; :: else od } ";

	return promela;
}


} /* namespace std */
