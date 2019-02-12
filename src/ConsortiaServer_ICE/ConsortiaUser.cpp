#include "ConsortiaUser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

CConsortiaUser::CConsortiaUser()
:PlayerID(-1),ConsortiaID(-1),ConJob(-1),Type(-1),Purview(0)
{
	init();
}
CConsortiaUser::CConsortiaUser(int playerid,int consortiaid,int jobid,int purview,int type):
PlayerID(playerid),ConsortiaID(consortiaid),ConJob(jobid),Purview(purview),Type(type)
{
	init();
}
CConsortiaUser::~CConsortiaUser()
{
}

void CConsortiaUser::init()
{
	Contribute = 0;					//贡献度
	LegionTitle = 0;				//军团称号
	memset(SalariedDay,0,9);		//领取工资日期
	Honors = 0;						//荣誉点
	Exp = 0;						//经验值
	Money = 0;						//捐献的金钱数
}
/**************************************************************************
*说明：设置公会成员信息
*输入参数：	
*返回值：0:成功；其他:失败
**************************************************************************/
int CConsortiaUser::SetUser(int playerid,int consortiaid,int jobid,int purview,int type)
{
	PlayerID = playerid;
	ConsortiaID = consortiaid;
	ConJob = jobid;
	Purview = purview;
	Type = type;

	return 0;
}
/**************************************************************************
*说明：设置公会成员的玩家ID
*输入参数：	
*返回值：0:成功；其他:失败
**************************************************************************/
int CConsortiaUser::SetPlayerID(int playerid)
{
	return (PlayerID = playerid);
}
/**************************************************************************
*说明：设置公会成员的公会ID
*输入参数：	
*返回值：0:成功；其他:失败
**************************************************************************/
int CConsortiaUser::SetConsortiaID(int consortiaid)
{
	return (ConsortiaID = consortiaid);
}
/**************************************************************************
*说明：设置职位ID
*输入参数：	
*返回值：0:成功；其他:失败
**************************************************************************/
int CConsortiaUser::SetConsortiaJob(int jobid)
{
	return (ConJob = jobid);
}
/**************************************************************************
*说明：设置成员的权限
*输入参数：	
*返回值：0:成功；其他:失败
**************************************************************************/
int CConsortiaUser::SetPurview(int purview)
{
	return (Purview = purview);
}
/**************************************************************************
*说明：设置玩家类型
*输入参数：	
*返回值：0:成功；其他:失败
**************************************************************************/
int CConsortiaUser::SetType(int type)
{
	return (Type = type);
}
/**************************************************************************
*说明：获取玩家ID
*输入参数：	
*返回值：玩家ID
**************************************************************************/

int CConsortiaUser::GetPlayerID()
{
	return PlayerID;
}
/**************************************************************************
*说明：获取公会ID
*输入参数：	
*返回值：公会ID
**************************************************************************/
int CConsortiaUser::GetConsortiaID()
{
	return ConsortiaID;
}
/**************************************************************************
*说明：获取职位ID
*输入参数：	
*返回值：职位ID
**************************************************************************/
int CConsortiaUser::GetConsortiaJob()
{
	return ConJob;
}
/**************************************************************************
*说明：获取权限
*输入参数：	
*返回值：权限
**************************************************************************/
int CConsortiaUser::GetPurview()
{
	return Purview;
}
/**************************************************************************
*说明：获取玩家类型
*输入参数：	
*返回值：玩家类型
**************************************************************************/
int CConsortiaUser::GetType()
{
	return Type;
}
//贡献度
int CConsortiaUser::setContribute(int val)
{
	return (Contribute = val);
}
int CConsortiaUser::addContribute(int val)
{
	Contribute += val;
	return Contribute;
}
int CConsortiaUser::getContribute()
{
	return Contribute;
}
//军团称号
int CConsortiaUser::setLegionTitle(int id)
{
	return (LegionTitle = id);
}
int CConsortiaUser::getLegionTitle()
{
	return LegionTitle;
}
//领取工资的日期
void CConsortiaUser::setSalariedDay(const char *day)
{
	strncpy(SalariedDay,day,8);
}
const char *CConsortiaUser::getSalariedDay()
{
	return SalariedDay;
}
//荣誉点
int CConsortiaUser::setCredit(int val)
{
	return (Honors = val);
}
int CConsortiaUser::addCredit(int val)
{
	Honors += val;
	return Honors;
}
int CConsortiaUser::getCredit()
{
	return Honors;
}
//经验值
int CConsortiaUser::setExp(int val)
{
	return (Exp = val);
}
int CConsortiaUser::addExp(int val)
{
	Exp += val;
	return Exp;
}
int CConsortiaUser::getExp()
{
	return Exp;
}
//捐献的金钱数
int CConsortiaUser::setMoney(int val)
{
	return (Money = val);
}
int CConsortiaUser::addMoney(int val)
{
	Money += val;
	return Money;
}
int CConsortiaUser::getMoney()
{
	return Money;
}

//是否可以领取工资
bool CConsortiaUser::isCanSalaried(const char* strtoday)
{
	if (strcmp(strtoday,SalariedDay) > 0)
	{
		return true;
	}
	return false;
}
int CConsortiaUser::update2DBG(DB_Proxy *pDBProxy)
{
	ConsortiaPersonnel user;
	user.setmasterid(ConsortiaID);
	user.setdetailid(PlayerID);

	user.conjob_ = ConJob;
	user.purview_ = Purview;
	user.type_ = Type; // consortiaid_ = id 多条
	user.contribute_ = Contribute;
	user.legiontitle_ = LegionTitle;
	strcpy(user.salariedday_,SalariedDay);
	user.honors_ = Honors;
	user.exp_ = Exp;
	user.money_ = Money;
	return pDBProxy->db_update(user,user);
}
