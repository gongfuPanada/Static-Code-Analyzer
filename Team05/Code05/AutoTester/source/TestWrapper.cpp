#include "TestWrapper.h"


// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constrSuctor
TestWrapper::TestWrapper() {
	pkb = PKB::getInstance();
}

// method for parsing the SIMPLE source and extracting relationships
void TestWrapper::parse(std::string filename) {
	CodeParser::parserDriver(filename,pkb);
	DesignExtractor::extractorDriver(pkb); 
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
	queryDriver(query, results, pkb);
	cout<<endl<<endl<<endl<<endl;
}
