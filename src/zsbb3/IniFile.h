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
	int	m_WarmTime;//Ԥ��ʱ��
	int m_HeroResumeTime;//Ӣ�����˻ָ�ʱ��
	int m_iNoProMaxTurn;
	string m_LogSvrIP;
	int	m_LogSvrPort;
	int m_RedeemHeroMoney;//���Ӣ����Ҫ��Ǯ����
	int m_BaseActionDot;//�����ж�ֵ
	int m_ActionDotQuotiety;//�ж�ֵ����ϵ��
	int m_GBHActionDot;		//���˿۳����ж�ֵ
	int m_CaptivedActionDot;//�����۳����ж�ֵ
};

extern IniFile g_IniFile;
