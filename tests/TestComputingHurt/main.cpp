// #include <cmath>
//#include "boost/serialization/singleton.hpp"
#include "testhurt.h"
#include "testbuildinghurt.h"
#include "testfightreport.h"



#if 0
static void geerateRandom()
{
	int y = RandomSingleton::get_mutable_instance().generator(1, 6);
	for (;;)
	{
		y = RandomSingleton::get_mutable_instance().generator(1, 6);
	}
	int x = 0;
	boost::mt19937 rng; 
	// ƾ�ղ�������ԣ��μ�α����������� 
	for (;;)
	{
		boost::uniform_int<> six(1,6); // ӳ�䵽1..6�ķֲ����μ�������ֲ� 
		boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(rng, six); // ճ������Ժ�ӳ�� 
		x = die(); // ģ��ҡ����
	}

	return;
}
#endif


// TODO:�⻷���ܵļ��� �����
// TODO:����������ID-��enum �����
// TODO:���ʣ�������� �����
// TODO:�������ֵ �����

// TODO:�������¼���ʱ�벿�����Ӣ�ۼ���ͬ��
// TODO:Ӣ���е������ǲ��ǻ�ı䣿
// TODO:��ʦ�ظ���������
// TODO:���ܵ��˺���hp =�� 0ʱ���ص�onDie()�� ����AI����
// TODO:soldier::processHurt()����
int main(void)
{
	// ��ʦ
	// ����
	// ����

	CppUnit::TextTestRunner runner; 
	std::ofstream ofs("tests.xml");
	runner.setOutputter( CppUnit::CompilerOutputter::defaultOutputter( 
		&runner.result(),
		std::cerr ) );

	runner.addTest(TestArmyHurt::suite() );
	runner.addTest(TestBuildingHurt::suite() );
	runner.addTest(TestFightReport::suite() );
	runner.run();
	ofs.close();
	return 0;
}
