/*
 * Artifact.h
 *
 *  Created on: Feb 2, 2016
 *      Author: lyl
 */

#ifndef ARTIFACT_H_
#define ARTIFACT_H_

#include "DBSchema.h"
#include "Task.h"
#include "Formula.h"

#include <vector>
#include <string>

namespace std {

class DBSchema;
class Task;
class Formula;

class Artifact {
public:
	// db schema
	DBSchema db;

	// values of constants
	vector<double> num_consts;
    vector<string> str_consts;

	// the list of services
	vector<Task> tasks;

	// Global pre-condition
	Formula* global_pre_cond;

	void dump();

	Artifact();
	virtual ~Artifact();
};

//// generate a random task
//void generate_task(int num_tasks, Task& task);
//
//// generate a random condition
//void generate_condition(int num_literals, vector<int>& var_types, DBSchema& schema, Condition& res);
//
//void add_state_to_condition(int state, int var_id, Condition& cond);
//


// get a random variable by type
int get_rand_var_type(int type, Task& task);

// get a random constant by type
int get_rand_const_type(int type, Artifact& art);


// generate a random artifact
void generate_artifact(int num_tasks, int num_const, int num_vars_per_task, int num_services_per_task, int form_size, DBSchema& d, bool without_set, Artifact& res);

} /* namespace std */

#endif /* ARTIFACT_H_ */
