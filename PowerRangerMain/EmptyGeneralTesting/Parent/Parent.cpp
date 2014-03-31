#pragma once

#include "Parent.h"

// constructor
Parent::Parent() {
	vector<STMTNUM> temp;
	temp.assign(1,-1);
	parentTable.assign(5, temp);
	childrenTable.assign(5, -1);
}

Parent::~Parent() {
	// vector<STMTNUM> temp;
	// temp.assign(1,-1);
	// vector<STMTNUM> newTable;
	// newTable.assign(5, temp);
	// newTable.swap(parentTable);
	parentTable.clear();
	vector<STMTNUM>().swap(childrenTable);
}

void Parent::setParent(STMTNUM s1, STMTNUM s2) {
	vector<STMTNUM> temp;
	temp.assign(1,-1);
	if (s1 >= (signed int) parentTable.size()) {
		parentTable.resize(s1+1, temp);
	}

	vector<STMTNUM> temp1 = parentTable[s1];
	temp1.push_back(s2);
	parentTable[s1] = temp1;

	if (s2 >= (signed int)childrenTable.size()) {
		childrenTable.resize(s2+1, -1);
	}
	childrenTable[s2] = s1; 
}

// TODO: Need to add exception handling
BOOLEAN Parent::isParent(STMTNUM s1, STMTNUM s2) {
	if (childrenTable[s2] == s1) {
		return true;
	}
	return false;
}

// TODO: Need to add exception handling
BOOLEAN Parent::isParent(Query::SynType t, STMTNUM s) {
	STMTNUM temp = childrenTable[s];
	if (table.getType(temp) == t) {
		return true;
	}
	return false;
}

BOOLEAN Parent::isChildren(Query::SynType t, STMTNUM s){
	LIST temp = parentTable[s];
	LIST::iterator it = temp.begin();
	for(;it!=temp.end();++it){
		if(table.getType(*it)==t)
			return true;
	}
	return false;
}

/*vector<STMTNUM> Parent::getParent(Query::SynType t1, Query::SynType t2, STMTNUM s) {
	STMTNUM temp = childrenTable[s];
	if(table.getType(temp) == t2){
		return table.getStmtNum(t1);
	}
	return vector<STMTNUM> (1,-1);
}

vector<STMTNUM> Parent::getChildren(Query::SynType t1, Query::SynType t2, STMTNUM s) {
	vector<STMTNUM> temp = parentTable[s];
	vector<STMTNUM>::iterator it = temp.begin();
	for(;it!=temp.end();++it){
		if(table.getType(*it) == t2){
			return table.getStmtNum(t1);
		}
	}
	return vector<STMTNUM> (1,-1);
}*/

vector<STMTNUM> Parent::getParent(Query::SynType t1, Query::SynType t2){
	vector<STMTNUM> v;
	return v;
}

vector<STMTNUM> Parent::getChildren(Query::SynType t1, Query::SynType t2){
	vector<STMTNUM> v;
	return v;
}