#include ".\inifile.h"
//
//IniFile::IniFile(void)
//{
//}
//
//IniFile::~IniFile(void)
//{
//}
IniFile g_IniFile ;
void IniFile::ReadFile(char* filePath)
{
	string buf;
	ifstream fsr(filePath);
	int line  = 0;
	while(fsr)
	{
		if(getline(fsr,buf))
		{
			switch(line)
			{
			case 0:
				m_mapID = atoi(buf.c_str());
				break;
			case 1 :
				strDBIP = buf;
				break;

			case 2:
				m_DBPort = atoi(buf.c_str());
				break;

			case 3:
				strAgentID = buf;
				break;

			case 4:
				m_AgentPort = atoi(buf.c_str());
				break;
			case 5:
				m_SpecialHeroRate=atoi(buf.c_str());
				break;
			case 6:
				sscanf(buf.c_str(),"%d,%d,%d,%d",&m_zsRate,&m_ssRate,&m_msRate,&m_fsRate);
				break;	
			case 7:
				sscanf(buf.c_str(),"%d,%d,%d,%d,%d,%d",&m_MaxTrunNum,&m_MaxArmyNum,&m_MaxPeasArmyNum,&m_MaxEnterPlayerNum,&m_iNoProMaxTurn,&m_iNeedKillTurnNum);
				break;	
			case 8:
				m_WarmTime = atoi(buf.c_str());
				if(m_WarmTime <=0)
					m_WarmTime = 30;
				break;
			case 9:
				m_HeroResumeTime = atoi(buf.c_str());
				if(m_HeroResumeTime <=0)
					m_HeroResumeTime = 10800;
				break;
			case 10:
				m_LogSvrIP = buf;
				break;
			case 11:
				m_LogSvrPort = atoi(buf.c_str());
				break;
			case 12:
				m_RedeemHeroMoney = atoi(buf.c_str());
				break;
			case 13:
				sscanf(buf.c_str(),"%d,%d,%d,%d",&m_BaseActionDot,&m_ActionDotQuotiety,&m_GBHActionDot,&m_CaptivedActionDot);
				break;
			}	
		}
		line ++;
	}	
}