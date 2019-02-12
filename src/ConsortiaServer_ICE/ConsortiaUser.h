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
	//贡献度
	int setContribute(int val);
	int addContribute(int val);
	int getContribute();
	//军团称号
	int setLegionTitle(int id);
	int getLegionTitle();
	//领取工资的日期
	void setSalariedDay(const char *day);
	const char *getSalariedDay();
	//荣誉点
	int setCredit(int val);
	int addCredit(int val);
	int getCredit();
	//经验值
	int setExp(int val);
	int addExp(int val);
	int getExp();
	//捐献的金钱数
	int setMoney(int val);
	int addMoney(int val);
	int getMoney();
	//是否可以领取工资
	bool isCanSalaried(const char* strtoday);

	int update2DBG(DB_Proxy *pDBProxy);
private:
	void init();
private:
	int		PlayerID;			//玩家编号
	int		ConsortiaID;		//公会编号
	int		ConJob;				//公会职位
	int		Purview;			//权限
	int		Type;				//类型
	int		Contribute;			//贡献度
	int		LegionTitle;		//军团称号
	char	SalariedDay[9];		//领取工资日期
	int		Honors;				//荣誉点
	int		Exp;				//经验值
	int		Money;				//捐献的金钱数
};
#endif
