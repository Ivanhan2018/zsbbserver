/*=================================================================*/
/*�з��Ŷ�  : �������·���
/* �ļ���   : Time_Protocol.h
/* �ļ����� : ʱ���¼��ṹ�嶨��
/* ����ʱ�� : 2008-5-7
/* ������   : �����
/* ��ע     :
/*
/*-----------------------------------------------------------------*/
/* �޸ļ�¼1:
/* �޸�ʱ�� :
/* . . .
/*=================================================================*/
#ifndef __TIME_PROTOCOL__
#define __TIME_PROTOCOL__

enum TIME_PROTOCOL
{
	//ʱ���¼�
	EN_TIMER_ARMY_OUT = 0x1301,				//���ӳ���
	EN_TIMER_CONSTRUCTION,					//����������
	EN_TIMER_PRODUCE_WEAPON,				//��������
	EN_TIMER_PRODUCE_DEFEND,				//��������
	EN_TIMER_PRODUCE_ANIMAL,				//����ѱ�޼���
	EN_TIMER_PRODUCE_SPECIAL,				//�������⼼��

	EN_TIMER_RESEARCH_WEAPON,				//�о�����
	EN_TIMER_RESEARCH_DEFEND,				//�о�����
	EN_TIMER_RESEARCH_ANIMAL,				//�о�ѱ�޼���
	EN_TIMER_RESEARCH_SPECIAL,				//�о����⼼��

	EN_TIMER_RESEARCH,						//���е��߼���

	EN_TIMER_LEVYING,						//�������б���...
	EN_TIMER_RECRUITING_HERO,				//������ļӢ��...
	EN_TIMER_ARMY_WARM,						//������Ϣ

	EN_TIMER_HERO_RESUME,					//Ӣ�ۻָ��������˻ָ�������
	EN_TIMER_FIGHT_NOTFIY,					//ս��֪ͨ��Ϣ
	EN_TIMER_LOG_NOTFIY ,                   //��������֪ͨ
	EN_TIMER_CREATENPCARMY,					//����NPC����,
	EN_TIMER_CLEARDAYTASK,					//����ճ�����ʱ��
	EN_TIMER_END = 0x1350

};
#pragma pack(push,1)
//ʱ���¼�ͷЭ��,modify by xujincai 20090205 ��ʱ����Ϣ��Ϊ��ͨ��Ϣ������Ϣ���д���
struct TIMER_HEADER:DISTRIBUTED_HEADER
{
	inline void serialize(int,int);
	int timer_command;				// ʱ��Э������
	long TimerID;					//��ʱ��ID
	bool isDeleted;					//�Ƿ���ɾ����ʱ��
};
struct MsgFightTimer : TIMER_HEADER{
	int uEctypeID;
	inline void serialize(int ectypeRoomID){
		uEctypeID = ectypeRoomID;
		TIMER_HEADER::serialize(sizeof(MsgFightTimer),CMD_FIGHT_TIMER_MSG);
	}
};
//��������
struct OUT_ARMY_PATH : TIMER_HEADER
{
	inline void serialize(unsigned long lPlayerID,unsigned long lArmyID, int lFromMapID, int lToMapID, int lOutMapID, int lTargetMapID,int command, unsigned long officerid/*=0*/);
	unsigned long							lPlayerID;					//������ҵ�ID
	unsigned long							lOfficerID;					//���ӵ�ָ�ӹ�ID
	unsigned long							lArmyID;

	int							lFromMapID;					//��ǰ��ʼ��ID
	int							lToMapID;					//��ǰ�ﵽ��ID

	int							lOutMapID;					//������ID
	int							lTargetMapID;				//Ŀ�꿤ID
	int							iCount;
	int							iMapList[50];
};
//����������
struct BUILD_CONSTRUCTION_NODE : TIMER_HEADER
{
	inline void serialize(unsigned long lCityID, unsigned long lPlayerID, unsigned long lSortID, unsigned long lActionID, unsigned long lConID, 
		int iSLevel, int iLevel, int iAuto, int command);
	unsigned long							lCityID;					//����ID
	unsigned long							lPlayerID;					//
	unsigned long							lSortID;
	unsigned long							lActionID;
	unsigned long							lConID;						//������ID
	int										iSLevel;					//
	int										iLevel;						//�ȼ�
	int										iAuto;						//�й�
};
//��������
struct PRODUCE_ITEM_NODE : TIMER_HEADER
{
	inline void serialize(unsigned long lCityID, unsigned long lPlayerID, int iItemID, int iType, unsigned long lSortID, int iCount, int command);
	unsigned long							lCityID;					//����ID
	unsigned long							lPlayerID;
	int										iItemID;					//����ID
	int										iCount;						//����
	int										iType;						//
	unsigned long							lSortID;
};
//���е���
struct RESEARCH_ITEM_NODE : TIMER_HEADER
{
	inline void serialize(unsigned long lPlayerID, int iType, int iItemID, int iLevel, int iCount, int command);
	unsigned long							lPlayerID;
	int										iType;
	int										iItemID;					//����ID
	int										iLevel;						//���ߵȼ�
	int										iCount;						//����

};
//�б�
struct LEVYING_SOLDIER_LIST : TIMER_HEADER
{
	inline void serialize(unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, int iReceID, int iCount,int sortid, int command);
	unsigned long	lPlayerID;
	unsigned long	lMapID;
	unsigned long	lCityID;
	int				iReceID;
	int				iCount;
	int				iSortID;
};
//������ļӢ��...
struct RECRUITING_HERO : TIMER_HEADER
{
	inline void serialize(unsigned long lPlayerID, int iSortID, int iCountry, int command);
	unsigned long		lPlayerID;
	int					iSortID;						//���к�
	int					iCountry;						//����
};
//������Ϣ
struct ARMY_WARM : TIMER_HEADER
{
	inline void serialize(unsigned long lPlayerID, unsigned long lMapID,unsigned long armyid, int command);
	unsigned long lPlayerID;
	unsigned long lMapID;
	unsigned long lArmyID;
};

//Ӣ�����˻ָ�
typedef struct tagTimer_Hero_Resume:TIMER_HEADER
{
	unsigned int PlayerID;			//���ID
	unsigned int OfficerID;			//Ӣ��ID��ָ�ӹ�ID��
	inline void serialize(unsigned int playerid,unsigned int officerid);
}Timer_Hero_Resume,*LPTimer_Hero_Resume;

struct FightNotfiy:TIMER_HEADER
{
	unsigned int iFightRoomID;
	inline void serialize( unsigned int roomid,int command = EN_TIMER_FIGHT_NOTFIY)
	{
		this->iFightRoomID = roomid;
		TIMER_HEADER::serialize(sizeof(*this),command);
	}
};
struct LOG_ONLINE_NUM_TIMER : TIMER_HEADER
{
	inline void serialize(int command = EN_TIMER_LOG_NOTFIY)
	{
		TIMER_HEADER::serialize(sizeof(*this),command);
	}
};
//����NPC��������EN_TIMER_CREATENPCARMY
struct Timer_CreateNPCArmy : TIMER_HEADER
{
	int CountryID;				//NPC���ӵ���Ӫ
	int Count;					//����
	int Level;					//���ӵļ���
	int FromMap;				//��һ��������ҪΪ����½���ȷ��);
	int TargetMap;				//Ŀ�꿤
	int Times;					//����
	int Interval;				//ʱ��������λ����
	inline void serialize(int countryid,int level,int count,int frommap,int targetmap,int times,int interval);
};
inline void Timer_CreateNPCArmy::serialize(int countryid,int level,int count,int frommap,int targetmap,int times,int interval)
{
	this->CountryID = countryid;
	this->Count = count;
	this->Level = level;
	this->FromMap = frommap;
	this->TargetMap = targetmap;
	this->Times = times;
	this->Interval = interval;
	TIMER_HEADER::serialize(sizeof(*this),EN_TIMER_CREATENPCARMY);
}
struct Timer_ClearDayTask:TIMER_HEADER
{
	inline void serialize();
};
inline void Timer_ClearDayTask::serialize()
{
	TIMER_HEADER::serialize(sizeof(*this),EN_TIMER_CLEARDAYTASK);
}
#pragma pack(pop)


//ʱ���¼�ͷЭ��
inline void TIMER_HEADER::serialize(int length, int command)
{
	//this->length = length;
	this->isDeleted = false;
	this->timer_command = command;
	DISTRIBUTED_HEADER::serialize(length,0,0,0,EN_C_TIMER_NOTIFY);
}

//��������
inline void
OUT_ARMY_PATH::serialize(unsigned long lPlayerID, unsigned long lArmyID, int lFromMapID, int lToMapID, int lOutMapID, int lTargetMapID,int command, unsigned long officerid)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lPlayerID = lPlayerID;
	this->lArmyID = lArmyID;
	this->lFromMapID = lFromMapID;
	this->lToMapID = lToMapID;
	this->lOutMapID = lOutMapID;
	this->lTargetMapID = lTargetMapID;
	this->lOfficerID = officerid;
}
//����������
inline void
BUILD_CONSTRUCTION_NODE::serialize(unsigned long lCityID, unsigned long lPlayerID, unsigned long lSortID,  unsigned long lActionID, unsigned long lConID,
								   int iSLevel, int iLevel, int iAuto,int command)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lSortID = lSortID;
	this->lActionID = lActionID;
	this->lCityID = lCityID;
	this->lPlayerID = lPlayerID;
	this->lConID = lConID;
	this->iSLevel = iSLevel;
	this->iLevel = iLevel;
	this->iAuto = iAuto;
}//��������
inline void
PRODUCE_ITEM_NODE::serialize(unsigned long lCityID, unsigned long lPlayerID, int iItemID, int iType, unsigned long lSortID, int iCount, int command)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lCityID = lCityID;
	this->lPlayerID = lPlayerID;
	this->iItemID = iItemID;
	this->iType = iType;
	this->iCount = iCount;
	this->lSortID = lSortID;
}
//���е���
inline void
RESEARCH_ITEM_NODE::serialize(unsigned long lPlayerID, int iType, int iItemID, int iLevel, int iCount, int command)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lPlayerID = lPlayerID;
	this->iType = iType;
	this->iItemID = iItemID;
	this->iLevel = iLevel;
	this->iCount = iCount;
}
//�б�
inline void
LEVYING_SOLDIER_LIST::serialize(unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, int iReceID, int iCount,int sortid, int command)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lPlayerID = lPlayerID;
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iReceID = iReceID;
	this->iCount = iCount;
	this->iSortID = sortid;
}
inline void 
RECRUITING_HERO::serialize(unsigned long lPlayerID, int iSortID, int iCountry, int command)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lPlayerID = lPlayerID;
	this->iSortID = iSortID;
	this->iCountry = iCountry;
}
//������Ϣ
inline void 
ARMY_WARM::serialize(unsigned long lPlayerID, unsigned long lMapID, unsigned long armyid,int command)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lPlayerID = lPlayerID;
	this->lMapID = lMapID;
	this->lArmyID = armyid;
}

inline void tagTimer_Hero_Resume::serialize(unsigned int playerid,unsigned int officerid)
{
	PlayerID = playerid;
	OfficerID = officerid;
	TIMER_HEADER::serialize(sizeof(*this),EN_TIMER_HERO_RESUME);
}
#endif