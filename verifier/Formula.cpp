/*
 * Formula.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: lyl
 */

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "Formula.h"

namespace std {

Formula::Formula() {
}
Formula::~Formula() {
}

string Parameter::to_string() {
	char res[10];
	if (is_const) {
        if (is_null)
            sprintf(res, "NULL");
        else {
		    if (type == -1)
	    		sprintf(res, "%d", id);
    		else
		    	sprintf(res, "\"S%d\"", id);
        }
	}
	else {
        if (is_wildcard)
            return "_";
		sprintf(res, "X%d", id);
    }
	return res;
}

bool Formula::is_const() {
	return false;
}

bool Formula::is_cmp() {
	return false;
}

bool Formula::is_relation() {
	return false;
}

bool Formula::is_internal() {
	return false;
}

bool CmpTerm::is_cmp() {
	return true;
}

string CmpTerm::to_string() {
	if (equal)
		return "(" + p1.to_string() + " == " + p2.to_string() + ")";
	else
		return "(" + p1.to_string() + " != " + p2.to_string() + ")";
}

bool ConstTerm::is_const() {
	return true;
}

string ConstTerm::to_string() {
	if (value)
		return "TRUE";
	else
		return "FALSE";
}

bool RelationTerm::is_relation() {
	return true;
}

string RelationTerm::to_string() {
	char str[10];
	if (negated)
		sprintf(str, "!R%d(", rel_id);
	else
		sprintf(str, "R%d(", rel_id);
	string res = str;
	for (int i = 0; i < (int) paras.size(); i++) {
		res += paras[i].to_string();
		if (i == (int) paras.size() - 1)
			res += ")";
		else
			res += ", ";
	}
	return res;
}

string Internal::to_string() {
	if (paras.size() == 1)
		return op + paras[0]->to_string();
	else
		return "(" + paras[0]->to_string() + " " + op + " "
				+ paras[1]->to_string() + ")";
}

Internal::Internal() {
}

Internal::Internal(string o) {
	op = o;
	paras.push_back(NULL);
	if (o != "!")
		paras.push_back(NULL); // two NULLs for && and ||
}

bool Internal::is_internal() {
	return true;
}

void pushdown_neg(Formula*& form, bool negated) {
	if (form->is_cmp()) {
		if (negated)
			((CmpTerm*) form)->equal = !((CmpTerm*) form)->equal;
	} else if (form->is_const()) {
		if (negated)
			((ConstTerm*) form)->value = !((ConstTerm*) form)->value;
	} else if (form->is_relation()) {
		if (negated)
			((RelationTerm*) form)->negated = !((RelationTerm*) form)->negated;
	} else {
		Internal* node = (Internal*) form;

		if (node->op == "!") {
			pushdown_neg(node->paras[0], !negated);
			// delete node;
			form = node->paras[0]; // TODO remember to delete
		} else {
			if (node->op == "->") {
				Formula* left = node->paras[0];
				Internal* new_left = new Internal();
				node->op = "||";
				new_left->op = "!";
				new_left->paras.push_back(left);
				node->paras[0] = new_left;
			}
			if (negated) {
				if (node->op == "&&") node->op = "||";
				else node->op = "&&";
			}

			pushdown_neg(node->paras[0], negated);
			pushdown_neg(node->paras[1], negated);
		}
	}
}

Formula* CmpTerm::copy() {
    CmpTerm* res = new CmpTerm();
    res->equal = equal;
    res->p1 = p1;
    res->p2 = p2;
    return res;
}


Formula* RelationTerm::copy() {
    RelationTerm* res = new RelationTerm();
    res->rel_id = rel_id;
    res->negated = negated;
    res->paras = paras;

    return res;
}

Formula* ConstTerm::copy() {
    return new ConstTerm(value);
}

Formula* Internal::copy() {
    Internal* res = new Internal(op);
    res->paras = paras;
    for (int i = 0; i < (int) paras.size(); i++)
        res->paras[i] = paras[i]->copy();
    return res;
}

string rand_op() {
	if (rand() % 5 == 0)
		return string("||");
	else
		return string("&&");
}

Formula* generate_formula(int size, Task& task, Artifact& art) {
	// get type info
	DBSchema& db = art.db;
	int num_rels = db.num_rels;
	vector<vector<int> > var_by_type(num_rels, vector<int>());
	vector<int> num_vars;
	vector<int> str_vars;
	for (int i = 0; i < task.num_var; i++) {
		if (task.var_types[i] == -1)
			num_vars.push_back(task.vars[i]);
		else if (task.var_types[i] == -2)
			str_vars.push_back(task.vars[i]);
		else
			var_by_type[task.var_types[i]].push_back(task.vars[i]);
	}

	// generate root
	vector<Internal*> leafs;
	Internal* root = new Internal(rand_op());
	leafs.push_back(root);

	// generate internal nodes
	for (int i = 0; i < size; i++) {
		int idx = rand() % leafs.size();
		Internal* node = leafs[idx];

		swap(leafs[idx], leafs.back());
		leafs.pop_back();

		node->paras[0] = new Internal(rand_op());
		node->paras[1] = new Internal(rand_op());

		leafs.push_back((Internal*) node->paras[0]);
		leafs.push_back((Internal*) node->paras[1]);
	}

	// generate terminal nodes
	for (Internal* node : leafs) {
		for (int i = 0; i < (int) node->paras.size(); i++) {
			int choice = rand() % 3;

			switch (choice) {
			case 0: { // x == y
				CmpTerm* term = new CmpTerm();
				term->equal = rand() % 2;
				term->p1.is_const = false;
				term->p2.is_const = false;

				int idx1 = rand() % task.num_var;
				int type = task.var_types[idx1];
				int v2 = get_rand_var_type(type, task);

				term->p1.id = task.vars[idx1];
				term->p2.id = v2;
				term->p1.type = term->p2.type = type;

				node->paras[i] = term;
				break;
			}
			case 1: { // x == c
				CmpTerm* term = new CmpTerm();
				term->equal = rand() % 2;
				term->p1.is_const = false;
				term->p2.is_const = true;

				int type = rand() % 2 - 2;
				term->p1.id = get_rand_var_type(type, task);
				term->p2.id = get_rand_const_type(type, art);

				term->p1.type = term->p2.type = type;

				node->paras[i] = term;
				break;
			}
			case 2: { // R(x, ...)
				int rel_id = rand() % num_rels;

				RelationTerm* term = new RelationTerm();
				term->rel_id = rel_id;
				for (int j = 0; j < db.relations[rel_id].arity; j++) {
					Parameter pa;
					int type = db.relations[rel_id].types[j];
					if (type >= 0) {
						pa.is_const = false;
						pa.id = get_rand_var_type(type, task);
					} else {
						pa.is_const = rand() % 2;
						if (pa.is_const)
							pa.id = get_rand_const_type(type, art);
						else
							pa.id = get_rand_var_type(type, task);
					}
					pa.type = type;
					term->paras.push_back(pa);
				}
				node->paras[i] = term;
				break;
			}
			}

		}
	}

	return root;
}

Formula* generate_safety(int task_id, Artifact& art) {
    CmpTerm* term = new CmpTerm();
    term->equal = true;
    Parameter p1;
    Parameter p2;

    Task& task = art.tasks[task_id];
    vector<int> str_vars;
    for (int i = 0; i < task.num_var; i++)
        if (task.var_types[i] == -2)
            str_vars.push_back(i);

    int num_str_consts = art.str_consts.size();

    p1.type = p2.type = -2;
    p2.is_const = true;
    p1.id = str_vars[rand() % str_vars.size()];
    p2.id = rand() % num_str_consts;
    term->p1 = p1;
    term->p2 = p2;
    return term;
}

} /* namespace std */
