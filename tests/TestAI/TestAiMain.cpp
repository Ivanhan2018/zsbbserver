// @brief ��Ҫ����: AIģ�鵥Ԫ�����ļ�
// @author ����: �� �� ��

#include "testSelfDefenceAI.h"

int main(int argc, char* argv[])
{
	CppUnit::TextTestRunner runner; 
	runner.addTest(TestSelfDefence::suite() );
	runner.run();
	
	system("pause");
	return 0;
}