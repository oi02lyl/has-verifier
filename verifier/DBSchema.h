/*
 * DBSchema.h
 *
 *  Created on: Feb 2, 2016
 *      Author: lyl
 */

#ifndef DBSCHEMA_H_
#define DBSCHEMA_H_

#include <vector>
#include <string>

namespace std {

struct Relation;

class DBSchema {
public:
	int num_rels; // number of relations
	vector<Relation> relations;

	DBSchema();
	virtual ~DBSchema();

	void dump();
};


struct Relation {
	string name;
	int arity;
	vector<string> attr_names;
	vector<int> types; // refs[i] = -1 for numeric type, -2 for str type

	Relation();
	virtual ~Relation();
};

void generate_dbschema(int size, int num_const_attr, DBSchema& res);


} /* namespace std */

#endif /* DBSCHEMA_H_ */
