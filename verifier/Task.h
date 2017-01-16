/*
 * Task.h
 *
 *  Created on: Feb 2, 2016
 *      Author: lyl
 */

#ifndef TASK_H_
#define TASK_H_

#include "DBSchema.h"
#include "Service.h"
#include "Formula.h"

#include <vector>
#include <string>

namespace std {

class Formula;
class Service;

class Task {
public:
    // name of the task
    string name;

	// number of variables
	int num_var;

    // the list of variables
    vector<int> vars;

    // types of each variable
	// -1 for numeric, -2 for string, var_type[i] = j means X[i] has the type of R[j]
	vector<int> var_types;

	// number of artifact relations
	int num_sets;

	// types of each set variables
	vector<vector<int> > set_var_types;

	// variable ids of each set
	vector<vector<int> > set_var_ids;

	// the list of services
	vector<Service> services;

	// opening-condition
	Formula* open_cond;

    // closing-condition
    Formula* close_cond;

    // set variables 
    // vector<int> set_vars;
    
    // list of input variables
    vector<pair<int, int> > input_vars;

    // list of return variables
    vector<pair<int, int> > return_vars;

    // the list of children
    vector<int> children;

    // add extra variables to support set ops in verification
    void add_set_vars();

	Task();
	virtual ~Task();
};


} /* namespace std */

#endif /* TASK_H_ */
