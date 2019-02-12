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
	// 凭空产生随机性，参见伪随机数发生器 
	for (;;)
	{
		boost::uniform_int<> six(1,6); // 映射到1..6的分布，参见随机数分布 
		boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(rng, six); // 粘合随机性和映射 
		x = die(); // 模拟摇骰子
	}

	return;
}
#endif


// TODO:光环技能的计算 已完成
// TODO:提炼出技能ID-》enum 已完成
// TODO:概率（随机数） 已完成
// TODO:生成随机值 已完成

// TODO:当尝到新技能时与部队里的英雄技能同步
// TODO:英雄有的属性是不是会改变？
// TODO:牧师回复的正负号
// TODO:当受到伤害，hp =＝ 0时，回调onDie()， （由AI处理）
// TODO:soldier::processHurt()测试
int main(void)
{
	// 牧师
	// 箭塔
	// 暴击

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
