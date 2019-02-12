#pragma once
#include <map>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;
typedef map<string, string, less<string> > strMap;
typedef strMap::iterator strMapIt;

const char*const MIDDLESTRING = "_____***_______";

class IniFile
{
public:
	IniFile( ){};
	~IniFile( ){};
	void ReadFile(char* filePath);
public:
	string filepath;
	string strDBIP;
	string strAgentID;
	int m_DBPort;
	int m_AgentPort ;
	int m_mapID;
	int m_SpecialHeroRate;
	int m_zsRate;
	int m_ssRate;
	int m_msRate;
	int m_fsRate;
	int m_iNeedKillTurnNum;
	int m_MaxArmyNum;
	int m_MaxPeasArmyNum;
	int m_MaxTrunNum;
	int m_MaxEnterPlayerNum;
	int	m_WarmTime;//预警时间
	int m_HeroResumeTime;//英雄重伤恢复时间
	int m_iNoProMaxTurn;
	string m_LogSvrIP;
	int	m_LogSvrPort;
	int m_RedeemHeroMoney;//赎回英雄需要金钱基数
	int m_BaseActionDot;//基础行动值
	int m_ActionDotQuotiety;//行动值增加系数
	int m_GBHActionDot;		//重伤扣除的行动值
	int m_CaptivedActionDot;//被俘扣除的行动值
};

extern IniFile g_IniFile;
