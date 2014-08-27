﻿//@author Steven Kester Yuwono
//TestQueryParser.h

#ifndef TestQueryParser_h
#define TestQueryParser_h

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class QueryParserTest : public CPPUNIT_NS::TestFixture{ 
	CPPUNIT_TEST_SUITE( QueryParserTest );
	CPPUNIT_TEST(testQueryAssignSelectModifies);
	CPPUNIT_TEST(testQueryAssignVariableSelectModifies);
	CPPUNIT_TEST(testQueryStmtSelectUses);
	CPPUNIT_TEST(testQueryStmtSelectParentStar);
	CPPUNIT_TEST(testQueryAssignSelectModifiesPattern);
	CPPUNIT_TEST(testQueryComplex1);
	CPPUNIT_TEST(testQueryFollows1);
	CPPUNIT_TEST(testQueryModifies1);
	CPPUNIT_TEST(testQueryModifies2);
	CPPUNIT_TEST(testQueryValidationModifies1);
	CPPUNIT_TEST(testQueryValidationModifies2);
	CPPUNIT_TEST(testQueryValidationModifies3);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testQueryAssignSelectModifies();
	void testQueryAssignVariableSelectModifies();
	void testQueryStmtSelectUses();
	void testQueryStmtSelectParentStar();
	void testQueryAssignSelectModifiesPattern();
	void testQueryComplex1();
	void testQueryFollows1();
	void testQueryModifies1();
	void testQueryModifies2();
	void testQueryValidationModifies1();
	void testQueryValidationModifies2();
	void testQueryValidationModifies3();
};
#endif
    