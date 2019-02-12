#ifndef CONSORTIAUSER_H__
#define CONSORTIAUSER_H__

#include "IceUtil.h"
#include "DB_Proxy.h"
#include "DB_Table.h"
class CConsortiaUser:public IceUtil::Shared
{
public:
	CConsortiaUser();
	CConsortiaUser(int playerid,int consortiaid,int jobid,int purview,int type);
	~CConsortiaUser();
	int SetUser(int playerid,int consortiaid,int jobid,int purview,int type);
	int SetPlayerID(int playerid);
	int SetConsortiaID(int consortiaid);
	int SetConsortiaJob(int jobid);
	int SetPurview(int purview);
	int SetType(int type);

	int GetPlayerID();
	int GetConsortiaID();
	int GetConsortiaJob();
	int GetPurview();
	int GetType();
	//���׶�
	int setContribute(int val);
	int addContribute(int val);
	int getContribute();
	//���ųƺ�
	int setLegionTitle(int id);
	int getLegionTitle();
	//��ȡ���ʵ�����
	void setSalariedDay(const char *day);
	const char *getSalariedDay();
	//������
	int setCredit(int val);
	int addCredit(int val);
	int getCredit();
	//����ֵ
	int setExp(int val);
	int addExp(int val);
	int getExp();
	//���׵Ľ�Ǯ��
	int setMoney(int val);
	int addMoney(int val);
	int getMoney();
	//�Ƿ������ȡ����
	bool isCanSalaried(const char* strtoday);

	int update2DBG(DB_Proxy *pDBProxy);
private:
	void init();
private:
	int		PlayerID;			//��ұ��
	int		ConsortiaID;		//������
	int		ConJob;				//����ְλ
	int		Purview;			//Ȩ��
	int		Type;				//����
	int		Contribute;			//���׶�
	int		LegionTitle;		//���ųƺ�
	char	SalariedDay[9];		//��ȡ��������
	int		Honors;				//������
	int		Exp;				//����ֵ
	int		Money;				//���׵Ľ�Ǯ��
};
#endif
