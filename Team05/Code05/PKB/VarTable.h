//@author Yohanes Lim
//VarTable.h

#ifndef VARTABLE_H
#define VARTABLE_H

#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <set>
#include <map>

using namespace std;
typedef int VARINDEX;
typedef string VARNAME; 

/*! \brief A VarTable class to store the variable names from the source code.
 *  
 *
 * Overview: VarTable is responsible to :
 * - Store all the variable names from the source code
 * - Assign each variable name an unique variable index
 * - Allow speedy access to the required datas
 * 
 * VarTable is a singleton class, it can be invoked using:
 * \code
 * static VarTable* getInstance();
 * \endcode
 *
 */

class VarTable {
private: 
	vector<VARNAME> variableTable;
	map<VARNAME,VARINDEX> variableMapTable;
	set<VARINDEX> varIndexList;
	static bool instanceFlag;
	static VarTable *varTable;
	int size;
public: 
	//! A constructor to initialize the VarTable class.
	VarTable();
	//! A destructor to clear all the tables and set the instance flag of the singleton class to false.
	~VarTable();
	//! Returns the instance of VarTable singleton class.
	static VarTable* getInstance();	// to be used to get instance of singleton class 
	//! Insert a variable name to the constant table
	void insertVar(VARNAME);
	//! Return the variable name given the variable index. If it is not found, return an empty string.
	VARNAME getVarName (VARINDEX);
	//! Return the variable index given the variable name. If it is not found, return an -1.
	VARINDEX getVarIndex (VARNAME);

	//! Return the total number of variables in the variable tables.
	int getNumVar(); 
	//! Return a set of all variable indexes in the variable tables.
	set<VARINDEX> getAllVarIndex();
	
};

#endif