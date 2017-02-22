/*
 * Artifact.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: lyl
 */

#include "Artifact.h"
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <set>

namespace std {

Artifact::Artifact() {
	global_pre_cond = NULL;
}

Artifact::~Artifact() {
	// TODO Auto-generated destructor stub
}

void Artifact::dump() {
	// print relations
	printf("Relations:\n\n");
	db.dump();
	// int num_rels = db.num_relations;

	// print global pre-cond
	printf("Global Pre-Condition:\n%s\n\n",
			global_pre_cond->to_string().c_str());

	// print tasks
	printf("Tasks:\n\n");
	for (Task& t : tasks) {
		int num_vars = t.vars.size();

		printf("%s(", t.name.c_str());

		// print input paras
		vector<bool> is_input(num_vars, false);
		for (int j = 0; j < (int) t.input_vars.size(); j++) {
			is_input[t.input_vars[j].second] = true;
			printf("X%d as X%d", t.input_vars[j].first, t.input_vars[j].second);
			if (j != (int) t.input_vars.size() - 1)
				printf(", ");
		}
		printf("):");

		// print local variables
		printf("\nVariables: ");
		for (int j = 0; j < (int) t.vars.size(); j++) {
			if (is_input[j]) continue;

			if (t.var_types[j] == -1)
				printf("NUM X%d", j);
			else if (t.var_types[j] == -2)
				printf("STR X%d", j);
			else
				printf("R%d X%d", t.var_types[j], j);
			if (j != num_vars - 1)
				printf(", ");
		}

		// open & closing conditions
		printf("\nOpening-Condition: %s\nClosing-Condition: %s\n",
				t.open_cond->to_string().c_str(),
				t.close_cond->to_string().c_str());

		// print services
		printf("\nServices:\n");
		for (int j = 0; j < (int) t.services.size(); j++) {
			Service& ser = t.services[j];
			printf("\n%s:\n", ser.name.c_str());
			printf("Pre-Condition: %s\n", ser.pre_cond->to_string().c_str());
			printf("Post-Condition: %s\n", ser.post_cond->to_string().c_str());
			printf("Variables: ");
			for (int k = 0; k < (int) ser.var_prop.size(); k++) {
				printf("X%d", ser.var_prop[k]);
				if (k != (int) ser.var_prop.size() - 1)
					printf(", ");
			}
			printf("\nUpdate-Rules: ");
			for (int k = 0; k < (int) ser.set_update.size(); k++) {
				if (ser.set_update[k].type == InsertBefore)
					printf("++S(");
				else if (ser.set_update[k].type == InsertAfter)
					printf("S++(");
				else if (ser.set_update[k].type == Retrieve)
					printf("--S(");

				for (int l = 0; l < (int) ser.set_update[k].vars.size(); l++) {
					printf("X%d", ser.set_update[k].vars[l]);
					if (l != (int) ser.set_update[k].vars.size() - 1)
						printf(", ");
				}
				printf(")");

				if (k != (int) ser.set_update.size() - 1)
					printf(", ");
			}
			printf("\n");
		}

		// print returns
		printf("\nReturn: ");
		for (int j = 0; j < (int) t.return_vars.size(); j++) {
			printf("X%d as X%d", t.return_vars[j].first,
					t.return_vars[j].second);
			if (j != (int) t.return_vars.size() - 1)
				printf(", ");
		}

		// print child tasks
		printf("\nChildren: ");
		for (int j = 0; j < (int) t.children.size(); j++) {
			printf("%s", tasks[t.children[j]].name.c_str());
			if (j != (int) t.children.size() - 1)
				printf(", ");
		}
		printf("\n\n\n");
	}
}


int get_rand_var_type(int type, Task& task) {
    vector<int> ids;
    for (int i = 0; i < (int) task.var_types.size(); i++)
        if (task.var_types[i] == type)
            ids.push_back(i);
    return task.vars[ids[rand() % ids.size()]];
}


int get_rand_const_type(int type, Artifact& art) {
	if (type == -1)
		return rand() % art.num_consts.size();
	return rand() % art.str_consts.size();
}

void generate_artifact(int num_tasks, int num_const, int num_vars_per_task,
		               int num_services_per_task, int form_size, DBSchema& db, bool without_set,
                       Artifact& res) {
	res.db = db;
	int num_rels = db.num_rels;

	// generate consts
	for (int i = 0; i < num_const / 2; i++) {
		res.num_consts.push_back(i);
		char str[10];
		sprintf(str, "S%d", i);
		res.str_consts.push_back(str);
	}

	// generate the task hierarchy
	res.tasks = vector<Task>(num_tasks, Task());
	for (int task_id = 0; task_id < num_tasks; task_id++) {
		Task& task = res.tasks[task_id];

		// find a random parent
		int parent_id = -1;
		if (task_id > 0) {
			parent_id = rand() % task_id;
			res.tasks[parent_id].children.push_back(task_id);
		}

		// task name
		char name[15];
		sprintf(name, "Task%d", task_id);
		task.name = name;

		// variables & types
		for (int i = 0; i < num_vars_per_task; i++) {
			task.vars.push_back(i);
			task.var_types.push_back(i % (num_rels + 2) - 2);
		}
		task.num_var = num_vars_per_task;

		// input and return mapping
		vector<int> vars_copy = task.vars;
		if (parent_id >= 0) {
            Task& parent_task = res.tasks[parent_id];
            vector<bool> used = vector<bool>(parent_task.num_var, false);

			// input
			for (int i = 0; i < num_vars_per_task / 10; i++) {
				int idx = i + rand() % (num_vars_per_task - i);
				pair<int, int> mp;
				int vid = vars_copy[idx];

                for (int j = 0; j < (int) used.size(); j++)
                    if (parent_task.var_types[j] == task.var_types[vid] && !used[j]) {
                        mp.first = j;
                        used[j] = true;
                        break;
                    }
				mp.second = vid;
				task.input_vars.push_back(mp);
				swap(vars_copy[i], vars_copy[idx]);
			}

			// return
			for (int i = num_vars_per_task / 10; i < num_vars_per_task / 5; i++) {
				int idx = i + rand() % (num_vars_per_task - i);
				pair<int, int> mp;
				int vid = vars_copy[idx];
				mp.first = vid;
                for (int j = 0; j < (int) used.size(); j++)
                    if (parent_task.var_types[j] == task.var_types[vid] && !used[j]) {
                        mp.second = j;
                        used[j] = true;
                        break;
                    }
				task.return_vars.push_back(mp);
				swap(vars_copy[i], vars_copy[idx]);
			}
		}

		// open and closing conditions
		if (parent_id >= 0) {
			task.open_cond = generate_formula(form_size, res.tasks[parent_id], res);
			task.close_cond = generate_formula(form_size, task, res);
		} else {
			task.open_cond = new ConstTerm(true);
			task.close_cond = new ConstTerm(false);
		}


		// Services
		task.services = vector<Service>(num_services_per_task, Service());

		// generate set variables (avoid input variables)
		vector<int> input_vars;
		vector<int> set_vars;
		for (int i = 0; i < num_vars_per_task / 10; i++)
			input_vars.push_back(vars_copy[i]);

		for (int i = num_vars_per_task / 10; i < num_vars_per_task / 5; i++) {
			int idx = i + rand() % (num_vars_per_task - i);
			set_vars.push_back(vars_copy[idx]);
			swap(vars_copy[i], vars_copy[idx]);
		}

		int service_id = 0;
		for (Service& ser : task.services) {
			ser.pre_cond = generate_formula(form_size, task, res);
			ser.post_cond = generate_formula(form_size, task, res);
			char name[20];
			sprintf(name, "Se%d", service_id++);
			ser.name = name;

			int rd = service_id % 3;
			if (rd == 0) {
				// no set ops
				ser.var_prop = input_vars;
				for (int i = num_vars_per_task / 10; i < num_vars_per_task / 5; i++) {
					int idx = i + rand() % (num_vars_per_task - i);
					ser.var_prop.push_back(vars_copy[idx]);
					swap(vars_copy[i], vars_copy[idx]);
				}
			} else if (rd == 1) {
				// insert
				ser.var_prop = input_vars;
				SetUpdate su;
				su.setid = 0;
                su.type = InsertBefore;
				su.vars = set_vars;
                if (!without_set)
    				ser.set_update.push_back(su);
			} else {
				// retrieve
				ser.var_prop = input_vars;
				SetUpdate su;
				su.setid = 0;
				su.type = Retrieve;
				su.vars = set_vars;
                if (!without_set)
    				ser.set_update.push_back(su);
			}
		}
	}

	// generate the global pre-condition
	res.global_pre_cond = new ConstTerm(true);
//			generate_formula(form_size, res.tasks[0], res);
}

//void generate_condition(int num_literals, vector<int>& var_types, DBSchema& schema,
//		Condition& res) {
//	int num_var = var_types.size() - 1;
//	int num_rel = schema.num_relations;
//	vector<Relation>& rels = schema.relations;
//
//	// numeric variables
//	vector<int> var_numeric;
//
//	// id variables
//	vector<vector<int> > var_id(num_rel, vector<int>());
//
//	// group variables by types
//	for (int i = 0; i < num_var; i++) {
//		if (var_types[i] < 0)
//			var_numeric.push_back(i);
//		else
//			var_id[var_types[i]].push_back(i);
//	}
//
//	// generate equalities
//	int num_eq_literals = num_literals / 2;
//	while (num_eq_literals--) {
//		Literal lt;
//		lt.atype = EQUAL;
//		int i = -1, j = -1;
//
//		int rd = rand() % (num_rel + 1);
//		if (rd == 0) {
//			// numeric
//			if (var_numeric.empty())
//				continue;
//			i = var_numeric[rand() % var_numeric.size()];
//			j = var_numeric[rand() % var_numeric.size()];
//		} else {
//			// id
//			rd--;
//			if (var_id[rd].empty())
//				continue;
//			i = var_id[rd][rand() % var_id[rd].size()];
//			j = var_id[rd][rand() % var_id[rd].size()];
//		}
//		if (i == j) continue;
//
//		lt.sign = ((i % 2) == (j % 2));
//		lt.vars.push_back(i);
//		lt.vars.push_back(j);
//		res.literals.push_back(lt);
//	}
//
//	// generate relations
//	int num_rel_literals = num_literals / 2;
//	while (num_rel_literals--) {
//		bool sign = true;// rand() % 2
//		int rel_id = rand() % num_rel;
//		Relation& R = rels[rel_id];
//		int arity = R.arity;
//		vector<int> vars;
//
//		// pick var for id att
//		vars.push_back(var_id[rel_id][rand() % var_id[rel_id].size()]);
//		for (int i = 1; i < arity; i++)
//			if (R.refs[i] < 0)
//				vars.push_back(var_numeric[rand() % var_numeric.size()]);
//			else
//				vars.push_back(
//						var_id[R.refs[i]][rand() % var_id[R.refs[i]].size()]);
//		Literal lt;
//		lt.atype = RELATION;
//		lt.sign = sign;
//		lt.vars = vars;
//		lt.oprand_id = rel_id;
//		res.literals.push_back(lt);
//	}
//}
//
//void add_state_to_condition(int state, int var_id, Condition& cond) {
//	Literal lt;
//	lt.atype = EQUAL_CONST;
//	lt.oprand_id = state;
//	lt.sign = true;
//	lt.vars.push_back(var_id); // the last variable is for state
//	cond.literals.push_back(lt);
//}
//
//// generate random service for each transition
//void generate_service(int state_u, int state_v, int num_literals, vector<int>& var_types,
//		DBSchema& schema, Service& res) {
//    /*
//	res.name = "";
//
//	int num_var = var_types.size() - 1;
//
//	// each variable is propagated wp. 1/2
//	for (int i = 0; i < num_var; i++)
//		if (rand() % 2)
//			res.var_prop.push_back(i);
//
//	// pre-condition
//	generate_condition(num_literals, var_types, schema, res.pre_cond);
//	add_state_to_condition(state_u, num_var, res.pre_cond);
//
//	// post-condition
//	add_state_to_condition(state_v, num_var, res.post_cond);
//
//    // set update
//    int su = rand() % 3;
//    res.set_update = static_cast<SetUpdate>(su);
//    */
//}
//
//void generate_artifact(int num_var_per_type, int num_literals, int num_states,
//		DBSchema& d, Artifact& res) {
//	// srand(time(NULL));
//
//    /*
//
//	int num_rel = d.num_relations;
//
//	res.db = d;
//	res.num_const = num_states;
//	res.num_var = (num_rel + 1) * num_var_per_type + 1;
//
//	// set variable names and types
//	for (int i = 0; i < res.num_var; i++) {
//		char name[10];
//		sprintf(name, "X%d", i);
//		res.var_names.push_back(name);
//	}
//	for (int i = -1; i < num_rel; i++)
//		for (int j = 0; j < num_var_per_type; j++)
//			res.var_types.push_back(i);
//	res.var_types.push_back(-1);
//
//	// add constants for states
//	for (int i = 0; i < num_states; i++)
//		res.constants.push_back(i);
//
//	// generate graphs
//	vector<vector<int> > graph(num_states, vector<int>());
//
//	// make sure the graph is connected
//	for (int i = 0; i < num_states - 1; i++)
//		graph[i].push_back(i + 1);
//
//	// add two random edges per node
//	for (int i = 0; i < num_states - 1; i++) {
//		graph[i].push_back(rand() % num_states);
//		graph[i].push_back(rand() % num_states);
//	}
//
//	// global pre-condition
//	generate_condition(num_literals, res.var_types, d, res.global_pre_cond);
//	add_state_to_condition(0, res.num_var - 1, res.global_pre_cond);
//
//	// add services
//	int service_counter = 0;
//	for (int state_u = 0; state_u < num_states; state_u++)
//		for (int state_v : graph[state_u]) {
//			Service ser;
//			generate_service(state_u, state_v, num_literals, res.var_types, d, ser);
//			char name[10];
//			sprintf(name, "S%d", service_counter++);
//			ser.name = name;
//			res.services.push_back(ser);
//		}
//
//    // choose set update variables
//    int num_set_vars = res.num_var / 10;
//    set<int> chosen;
//    for (int i = 0; i < num_set_vars; i++) {
//        while (true) {
//            int var = rand() % res.num_var;
//            if (chosen.count(var) == 0) {
//                chosen.insert(var);
//                break;
//            }
//        }
//    }
//
//    res.set_vars = vector<int>(chosen.begin(), chosen.end());
//    */
//}

} /* namespace std */
