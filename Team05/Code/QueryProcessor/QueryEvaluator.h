#pragma once
#ifndef QUERYEVALUATOR_H
#define QUERYEVALUATOR_H

#include <iostream>
#include "Relationship.h"
#include "Query.h"
#include "PKB.h"
#include "Pair.h"
#include <vector>
#include <set>


class QueryEvaluator{ 
private:



	void evaluateFollowsStar(Relationship, std::unordered_map<std::string, TypeTable::SynType>, int);
	
	std::vector<int> evaluateParentStar(Relationship, std::unordered_map<std::string, TypeTable::SynType>, std::string);
	
	void evaluateCallsStar(Relationship, std::unordered_map<string, TypeTable::SynType>, int);
	void evaluateWith(Relationship, std::unordered_map<string, TypeTable::SynType>, int);

	void evaluateNext(Relationship, std::unordered_map<string, TypeTable::SynType>, int);
	void recursiveCall(int, int, std::vector<Pair> *);
	void recursiveInverseCall(int, int, std::vector<Pair> *);
	void recursiveCallBoolean(int, int, int, std::vector<Pair> *);

	void intersectPairs(std::string, std::string, std::vector<Pair> *, int);

	//std::vector<int> evaluateCallsStar(Relationship);
	std::vector<int> evaluatePattern(Query, std::string, std::string);
	std::vector<Relationship> orderRelationship(vector<Relationship>);
	std::vector<int> * findAnswerVectorFromToken(std::string);
	//std::vector<std::string> findLinks(std::string);

	std::set<int> retrieveTokenEvaluatedAnswers(std::string );
	void removePairs(std::vector<Pair>, std::string, int);
	void removePairsFromRelAns(std::vector<Pair> *,std::string, int);
	void insertLinks(std::string, int);


	/*
	bool evaluateCallsBoolean(Relationship);
	bool evaluateCallsStarBoolean(Relationship);*/
	bool evaluateRightHandSide(std::string, Node);
	bool evaluateLeftHandSide(std::string, Node);
	/*
	bool evaluateFollowsBoolean(Relationship, std::unordered_map<std::string, TypeTable::SynType>);
	bool evaluateFollowsStarBoolean(Relationship, std::unordered_map<std::string, TypeTable::SynType>);
	bool evaluateParentBoolean(Relationship, std::unordered_map<std::string, TypeTable::SynType>);
	bool evaluateParentStarBoolean(Relationship, std::unordered_map<std::string, TypeTable::SynType>);
	bool evaluateModifiesBoolean(Relationship, std::unordered_map<std::string, TypeTable::SynType>);
	bool evaluateUsesBoolean(Relationship, std::unordered_map<std::string, TypeTable::SynType>);*/
	bool isExistInLinkages(std::string);
	//bool isLinked(std::string, std::string);
	
	//std::vector<int> intersectAnswers(std::vector<std::vector<int> >);
	std::string convertEnumToString(TypeTable::SynType);

public:
	QueryEvaluator(PKB*);
	~QueryEvaluator();
	PKB *pkb;
	std::unordered_map<std::string, std::vector<int>> evaluateQuery(Query);
	bool evaluateQueryBoolean(Query);

	void evaluateCalls(Relationship, int);
	void evaluateUses(Relationship, std::unordered_map<std::string, TypeTable::SynType>, int);
	void evaluateFollows(Relationship, std::unordered_map<std::string, TypeTable::SynType>, int);
	void evaluateParent(Relationship, std::unordered_map<std::string, TypeTable::SynType>, int);
	void evaluateModifies(Relationship, std::unordered_map<std::string, TypeTable::SynType>, int);
	void evaluateNextStar(Relationship, std::unordered_map<string, TypeTable::SynType>, int);
	void recursiveNext(int, int, std::vector<Pair> *);
	static std::unordered_map<string, std::vector<int>> linkages;
	static std::unordered_map<int, std::vector<Pair>> relAns;
	static std::unordered_map<int, vector<std::string>> relParameters;
};

#endif