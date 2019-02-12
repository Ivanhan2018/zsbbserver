#ifndef TESTCOMPUTINGHURT_TESTFIGHTREPORT_H_
#define TESTCOMPUTINGHURT_TESTFIGHTREPORT_H_
#include "testmacro.h"

class TestFightReport : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(TestFightReport);
	CPPUNIT_TEST(testReport);
	CPPUNIT_TEST_SUITE_END();

private:
	void testReport();
};


#endif // TESTCOMPUTINGHURT_TESTFIGHTREPORT_H_