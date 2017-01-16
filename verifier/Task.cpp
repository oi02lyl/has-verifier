/*
 * Task.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: lyl
 */

#include "Task.h"
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <set>

namespace std {

Task::Task() {
    name = "";
	num_var = 0;
	num_sets = -1;
	open_cond = NULL;
	close_cond = NULL;
}

Task::~Task() { }


void Task::add_set_vars() {
	if (num_sets >= 0)
		return;

	// compute variable types for each set
	num_sets = 0;
	for (Service& ser : services)
		for (SetUpdate& su : ser.set_update) {
			if (su.type != InsertBefore && su.type != InsertAfter && su.type == Retrieve)
				continue;
			if (su.setid + 1 > num_sets)
				num_sets = su.setid + 1;
		}

	set_var_types = vector<vector<int> >(num_sets, vector<int>());
	set_var_ids = vector<vector<int> >(num_sets, vector<int>());

	for (Service& ser : services) {
		for (SetUpdate& su : ser.set_update) {
			if (su.type != InsertBefore && su.type != InsertAfter && su.type == Retrieve)
				continue;
			// check inconsistency
			if (set_var_types[su.setid].size() > 0) {
				if (set_var_types[su.setid].size() != su.vars.size()) {
					printf("Set %d of task %s variables length inconsistent!\n", su.setid, name.c_str());
					exit(0);
				}
				for (int j = 0; j < (int) su.vars.size(); j++) {
					int vid = su.vars[j];
					if (set_var_types[su.setid][j] != var_types[vid]) {
						printf("Set %d of task %s variables type inconsistent!\n", su.setid, name.c_str());
						exit(0);
					}
				}
			} else {
				set_var_types[su.setid] = vector<int>(su.vars.size(), 0);
				set_var_ids[su.setid] = vector<int>(su.vars.size(), 0);

				for (int j = 0; j < (int) su.vars.size(); j++) {
					int vid = su.vars[j];
					set_var_types[su.setid][j] = var_types[vid];

					int svid = num_var++;
					vars.push_back(svid);
					var_types.push_back(var_types[vid]);
					set_var_ids[su.setid][j] = svid;
				}
			}
		}
	}
}


} /* namespace std */
