/*
 * DBSchema.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: lyl
 */

#include "DBSchema.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

namespace std {

DBSchema::DBSchema() {
	// TODO Auto-generated constructor stub
	num_rels = 0;
}

DBSchema::~DBSchema() {
	// TODO Auto-generated destructor stub
}

void DBSchema::dump() {
	int i = 0;
	for (Relation& rel : relations) {
		printf("R%d(i %s", i++, rel.attr_names[0].c_str());

		for (int j = 1; j < rel.arity; j++) {
			if (rel.types[j] == -1)
				printf(", NUM %s", rel.attr_names[j].c_str());
			else if (rel.types[j] == -2)
				printf(", STR %s", rel.attr_names[j].c_str());
			else
				printf(", R%d %s", rel.types[j], rel.attr_names[j].c_str());
		}
		printf(")\n");
	}
	printf("\n");
}

Relation::Relation() {
	arity = 0;
}

Relation::~Relation() { }

void generate_dbschema(int size, int num_const_attr, DBSchema& res) {
	res.num_rels = size;
	res.relations.clear();

	vector<vector<int> > children(size, vector<int>());

	for (int i = 1; i < size; i++) {
		int parent = rand() % i;
		children[parent].push_back(i);
	}

	char tmp_name[30];
	for (int i = 0; i < size; i++) {
		Relation rel;

		sprintf(tmp_name, "R%d", i);
		rel.name = tmp_name;

		int num_child = children[i].size();
		rel.arity = 1 + num_const_attr + num_child;
		for (int j = 0; j < rel.arity; j++) {
			sprintf(tmp_name, "A%d", j);
			rel.attr_names.push_back(tmp_name);
			if (j == 0)
				rel.types.push_back(i);
			else if (j <= num_const_attr) {
				if (j % 2)
					rel.types.push_back(-1);
				else
					rel.types.push_back(-2);
            } else
				rel.types.push_back(children[i][j - num_const_attr - 1]);
		}

		res.relations.push_back(rel);
	}
}


} /* namespace std */
