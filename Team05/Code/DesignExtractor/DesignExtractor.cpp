//@author Ipsita Mohapatra
//DesignExtractor.cpp

#pragma once

#include "DesignExtractor.h"
using namespace std;

int counter = -1;
Node* rootCFGNode;
Node* currASTNode;
Node* currCFGNode;
// Node* CFGRoot;
// Node* ASTRoot;

//unordered_map<PROCINDEX, vector<CALLSPAIR>> callsTable; 
// VarTable* varTable; 
// ProcTable* procTable; 
queue<QueueItem> queueToProcess;

void extractorDriver(PKB *pkb) {
	TypeTable* typeTable = pkb->getTypeTable();
	ConstTable* constTable = pkb->getConstTable();
	Parent* parent = pkb->getParent();
	Follows* follows = pkb->getFollows();
	Modifies* modifies = pkb->getModifies();
	Uses* uses = pkb->getUses();
	VarTable* varTable = pkb->getVarTable();
	ProcTable* procTable = pkb->getProcTable();

	unordered_map<PROCINDEX, vector<CALLSPAIR>> callsTable; 
	callsTable = pkb->getCallsTable();

	// printCallsTable(callsTable);
	// cout << "DE has obtained tables from PKB" << endl;
	
	Node* ASTRoot = pkb->getASTRoot();
	Node* CFGRoot = pkb->getCFGRoot();
	
	extractRelationships(*ASTRoot, callsTable, *procTable, *modifies, *uses);
	/*
	try {
		// cout << "Building CFG..." << endl;
		// CFGRoot = buildCFG(*ASTRoot);
		extractRelationships(*ASTRoot, callsTable);
	} catch(const std::runtime_error& re) {
		// speciffic handling for runtime_error
		std::cerr << "Runtime error: " << re.what() << std::endl;
	} catch(const std::exception& ex) {
		// speciffic handling for all exceptions extending std::exception, except
		// std::runtime_error which is handled explicitly
		std::cerr << "Error occurred: " << ex.what() << std::endl; 
	} catch(...) {
		// catch any other errors (that we have no information about)
		std::cerr << "Unknown failure occured. Possible memory corruption" << std::endl;
	}
	*/
	// pkb->setCFGRoot(CFGRoot);
}

// extracting of modifies and uses relationship for procedures and statements.
// set the modifies and uses relationships for statements and procedures. 
void extractRelationships(Node &ASTRoot, unordered_map<PROCINDEX, vector<CALLSPAIR>> callsTable, ProcTable &procTable, Modifies &modifies, Uses &uses) {
	// Run DFS on callsTree to generate toposort queue
	runDFSDriver(callsTable); 

	// printQueue();
	// For each of the entries in the queue, dequeue and do the following 
	// For the procedure, find the min and max prog line 
	// Find all the variables modified, then set modifies relationship for proglines and procedures
	// Find all the varaibles used, then set uses relationship for proglines and procedures
	
	while (!queueToProcess.empty()) {
		QueueItem item;
		item = queueToProcess.front();
		queueToProcess.pop();
		
		vector<int> progLines = item.getCallsProgLine();
		int procIndex = item.getProcIndex();
		unsigned int firstProgLine = getFirstProgLine(procIndex, ASTRoot, procTable);
		unsigned int lastProgLine = getLastProgLine(procIndex, ASTRoot, procTable);
		// cout << "Min: " << firstProgLine << ", Max: " << lastProgLine << endl;

		
		for (unsigned int i=firstProgLine; i<=lastProgLine; i++) {
			vector<VARINDEX> variablesModifiedByProgLine = modifies.getModifies(i);
			// SET: procedure procIndex modifies these variables too
			modifies.setModifiesProc(procIndex, variablesModifiedByProgLine);

			vector<VARINDEX> variablesUsedByProgLine = uses.getUses(i);
			// SET: procedure procIndex uses these variables too
			uses.setUsesProc(procIndex, variablesUsedByProgLine); // if there already were variables modifies by this procedure, then just add the 2 vectors
			
			try {
				for (signed int j=(progLines.size()-1); j>=0; j--) {
					int progLine = progLines[j];
					// SET:
					modifies.setModifies(progLine, variablesModifiedByProgLine); 
					// SET:
					uses.setUses(progLine, variablesUsedByProgLine); 
				}
			} catch(const std::runtime_error& re) {
				// specific handling for runtime_error
				std::cerr << "DE: Runtime error: " << re.what() << std::endl;
			} catch(const std::exception& ex) {
				// specific handling for all exceptions extending std::exception, except
				// std::runtime_error which is handled explicitly
				std::cerr << "DE: Error occurred: " << ex.what() << std::endl; 
			} catch(...) {
				// catch any other errors (that we have no information about)
				std::cerr << "DE: Unknown failure occured. Possible memory corruption" << std::endl;
			}	

		}
	}

}

void runDFSDriver(unordered_map<PROCINDEX, vector<CALLSPAIR>> callsTable) {
	DFS(0, vector<int>(), callsTable);
}

void DFS(int source, vector<int> progLine, unordered_map<PROCINDEX, vector<CALLSPAIR>> callsTable) {
	try {
		// cout << callsTable.at(source).empty() << endl;
		for (int i=0; i<(int) callsTable.at(source).size(); i++) {
			vector<int> tempProgLine = progLine; 
			tempProgLine.push_back(callsTable.at(source).at(i).second);
			DFS(callsTable.at(source).at(i).first, tempProgLine, callsTable); 
		}
		// cout << "push to queue: ";
		// QueueItem(source, progLine).print();
		queueToProcess.push(QueueItem(source, progLine));
	} catch(const std::runtime_error& re) {
		// specific handling for runtime_error
		std::cerr << "DE: Runtime error: " << re.what() << std::endl;
	} catch(const std::exception& ex) {
		// specific handling for all exceptions extending std::exception, except
		// std::runtime_error which is handled explicitly
		// cout << "Source: " << source << endl; 
		// std::cerr << "Error occurred: " << ex.what() << std::endl; 
		// cout << "push to queue: ";
		// QueueItem(source, progLine).print();
		queueToProcess.push(QueueItem(source, progLine));
	} catch(...) {
		// catch any other errors (that we have no information about)
		std::cerr << "DE: Unknown failure occured. Possible memory corruption" << std::endl;
	}

}

// actual building of CFG 
Node* buildCFG(Node &ASTNode) {
	currASTNode = &ASTNode; 
	counter++; 

	string ASTNodeType = ASTNode.getType();
	if (ASTNodeType == "program") {
		currCFGNode = new Node("program", counter);
		rootCFGNode = currCFGNode;
	} else if (ASTNodeType == "procedure") {
		cout << "this is a procedure" << endl; 
	} else {
		cout << "yay!" << endl; 
	}

	// go through all children of current AST node 
	for (unsigned i=0;i<ASTNode.getChild().size();i++) {
		currCFGNode = buildCFG(*ASTNode.getChild(i));	
	}
	return currCFGNode;
}

void printCallsTable(unordered_map<PROCINDEX, vector<CALLSPAIR>> callsTable) {
	cout << "Calls Table is:" << endl; 
	for (auto it1 = callsTable.begin(); it1 != callsTable.end(); ++it1) {
		cout << it1->first  << ": "; 
		vector<CALLSPAIR> v = it1->second;
		for (vector<CALLSPAIR>::iterator it2 = v.begin(); it2 != v.end(); ++it2) {
			cout << "(" << it2->first << ", " << it2->second << ") ";
		}
		cout << endl; 
	}

}

void printQueue() {
	queue<QueueItem> backup;
	cout << "Toposort queue is: " << endl;
	while (!queueToProcess.empty()) {
		QueueItem item;
		item = queueToProcess.front();
		queueToProcess.pop();
		item.print();
		backup.push(item);
	}
	queueToProcess = backup; 
}

int getFirstProgLine(int procIndex, Node &ASTRoot, ProcTable &procTable) {
	Node* curr = &ASTRoot; 
	string currType = curr->getType();
	// vector<Node*> children = curr->getChild();
	for (unsigned int i=0;i<curr->getChild().size();i++) {
		if (procIndex == procTable.getProcIndex(curr->getChild(i)->getData())) {
			curr = (curr->getChild(i))->getChild(0);
			break;
		}
	}
	while (curr->getChild(0) != NULL) {
		curr = curr->getChild(0); 
	}
	return curr->getProgLine();
}


int getLastProgLine(int procIndex, Node &ASTRoot, ProcTable &procTable) {
	Node* curr = &ASTRoot; 
	string currType = curr->getType();
	// vector<Node*> children = curr->getChild();
	for (unsigned int i=0;i<curr->getChild().size();i++) {
		if (procIndex == procTable.getProcIndex(curr->getChild(i)->getData())) {
			curr = (curr->getChild(i))->getChild(0);
			break;
		}
	}
	vector<Node*> children = vector<Node*>(); 
	children = curr->getChild();
	while (!children.empty()) {
		curr = children[children.size()-1];
		children = curr->getChild();
	}
	return curr->getProgLine();
}

