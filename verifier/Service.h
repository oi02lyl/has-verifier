/*
 * Service.h
 *
 *  Created on: Feb 2, 2016
 *      Author: lyl
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include "Formula.h"
#include "Service.h"
#include "Task.h"
#include "DBSchema.h"
#include <vector>
#include <string>

namespace std {

struct SetUpdate;
class Formula;

class Service {
public:
    // pre- and post-conditions	
    Formula* pre_cond;
	Formula* post_cond;

    // variables propagated
	vector<int> var_prop;

    // service name
	string name;

    // set update rules
    vector<SetUpdate> set_update;

	Service();
	virtual ~Service();
};

enum SetUpdateType {Write, Prop, Retrieve, InsertBefore, InsertAfter};

struct SetUpdate {
    vector<int> vars;
    SetUpdateType type;
    int setid;

    SetUpdate(): type(Write), setid(-1) { }
};

// randomly generate a service;
// void generate_service(Task& task, DBSchema& db, Service& res);

} /* namespace std */



#endif /* SERVICE_H_ */
