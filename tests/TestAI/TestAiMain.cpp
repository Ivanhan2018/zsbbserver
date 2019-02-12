// @brief 简要描述: AI模块单元测试文件
// @author 作者: 黄 文 才

#include "testSelfDefenceAI.h"

int main(int argc, char* argv[])
{
	CppUnit::TextTestRunner runner; 
	runner.addTest(TestSelfDefence::suite() );
	runner.run();
	
	system("pause");
	return 0;
}