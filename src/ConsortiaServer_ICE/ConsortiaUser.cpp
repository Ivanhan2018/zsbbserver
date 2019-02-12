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
	Contribute = 0;					//���׶�
	LegionTitle = 0;				//���ųƺ�
	memset(SalariedDay,0,9);		//��ȡ��������
	Honors = 0;						//������
	Exp = 0;						//����ֵ
	Money = 0;						//���׵Ľ�Ǯ��
}
/**************************************************************************
*˵�������ù����Ա��Ϣ
*���������	
*����ֵ��0:�ɹ�������:ʧ��
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
*˵�������ù����Ա�����ID
*���������	
*����ֵ��0:�ɹ�������:ʧ��
**************************************************************************/
int CConsortiaUser::SetPlayerID(int playerid)
{
	return (PlayerID = playerid);
}
/**************************************************************************
*˵�������ù����Ա�Ĺ���ID
*���������	
*����ֵ��0:�ɹ�������:ʧ��
**************************************************************************/
int CConsortiaUser::SetConsortiaID(int consortiaid)
{
	return (ConsortiaID = consortiaid);
}
/**************************************************************************
*˵��������ְλID
*���������	
*����ֵ��0:�ɹ�������:ʧ��
**************************************************************************/
int CConsortiaUser::SetConsortiaJob(int jobid)
{
	return (ConJob = jobid);
}
/**************************************************************************
*˵�������ó�Ա��Ȩ��
*���������	
*����ֵ��0:�ɹ�������:ʧ��
**************************************************************************/
int CConsortiaUser::SetPurview(int purview)
{
	return (Purview = purview);
}
/**************************************************************************
*˵���������������
*���������	
*����ֵ��0:�ɹ�������:ʧ��
**************************************************************************/
int CConsortiaUser::SetType(int type)
{
	return (Type = type);
}
/**************************************************************************
*˵������ȡ���ID
*���������	
*����ֵ�����ID
**************************************************************************/

int CConsortiaUser::GetPlayerID()
{
	return PlayerID;
}
/**************************************************************************
*˵������ȡ����ID
*���������	
*����ֵ������ID
**************************************************************************/
int CConsortiaUser::GetConsortiaID()
{
	return ConsortiaID;
}
/**************************************************************************
*˵������ȡְλID
*���������	
*����ֵ��ְλID
**************************************************************************/
int CConsortiaUser::GetConsortiaJob()
{
	return ConJob;
}
/**************************************************************************
*˵������ȡȨ��
*���������	
*����ֵ��Ȩ��
**************************************************************************/
int CConsortiaUser::GetPurview()
{
	return Purview;
}
/**************************************************************************
*˵������ȡ�������
*���������	
*����ֵ���������
**************************************************************************/
int CConsortiaUser::GetType()
{
	return Type;
}
//���׶�
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
//���ųƺ�
int CConsortiaUser::setLegionTitle(int id)
{
	return (LegionTitle = id);
}
int CConsortiaUser::getLegionTitle()
{
	return LegionTitle;
}
//��ȡ���ʵ�����
void CConsortiaUser::setSalariedDay(const char *day)
{
	strncpy(SalariedDay,day,8);
}
const char *CConsortiaUser::getSalariedDay()
{
	return SalariedDay;
}
//������
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
//����ֵ
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
//���׵Ľ�Ǯ��
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

//�Ƿ������ȡ����
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
	user.type_ = Type; // consortiaid_ = id ����
	user.contribute_ = Contribute;
	user.legiontitle_ = LegionTitle;
	strcpy(user.salariedday_,SalariedDay);
	user.honors_ = Honors;
	user.exp_ = Exp;
	user.money_ = Money;
	return pDBProxy->db_update(user,user);
}
