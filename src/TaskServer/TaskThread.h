#ifndef __TaskThread_H__
#define __TaskThread_H__
#include "ProTaskSystem.h"
#include "FixTaskTable.h"
#include "MapServer_Protocol.h"
#include "CommonFunction.h"
#include "Time_Protocol.h"
#include <map>
class CNetTask;

class CTaskThread
{
public:
	CTaskThread(CNetTask *netTask);
	~CTaskThread();

	bool InitTaskThread(const char * addr, short port);
	bool FInitTaskThread();

	/* �����������ݲ���Function */

	//��ȡ���������ɸ���
	int OnGetTaskCount(const DISTRIBUTED_HEADER * pNetRead);
	//��ȡ�����б�
	int OnGetTaskList(const DISTRIBUTED_HEADER * pNetRead);
	int OnGetTaskListEx(const DISTRIBUTED_HEADER * pNetRead);
	//����״̬�������
	int OnModiTaskState(const DISTRIBUTED_HEADER * pNetRead);
	//��ҵȼ�����¼�
	int OnPlayerLeveEvent(const DISTRIBUTED_HEADER * pNetRead);
	//��������¼�
	int OnTaskItemEvent(const DISTRIBUTED_HEADER * pNetRead);
	//�����������¼�
	int OnTaskCompleteEvent(const DISTRIBUTED_HEADER * pNetRead);
	//��������¼�
	int OnEctypeEvent(const DISTRIBUTED_HEADER * pNetRead);
	//����������û��Ƿ����	
	int OnCheckTask(const DISTRIBUTED_HEADER * pNetRead);

	int OnNotify_QueryMoney(const DISTRIBUTED_HEADER * pNetRead);

	//��ʱ��
	int onTimerMsg(const char *timermsg);
	//�ճ��������֪ͨ
	int onNotifyDayTaskFinished(const char *notifymsg);


private:
	/* other-��������״̬�ֽ� */
	//�����ճ�����
	void issueDayTask(int agent,int playerid, int countryID);
	void checkDayTask(int agent,int playerid);
	bool checkColonialDayTask(int playerid,int mapid,int number,int level);
	//�ύ����
	int OnReferTask(const DISTRIBUTED_HEADER * pNetRead);
	//��������
	int OnAcceptTask(const DISTRIBUTED_HEADER * pNetRead);
	//ɾ������
	int OnDeleteTask(const DISTRIBUTED_HEADER * pNetRead);

	//�������
	int OnCompleteTask(int handle, unsigned long playerid, unsigned int taskid);
	//��������
	int OnIssueSystemTask(int handle,unsigned long ulPlayerID,unsigned int TaskID,int tasktype);

	/* ����Function */
	//��ѯ����״̬
	int OnQryTask(unsigned long playerid,unsigned int taskid, int & ts);
	
	//��������Ƿ����
	int CheckTaskIsComplete(int agent,unsigned long playerid, unsigned int taskid,const char * pContest);

    //��ȡ������
	void GetTaskHortation(unsigned long playerID ,unsigned int TaskID,const int tasktype);
	
	//��ѯ����(��ұ��, ��ͼ���, ���߱��, ��ѯ��־)
	int  CheckItem(int agent,unsigned long PlayerID, int taskid,int TypeID, int ItemID, int ItemNum, int iFlag,int iLevel); 
	bool AllTaskComplete(int playerid);//�������Ƿ��������������

	int GetItemCount(int playerid,int officerid,int itemid,int pos);
	//����ճ������
	void clearAllDayTask();
private:
	//bool					bAlive;
	DB_Proxy				Proxy ;
	CNetTask				*m_NetTask;
};

#endif