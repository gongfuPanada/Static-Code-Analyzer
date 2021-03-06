#pragma once

#include "ProcTable.h"

bool ProcTable::instanceFlag=false;
ProcTable* ProcTable::procTable=NULL;

// constructor
ProcTable::ProcTable() {
	vector<PROCNAME> temp (1,"");
	procedureTable = temp;
	size=0;
}

ProcTable::~ProcTable(){
	procedureTable.clear();
	procedureMapTable.clear();
	procIndexList.clear();
	size=0;
	instanceFlag=false;
}


ProcTable* ProcTable::getInstance() {
	if(!instanceFlag)
    {
        procTable = new ProcTable();
        instanceFlag = true;
        return procTable;
    }
    else
    {
        return procTable;
    }
}

// If procName is not in the ProcTable, inserts procName into the
// ProcTable. if procName already exists, the table remains unchanged.
void ProcTable::insertProc(PROCNAME procName) {
	PROCINDEX procIndex = getProcIndex(procName);
	bool containsVariable = (procIndex != -1);
	
	if (!containsVariable) {
		procedureMapTable[procName]=procedureTable.size();
		procIndexList.insert(procedureTable.size());
		procedureTable.push_back(procName);
		size++;
	}
}

// Returns the name of a proc at ProTable [ind]
// If �ind� is out of range, error (or throw exception)
PROCNAME ProcTable::getProcName (PROCINDEX i){
	try{
		size_t index = i;
		if(index>=procedureTable.size())
			return "";
		return procedureTable.at(index);
	}catch(...){
		return "";
	}
}

// If procName is in procTable, returns its index; otherwise, returns -1 (special value)
PROCINDEX ProcTable::getProcIndex (PROCNAME procName){
	try{
		return procedureMapTable.at(procName);
	}
	catch(...){
		return -1;
	}
}

int ProcTable::getNumProcedures() {
	return size;
}

set<PROCINDEX> ProcTable::getAllProcIndexes(){
	return procIndexList;
}