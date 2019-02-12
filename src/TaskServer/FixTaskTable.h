#ifndef __FixTaskTable_H__
#define __FixTaskTable_H__

#include <map>
#include "ProTaskSystem.h"
#include "DB_ProxyHeader.h"
#include "DBTableHeader.h"
#include "MapServer_Protocol.h"
#include "CommonFunction.h"
#include "ZSBB_DataDefine.h"
#pragma warning(disable:4267)
#pragma warning(disable:4706)
class CNetTask;
//20090206
/*
�������������ͣ������ܲ�����
1������������Ҿ���ָ���Ľ�Ǯ
	PlayerChatRoomTable
	contribute_
	8(����)

2����ļӢ�ۡ��ж���ҵ�Ӣ�۸������ȼ�
	PlayerOfficerTable
	9

3��������š��ж�����Ƿ��Ǿ��ų�Ա�������롢�������У�
	PlayerChatRoomTable 
	����ID>0
	10

4����Ӻ��ѡ��ж���ҵĺ������Ƿ�ﵽ3��
	firendtable
	11


5���г����ƽ��ס��ж�����Ƿ�����Ʒ���г��Ϲ���
	Market
	12

6���ռ���װ���ж����Ӣ�۱����Ƿ�ӵ��ָ����װװ����
	playeritemstoragetable
	13
7��������Դ
	14
	����:����ID:����:����:�ȼ�
	����:1��ʳ2ľ�� 3Ƥ4�� ��������ٱ��
16:GO���ѯ����
	16:0:0:GO����:0
17:����ѯ����
    17:��ID:Pos:����:0
*/


//�������ݸ�ʽ
//����ID:boss:npc:build:����ID:boss:npc:build


//��������ʽΪ
//0.����(���3��)		0:����ID:��������:����:����ID:��������:����:����ID:��������:����
//1.װ��(���3��)		1:װ��ID:����:װ��ID:����:װ��ID:����
//2.�����㣬����ֵ		2:������:����ֵ
//3.��Ǯ				3:��Ǯ
//4.��ʳ,ľ�ģ�����Ƥ�� 4:��ʳ:ľ��:Ƥ��:��
//5.�����������5��)	5����ID����������ID����������ID����������ID����������ID������
//6.����GO��			6������
//7.����ʿ��			7������
//8.�������ܵ�			8������
//�����÷ֺż����������ֻ��2��4���ʾΪ 2:������:����ֵ;4:��ʳ:ľ��:Ƥ��:��


//����
struct tagH_zero
{
	int hid;
	int htype;
	int num;
};
struct tagH_zero_list
{
	int count;
	tagH_zero hlist[3];
};

//װ����װ��ID������
struct tagH_one
{
	int hid;
	int num;
};
struct tagH_one_list
{
	int count;
	tagH_one hlist[3];
};

//�����㣬����ֵ
struct tagH_two
{
	int i1;   
	int i2;
};

//��ʳ��ľ�ģ�����Ƥ��
struct tagH_four
{
	int nums[4];
};

//��������
struct tagEctypeTask
{
	unsigned int taskid;
	int taskType;
	unsigned char boos;
	unsigned int npc;
	unsigned int build;
};

struct tagCard_Info
{
	int CardItemID;
	int Count;
};
struct  HORTATION_Plus
{
	bool b0;
	tagH_zero_list h0;

	bool b1;
	tagH_one_list h1;

	bool b2;
	tagH_two h2;

	bool b3;
	int money;

	bool b4;
	tagH_four h4;

	//��,���5��
	int CardCount;
	tagCard_Info CardItem[5];

	//GO��
	int GoDian;
	//�������ܵ�
	int TechDot;
	//������
	int SoldierCount;
};

class CFixTaskTable
{
private:
	CFixTaskTable(){}
public:
	bool LoadData(CNetTask *pNetTask,const char * addr, short port);
	void UnLoadData();//��ȡ�������
	bool loadTaskBaseTable(CNetTask *pNetTask);

	static CFixTaskTable & Instance()
	{
		static CFixTaskTable ftt;
		return ftt;
	}
	LPMailInfoParam_Item getMailInfoParam(int countryid,int type);
	//��ȡ�ʼ�������
	void loadMailInfoParam();
	void clearMailInfoParam();
	LPCardParam getCardConfig(int itemid);
	int loadCardConfigTable();
	int clearCardConfig();
	PackStoreParam *getPackStoreParam(int postion);
	int loadPackStoreParam();
	void clearPackStoreParam();
	HeroEquipAttribute * getHeroEquipAttribute(int itemid);
	//��ȡװ�����Ա�
	void loadEquipAttribute();
	void clearEquipAttribute();


	//key(TaskID) 
	TaskBaseTable_Map m_BaseTaskMap;
	//key(taskID)
	std::map<unsigned int,HORTATION_Plus> m_HortationMap;
	
	HeroEquipAttribute_Map m_HeroEquipAttributeList; 
	//����
	std::map<int,tagEctypeTask> m_EctypeTaslContainerMap;

	//�ճ�����
	std::map<int,DayTask_DetailInfo> m_DayTaskMap;

private:
	DB_Proxy mProxy;
	//�ʼ�������
	MailInfoParam_Map									m_MailInfoParamMap;
	CardParam_Map										m_CardConfigMap;
	PackStoreParam_Map									m_PackStoreParamMap;
};

#endif