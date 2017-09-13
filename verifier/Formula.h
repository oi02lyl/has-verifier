/*
 * Condition.h
 *
 *  Created on: Feb 2, 2016
 *      Author: lyl
 */

#ifndef FORMULA_H_
#define FORMULA_H_

#include <vector>
#include <string>
#include "Artifact.h"

namespace std {

class Task;
class Artifact;

// enum LOp {AND = 0, OR, IMPLY, NEG};
// enum EOp {EQ = 0, UNEQ, LEQ, LE, GEQ, GE};
// enum NOp {PLUS = 0, MINUS, TIMES};

// parameter, constant or variable
struct Parameter {
    bool is_wildcard;
	bool is_const;
    bool is_null;
	int type;
	int id;

    Parameter(): is_wildcard(false), is_const(false), is_null(false), type(0), id(0) { }

	string to_string();
};

// arbitrary formulas
class Formula {
public:
	virtual string to_string() = 0;
	virtual bool is_internal();
	virtual bool is_relation();
	virtual bool is_cmp();
	virtual bool is_const();
    virtual Formula* copy() = 0;

	Formula();
	virtual ~Formula();
};

// Comparison atoms
class CmpTerm: public Formula {
public:
	bool equal;
	Parameter p1;
	Parameter p2;

	bool is_cmp();
    Formula* copy();
	string to_string();

};

// relational atoms
class RelationTerm: public Formula {
public:
	int rel_id;
	bool negated;
	vector<Parameter> paras;

	bool is_relation();
    Formula* copy();
	string to_string();

	RelationTerm() : rel_id(-1), negated(false) { };
};

class ConstTerm: public Formula {
public:
	bool value;

	bool is_const();
    Formula* copy();
	string to_string();

	ConstTerm(bool v) : value(v) { }
};

// Intermediate sub-formulas
class Internal: public Formula {
public:
	string op;
	vector<Formula*> paras;

	bool is_internal();
    Formula* copy();
	string to_string();

	// default constructor
	Internal();

	// default with operator, paras will be NULLs
	Internal(string op);
};

void pushdown_neg(Formula*& form, bool negated);

// generate a formula of a task T
Formula* generate_formula(int size, Task& task, Artifact& art);

// generate a safety property of task T
Formula* generate_safety(int task_id, Artifact& art);

Formula* generate_safety_synthetic(int task_id, Artifact& art);

} /* namespace std */

#endif /* FORMULA_H_ */
