#ifndef QUERY_H
#define QUERY_H
#include "Relationship.h"
#include "TypeTable.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Query
{
    public:
        Query(string);
		string getSelectedSyn();
        void addRelationship(Relationship);
        vector<Relationship> getRelVect();
	//	enum STYPE{ASSIGN, IF, WHILE, BOOLEAN};
		void addSynTable(unordered_map<string, STYPE>);
		unordered_map<string, STYPE> getSynTable();

	private:
        string selectedSyn; 
        vector<Relationship> relVect;
		unordered_map<string, STYPE> synTable;
};

#endif // QUERY_H_INCLUDED
