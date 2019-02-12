#pragma once
#include <iostream>
using namespace std;
class CLog
{
public:
	CLog(void);
	~CLog(void);
	void WriteLog(char* loginfo);
private:
	FILE * file;
};
//extern CLog g_Log;
