#pragma once

#include "Next.h"

bool Next::instanceFlag=false;
Next* Next::next=NULL;

//Constructor

Next::~Next(){
	nextTable.clear();
	instanceFlag=false;
}


Next* Next::getInstance(TypeTable* tt) {
	if(!instanceFlag)
    {
        next = new Next(tt);
        instanceFlag = true;
        return next;
    }
    else
    {
        return next;
    }
}


Next::Next(TypeTable* tt){
	typeTable = tt;
}


void Next::setNext(STMTNUM s1, STMTNUM s2){
	try{
		vector<STMTNUM> temp (1,s2);

		try{
			vector<STMTNUM> temp1 = nextTable.at(s1);
			temp1.push_back(s2);
			nextTable[s1] = temp1;
		} catch(...){
			nextTable[s1] = temp;
		}
		previousTable[s2] = s1; 
	}catch(...){
	}
}

vector<STMTNUM> Next::getNext(STMTNUM s){
	vector<STMTNUM> ans;
	try{
		ans = nextTable.at(s);
		return ans;
	} catch(...) {
		return ans;
	}
}

STMTNUM Next::getPrevious(STMTNUM s){
	try{
		STMTNUM ans = previousTable.at(s);
		return ans;
	} catch(...) {
		return -1;
	}
}

bool Next::isNext(STMTNUM s, STMTNUM s1){
	return true;
}

void Next::printNextTable() {
	cout<< "Next Table" << endl;
	for(unordered_map<STMTNUM, vector<STMTNUM>>::iterator it = nextTable.begin(); it != nextTable.end(); it++) {
		cout<< it->first << " has next consisting of ";
		vector<STMTNUM> temp = it->second; 
		vector<STMTNUM>::iterator iter;
		for (iter = temp.begin(); iter!=temp.end(); iter++) {
			cout<< *iter<< ",";
		}		
		cout<<endl;
	}
}

bool Next::isNext(STMTNUM s1, STMTNUM s2){
	try{
		vector<STMTNUM> temp = nextTable.at(s1);
		vector<STMTNUM>::iterator iter;
		for (iter = temp.begin(); iter!=temp.end(); iter++) {
			if(*iter==s2)
				return true;
		}		
	} catch(...){
		return false;
	}
	return false;
}