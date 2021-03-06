//@author Steven Kester Yuwono
//TestNode.h

#ifndef TestNode_h
#define TestNode_h

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class NodeTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
	CPPUNIT_TEST_SUITE( NodeTest ); // Note 3 
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testType );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	// method to test the constructor
	void testConstructor();

	// method to test the assigning and retrieval of grades
	void testType();
};
#endif
    