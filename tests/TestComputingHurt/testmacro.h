// $Id: testmacro.h 7 2007-04-10 09:46:29 Zhangwenlong $ 
#ifndef _TESTMACRO_H_
#define _TESTMACRO_H_

#pragma warning(push)
#pragma warning(disable:4512)
#include <fstream>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/CompilerOutputter.h>
#include <tchar.h>
#pragma warning(pop)

/// 用法如下:
#if 0
class LoadModelTest : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(LoadModelTest);
	CPPUNIT_TEST(TestInvalidHeaderNullValue);
	CPPUNIT_TEST(TestInvalidHeaderIllFormatted);
	CPPUNIT_TEST(TestInvalidHeaderInvalidVersionSection);
	CPPUNIT_TEST(TestInvalidValidHeader);
	CPPUNIT_TEST_SUITE_END();

	void TestInvalidHeaderNullValue()
	{
		model my_model;
		CPPUNIT_ASSERT_THROW(my_model.parse_header_section(NULL),
			model_invalid_header);
	}

	void TestInvalidHeaderIllFormatted()
	{
		model my_model;
		CPPUNIT_ASSERT_THROW(my_model.parse_header_section(
			"<HEADER"
			"<VERSION>1.1</VERSION>"
			"<TYPE>WorldLevel</TYPE>"
			"<NAME>Character Select Gallery</NAME>"
			"<AUTHOR>Blake Madden</AUTHOR>"),
			model_invalid_header);
	}

	void TestInvalidHeaderInvalidVersionSection()
	{
		model my_model;
		//Version section is missing the version number
		CPPUNIT_ASSERT_THROW(my_model.parse_header_section(
			"<HEADER>"
			"<VERSION></VERSION>"
			"<TYPE>WorldLevel</TYPE>"
			"<NAME>Character Select Gallery</NAME>"
			"<AUTHOR>Blake Madden</AUTHOR>"
			"</HEADER>"),
			model_invalid_version);
		//Version section is missing
		CPPUNIT_ASSERT_THROW(my_model.parse_header_section(
			"<HEADER>"
			"<TYPE>WorldLevel</TYPE>"
			"<NAME>Character Select Gallery</NAME>"
			"<AUTHOR>Blake Madden</AUTHOR>"
			"</HEADER>"),
			model_invalid_version);
		//Ill-formatted version section
		CPPUNIT_ASSERT_THROW(my_model.parse_header_section(
			"<HEADER>"
			"<VERSION>1.2"
			"<TYPE>WorldLevel</TYPE>"
			"<NAME>Character Select Gallery</NAME>"
			"<AUTHOR>Blake Madden</AUTHOR>"
			"</HEADER>"),
			model_invalid_version);
	}

	void TestInvalidValidHeader()
	{
		model my_model;
		//Perfectly valid header
		my_model.parse_header_section(
			"<HEADER>"
			"<VERSION>1.2</VERSION>"
			"<TYPE>WorldLevel</TYPE>"
			"<NAME>Character Select Gallery</NAME>"
			"<AUTHOR>Blake Madden</AUTHOR>"
			"</HEADER>");
		CPPUNIT_ASSERT(my_model.get_version() == 1.2);
		CPPUNIT_ASSERT(my_model.get_type() == world_level);
		CPPUNIT_ASSERT(strcmp(my_model.get_name(), "Character Select Gallery") == 0);
		CPPUNIT_ASSERT(strcmp(my_model.get_author(), "Blake Madden") == 0);
		//Author and name are not included
		my_model.parse_header_section(
			"<HEADER>"
			"<VERSION>1.2</VERSION>"
			"<TYPE>WorldLevel</TYPE>"
			"</HEADER>");
		CPPUNIT_ASSERT(my_model.get_version() == 1.2);
		CPPUNIT_ASSERT(my_model.get_type() == world_level);
		CPPUNIT_ASSERT(strcmp(my_model.get_name(), "") == 0);
		CPPUNIT_ASSERT(strcmp(my_model.get_author(), "") == 0);
	}
};



int _tmain(int argc, _TCHAR* argv[])
{
	CppUnit::TextTestRunner runner; 
	std::ofstream ofs("tests.xml");
	CppUnit::XmlOutputter* xml = new CppUnit::XmlOutputter(&runner.result(), ofs);
	xml->setStyleSheet("report.xsl");
	runner.setOutputter(xml);

	runner.addTest(LoadModelTest::suite() );
	runner.run();
	ofs.close();
	return 0;
}
或
int _tmain(int argc, _TCHAR* argv[])
{
	CppUnit::TextTestRunner runner; 
	std::ofstream ofs("tests.xml");
	runner.setOutputter( CppUnit::CompilerOutputter::defaultOutputter( 
	&runner.result(),
	std::cerr ) );

	runner.addTest(LoadModelTest::suite() );
	runner.addTest(HeapAllocatorTest::suite() );
	runner.run();
	ofs.close();
	return 0;
}
#endif




#endif // _TESTMACRO_H_ 
