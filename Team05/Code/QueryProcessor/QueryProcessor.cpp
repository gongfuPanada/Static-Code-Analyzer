#pragma once

#include "QueryProcessor.h"
#include <iostream>
using namespace std;


void queryDriver(string query, list<string> &result, PKB *pkb){
	cout<<"Begin parse query"<<endl;
	
	QueryParser qp;
	QueryEvaluator qe = QueryEvaluator::QueryEvaluator(pkb);

	bool isValid = true;
	Query parsedQuery = qp.queryParse(query,isValid);

	if(!isValid){
		cout << "Query Invalid" << endl;
		return;
	}

	cout<<"End parse query"<<endl;
	

	cout<<"Begin evaluate query"<<endl;

	vector<Relationship> newRelations;
	unordered_map<string, int> synIndexMap;
	unordered_map<int, vector<Pair>> clauseAnswers = qe.evaluateQuery(parsedQuery, &newRelations);

	cout<<"End evaluate query"<<endl;

	cout <<"Begin projecting results"<< endl;
	vector<vector<int>> tupleTable = createTupleTable(clauseAnswers, newRelations, &synIndexMap );

	vector<string> selectedSyn = parsedQuery.getSelectedSyn();
	unordered_map<string, TypeTable::SynType> synTable = parsedQuery.getSynTable();
	set<string> ansSet;
	ansSet.insert("");

	if(synTable.find(selectedSyn.at(0))->second == TypeTable::BOOLEAN) {
		if(tupleTable.size()!=0)
			result.push_back("true");
		else
			result.push_back("false");

		return;
	}

	cout << "Begin handling unprocessed selected synonyms" << endl;
	for(int i=0; i<selectedSyn.size(); i++) {
		cout << "Checking for the: " << selectedSyn.at(i) << endl;
		if(synIndexMap.count(selectedSyn.at(i)) == 0) {
			cout << selectedSyn.at(i) << " has not been processed" << endl;
			unordered_map<string, TypeTable::SynType>::iterator it = synTable.find(selectedSyn.at(i));

			vector<vector<int>> newTable;
			set<int> allOfType;

			if(it->second == TypeTable::VARIABLE) {
				allOfType = pkb->getAllVarIndex();
			}
			else if(it->second == TypeTable::CONSTANT) {
				allOfType = pkb->getAllConstIndex();
			}
			else if(it->second == TypeTable::PROCEDURE) {
				allOfType = pkb->getAllProcIndexes();
			}
			else {
				allOfType = pkb->getAllStmts(it->second);
			}

			for(vector<vector<int>>::iterator it1=tupleTable.begin(); it1!=tupleTable.end(); it1++) {
				for(set<int>::iterator it2=allOfType.begin(); it2!=allOfType.end(); it2++) {
					vector<int> newTuple;
					newTuple.insert(newTuple.begin(), it1->begin(), it1->end());
					newTuple.push_back(*it2);
					newTable.push_back(newTuple);
				}				
			}

			synIndexMap.insert(make_pair(selectedSyn.at(i), synIndexMap.size()));
			tupleTable = newTable;
		}
	}
	cout << "End handling unprocessed selected synonyms" << endl;

	cout << tupleTable.size() << endl;
	cout << "Begin creating tuple of answers" << endl;
	for(int i=0; i<tupleTable.size(); i++) {
		string ans = "";
		for(int j=0; j<selectedSyn.size(); j++) {
			unordered_map<string, TypeTable::SynType>::iterator it = synTable.find(selectedSyn.at(j));
			int index = synIndexMap.at(selectedSyn.at(j));
			int tempAns = tupleTable.at(i).at(index);

			if(it->second == TypeTable::VARIABLE) {
				VarTable* varTable = pkb->getVarTable();
				ans = ans + varTable->getVarName(tempAns) + " ";
			}
			else if(it->second == TypeTable::CONSTANT) {
				ConstTable* constTable = pkb->getConstTable();
				ans = ans + constTable->getConst(tempAns) + " ";
			}
			else if(it->second == TypeTable::PROCEDURE) {
				ProcTable* procTable = pkb->getProcTable();
				ans = ans + procTable->getProcName(tempAns) + " ";
			}
			else {
				ans = ans + to_string(static_cast<long long>(tempAns)) + " ";
			}
		}
		ans = ans.substr(0, ans.length()-1);
		if(ansSet.find(ans) == ansSet.end()) {
			result.push_back(ans);
			ansSet.insert(ans);
		}
	}
	cout << "End creating tuple of answers" << endl;

	cout <<"End projecting results"<< endl;
	
}

vector<vector<int>> createTupleTable(unordered_map<int, vector<Pair>> clauseAnswers, vector<Relationship> relationships, unordered_map<string, int> *synIndexMap) {
	cout << "Begin creating tuple table" << endl;

	vector<vector<int>> tupleTable;
	vector<int> emptyTuple;
	tupleTable.push_back(emptyTuple);

	for(int i=0; i<relationships.size(); i++) {
		cout << "Adding relationship " << i << " to tuple table" << endl;

		vector<Pair> clauseAns = clauseAnswers.at(i);
		// Check if clause has no answer, return empty tuple table
		if(clauseAns.size()==0) {
			cout << "empty clause detected, returning empty table" << endl;
			tupleTable.clear();
			break;
		}
		
		//Initialize variables
		Relationship r = relationships.at(i);
		Relationship::RelType rt = r.getRelType();
		cout << "Relation Type being added to tuple table: " << rt << endl;
		string token1;
		string token2;
		Relationship::TokenType type1;
		Relationship::TokenType type2;

		if(rt == Relationship::PATTERN) {
			type1 = r.getToken1Type();
			if(type1 == Relationship::SYNONYM) {
				token1 = r.getPatternSyn();
				token2 = r.getToken1();
				type2 = r.getToken1Type();
			}
			else {
				token1 = r.getPatternSyn();
				token2 = r.getPatternSyn();
				type1 = Relationship::SYNONYM;
				type2 = Relationship::INVALIDTOKEN;
			}
		}
		else {
			token1 = r.getToken1();
			token2 = r.getToken2();
			type1 = r.getToken1Type();
			type2 = r.getToken2Type();
		}
		bool ignore1 = false;
		bool ignore2 = false;

		if(type1 != Relationship::SYNONYM)
			ignore1 = true;
		if(type2 != Relationship::SYNONYM)
			ignore2 = true;

		//Case 1: both are Synonyms
		if(!ignore1 && !ignore2) {
			cout << "Case 1" << endl;
			bool tk1InMap = false;
			bool tk2InMap = false;

			if(synIndexMap->find(token1) != synIndexMap->end())
				tk1InMap = true;
			else
				synIndexMap->insert(make_pair(token1, synIndexMap->size()));
			
			if(synIndexMap->find(token2) != synIndexMap->end())
				tk2InMap = true;
			else
				synIndexMap->insert(make_pair(token2, synIndexMap->size()));

			vector<vector<int>> newTable;

			//Case 1.1: Both Synonyms are new
			if(!tk1InMap && !tk2InMap) {
				cout << "Case 1.1" << endl;
				for(int x=0; x<tupleTable.size(); x++) {
					for(int y=0; y<clauseAns.size(); y++) {
						vector<int> tuple;
						vector<int> tuple1 = tupleTable.at(x);
						Pair tuple2 = clauseAns.at(y);
						tuple.insert(tuple.begin(), tuple1.begin(), tuple1.end());
						tuple.push_back(tuple2.ans1);
						tuple.push_back(tuple2.ans2);
						
						newTable.push_back(tuple);
					}
				}
			}
			//Case 1.2: First Synonym is new
			else if(!tk1InMap) {
				cout << "Case 1.2" << endl;
				for(int x=0; x<tupleTable.size(); x++) {
					for(int y=0; y<clauseAns.size(); y++) {
						vector<int> tuple;
						vector<int> tuple1 = tupleTable.at(x);
						Pair tuple2 = clauseAns.at(y);
						
						int index = synIndexMap->at(token2);
						if(tuple1.at(index) == tuple2.ans2) {
							tuple.insert(tuple.begin(), tuple1.begin(), tuple1.end());
							tuple.push_back(tuple2.ans1);
							newTable.push_back(tuple);
						}
					}
				}
			}
			//Case 1.3: Second Synonym is new
			else if(!tk2InMap) {
				cout << "Case 1.3" << endl;
				for(int x=0; x<tupleTable.size(); x++) {
					for(int y=0; y<clauseAns.size(); y++) {
						vector<int> tuple;
						vector<int> tuple1 = tupleTable.at(x);
						Pair tuple2 = clauseAns.at(y);
						
						int index = synIndexMap->at(token1);
						if(tuple1.at(index) == tuple2.ans1) {
							tuple.insert(tuple.begin(), tuple1.begin(), tuple1.end());
							tuple.push_back(tuple2.ans1);
							newTable.push_back(tuple);
						}
					}
				}
			}
			//Case 1.4: Both Synonyms have been seen before
			else {
				cout << "Case 1.4" << endl;
				for(int x=0; x<tupleTable.size(); x++) {
					for(int y=0; y<clauseAns.size(); y++) {
						vector<int> tuple;
						vector<int> tuple1 = tupleTable.at(x);
						Pair tuple2 = clauseAns.at(y);
						int index1 = synIndexMap->at(token1);
						int index2 = synIndexMap->at(token2);

						if(tuple1.at(index1) == tuple2.ans1 && tuple1.at(index2) == tuple2.ans2) {
							tuple.insert(tuple.begin(), tuple1.begin(), tuple1.end());
							newTable.push_back(tuple);
						}

					}
				}
			}

			tupleTable = newTable;
		}
		//Case 2: only first arg is Synonyms
		else if(!ignore1) {
			cout << "Case 2" << endl;
			bool tkInMap = false;

			if(synIndexMap->find(token1) != synIndexMap->end())
				tkInMap = true;
			else
				synIndexMap->insert(make_pair(token1, synIndexMap->size()));
			
			vector<vector<int>> newTable;

			//Case 2.1: have seen Synonym
			if(tkInMap) {
				cout << "Case 2.1" << endl;
				for(int x=0; x<tupleTable.size(); x++) {
					for(int y=0; y<clauseAns.size(); y++) {
						vector<int> tuple;
						vector<int> tuple1 = tupleTable.at(x);
						Pair tuple2 = clauseAns.at(y);
						
						int index = synIndexMap->at(token1);
						if(tuple1.at(index) == tuple2.ans1) {
							tuple.insert(tuple.begin(), tuple1.begin(), tuple1.end());
							newTable.push_back(tuple);
						}
					}
				}
			}
			//Case 2.2: haven't seen Synonym
			else {
				cout << "Case 2.2" << endl;
				for(int x=0; x<tupleTable.size(); x++) {
					for(int y=0; y<clauseAns.size(); y++) {
						vector<int> tuple;
						vector<int> tuple1 = tupleTable.at(x);
						Pair tuple2 = clauseAns.at(y);
						tuple.insert(tuple.begin(), tuple1.begin(), tuple1.end());
						tuple.push_back(tuple2.ans1);
						
						newTable.push_back(tuple);
					}
				}
			}

			tupleTable = newTable;
		}
		//Case 3: only second arg is Synonyms
		else if(!ignore2) {
			cout << "Case 3" << endl;
			bool tkInMap = false;

			if(synIndexMap->find(token2) != synIndexMap->end())
				tkInMap = true;
			else
				synIndexMap->insert(make_pair(token2, synIndexMap->size()));

			vector<vector<int>> newTable;

			//Case 3.1: have seen Synonym
			if(tkInMap) {
				cout << "Case 3.1" << endl;
				for(int x=0; x<tupleTable.size(); x++) {
					for(int y=0; y<clauseAns.size(); y++) {
						vector<int> tuple;
						vector<int> tuple1 = tupleTable.at(x);
						Pair tuple2 = clauseAns.at(y);
						
						int index = synIndexMap->at(token2);
						if(tuple1.at(index) == tuple2.ans2) {
							tuple.insert(tuple.begin(), tuple1.begin(), tuple1.end());
							newTable.push_back(tuple);
						}
					}
				}
			}
			//Case 3.2: haven't seen Synonym
			else {
				cout << "Case 3.2" << endl;
				for(int x=0; x<tupleTable.size(); x++) {
					for(int y=0; y<clauseAns.size(); y++) {
						vector<int> tuple;
						vector<int> tuple1 = tupleTable.at(x);
						Pair tuple2 = clauseAns.at(y);
						tuple.insert(tuple.begin(), tuple1.begin(), tuple1.end());
						tuple.push_back(tuple2.ans2);
						
						newTable.push_back(tuple);
					}
				}
			}

			tupleTable = newTable;
		}
		//Case 4: neither are Synonyms(answer is boolean)
		else {
			cout << "Case 4" << endl;
			Pair p = clauseAns.at(0);
			int boolean = p.ans1;
			if(boolean==-1)
				continue;
			else
				tupleTable.clear();
		}

		//If tuple table is ever empty, return empty tuple table
		if(tupleTable.size() == 0)
			return tupleTable;
		cout <<"Tuple Table Size: " << tupleTable.size() << endl;

	}

	cout << "End creating tuple table" << endl;
	return tupleTable;
}
