//@author Ipsita Mohapatra
//DesignExtractor.h

#pragma once
#ifndef DESIGNEXTRACTOR_H
#define DESIGNEXTRACTOR_H

#pragma once
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stack>
#include <queue>
#include <vector>
#include "Node.h"
#include "PKB.h"
#include "QueueItem.h"


void extractorDriver(PKB* pkb);
void getTablesFromPKB(PKB* pkb); 
void extractRelationships(Node &ASTRoot, unordered_map<PROCINDEX, vector<CALLSPAIR>> callsTable); 
queue<QueueItem> runDFSDriver(unordered_map<PROCINDEX, vector<CALLSPAIR>> callsTable);
void DFS(int source, vector<int> progLine, unordered_map<PROCINDEX, vector<CALLSPAIR>> callsTable);
Node* buildCFG(Node &ASTroot);
	
#endif