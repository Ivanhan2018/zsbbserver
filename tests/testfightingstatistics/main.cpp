#include "testbattlefieldstatistics.h"
#include "testfightingstatistics.h"


int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	CppUnit::TextTestRunner runner; 
	std::ofstream ofs("tests.xml");
	runner.setOutputter( CppUnit::CompilerOutputter::defaultOutputter( 
		&runner.result(),
		std::cerr ) );

	runner.addTest(statisticssystem::TestBattlefieldStatistics::suite() );
	runner.addTest(statisticssystem::TestFightingStatistics::suite() );
	runner.run();
	ofs.close();
	return 0;
}
