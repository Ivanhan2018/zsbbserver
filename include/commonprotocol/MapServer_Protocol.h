/*=================================================================*/
/*�з��Ŷ�  : �������·���
/* �ļ���   : MapServer_Protocol.h
/* �ļ����� : ��Ϸ��������ͻ���ͨ�Žṹ�嶨��
/* ����ʱ�� : 2008-3-27
/* ������   : �����
/* ��ע     :
/*
/*-----------------------------------------------------------------*/
/* �޸ļ�¼1:
/* �޸�ʱ�� :
/* . . .
/*=================================================================*/
#include <memory.h>
#include <string.h>
#include "Distributed_Header.h"
#include "Global_Macro.h"

#pragma warning(disable:4100)
#ifndef GAME_STRUCT_H
#define GAME_STRUCT_H

#define INVALID_USER_ID								(~0)						//��Ч�û�ID

#define	MAX_CONSTRUCTION_NUM						12							//������
#define MAX_CONSTRUCTION_LEVEL						20							//������ȼ�

#define	MAX_WEAPON_NUM								11							//��������
#define	MAX_DEFEND_NUM								3							//��������
#define	MAX_ANIMALS_NUM								5							//Ұ������
#define	MAX_SPECIAL_TOOL_NUM						2							//���⹤������

#define	MAX_WEAPON_LEVEL							5							//�����ȼ�
#define	MAX_DEFEND_LEVEL							5							//���ߵȼ�
#define	MAX_ANIMALS_LEVEL							5							//Ұ�޵ȼ�
#define MAX_SPECIAL_LEVEL							3							//���⹤�ߵȼ�

#pragma pack(push,1)

/*********************************************************************/
//��ͼ����
enum MapType
{
	Country_Map = 0,//����ͼ
	Ectype_Map = 1//������ͼ
};
//������Ϣ��
enum ERROR_INFO_CODE
{
	EN_SUCCESSED = 0,									//�ɹ�
	EN_ERROR_START	= 0x1310,
	
	EN_FOOD_COUNT,										//��ʳ   ��������������
	EN_WOOD_COUNT,										//ľ��
	EN_IRON_COUNT,										//��
	EN_SKIN_COUNT,										//Ƥ
	EN_MONEY_COUNT,										//��Ǯ
	EN_SCIENCE_COUNT,									//�Ƽ���
	EN_RED_STONE_COUNT,									//�챦ʯ
	EN_YELLOW_STONE_COUNT,								//�Ʊ�ʯ
	EN_GREEN_STONE_COUNT,								//�̱�ʯ
	EN_BACK_IRON_COUNT,									//����

	EN_COUNT_EEROR,										//������������Ϸ�
	EN_COMPLETE_TIME_ERROR,								//���ʱ��
	EN_LEVEL_UPPER,										//�ȼ�������Χ

	EN_DATABASE_CONNECT_FAILED,							//���ݿ�����ʧ��
	EN_RECRUIT_OFFICER_PLAYERID,						//��ļָ�ӹ�ʱ,��������ID���������ݿ���
	EN_OFFICER_COUNT_UPPER,								//����ҵ�ָ�ӹ����Ѵﵽ����,��������ļָ�ӹ�
	
	EN_CREATE_ARMY_RACE,								//����  ��������  ��������
	EN_CREATE_ARMY_WEAPON,								//����  �������� 
	EN_CREATE_ARMY_DEFEND,								//����  �������� 
	EN_CREATE_ARMY_JOB,									//�����ְҵ��ƥ��
	EN_CREATE_ARMY_COUNT,								//��������������

	EN_OFFICER_ID_ERROR,								//ָ�ӹ�ID�Ƿ�
	EN_FIND_OFFICER_ERROR,								//�ڱ����Ҳ������ָ�ӹ�

	EN_TECH_ID_ERROR,									//����ID �Ƿ�
	EN_TECH_COUNT_UPPER,								//Ŀǰ������ѧϰ������

	EN_DATA_ERROR,										//���ݿ�û�д�����

	EN_CONSTRUCTION_UPPER,								//�����������������޲����ٽ���

	EN_PRODUCE_ITEM_UPPER,								//�������� ���ж��г������� ��ʱ�����������������
	EN_ITEM_TYPE_ERROR,									//�������Ͳ�������Ч��Χ��

	EN_CLIENT_DATA_ERROR,								//�ͻ��˷��͹����ĵ�������Ч

	EN_RACE_COUNT,										//Ŀǰ�����д�����������        �б�
	EN_LEVY_FOOD_COUNT,									//��ʳ����
	EN_LEVY_MONEY,										//��Ǯ����
	EN_LEVY_RACE_ID,									//����ID����ȷ
	EN_LEVY_COUNT,										//������������
	EN_OPERATE_DBG_FAILED,								//����DBGʧ��

	EN_ITEM_ID_ERROR,									//����ID���Ͳ�����Ч��Χ��

	EN_ARMY_TRANSFER,									//����ת��ʧ��

	EN_COMEIN_NEW_COUNTRY,								//�������ִ�
	EN_USER_ACCOUNTS_ERROR,								//�û��ʺŲ���ȷ

	EN_BUILD_CONSTRUCTION_ERROR,						//�����Ӧ�Ľ����� �ſ�����
	EN_RESEARCH_ITEM_ERROR,								//���ж�Ӧ�Ŀ��м��� �ſ�����
	EN_RESEARCH_ITEM_LOWER,								//������Ҫ���е׼��Ŀ��м���
	EN_NO_BULID_CASERN,									//������Ҫ�����Ӫ��������

	EN_RECRUITING_HERO,									//������ļӢ��...
	EN_NOT_HERO_ID,										//û�д�Ӣ��ID
	EN_CANNOT_DISMISS_HEADER_HERO,						//���ܿ�������Ӣ��
	EN_HERO_LEISURE,									//���� 
	EN_HERO_NOT_LEISURE,								//�ǿ��� 
	EN_HERO_HAVE_ARMY,									//����
	EN_HERO_GBH,										// ���� 
	EN_HEADER_HERO_BY_CAPTIVE,							//����Ӣ�۱��� 
	EN_HERO_BY_CAPTIVE,									//���� 
	EN_HERO_CAPTIVE,									//��²
	EN_TECH_DOT_NOT,									//���ܵ㲻��
	EN_SCIENCE_DOT_NOT,									//�Ƽ��㲻��
	EN_LEVEL_NOT_ENOUGH,								//Ӣ�۵ȼ�����������
	EN_STAMINA_NOT_ENOUGH,								//Ӣ����������
	EN_POWER_NOT_ENOUGH,								//Ӣ����������
	EN_CELERITY_NOT_ENOUGH,								//Ӣ�����ݲ���
	EN_WIT_NOT_ENOUGH,									//Ӣ����������

	EN_NOT_ARMY_ID,										//����ID������
	EN_NOT_TRANSIT_ARMY,								//�������䲿��
	EN_NOT_COLONIZE_ARMY,								//����ֳ�񲿶�
	EN_ARMY_NO_CONTENT,									//������������
	EN_CITY_NO_CONTENT,									//������������
	EN_NOT_SELF_MAP,									//�Ǳ�����
	EN_HAVE_CITY,										//�ѿ������г���
	EN_COLONIZE_CITY_FAILED,							//ֳ�����ʧ��
	EN_COLONIZE_CITY,									//ֳ�����ok
	EN_CONTRIBUTE_OK,									//����ok
	EN_CONTRIBUTE_1000,									//���׽�Ǯ��������1000
	EN_COLONIZE_UP_10,									//ֳ�����ʱ,����������10��
	EN_MAX_ARMYCOUNT,									//�ѵ����ر�����������
	EN_NOT_CREATGARRISON_INSAFEMAP,						//��ȫ�岻�ܴ������ر���
	EN_NOT_REACHMAP,									//�޷����������
	EN_NOT_ACCTIONDOT,									//�ж�ֵ����
	EN_PACKFULL,										//��������
	EN_JOBNOMATCHING,									//ְҵ��ƥ��
	EN_SKILL_CANTSTUDY,									//�ǿ�ѧϰ����
	EN_MAP_INWAR,										//����ս��״̬�У��޷����졢����
	EN_OFFICER_INWAR,									//Ӣ����ս���У����ܲ���װ���ͼ���
	EN_PACKPOS_HAVEITEM,								//��λ��������Ʒռ��
	EN_NO_ExpeditionSpeedCard,							//�޼��ٿ�,���ٳ���ʧ��
	EN_OFFICER_NOSKILL,									//Ӣ���޿��������
	EN_NO_HAVECARD,										//��������
	EN_HERONAMEEXIST,									//Ӣ�����Ѵ���
	EN_HAVEDRESSEQUIPOFPOSTION,							//��װ��ʱ��Ӣ���Ѵ��ϸ����͵�װ��
};
//����,����,���гɹ���Ϣ��
enum SUCCEED_INFO_CODE
{
	EN_SUCCEED_START = 0x1411,

	EN_PRODUCE_WEAPON,									//���������ɹ�
	EN_PRODUCE_DEFEND,									//��������
	EN_PRODUCE_ANIMAL,									//����ѱ�޼���
	EN_PRODUCE_SPECIAL,									//�������⹤��

	EN_RESEARCH_WEAPON,									//���������ɹ�
	EN_RESEARCH_DEFEND,									//���з���
	EN_RESEARCH_ANIMAL,									//����ѱ�޼���
	EN_RESEARCH_SPECIAL,								//�������⼼��

	EN_BUILD_CONSTRUCTION,								//�����ɹ�
	EN_CANCEL_BUILD_CONSTRUCTION,						//ȡ�������ｨ��
	EN_DEMOLISH_CONSTRUCTION,							//��ٽ�����
	EN_DEMOLISH_A_CONSTRUCTION,							//���A������

	EN_BUILDING_CON,									//�˽��������ڽ�����
	EN_PRODUCEING_ITEM,									//�˵�������������
	EN_RESEARCHING_ITEM,								//�˵��߼������ڿ���

	EN_START_BUILD_CON,									//��ʼ�����ｨ��
	EN_START_PRODUCE,									//��ʼ����
	EN_START_RESEARCH,									//��ʼ����

	EN_WAIT_BUILD_CON,									//���ｨ�������ڵȴ���...
	EN_WAIT_PRODUCE_ITEM,								//�����������߽���ȴ��ж���...
	EN_WAIT_RESEARCH_ITEM,								//�˿��м�������ȴ��ж���...
	
	EN_CANCEL_PRODUCE_ITEM,								//ȡ����������

	EN_CANCEL_RESEARCH_ITEM,							//ȡ�����е���

	EN_SUCCEED_END = 0x1500
};
//�г���Ϣ��
enum MARKET_INFO_CODE
{
	EN_MARKET_INFO_START = 0x1501,
	EN_GOODS_SALED,										//������Ʒ�ѳ��ۻ��ѹ���
	EN_GOODS_PRICE,										//������Ʒ�۸��Ѹı�,�뼰ʱ����
	EN_GOODS_COUNT,										//������Ʒ��������
	EN_GOODS_BASE_PRICE,								//������Ʒ�׼۲���ȷ
	EN_GOODS_ONCE_PRICE,								//������Ʒһ�ڼ۲���ȷ
	EN_GOODS_BIDDING_FAILED,							//������Ʒ��������
	EN_GOODS_BIDDING_SUCCEED,							//������Ʒ���۳ɹ�
	EN_GOODS_HANG_SUCCEED,								//������Ʒ���Ƴɹ�
	EN_GOODS_HANG_FAILED,								//������Ʒ����ʧ��
	EN_GOODS_NOT_ENOUGHT_GOLD,							//���������Ʒʱ,��Ǯ����
	EN_GOODS_BUY_SUCCEED,								//���׳ɹ�
	EN_GOODS_BUY_FAILED,								//����ʧ��
	EN_GOODS_SALE_SUCCEED,								//��  ���׳ɹ�
	EN_GOODS_SALE_FAILED,								//��  ����ʧ��
	EN_GOODS_COUNT_ERROR,								//��������������Ƿ�
	EN_BUY_SUCCEED,										//��ɹ�
	EN_BUY_FAILED,										//��ʧ��
		
	EN_ALLOW_BIDDEN,									//��������������
	EN_BIDDEN_ERROR,									//����С�ڵ�ǰ��
	EN_BIDDEN_OWN,										//���ܸ��Լ�����Ʒ����
	EN_STORAGE_CONTENT_ERROR,							//�˳����ڵĲֿ���������
	EN_NOT_CANSALE,										//������
	EN_MARKET_INFO_END = 0x1520
};
//Begin �������б��

//���еı�Ų���:  ������.xls

//����ͼ���(Laiyi)
enum COUNTY_NO
{
	COUNTY_UNION_NEW=1001,   //�������ִ�
	COUNTY_CLAN_NEW =1004,   //�������ִ�
	COUNTY_WAR_SMZL =1003,   //��ս��(����֮��)
   //...


};

//���еĳ��б�� 
enum CITY_NO
{
	//�������ִ�(1001)�е�6������
	CITY_UNION_NEW1 =1,
	CITY_UNION_NEW2 =2,
	CITY_UNION_NEW3 =3,
	CITY_UNION_NEW4 =4,
	CITY_UNION_NEW5 =5,
	CITY_UNION_NEW6 =6,
	
    //�������ִ�(1002)����
	CITY_CLAN_NEW1 = 7,
	CITY_CLAN_NEW2 = 8,
	CITY_CLAN_NEW3 = 9,
	CITY_CLAN_NEW4 = 10,
	CITY_CLAN_NEW5 = 11,
	CITY_CLAN_NEW6 = 12,

	//....

};
//End �������б��
//*****************************************************************/

/**********************************************************************************************
//���ͼ�������ṹ��
**********************************************************************************************/
struct InlayInfo
{
private:
	unsigned int ItemID:24;
	unsigned int Inlay0:8;
	unsigned int Inlay1:8;
	unsigned int Inlay2:8;
	unsigned int Inlay3:8;
	unsigned int Inlay4:8;
public:
	void SetItemID(int itemid)
	{
		Inlay0 = Inlay1 = Inlay2 = Inlay3 = Inlay4 = 0;
		ItemID = itemid;
	}
	unsigned int GetItemID() const
	{
		return ItemID;
	}
	unsigned int GetInlayCount() const
	{
		int count = 0;
		if (Inlay0>0)
		{
			count++;
		}
		if (Inlay1>0)
		{
			count++;
		}
		if (Inlay2>0)
		{
			count++;
		}
		if (Inlay3>0)
		{
			count++;
		}
		if (Inlay4>0)
		{
			count++;
		}
		return count;
	}
	void GetInlayInfo(int *addinfo)
	{
		addinfo[0]=Inlay0;
		addinfo[1]=Inlay1;
		addinfo[2]=Inlay2;
		addinfo[3]=Inlay3;
		addinfo[4]=Inlay4;
	}
	int Inlay(int itemid)
	{
		if(Inlay0 <=0)
		{
			Inlay0 = itemid;
			return 1;
		}
		if(Inlay1 <=0)
		{
			Inlay1 = itemid;
			return 2;
		}
		if(Inlay2 <=0)
		{
			Inlay2 = itemid;
			return 3;
		}
		if(Inlay3 <=0)
		{
			Inlay3 = itemid;
			return 4;
		}
		if(Inlay4 <=0)
		{
			Inlay4 = itemid;
			return 5;
		}
		return 0;
	}
};
union Inlay_Union
{
	__int64 NewItemID;
	InlayInfo DetailInfo;
};
//��Ϸ�е�ʱ��
struct GAME_TIME : DISTRIBUTED_HEADER
{
	inline void serialize(long long lGameTime, int from, int to, int command);
	long long lGameTime;//��Ϸ�е�ʱ��(��λ:��)
};
//���Ӵﵽ
struct ARMY_REACH
{
	unsigned long			lMapID;						//��ID
	unsigned long			lArmyID;					//����ID
};

//������Ϣ
struct PLAYER_BASE_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lPlayerID, char Name[], int iFace, unsigned long lConsortiaID, long lExp, int iRaceID, 
		int iJobID, int iCredit, int iLevel, int iMilitaryRank, unsigned long lGameTime,int iCountry, long lScience, long lMoney,
		int from=0, int to=0);

	unsigned long		lPlayerID;						//���ID
	char				szNick[SIZE_32];				//�ǳ�
	int					iFace;							//ͷ��
	unsigned long		lConsortiaID;					//����ID
	long				lExp;							//����ֵ
	int					iRaceID;						//����
	int					iJobID;							//ְϵ
	int					iCredit;						//������
	int					iLevel;							//�ȼ�
	int					iMilitaryRank;					//����
	unsigned long		lGameTime;						//��Ϸʱ��
	int					iCountry;						//1:����  2: ����
	long				lScience;						//�Ƽ���
	long				lMoney;							//��Ǯ
	
	void operator = (PLAYER_BASE_INFO& pInfo)   //�����=������  Laiyi  
	{
		this->lPlayerID=pInfo.lPlayerID;
		strcpy(this->szNick,pInfo.szNick);
		this->iFace=pInfo.iFace;
		this->lConsortiaID=pInfo.lConsortiaID;
		this->lExp=pInfo.lExp;
		this->iRaceID=pInfo.iRaceID;
		this->iJobID=pInfo.iJobID;
		this->iCredit=pInfo.iCredit;
		this->iLevel=pInfo.iLevel;
		this->iMilitaryRank=pInfo.iMilitaryRank;
		this->lGameTime=pInfo.lGameTime;
		this->lScience=pInfo.lScience;
		this->lMoney=pInfo.lMoney;
		
		//�����Ա
		strcpy(this->http_tunnel,pInfo.http_tunnel);
		this->length=pInfo.length;
		this->agent=pInfo.agent;
		this->from=pInfo.from;
		this->to=pInfo.to;
		this->command=pInfo.command;
		
	
	}

};
//�����б���Ϣ
typedef struct
{
	short							iTypeID;				//����ID
	unsigned long					lMapID;					//��ID
	unsigned long					lCityID;				//����ID
	bool							bState;					//״̬
} tagCityList;
//��״̬��Ϣ
struct tagMapState
{
	unsigned long lMapID;									//��ID
	int	iCountry;											//����ID  //1:����  2: ����
	int iState;												//״̬ //1: ս��״̬
};
struct MAP_STATE_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to, int command=0);
	inline tagMapState & operator [](unsigned int);
};
struct CITY_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagCityList & operator [](unsigned int);
};
//��Կ��֤
struct MAP_SERVER_VALIDATE : DISTRIBUTED_HEADER
{
	inline void serialize(char const szAccounts[], char const szValidate[], int from=0, int to=0);
	char				szAccounts[SIZE_16];		//�˺�
	char				szValidate[SIZE_16];		//��֤��
};
//��ҳ��иı�
struct PLAYER_CITY_MODFIY : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, int from=0, int to=0);
	unsigned long					lMapID;					//��ID
	unsigned long					lCityID;				//����ID
	int								iState;					//1:�õ�һ������,0:ʧȥһ������
};
//��ҳ����б���Ϣ
struct tagPlayerCityList
{
	unsigned long					lMapID;					//��ID
	unsigned long					lCityID;				//����ID
	short							lState;					//����״̬��100�����ִ�
};
struct PLAYER_CITY_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagPlayerCityList & operator [](unsigned int);
};

//��������������
struct tagCityAttributeList
{
	int									iConID;								//������ID
	int									iLevel;								//�ȼ�
	int									iContribute;						//���׶�
	int									ihp;								//HP
	int									iRecovery;							//����
	int									iAttack;							//����
	int									iX;									//����������
	int									iY;									//
};
struct CITY_ATTRIBUTE_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagCityAttributeList & operator [](unsigned int);
};
//������Ϣ
struct CITY_INFO : DISTRIBUTED_HEADER
{
	unsigned long					lMapID;					//��ID
	unsigned long					lCityID;				//����ID
	inline void serialize(unsigned long lMapID, unsigned long lCityID,int from=0, int to=0, int command=0);
};
//������Ϣ
struct REQ_CITY_INFO : DISTRIBUTED_HEADER
{
	int								ectypeID;				//����ID 
	unsigned long					lMapID;					//��ID
	unsigned long					lCityID;				//����ID
	inline void serialize(unsigned long lMapID, unsigned long lCityID,int from=0, int to=0, int command=0);
};
//���뿤
struct COMEIN_MAP : DISTRIBUTED_HEADER
{
	unsigned long					lMapID;					//��ID
	inline void serialize(unsigned long lMapID, int from=0, int to=0);
};
//��ҵı�����
struct PLAYER_SOLDIER_LIST : DISTRIBUTED_HEADER
{
	inline void serialize(int iHuman, int iFairy, int iMannikin, int iDandiprat, int iHalfAnimal, int iCattle, int iBlackFairy, int iDecPerson, int from, int to);
	int								iHuman;					//����
	int								iFairy;					//����
	int								iMannikin;				//����
	int								iDandiprat;				//٪��
	int								iHalfAnimal;			//������
	int								iCattle;				//ţͷ��
	int								iBlackFairy;			//�ھ���
	int								iDecPerson;				//����

};
//��ҵ���Դ��
struct PALEYER_RESOURCE : DISTRIBUTED_HEADER
{
	inline void serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,int iRedStoneCount,
		int iYellowStoneCount,int iGreenStoneCount,int iBackIronCount,int iScience,int iMoney, int iContribute,
		int content,int from/*=0*/, int to/*=0*/);
	int								iFoodCount;				//��ʳ����
	int								iWoodCount;				//ľ������
	int								iIronCount;				//������
	int								iSkinCount;				//Ƥ����
	int								iRedStoneCount;			//�챦ʯ
	int								iYellowStoneCount;		//�Ʊ�ʯ
	int								iGreenStoneCount;		//�̱�ʯ
	int								iBackIronCount;			//����
	int								iMoney;					//
	int								iScience;				//�Ƽ���
	int								iContribute;			//���׶�
	int								iContent;				//�ֿ�ʣ������
};
//��ҵĻ�����Դ
struct PALEYER_RESOURCE_LIST : DISTRIBUTED_HEADER
{
	inline void serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,int iRedStoneCount,
		int iYellowStoneCount,int iGreenStoneCount,int iBackIronCount,int content,int from, int to);
	int								iFoodCount;				//��ʳ����
	int								iWoodCount;				//ľ������
	int								iIronCount;				//������
	int								iSkinCount;				//Ƥ����
	int								iRedStoneCount;			//�챦ʯ
	int								iYellowStoneCount;		//�Ʊ�ʯ
	int								iGreenStoneCount;		//�̱�ʯ
	int								iBackIronCount;			//����
	int								iContent;				//�ֿ�ʣ������
};
//��������
struct LEVY_SOLDIER_COUNT : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID,unsigned long lCityID,int iRace,int iCount,int from=0, int to=0);
	unsigned long						lMapID;					//��ID
	unsigned long						lCityID;				//����ID
	int									iRace;					//����ID
	int									iCount;					//����
};
//��������״̬��
struct LEVY_SOLDIER_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iRace,int iCount, int iState, int from, int to);
	int									iRace;					//����ID
	int									iCount;					//����
	int									iState;					//����״̬ 1:�������б� 2: �ȴ���...
};
//����ָ�ӹ���Դ����
struct Officer_Resource_Info
{
	unsigned long 	lPlayerID;					//���ID
	char			Accounts[SIZE_32];			//�ǳ�
	long int		lResource;					//��Դ����

	Officer_Resource_Info()
	{
		InitData();
	}
	void InitData()
	{
		memset(this,0,sizeof(Officer_Resource_Info));
		lPlayerID = (unsigned long)INVALID_USER_ID;
	}
};

//��ȡ����ͼ��Դ��Ϣ
struct tagGet_Map_Info
{
	unsigned long					lMapID;					//��ID
};

//����ͼ��Դ��Ϣ
struct tagSend_Map_Info
{
	int					iFood;					//��ʳ
	int					iWood;					//ľ��
	int					iIron;					//��
	int					iSkin;					//Ƥ
};

//������Ϣ
struct MAP_CITY_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,
		int iHuman,int iMannikin,int iFairy,int iDandiprat,int iHalfAnimal,int iCattle,int iBlackFairy,int iDecPerson,
		int from=0, int to=0);
	int										iFoodCount;							//��ʳ
	int										iWoodCount;							//ľ��
	int										iIronCount;							//��
	int										iSkinCount;							//Ƥ

//	int										iMoney;
	int										iHuman;					//����
	int										iFairy;					//����
	int										iMannikin;				//����
	int										iDandiprat;				//٪��
	int										iHalfAnimal;			//������
	int										iCattle;				//ţͷ��
	int										iBlackFairy;			//�ھ���
	int										iDecPerson;				//����
};

//Ӣ��ϵͳ��Ϣ
struct HERO_SYSTEM_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iCode, int from, int to, int command);
	int iCode;//��Ϣ��
};
//����ָ�ӹ�
struct CREATE_OFFICER : DISTRIBUTED_HEADER
{
	inline void serialize(char szNick[], int iFace, int iCountry, int iRaceID, int iJobID, int from=0, int to=0);
	char				szNick[SIZE_16];				//�ǳ�
	int					iFace;							//ͷ��
	int					iCountry;						//����
	int					iRaceID;						//���� 1:����;2:����;3:����;4:٪��;5:������;6:ţͷ��;7:�ھ���;8:����;
	int					iJobID;							//ְҵ: 1:ʥ��ʿ,2:սʿ,3:������,4:��ʦ,5:��ʦ,
};
//����ָ�ӹ�ʧ��
struct CREATE_OFFICER_FAILD : DISTRIBUTED_HEADER
{
	inline void serialize(int iError, int from=0, int to=0, int command=0);
	int					iError;							//������ 1:����ID������Ч��Χ�� 2: ����ID������Ч��Χ�� 3:Ӣ�������ѱ�ռ�� 5: ��������ʧ�� 6:ְҵID������Ч��Χ��.7:������ְҵ����Ӧ
};

//����Ӣ��
struct DISMISS_HERO : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lHeroID, int from, int to);
	unsigned long lHeroID;//Ӣ��ID
};
//����Ӣ����Ϣ
struct DISMISS_HERO_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iCode, int from, int to, int command);
	int iCode;//��Ϣ��
};
//��ȡ������Ϣ
struct GET_ARMY_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lArmyID,int from=0, int to=0);
	unsigned long						lMapID;								//��ID
	unsigned long						lCityID;							//����ID
	unsigned long						lArmyID;							//����ID
};
//���ָ�ӹ��б���Ϣ
struct tagPlayer_Officer_List
{
	unsigned long lOfficerID;
	char szOfficerName[SIZE_32];
	int									iLevel;
	int									iFaceID;						//ͷ��
	int									iRaceID;						//����
	int									iJobID;							//ְϵ
	int									iMilitaryRank;					//����
	int									iState;							//״̬ 0:û�д����� 1:���в��� 2,���� 3,���� 4 ��² (�μ� �ṹ��HERO_STATE)
	int									iTechDon;						//����
	int									iType;							//�������� 0:����ļ��Ӣ�� 1:������ļ�е�Ӣ��;....
	long								lTime;							//����೤ʱ�����
};
struct PLAYER_OFFICER_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0, int command=0);
	inline tagPlayer_Officer_List & operator [](unsigned int);
};
//��������
struct CREATE_ARMY : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lOfficerID, char szName[],int iRaceID, int iSoldiertype, int iFSoldierCount, 
		int iFWeaponID, int iWeaponLevel,int iDefendID, int iDefendLevel,  int iX, int iY, int from=0, int to=0);
	unsigned long							lMapID;					//��ID
	unsigned long							lCityID;					//����ID
	unsigned long							lOfficerID;				//ָ�ӹ�

	char									szName[SIZE_32];		//��������
	int										iRaceID;				//����
	int										iSoldiertype;			//ְҵ
	int										iFSoldierCount;			//����
	int										iFWeaponID;				//����
	int										iWeaponLevel;			//��������
	int										iDefendID;				//����
	int										iDefendLevel;			//���߼���
	int										iX;						//
	int										iY;
};
//��������ʧ��
struct CREATE_ARMY_FAILD : DISTRIBUTED_HEADER
{
	inline void serialize(int iError, int from=0, int to=0, int command=0);
	int										iError;						//1:����; 2:����; 3:����;�������� 4:����,������ƥ��;5:��������������,6:ָ�ӹ�ID �Ƿ�,7 ����ID �Ƿ� 8 ����ID �Ƿ� 9:����ר�����ܼ��𲻹�����Ϊѧ����10������ר�����ܼ��𲻹�����Ϊѧ��;
};
//��ಿ��
struct EDIT_ARMY : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lOfficerID, 
		int iArmyID, int iDefendID, int iCount, int from=0, int to=0);
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	unsigned long							lOfficerID;					//���ڵ�ָ�ӹ�ID
	int										iArmyID;					//����ID
	int										iDefendID;					//����ID
	int										iCount;						//����
	char									szName[SIZE_32];			//�²�������
};

//��ɢ����EN_C_UNLAYARMY,EN_S_UNLAYARMY,
typedef struct tagUnlayArmy_Request:DISTRIBUTED_HEADER
{
	//int MapID;							//��ǰ��ID
	int ArmyID;							//����ID
	inline void serialize(int playerid,int mapid,int armyid);
}UnlayArmy_Request,*LPUnlayArmy_Request;

typedef struct tagUnlayArmy_Response:DISTRIBUTED_HEADER
{
	int Error_Code;						//0:�ɹ���1��û�б��˾ݵ㣬�޷���ɢ��2��ս��״̬�У��޷���ɢ��3:���Ӳ����ڣ�4:�ֿ���������������װ����������5:���䲿��װ�������ʣ����ܽ�ɢ��������ʧ��
	inline void serialize(int agent,int mapid,int playerid,int code);
}UnlayArmy_Response,*LPUnlayArmy_Response;

//ɾ������EN_C_DELETEARMY,EN_S_DELETEARMY,
typedef struct tagDeleteArmy_Request:DISTRIBUTED_HEADER
{
	//int MapID;						//���ڿ�ID
	int ArmyID;						//����ID
	inline void serialize(int playerid,int mapid,int armyid);
}DeleteArmy_Request,*LPDeleteArmy_Request;

typedef struct tagDeleteArmy_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//0:�ɹ���1:���Ӳ����ڣ�������ʧ��
	inline void serialize(int agent,int mapid,int playerid,int code);
}DeleteArmy_Response,*LPDeleteArmy_Response;

//���ӳ����б�
struct ARMY_OUT_LIST : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lTargetMapID, unsigned long  iArmyID, int from=0, int to=0);
	unsigned long							lMapID;						//��������ID
	unsigned long							lCityID;					//����������ID
	unsigned long							lTargetMapID;				//����Ŀ�꿤ID
	unsigned long							iArmyID;					//����ID 
	//int										iArmy2ID;					//����ID
	//int										iArmy3ID;					//����ID
	//int										iArmy4ID;					//����ID
	//int										iArmy5ID;					//����ID
};
// ��������·��
struct tagArmyMovePath 
{
	int									iRMapID;				//��ǰ��ʼ��ID
	int									iDMapID;				//��ǰ�ﵽ��ID
	int									iAllTime;				//��ʱ�� 
	int									iTime;					//��Ҫ�೤ʱ��ﵽĿ�꿤
};
//�����ڳ����еĲ����б�
struct tagOutingArmysList
{
	char						szArmyName[SIZE_32];	//��������
	short						iRaceID;				//����
	short						iSoldierType;			//
	short						iSoldierCount;			//
	unsigned long				lOfficerID;				//ָ�ӹ�ID
	char						szOfficerName[SIZE_32];	//ָ�ӹ�����
	unsigned long				lArmyID;				//����ID
	short						iItem;					//����������
	short						iFMapID;				//������ID
	short						iTMapID;				//Ŀ�꿤ID
	tagArmyMovePath				Path[50];				//ÿ��������������ID
};
struct ARMY_MOVE_PATH : DISTRIBUTED_HEADER
{
	int							iCount;					//��������
	inline void serialize(int count, int from, int to, int command);
	inline tagOutingArmysList & operator [](unsigned int);
};
//���Ӳ��ܾ���
struct ARMY_CANT_MOVE : DISTRIBUTED_HEADER
{
	inline void serialize(int iCode, int from, int to);
	int iCode;			//��Ϣ�� 1 ���ܿ�������ս��״̬�� 2 ���ܿ�п�
};
//����ת��
struct ARMY_TRANSFER : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lRMapID, unsigned long lTargetMapID, unsigned long  lArmyID, int from=0, int to=0);
	unsigned long							lRMapID;					//ת����ʼ��ID
	unsigned long							lTargetMapID;				//����Ŀ�꿤ID
	unsigned long 							lArmyID;					//����ID 
};
//��ٽ�����
struct DEMOLISH_CONSTRUCTION : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID,int from=0, int to=0);
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	unsigned long							lPlayerID;					//���ָ����ҵ�ID
};
//�������б�
struct tagConstructionList
{
	unsigned long							lConstructionID;			//������ID
	int										iLevel;						//�����ȼ�
	int										iModify;					//����
	int										iFoodCount;
	int										iWoodCount;
	int										iIronCount;
	int										iSkinCount;
	int										iScience;
	int										iMoney;
	int										iCompleteTime;				//������ɵ�ʱ��
	int										bBuild;						//���㽨������
	int										iHave;						// 1: �����н�������ѽ�����
};
struct CONSTRUCTION_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagConstructionList & operator [](unsigned int);
};
//����ѽ����õĽ�����
struct tagPlayerConstrList
{
	unsigned long							lSortID;					//ID  ����ǽ��� ��ֵΪ:0 �����Ǵӷ�����ת�������Ǹ�ֵ
	unsigned long							lConstrID;					//������ID
	int										iLevel;						//�����ȼ�
	int										iUpGrade;					//1 : ������������
};
//����ѽ����õĽ�����
struct PLAYER_CONSTR_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagPlayerConstrList & operator [](unsigned int);
};
//������ڽ����еĽ�����
struct tagBuildingConstrList
{
	unsigned long							lSortID;
	unsigned long							lConstrID;					//������ID
	int										iLevel;						//�����ȼ�
	int										iTime;						//����Ҫ����ʱ����� ��
};
//������ڽ����еĽ�����
struct BUILDING_CONSTR_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagBuildingConstrList & operator [](unsigned int);
};
//ȡ�������ｨ��
struct CANCEL_BUILD : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lSortID,unsigned long lConID,int iLevel, int from=0, int to=0);
	unsigned long							lSortID;
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	unsigned long							lConID;						//������ID
	int										iLevel;						//�����ȼ�
};
//ȡ����������
struct CANCEL_PRODUCE : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lSortID,int iType, int iItemID,int iLevel, int iCount, int from=0, int to=0);
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	unsigned long							lSortID;					//��ˮ��
	int										iType;						//�������� 1,����,2,����,3,Ұ��ѱ��,4,���⹤��
	int										iItemID;					//����ID
	int										iLevel;						//���ߵȼ�
	int										iCount;						//����
};
//ȡ�����е���
struct CANCEL_RESEARCH : DISTRIBUTED_HEADER
{
	inline void serialize(int iType, int iItemID,int iLevel,int from=0, int to=0);
	int										iType;						//�������� 1,����,2,����,3,Ұ��ѱ��,4,���⹤��
	int										iItemID;					//����ID
	int										iLevel;						//���ߵȼ�
}; 
//�㲥������Ϣ
struct BROADCAST_CONTRIBUTE : DISTRIBUTED_HEADER
{
	inline void serialize(char szPlayerName[], unsigned long lMapID,unsigned long lCityID, int iLevel, int iType, int iMoney,int iCredit, int from=0, int to=0);
	char									szPlayerName[SIZE_24];
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	int										iLevel;						//������ȼ�
	int										iType;						//����������
	int										iMoney;						//��Ǯ 
	int										iCredit;					//������
};
//������Ϣ
struct CONTRIBUTE_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lCityID, int iLevel, int iCurMoney, int iTolMoney, int iAttack, int iHP, int iAttackScop, int iVisualRange, long lCMoney, long lPMoney, int from, int to);
	unsigned long lCityID;
	int iLevel;
	int iCurMoney;
	int iTolMoney;
	int iAttack;
	int iHP;
	int iAttackScop;
	int iVisualRange;
	long lCMoney;
	long lPMoney;
};
//����
struct PLAYER_CONTRIBUTE : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID,unsigned long lCityID, int iMoney, int iType, int from=0, int to=0);
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	int										iType;						//�������� ������:-1
	int										iMoney;						//��Ǯ 
};
//����������
struct BUILD_CONSTRUCTION : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID,unsigned long lCityID,unsigned long lSortID,unsigned long lConstructionID,int iLevel,int from=0, int to=0, int command=0);
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	unsigned long							lSortID;					//ID  ����ǽ��������� ��ֵΪ:0 ����(����/���)�Ǵӷ�����ת�������Ǹ�ֵ
	unsigned long							lConstructionID;			//������ID
	int										iLevel;						//�����ȼ�
};
//�о�
struct	ITEM_RESEARCH : DISTRIBUTED_HEADER
{
	inline void serialize(int from=0, int to=0, int command=0);
};
//�����о�
struct tagItemResearch
{
	int										iType;						//�������� 1,����,2,����,3,Ұ��ѱ��,4,���⹤��
	int										iItemID;					//����ID
	int										iLevel;						//���ߵȼ�
	int										iCount;						//����
	int										bResearch;					//������һ�ȼ���Դ����Ϊ 1
};
//�Ѿ��о��˵ĵ����б�
struct ITEM_RESEARCH_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0, int command=0);
	inline tagItemResearch & operator [](unsigned int);
};
//�����о��еĵ���
struct tagResearchingItemList
{
	unsigned long							lSortID;
	int										iType;						//�������� 1,����,2,����,3,Ұ��ѱ��,4,���⹤��
	int										iItemID;					//����ID
	int										iLevel;						//���ߵȼ�
	int										iCompletetime;				//����Ҫ����ʱ�����ʱ��  ��
};
//�����о��еĵ���
struct RESEARCHING_ITEM_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0, int command=0);
	inline tagResearchingItemList & operator [](unsigned int);
};
//�о������б�
struct RESEARCH_WEAPON_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0);
	inline tagItemResearch & operator [](unsigned int);
};
//�о������б�
struct RESEARCH_DEFEND_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0);
	inline tagItemResearch & operator [](unsigned int);
};
//�о�ѱ�޼����б�
struct RESEARCH_ANIMAL_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0);
	inline tagItemResearch & operator [](unsigned int);
};
//�о����⼼���б�
struct RESEARCH_SPECIAL_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0);
	inline tagItemResearch & operator [](unsigned int);
};
//�о�����
struct RESEARCH_ITEM : DISTRIBUTED_HEADER
{
	inline void serialize(int iType, int iItemID, int iLevel, int from=0, int to=0, int command=0);
	int										iType;						//�������� 1,����,2,����,3,Ұ��ѱ��,4,���⹤��
	int										iItemID;					//����ID
	int										iLevel;						//���ߵȼ�
};
//���������еĵ���
struct tagProduceingItemList
{
	unsigned long							lSortID;				//��ˮ��
	int										iItemID;				//����ID
	int										iLevel;					//�ȼ�
	int										iType;					//�������� 1,����,2,����,3,Ұ��ѱ��,4,���⹤��
	int										iCount;					//����
	int										iCompletetime;			//����Ҫ����ʱ�����ʱ��  ��
};
//���������еĵ���
struct PRODUCEING_ITEM_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to,int command);
	inline tagProduceingItemList & operator [](unsigned int);
};
//������
struct tagProduction
{
	int										iItemID;			//����ID
	int										iLevel;					//�ȼ�
	int										iType;					//�������� 1,����,2,����,3,Ұ��ѱ��,4,���⹤��
	int										iFoodCount;
	int										iWoodCount;
	int										iIronCount;
	int										iSkinCount;
	int										iRedStoneCount;			//�챦ʯ
	int										iYellowStoneCount;		//�Ʊ�ʯ
	int										iGreenStoneCount;		//�̱�ʯ
	int										iBackIronCount;			//����
	int										iMoney;
	int										iScience;				//�Ƽ���
	bool									bPro;					//������������
	int										iCompletetime;			//���ʱ��
};
//���������б�
struct PRODUCTION_WEAPON_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagProduction & operator [](unsigned int);
};
//���������б�
struct PRODUCTION_DEFEND_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagProduction & operator [](unsigned int);
};
//ѱ�޼�������
struct PRODUCTION_ANIMAL_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagProduction & operator [](unsigned int);
};
//���⹤������
struct PRODUCTION_SPECIAL_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagProduction & operator [](unsigned int);
};
//��������
struct PRODUCT_ITEM : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID,unsigned long lCityID, int iType, int iItem, int iLevel, int iCount,	int from=0, int to=0, int command=0);
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	int										iType;						//�������� 1,����,2,����,3,Ұ��ѱ��,4,���⹤��
	int										iItem;						//����ID
	int										iLevel;						//�ȼ�
	int										iCount;
};
//��������
struct tagLevy_Soldier_Param
{
	int										iRaceType;
	int										iCurrentCount;
};
//��������
struct LEVY_SOLDIER_PARAM : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagLevy_Soldier_Param & operator [](unsigned int);
};
//�������б���...
struct tagLevying_Soldier
{
	int									iRaceID;							//����ID
	int									iCount;								//�б���
	int									iState;								//״̬ 1: ��������ļ��... 2: �ȴ���...
	int									iTime;								//���ж೤ʱ�����  ��
};
struct LEVYING_SOLDIER : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagLevying_Soldier & operator [](unsigned int);
};
//�����б�
struct tagItem_List
{
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	int										iItemID;
	int										iType;
	int										iCount;
};
//�����б�
struct WEAPON_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to, int command);
	inline tagItem_List & operator [](unsigned int);
};
//�����б�
struct DEFEND_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to, int command);
	inline tagItem_List & operator [](unsigned int);
};
//ѱ�޼����б�
struct ANIMAL_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to, int command);
	inline tagItem_List & operator [](unsigned int);
};
//���⹤���б�
struct SPECIAL_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to, int command);
	inline tagItem_List & operator [](unsigned int);
};
//�г���Ʒ����  
struct COUNTRY_MARKET_BUSINESS : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, int iType, int iSaleID, int iLevel, int iCount, int iMode, int from=0, int to=0, int command=0);
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	int										iType;						//������Ʒ����:1,����; 2,����;3,ѱ�޼���,4,���⹤��,5,Ӣ��װ��.6,�������� GOODS_TYPE
	int										iSaleID;					//����Ϊ����,����ʱ:iSaleIDΪ����,���ߵ�ID; ��������ʱ:1,��ʳ;2,ľ��;3,��;4,Ƥ 5,�Ʊ�ʯ,6,�챦ʯ,7,�̱�ʯ,8,����
	int										iLevel;						//�����ȼ�
	int										iCount;						//����
	int										iMode;						//��ʽ 1:��; 2:��
};
//�����г���Ϣ
struct COUNTRY_MARKET_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int from=0, int to=0, int command=0);
};
//�г���Ʒ��Ϣ
struct PEOPLE_MARKET : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lSMapID, unsigned long lSCityID, int iType, int iMarketType, int from=0, int to=0, int command=0);	
	unsigned long							lSMapID;					//��ID
	unsigned long							lSCityID;					//����ID
	int										iType;						//��Ʒ����:1,����; 2,����;3,ѱ�޼���,4,���⹤��,5,Ӣ��װ��.6,�������� GOODS_TYPE
	int										iMarketType;				//�г����� 1,�����г�,2,����г�,3,����
};
//��Ʒ����
struct PUT_UP : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, int iType, int iSaleID, unsigned long lSortID, 
		int iCount, int iMinPrice, int iOncePrice, int from=0, int to=0, int command=0);	
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	int										iType;						//������Ʒ����:1,����; 2,����;3,ѱ�޼���,4,���⹤��,5,Ӣ��װ��.6,�������� GOODS_TYPE
	int										iSaleID;					//����Ϊ����ʱ:iSaleIDΪ���ߵ�ID; ����ʱ:1,��ʳ;2,ľ��;3,��;4,Ƥ;6,�챦ʯ;5,�Ʊ�ʯ;7,�̱�ʯ;8,���� BASE_RESOURCE_TYPE;
	unsigned long							lSortID;					//Ӣ��װ�� ��ˮ��
	int										iCount;						//����
	int										iMinPrice;					//��ͼ�
	int										iOncePrice;					//һ�ڼ�
};
//��Ʒ�б�
struct tagGoodsList
{
	unsigned long							lSortID;					//��ˮ��
	char									szPlayerName[SIZE_32];		//�������
	__int64									iSaleID;					//����Ϊ����ʱ:iSaleIDΪ���ߵ�ID; ����ʱ:1,��ʳ;2,ľ��;3,��;4,Ƥ;6,�챦ʯ;5,�Ʊ�ʯ;7,�̱�ʯ;8,���� BASE_RESOURCE_TYPE
	int										iCount;						//����
	int										iMinPrice;					//��ͼ�
	int										iCurrentPrice;				//��ǰ��
	int										iOncePrice;					//һ�ڼ�
	int										iLeavingTime;				//ʣ��ʱ��
	int										iTax;						//˰��
};
//��Ʒ�б�
struct GOODS_LIST : DISTRIBUTED_HEADER
{
	int iPage;				//ҳ��(30����¼Ϊ1ҳ)
	unsigned int iCount;	//��¼����
	int	iGoodsType;			//��Ʒ����
	inline void serialize(int count, int iGoodsType, int iPage, int from, int to);
	inline tagGoodsList & operator [](unsigned int);
};
//����
struct GOODS_BIDDEN : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lSortID, int iCurrentPrice, int from=0, int to=0, int command=0);	
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	int										iType;						//��Ʒ����:1,����; 2,����;3,ѱ�޼���,4,���⹤��,5,Ӣ��װ��.6,�������� GOODS_TYPE
	unsigned long							lSortID;					//��ˮ��
	int										iCurrentPrice;				//��ǰ����
};
//������Ʒ
struct BUY_GOODS : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lSortID, int from=0, int to=0, int command=0);
	unsigned long							lMapID;						//��ID
	unsigned long							lCityID;					//����ID
	int										iType;						//��Ʒ����:1,����; 2,����;3,ѱ�޼���,4,���⹤��,5,Ӣ��װ��.6,�������� GOODS_TYPE
	unsigned long							lSortID;					//��ˮ��
};
//�����б���Ϣ
struct tagArmysList
{
	unsigned long						lMapID;						    	//Ŀ�꿤ID
	unsigned long						lPlayerID;							//���ID
	unsigned long						lArmyID;							//����ID
	unsigned long						lOfficerID;							//ָ�ӹ�ID
	char								szOfficerName[SIZE_32];				//ָ�ӹ�����
	char								szArmyName[SIZE_32];				//��������
	int									iFace;								//ָ�ӹ�ͷ��
	int									iRaceID;							//����
	int									iJobID;								//ְϵ
	int									iX;									//��ǰ��������
	int									iY;									//
	int									iCount;
	int									iDirection;							//��ǰ���ӷ���
	int									iState;								//0:���� 1:����

	int									iWeaponID;							//����ID
	int									iDefendID;							//����ID
	int									iWeaponLevel;						//�����ȼ�
	int									iDefendLevel;						//���ߵȼ�
	int									iExp;
};
struct MAP_ARMY_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0, int command=0);
	inline tagArmysList & operator [](unsigned int);
};
//������Ϣ
struct ARMY_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iCount, int iDefendID, int iWeaponLevel,int iDefendLevel,int iExp, int from=0, int to=0);
	
	
	int									iCount;
	int									iDefendID;						//����ID
	int									iWeaponLevel;					//�����ȼ�
	int									iDefendLevel;					//���ߵȼ�
	int									iExp;
};
//�����б�
struct tagArmyList
{
	unsigned long							lArmyID;							//����ID
	char									szName[SIZE_32];					//��������
	
};
//�����б�
struct ARMY_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0);
	inline tagArmyList & operator [](unsigned int);
};
//�����ڳ��е�ͼ���ƶ�
struct ARMY_MOVE_MAP : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lArmyID, int iStartX, int iStartY,  int iEndX, int iEndY, int iflag, int from=0, int to=0, int command=0);
	unsigned long						lMapID;								//��ID
	unsigned long						lArmyID;							//����ID
	int									iStartX;							//�ƶ���ʼ����
	int									iStartY;							//
	int									iEndX;								//�ƶ���������
	int									iEndY;								//
	int									iflag;								//��ǰ���ӷ���
};
//���ӳ��е�ͼ�Ͽ��ƶ�������
struct tagArmyMoveList
{
	unsigned long						lArmyID;							//����ID
	int									iX;									//���ƶ�������
	int									iY;
	int									iWeight ;							//�ϰ���
};
//���ӳ��е�ͼ�Ͽ��ƶ��������б�
struct ARMY_CAN_MOVE_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0);
	inline tagArmyMoveList & operator [](unsigned int);
	
};
//�޸�ָ�ӹ�����
struct MODIFY_OFFICER_NAME : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lOfficerID, char szOfficerName[],int from=0, int to=0);
	unsigned long					lOfficerID;
	char							szOfficerName[SIZE_24];
};
//�ɹ���Ϣ��
struct SUCCEED_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lCityID,unsigned long lSortID,int iEventID, int iLevel, int iCount, int iCode, int from=0, int to=0);
	unsigned long lCityID;			//����
	unsigned long lSortID;			//��ˮ��
	int iEventID;					//����,���е��ߵ�ID �� �������ID
	int iLevel;						//�ȼ�
	int iCount;						//����
	int iCode;//�ɹ���:
};
//�г���Ϣ��
struct MARKET_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iTyeID, int iGoodsID,int iCount, int iCode, int from=0, int to=0);
	int	iTypeID;					//����ID
	int iGoodsID;					//��ƷID
	int iCount;						//����
	int iCode;						//��Ϣ��
};
//������Ϣ
struct ERROR_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iCode, int from, int to);
	int iCode;//������:
};
//�˳���Ϸ
struct EXIT_MAP_SERVER : DISTRIBUTED_HEADER 
{
	inline void serialize(int from, int to);
};
//�˳���Ϸ
struct EXIT_GAME : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lPlayerID, unsigned long lMapID, int from, int to);
	unsigned long lPlayerID;
	unsigned long lMapID;
};
//ָ�ӹټ���
struct tagOfficerTechList 
{
	int					iTechID;	//����ID
	int					Level;		//���ܵȼ�	
};
struct OFFICER_TECH_LIST : DISTRIBUTED_HEADER 
{
	unsigned long		lOfficerID;		//ָ�ӹ�ID
	unsigned int iCount;
	inline void serialize(int officerid,int count, int from/*=0*/, int to/*=0*/, int command/*=0*/);
	inline tagOfficerTechList & operator [](unsigned int);
};
//ָ�ӹ�ѧϰ����
struct STUDY_OFFICER_TECH : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lOfficerID, int iTechID, int iType, int from=0, int to=0, int command=0);
	unsigned long		lOfficerID;
	int					iTechID;	//����ID
	int					iType;		//��������
};
//ָ�ӹ���Ϣ
struct GET_OFFICER_INFO : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lOfficerID,int from=0, int to=0, int command=0);
	unsigned long		lOfficerID;
};
//ָ�ӹ�����
struct OFFICER_ATTRIBUTE : DISTRIBUTED_HEADER 
{
	inline void serialize(int officerid,int iCredit, int iStamina, int iPower, int iCelerity, int iWit, int iAttack,
		int iPhysicRecovery, int iMagicRecovery, int iLevel, int iMilitaryRank_,int iExp, int techdot,
		int iActionDot,int iEquipStamina,int iEquipPower,int iEquipCelerity,int iEquipWit,int iHeroHP,int iMagicAttack,
		int from/*=0*/, int to/*=0*/);
	int iCredit;				//������
	int iStamina;				//����
	int iPower;					//����
	int iCelerity;				//����
	int iWit;					//����
	int iAttack;				//����
	int iPhysicRecovery;		//����
	int iMagicRecovery;			//ħ��
	int iLevel;					//�ȼ�
	int iMilitaryRank_;			//����
	int OfficerID;				//Ӣ��ID
	int Exp;					//����ֵ
	int TechDot;				//���ܵ�
	int ActionDot;				//�ж�ֵ
	int EquipStamina;			//װ����������
	int EquipPower;				//װ����������
	int EquipCelerity;			//װ����������
	int EquipWit;				//װ����������
	int HeroHP;					//����ֵ
	int MagicAttack;			//ħ������
};
//Ӣ��װ���б�
struct tagHeroEquipList
{
	int iSortID;				//��ˮ��
	Inlay_Union InlayItem;		//װ��ID�������Ǹ��ϵ�ID������������Ϣ
	int iLevel;					//�ȼ�
	int iPos;					//ʹ�ò�λ
	int iShowPos;				//��������ʾλ�� ��Ӣ����
	int	iType;					//���� 1:Ӣ������ 2:Ӣ�۱��� 3: �������� (���� PACK_TYPE)
	int iCount;					//����
	int Skill;					//����
	int SkillLevel;				//���ܵȼ�
};
struct HERO_EQUIP_LIST : DISTRIBUTED_HEADER 
{
	int iCount;
	inline void serialize(int count, int from=0, int to=0, int command=0);
	inline tagHeroEquipList & operator [](unsigned int);
};
//���������е�װ��
struct tagExchangeEquipList
{
	int iSortID;				//��ˮ��
	int iShowPos;				//��������ʾλ��
	int	iType;					//���� 1:Ӣ������ 2:Ӣ�۱��� 3: ��������
	int iMode;					//������ʽ 1:����װ��,2:����װ�� 3:���뱳����,4:�ӱ�����ȡ��,5:ͬ��������װ������λ��(���� EXCHANGE_EQUIP_MODE)
};
struct EXCHANGE_EQUIP_LIST : DISTRIBUTED_HEADER 
{
	int iCount;
	unsigned long lHeroID;		//Ӣ��ID
	inline void serialize(int count, int from, int to);
	inline tagExchangeEquipList & operator [](unsigned int);
};
//����װ��������Ϣ
struct EXCHANGE_EQUIP_INFO : DISTRIBUTED_HEADER 
{
	inline void serialize(int iCode, int iSortID, int from, int to);
	int iCode;					//������
	int iSortID;				//��ˮ��
//	int iItemID;				//װ��ID
};
//����װ��
struct DESTROY_EQUIP : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lHeroID,int iSortID,int count, int from, int to);
	unsigned long lHeroID;		//Ӣ��ID
	int iSortID;				//��ˮ��
	int Count;					//��������������������
};
//���Ӣ��
struct REDEEM_HERO : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lHeroID, int iMoney, int from, int to);
	unsigned long lHeroID;		//Ӣ��ID
	int iMoney;
};
//Ӣ��ս����¼
struct tagHeroWarRecord
{
	int iSortID;						//
	__int64 lEnterDate;					//����ս������
	unsigned long lMap;					//ս��(��ID)
	unsigned long lArmyID;				//���첿��ID
	int iEnemyCount;					//ɱ������
	int iGeneralCount;					//ɱ������
	__int64 lExitDate;					//�˳�ս������
	int iState;							//Ӣ��״̬
	__int64 lFreeDate;					//�ͷ�����
	char Armyname[SIZE_32];			//��������
};
struct HERO_WAR_RECORD_LIST : DISTRIBUTED_HEADER 
{
	int iCount;
	inline void serialize(int count, int from, int to);
	inline tagHeroWarRecord & operator [](unsigned int);
};
//Ӣ�ۼ�ɳ���¼
struct tagHeroGrowUpRecord
{
	int iSortID;						//
	int iType;							//��¼���� 1:Ӣ������ 2:Ӣ�۾���
	__int64 lGrowUpDate;				//��������
	int	iLevel;							//�����ȼ�
};
struct HERO_GROWUP_RECORD_LIST : DISTRIBUTED_HEADER 
{
	int iCount;
	inline void serialize(int count, int from, int to);
	inline tagHeroGrowUpRecord & operator [](unsigned int);
};
//װ��/ж������
struct tagMaterials
{
	int iType;	//������Ʒ����:1,����; 2,����;3,ѱ�޼���,4,���⹤��,5,Ӣ��װ��.6,�������� (��GOODS_TYPEö��)
	int iMatID;	//����Ϊ����,����ʱ:iMatIDΪ����,���ߵ�ID; ��������ʱ:1,��ʳ;2,ľ��;3,��;4,Ƥ 5,�Ʊ�ʯ,6,�챦ʯ,7,�̱�ʯ,8,���� (��BASE_RESOURCE_TYPEö��)
	int iCount;	//��������
};
//װ��/ж������
struct MATERIAL_LIST : DISTRIBUTED_HEADER 
{
	int iCount;	//��¼����
	unsigned long lArmyID;	//����ID
	unsigned long lMapID;	//��ID
	unsigned long lCityID;	//����ID
	inline void serialize(int count, int from, int to, int command);
	inline tagMaterials & operator [](unsigned int);
};
//ֳ�����
struct COLONIZE_CITY : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lArmyID, unsigned long lMapID, unsigned long lCityID, int from, int to, int command);
	unsigned long lArmyID;	//����ID
	unsigned long lMapID;	//��ID
	unsigned long lCityID;	//����ID
};
//���ʲ鿴
struct LOOK_MATERIALS : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lArmyID, unsigned long lMapID, int from, int to, int command);
	unsigned long lArmyID;	//����ID
	unsigned long lMapID;	//��ID
};
//���ν���
struct MILITARY_RANK : DISTRIBUTED_HEADER 
{
	inline void serialize(char szName[], int iRank, int from, int to, int command);
	char szName[SIZE_24];			//Ӣ������
	int iRank;						//Ӣ�۵ȼ�
};
//Ӣ�۱���
struct CAPTIVE_HERO : DISTRIBUTED_HEADER 
{
	inline void serialize(char szByPlayerName[], char szByHeroName[], char szPlayerName[], int from, int to, int command);
	char szByPlayerName[SIZE_24];			//����Ӣ�۵��������
	char szByHeroName[SIZE_24];			//����Ӣ�۵�����
	char szPlayerName[SIZE_24];			//�������
};
//����/�뿪��ͼ
struct COMEIN_EXIT_MAP : DISTRIBUTED_HEADER 
{
	inline void serialize(char szName[], unsigned long lMapID, int iType, int from, int to, int command);
	char szName[SIZE_24];			//�������
	unsigned long lMapID;			//��ID
	int iType;						//1;����. 2:�뿪��ͼ
};
//���ӴﵽԤ��
struct ARMY_REACH_WARM : DISTRIBUTED_HEADER 
{
	inline void serialize( unsigned long lMapID,int from, int to,int unioncnt,int clancnt,int aboriginalcnt);
	unsigned long lMapID;			//��ID
	unsigned int  UnionCount;		//���˼�������Ĳ�������
	unsigned int ClanCount;			//���伴������Ĳ�������
	unsigned int AboriginalCount;	//������������Ĳ�������
};
//��Դ�ɼ���cmd = EN_S_RESOURCE_RATE  or EN_S_RESOURCE_PERCENTRATE
struct RESOURCE_RATE : DISTRIBUTED_HEADER
{
	inline void serialize(int iFoodRate, int iWoodRate, int iSkinRate, int iIronRate, int iScience, int from, int to,int cmd = EN_S_RESOURCE_RATE);
	int iFoodRate;
	int iWoodRate;
	int iSkinRate;
	int iIronRate;
	int iScience;
};


//����һ���µı��䵽Ӣ�۱���EN_C_NEWTREASUREPACK,EN_S_NEWTREASUREPACK,
typedef struct tagNewTreasurePack_Request:DISTRIBUTED_HEADER
{
	int OfficerID;					//Ӣ��ID
	int ItemID;						//�������Ʊ��
	int ItemType;					//���ͣ��ο�ö�٣������䣺TREASUREPACK�����ƣ�ARMYBRAND
	inline void serialize(int playerid,int officerid,int itemid,int type);
}NewTreasurePack_Request,*LPNewTreasurePack_Request;

typedef struct tagNewTreasurePack_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//0:�ɹ���EN_PACKFULL����������;EN_OPERATE_DBG_FAILED�����ʧ��
	tagHeroEquipList itemInfo;		//�����ı��䡢������Ϣ
	inline void serialize(int agent,int playerid,int code);
}NewTreasurePack_Response,*LPNewTreasurePack_Response;

//�򿪱���EN_C_OPENTREASUREPACK,EN_S_OPENTREASUREPACK,
typedef struct tagOpenTreasurePack_Request:DISTRIBUTED_HEADER
{
	int MapID;					//��ǰ����ͼID	
	int CityID;
	int OfficerID;				//�������ڱ�����Ӣ��ID������������Ϊ1
	int SortID;					//��ˮ��
	inline void serialize(int playerid,int mapid,int cityid,int officerid,int sortid);
}OpenTreasurePack_Request,*LPOpenTreasurePack_Request;

typedef struct tagOpenTreasurePack_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//0:�ɹ���1���ޱ��䣻2���ֿ�����������
	int Money;						//�ɹ�ʱ���򿪵Ľ�Ǯ��
	int ItemID;						//EN_YELLOW_STONE = 5,�Ʊ�ʯ
									//EN_RED_STONE = 6,		�챦ʯ
									//EN_GREEN_STONE = 7,		�̱�ʯ
									//EN_BLACK_STONE = 8		����
	int Count;						//����
	tagHeroEquipList EquipItem;		//װ����Ϣ��ItemID<=0ʱ��δ����װ��
	inline void serialize(int agent,int playerid,int code,int money = 0,int itemid = 0,int randcount = 0,
		int equipid = 0,int sortid = 0,int level = 0,int pos = 0,int showpos = 0,int type = 0,int equipcount=0);
}OpenTreasurePack_Response,*LPOpenTreasurePack_Response;

//Ӣ������֪ͨEN_C_HEROGBH_NOTIFY,EN_S_HEROGBH_NOTIFY
typedef struct tagHeroGBH_Notify:DISTRIBUTED_HEADER
{
	int OfficerID;
	inline void serialize(int playerid,int officerid);
}HeroGBH_Notify,*LPHeroGBH_Notify;

typedef struct tagHeroGBH_Broadcast:DISTRIBUTED_HEADER
{
	char	PlayerName[SIZE_24];	//�������
	char	OfficerName[SIZE_24];	//Ӣ������
	inline void serialize(const char *playername,const char *officername);
}HeroGBH_Broadcast,*LPHeroGBH_Broadcast;

//�������CGC_C_IMMEDCOMPLETE,CGC_S_IMMEDCOMPLETE,
typedef struct tagImmedComplete_Request:DISTRIBUTED_HEADER
{
	/*
	Construction_Speed_Card,		//�������ٿ�
	Research_Speed_Card,			//�������ٿ�
	Conscription_Speed_Card,		//�������ٿ�
	Recruit_Card,					//��ļӢ�ۿ�
	Produce_Speed_Card,				//�������ٿ�
	*/
	int Type;						//�뿨����һ�£�����������������ɡ�����������ɡ�����������ɡ���ļӢ�ۡ�����
	int MapID;
	int CityID;
	int ProductionType;				//��Ҫ�������������ɣ���ʾ��������Ʒ���ͣ������������ߡ����⹤�ߵȣ�����ο�GOODS_TYPEö��
	int ItemID;						//����ʱΪ����ID������ʱΪ����ID�������������ߵȣ�
	int SortID;						//��ˮ��
	int Level;						//����
	inline void serialize(int playerid,int type,int mapid,int cityid,int itemid,int sortid,int level,int protype);
}ImmedComplete_Request,*LPImmedComplete_Request;
inline void tagImmedComplete_Request::serialize(int playerid,int type,int mapid,int cityid,int itemid,int sortid,int level ,int protype)
{
	Type = type;
	MapID = mapid;
	CityID = cityid;
	ItemID = itemid;
	SortID = sortid;
	Level = level;
	ProductionType = protype;
	int svrid = 0;
	switch(type)
	{
	case Construction_Speed_Card:
	case Conscription_Speed_Card:
	case Produce_Speed_Card:
		svrid = mapid;
		break;
	case Research_Speed_Card:
		svrid = MDM_GP_MAP_RESEARCH;
		break;
	case Recruit_Card:
		svrid = SERVER_BOUND_MAP_OTHER;
		break;
	default:
		svrid = SERVER_BOUND_MAP_OTHER;
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,svrid,EN_C_IMMEDCOMPLETE);
}

typedef struct tagImmedComplete_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//0:�ɹ���1��Go�㲻�㣻2��δ����������У�������ɣ�;3:����Ҫʹ��������ļ��111������
	int Type;						//����
	int SortID;						//��ˮ��
	int Count;						//�ķѵ�Go������
	inline void serialize(int agent,int from,int playerid,int code,int type,int sortid,int count);
}ImmedComplete_Response,*LPImmedComplete_Response;
inline void tagImmedComplete_Response::serialize(int agent,int from,int playerid,int code,int type,int sortid,int count)
{
	Error_Code = code;
	Type = type;
	SortID = sortid;
	Count = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,playerid,EN_S_IMMEDCOMPLETE);
}

//���ÿ���ͼ��ϢEN_C_RESETMAPINFO,���ͼ֪ͨ��������
typedef struct tagResetMapInfo_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int mapid);
}ResetMapInfo_Request,*LPResetMapInfo_Request;
inline void tagResetMapInfo_Request::serialize(int mapid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,SERVER_BOUND_MAP_OTHER,mapid,EN_C_RESETMAPINFO);
}


//��ȡ��Ҳ����б�EN_C_GETARMYLIST,	EN_S_GETARMYLIST,
typedef struct tagGetArmyList_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}GetArmyList_Request,*LPGetArmyList_Request;
inline void tagGetArmyList_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_GETARMYLIST);
}

typedef struct tagPlayerArmyInfo
{
	int				OfficerID;				//ָ�ӹ�ID
	unsigned int	ArmyID;					//����ID
	char			ArmyName[SIZE_32];		//��������
	char			OfficerName[SIZE_24];	//ָ�ӹ�����
	int				Mapid;					//�������ڵ�ͼ
	int				Soldiertype;			//ʿ������
	int				Raceid;					//����ID
	int				Firstcount;				//����
	int				Fweapon;				//��������
	int				Fdefendtype;			//��������
	int				State;					//״̬���ο�ö��ARMY_TRUST_STATE
}PlayerArmyInfo,*LPPlayerArmyInfo;

typedef struct tagGetArmyList_Response:DISTRIBUTED_HEADER
{
	int Count;								//����������
	inline void serialize(int agent,int playerid,int count);
	inline PlayerArmyInfo & operator [](unsigned int);
}GetArmyList_Response,*LPGetArmyList_Response;
inline void tagGetArmyList_Response::serialize(int agent,int playerid,int count)
{
	Count = count;
	if(count <0)
		count = 0;
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(PlayerArmyInfo),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_GETARMYLIST);
}
inline PlayerArmyInfo & tagGetArmyList_Response::operator [](unsigned int index)
{
	return ((LPPlayerArmyInfo)(this + 1))[index];
}
//ս�����񡢸����������ͼ��֪ͨ��Ϣ������״̬�ı�EN_C_MODIFYARMYSTATE,	
typedef struct tagNotify_ModifyArmyState:DISTRIBUTED_HEADER
{
	PlayerArmyInfo armyInfo;
	inline void serialize(int playerid,int armyid,int mapid,int soldiercount,int state,int weapontype);
}Notify_ModifyArmyState,*LPNotify_ModifyArmyState;
inline void tagNotify_ModifyArmyState::serialize(int playerid,int armyid,int mapid,int soldiercount,int state,int weapontype)
{
	armyInfo.ArmyID = armyid;				//����ID
	armyInfo.Mapid = mapid;					//�������ڿ�ID������0����Ч
	armyInfo.Firstcount = soldiercount;		//��������������0ʱ��Ч
	armyInfo.State = state;					//״̬���ο�ö��ARMY_TRUST_STATE
	armyInfo.Fweapon = weapontype;			//��������
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_MODIFYARMYSTATE);
}
//���ͼ�������·�����ң�����״̬�ı�EN_S_MODIFYARMYSTATE,	
typedef struct tagModifyArmyState_Response:DISTRIBUTED_HEADER
{
	PlayerArmyInfo armyInfo;
	inline void serialize(int agent,int playerid,int armyid,int mapid,int soldiercount,int state,int from = SERVER_BOUND_MAP_OTHER);
}ModifyArmyState_Response,*LPModifyArmyState_Response;
inline void tagModifyArmyState_Response::serialize(int agent,int playerid,int armyid,int mapid,int soldiercount,int state,int from)
{
	armyInfo.ArmyID = armyid;//����ID
	armyInfo.Mapid = mapid;//�������ڿ�ID������0����Ч
	armyInfo.Firstcount = soldiercount;//��������������0ʱ��Ч
	armyInfo.State = state;//״̬���ο�ö��ARMY_TRUST_STATE
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,playerid,EN_S_MODIFYARMYSTATE);
}
//��Ҳ��һ���ݵ�EN_C_DESTROYCITY,	EN_S_DESTROYCITY,
typedef struct tagDestroyCity_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid,int mapid);
}DestroyCity_Request,*LPDestroyCity_Request;
inline void tagDestroyCity_Request::serialize(int playerid,int mapid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,mapid,EN_C_DESTROYCITY);
}

typedef struct tagDestroyCity_Response:DISTRIBUTED_HEADER
{
	int Error_Code;			//0:�ɹ�;1:����ڸõ�ͼû�оݵ㣻2:���ִ��޷������111:ʧ��
	inline void serialize(int agent,int playerid,int mapid,int code);
}DestroyCity_Response,*LPDestroyCity_Response;
inline void tagDestroyCity_Response::serialize(int agent,int playerid,int mapid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,mapid,playerid,EN_S_DESTROYCITY);
}
//��ѯ��²��ϢEN_C_QUERYCAPTIVELIST,EN_S_QUERYCAPTIVELIST,
typedef struct tagQueryCaptiveList_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}QueryCaptiveList_Request,*LPQueryCaptiveList_Request;
inline void tagQueryCaptiveList_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_QUERYCAPTIVELIST);
}

typedef struct tagCaptiveInfo
{
	int	PlayerID;						//���ID
	int OfficerID;						//Ӣ��ID
	char PlayerName[SIZE_24];			//�������
	char OfficerName[SIZE_24];			//Ӣ������
	
	int Officerrace;					//����
	int Level;							//����
	int Militaryrank;					//����
	int Jobid;							//ְҵ
	int Faceid;							//ͷ��
	int Stamina;						//����
	int Power;							//����
	int Celerity;						//����
	int Wit;							//����
	int EquipStamina;					//װ������
	int EquipPower;						//װ������
	int EquipCelerity;					//װ������
	int EquipWit;						//װ������
	int Techdot;						//���ܵ�
	int Attack;							//����-����
	int Physicrecovery;					//�������
	int Magicrecovery;					//ħ������
	int MagicAttack;					//ħ������
	unsigned int Times;					//��ץ��ʱ�䣬��λ����
}CaptiveInfo,*LPCaptiveInfo;
typedef struct tagQueryCaptiveList_Response:DISTRIBUTED_HEADER
{
	int Count;//��²����>0Ϊ�з�²,-1��ʧ��
	inline void serialize(int agent,int playerid,int count);
	inline CaptiveInfo & operator [](unsigned int index);
}QueryCaptiveList_Response,*LPQueryCaptiveList_Response;
inline void tagQueryCaptiveList_Response::serialize(int agent,int playerid,int count)
{
	Count = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + Count*sizeof(CaptiveInfo),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_QUERYCAPTIVELIST);
}
inline CaptiveInfo & tagQueryCaptiveList_Response::operator [](unsigned int index)
{
	return ((LPCaptiveInfo)(this + 1))[index];
}
//����NPC����EN_C_CREATENPCARMY,EN_S_CREATENPCARMY,
typedef struct tagNotify_CreateNPCArmy_Request:DISTRIBUTED_HEADER
{
	int GMCmd;					//GM����
	int PlayerID;				//GM��ID
	int CountryID;				//NPC���ӵ���Ӫ
	int Count;					//����
	int Level;					//���ӵļ���
	int FromMap;				//����������ҪΪ����½���ȷ��);
	int TargetMap;				//Ŀ�꿤
	int Times;					//����
	int Interval;				//���ʱ�䣬��λ����
	inline void serialize(int agent,int playerid,int cmd,int countryid,int level,int count,int frommap,int targetmap,int times,int interval,int from);
}Notify_CreateNPCArmy_Request,*LPNotify_CreateNPCArmy_Request;
inline void tagNotify_CreateNPCArmy_Request::serialize(int agent,int playerid,int cmd,int countryid,int level,int count,int frommap,int targetmap,int times,int interval,int from)
{
	this->GMCmd = cmd;
	this->PlayerID = playerid;
	this->CountryID = countryid;
	this->Count = count;
	this->Level = level;
	this->FromMap = frommap;
	this->TargetMap = targetmap;
	this->Times = times;
	this->Interval = interval;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,SERVER_BOUND_MAP_OTHER,EN_C_CREATENPCARMY);
}
//���ͼ����
typedef struct tagNotify_CreateNPCArmy_Response:DISTRIBUTED_HEADER
{
	short Error_Code;				//0���ɹ���1���޷�����;2:NPC���������ڣ�3:��������
	int GMCmd;						//GM����
	int CountryID;					//NPC���ӵ���Ӫ
	int Count;						//����
	int Level;						//���ӵļ���
	int FromMap;					//����������ҪΪ����½���ȷ��);
	int TargetMap;					//Ŀ�꿤
	int Times;					//����
	int Interval;				//���ʱ�䣬��λ����
	inline void serialize(int agent,int playerid,short code,int cmd,int countryid,int level,int count,int frommap,int targetmap,int times,int interval);
}Notify_CreateNPCArmy_Response,*LPNotify_CreateNPCArmy_Response;
inline void tagNotify_CreateNPCArmy_Response::serialize(int agent,int playerid,short code,int cmd,int countryid,int level,int count,int frommap,int targetmap,int times,int interval)
{
	this->Error_Code = code;
	this->GMCmd = cmd;
	this->CountryID = countryid;
	this->Count = count;
	this->Level = level;
	this->FromMap = frommap;
	this->TargetMap = targetmap;
	this->Times = times;
	this->Interval = interval;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_CREATENPCARMY);
}

//ɾ��NPC����EN_C_DELNPCARMY,EN_S_DELNPCARMY,ֱ��֪ͨ��Ӧ�Ŀ�������
typedef struct tagNotify_DelNPCArmy_Request:DISTRIBUTED_HEADER
{
	int GMCmd;				//GM����
	int PlayerID;			//GM��ID
	int CountryID;
	inline void serialize(int agent,int playerid,int cmd,int mapid,int countryid,int from);
}Notify_DelNPCArmy_Request,*LPNotify_DelNPCArmy_Request;
inline void tagNotify_DelNPCArmy_Request::serialize(int agent,int playerid,int cmd,int mapid,int countryid,int from)
{
	this->PlayerID = playerid;
	this->GMCmd = cmd;
	this->CountryID = countryid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,mapid,EN_C_DELNPCARMY);
}
typedef struct tagNotify_DelNPCArmy_Response:DISTRIBUTED_HEADER
{
	int DelCount;			//ɾ����NPC������
	int GMCmd;				//GM����
	int CountryID;
	inline void serialize(int agent,int playerid,int count,int cmd,int mapid,int countryid);
}Notify_DelNPCArmy_Response,*LPNotify_DelNPCArmy_Response;
inline void tagNotify_DelNPCArmy_Response::serialize(int agent,int playerid,int count,int cmd,int mapid,int countryid)
{
	this->DelCount = count;
	this->GMCmd = cmd;
	this->CountryID = countryid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,mapid,playerid,EN_S_DELNPCARMY);
}

//֪ͨս���߳���������EN_C_FIGHT_CREATEARMY,EN_S_FIGHT_CREATEARMY,
typedef struct tagFIGHT_CreateArmy_Request:DISTRIBUTED_HEADER
{
	unsigned int ArmyID;			//����ID
	inline void serialize(int agent,int playerid,int mapid,int armyid);
}FIGHT_CreateArmy_Request,*LPFIGHT_CreateArmy_Request;
inline void tagFIGHT_CreateArmy_Request::serialize(int agent,int playerid,int mapid,int armyid)
{
	ArmyID = armyid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,mapid,EN_C_FIGHT_CREATEARMY);
}

//��������֪ͨ���ͼ��������״̬�ı�EN_C_NOTIFY_MAPSTATE,EN_S_NOTIFY_MAPSTATE,
typedef struct tagNotify_MapState_Request:DISTRIBUTED_HEADER
{
	int CountryID;				//����
	int State;					//״̬
	inline void serialize(int mapid,int countryid,int state);
}Notify_MapState_Request,*LPNotify_MapState_Request;
inline void tagNotify_MapState_Request::serialize(int mapid,int countryid,int state)
{
	CountryID = countryid;
	State = state;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,mapid,SERVER_BOUND_MAP_OTHER,EN_C_NOTIFY_MAPSTATE);
}

typedef struct tagNotify_MapState_Response:DISTRIBUTED_HEADER//������
{
	short Error_Code;//0:�ɹ�
	inline void serialize(int from,int to,short code);
}Notify_MapState_Response,*LPNotify_MapState_Response;
inline void tagNotify_MapState_Response::serialize(int from,int to,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,EN_S_NOTIFY_MAPSTATE);
}
//������������EN_C_MODIFY_PUBLICBUILD,		
typedef struct tagModify_PublicBuild_Request:DISTRIBUTED_HEADER
{
	int BuildType;
	unsigned long BuildID;
	int CurrentLevel;
	inline void serialize(int agent,int playerid,int buildtype,unsigned long buildid,int level);
}Modify_PublicBuild_Request,*LPModify_PublicBuild_Request;
inline void tagModify_PublicBuild_Request::serialize(int agent,int playerid,int buildtype,unsigned long buildid,int level)
{
	BuildType = buildtype;
	BuildID = buildid;
	CurrentLevel = level;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,0,EN_C_MODIFY_PUBLICBUILD);
}

//��������Ƕ��װ��EN_C_INLAYEQUIP,EN_S_INLAYEQUIP,
typedef struct tagInlayEquip_Request:DISTRIBUTED_HEADER
{
	int SortID_Equip;			//װ��sortid
	int SortID_InlayResource;	//��Ƕԭ�ϵ�sortid
	int SortID_Cards[2];		//���˷��ͱ��׿���sortid
	inline void serialize(int playerid,int sortid_eqiup,int sortid_inlayres,int sortid_card1,int sortid_card2);
}InlayEquip_Request,*LPInlayEquip_Request;
inline void tagInlayEquip_Request::serialize(int playerid,int sortid_eqiup,int sortid_inlayres,int sortid_card1,int sortid_card2)
{
	SortID_Equip = sortid_eqiup;
	SortID_InlayResource = sortid_inlayres;
	SortID_Cards[0] = sortid_card1;
	SortID_Cards[1] = sortid_card2;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_INLAYEQUIP);
}

typedef struct tagInlayEquip_Response:DISTRIBUTED_HEADER
{
	short Error_Code;				//0:�ɹ�;-1:���ܾ���(װ�����ܱ����ܾ��������Ѿ�����������);-2:װ���뱦ʯ��ƥ��;-3:�Ǳ�ʯ;-4:��װ��;1:ʧ�ܣ�111:ʧ��
	inline void serialize(int agent,int playerid,short code);
}InlayEquip_Response,*LPInlayEquip_Response;
inline void tagInlayEquip_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_INLAYEQUIP);
}
//����������һ��Ʒ�������������·�EN_S_NEWITEMINPACK,

//ʹ��Ԯ����EN_C_USEADDSOLDIERCARD,EN_S_USEADDSOLDIERCARD,
struct ReqUsePropCard:DISTRIBUTED_HEADER
{
	int armyID;						//����ID
	int cardType;					//�����ͣ�
	int roomID;						//����ţ�����ͼ���ԣ�����ʱ����д�����
	inline void serialize(int playerid,int armyid,int cardType,int svrId,int roomid);
};
inline void ReqUsePropCard::serialize(int playerid,int armyid,int cardType,int svrId,int roomid)
{
	armyID = armyid;
	cardType = cardType;
	roomID = roomid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,svrId,REQ_USE_PROP_CARD);
}
struct RspUsePropCard:DISTRIBUTED_HEADER
{
	int cardType;					//�����ͣ�
	short error_Code;				//1:Ŀ�겿�Ӳ����ڣ�2�������͵Ŀ������ڣ�3���������Ϳ��Ѵ�ʹ������
	inline void serialize(int agent,int playerid,int mapid, int cardType ,short code);
};
inline void RspUsePropCard::serialize(int agent,int playerid,int mapid, int cardType ,short code)
{
	error_Code = code; //1:Ŀ�겿�Ӳ����ڣ�2�������͵Ŀ������ڣ�3���������Ϳ��Ѵ�ʹ������
	cardType = cardType;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,mapid,playerid,RSP_USE_PROP_CARD);
}
//�ο�HERO_EQUIP_LIST


//�һ�EN_C_CONVERT,EN_S_CONVERT,����һ���ʽһ�£��һ�����Ʒ�ǹ̶��ģ�������ʹ��ͳһ�Ľӿڣ�Ŀǰֻ���ھ���
typedef struct tagConvert_Request:DISTRIBUTED_HEADER
{
	int officerID;				//�������ڱ�����Ӣ��ID������������Ϊ1
	int sortID;					//��ˮ��
	inline void serialize(int playerid,int officerid,int sortid);
}Convert_Request,*LPConvert_Request;
inline void tagConvert_Request::serialize(int playerid,int officerid,int sortid)
{
	officerID = officerid;
	sortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_CONVERT);
}
typedef struct tagConvert_Response:DISTRIBUTED_HEADER
{
	short error_Code;					//0:�ɹ���1�������ڣ�111��ʧ��
	inline void serialize(int agent,int playerid,short code);
}Convert_Response,*LPConvert_Response;
inline void tagConvert_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_CONVERT);
}

//��ļ������Ӣ�۽��й㲥EN_S_BROAD_SPECIALHERO,
typedef struct tagBroad_SpecialHero:DISTRIBUTED_HEADER
{
	char playerName[SIZE_24];				//�������
	int countryID;							//��Ӫ
	int specialHeroID;						//Ӣ��ID,>0:����Ӣ��ID;<=0��Ϊ��ͨӢ��
	int  totalAttribute;					//������
	inline void serialize(const char *playername,int country,int heroid,int attribute);
}Broad_SpecialHero,*LPBroad_SpecialHero;
inline void tagBroad_SpecialHero::serialize(const char *playername,int country,int heroid,int attribute)
{
	if(playername)
	{
		memcpy(playerName,playername,SIZE_24);
	}
	else
	{
		playerName[0] = 0;
	}
	countryID = country;
	specialHeroID = heroid;
	totalAttribute = attribute;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,SERVER_BOUND_MAP_OTHER,0,EN_S_BROAD_SPECIALHERO);
}

//�򿪿���ͳһ�Ҽ�������ܴ򿪵Ŀ���ԭ���Ҽ��򿪵Ĳ���ԭ�� ��ʽ���Ժ��Ҽ��򿪵Ŀ��ɲ���ͳһ��Э�飩EN_C_COMMONOPENCARD,EN_S_COMMONOPENCARD
typedef struct tagCommonOpenCard_Request:DISTRIBUTED_HEADER
{
	int heroID;					//Ӣ��ID��ʹ�ÿ���Ӣ��ID
	int sortID;					//������ˮ��
	inline void serialize(int playerid,int heroid,int sortid);
}CommonOpenCard_Request,*LPCommonOpenCard_Request;
inline void tagCommonOpenCard_Request::serialize(int playerid,int heroid,int sortid)
{
	heroID = heroid;
	sortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_COMMONOPENCARD);
}
typedef struct tagCommonOpenCard_Response:DISTRIBUTED_HEADER
{
	short error_Code;				//0:�ɹ���EN_OFFICER_INWAR��ս���в���ʹ�ã�EN_OPERATE_DBG_FAILED������DBGʧ�ܣ�EN_OFFICER_NOSKILL��û�п�����ļ���
	inline void serialize(int agent,int playerid,short code);
}CommonOpenCard_Response,*LPCommonOpenCard_Response;
inline void tagCommonOpenCard_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_COMMONOPENCARD);
}

//ʹ�ø������޸�Ӣ�۵�����EN_C_MODIFYHERONAME,EN_S_MODIFYHERONAME,
typedef struct tagModifHeroName_Request:DISTRIBUTED_HEADER
{
	int heroID;					//���޸ĵ�Ӣ��ID
	char newName[SIZE_24];		//������
	int sortID;					//����������ˮ��
	inline void serialize(int playerid,int heorid,const char *name,int sortid);
}ModifHeroName_Request,*LPModifHeroName_Request;
inline void tagModifHeroName_Request::serialize(int playerid,int heroid,const char *name,int sortid)
{
	heroID = heroid;
	memset(newName,0,SIZE_24);
	if(name)
	{
		strncpy(newName,name,SIZE_24-1);
	}
	sortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_MODIFYHERONAME);
}
typedef struct tagModifHeroName_Response:DISTRIBUTED_HEADER
{
	short error_Code;				//0���ɹ���EN_OFFICER_ID_ERROR��Ӣ��ID�Ƿ���EN_NO_HAVECARD����������;EN_HERONAMEEXIST��Ӣ�����Ѵ��ڣ�111:����ʧ��
	inline void serialize(int agent,int playerid,short code);
}ModifHeroName_Response,*LPModifHeroName_Response;
inline void tagModifHeroName_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_MODIFYHERONAME);
}

/*
mapID��ֻ�Խ�����������Ч
itemID:����Ϊ������ID������ID��װ��ID������ID������������װ��ID;�ҵ�Ϊװ��IDʱ�����Ը���ID��ȡ������Ϣ
itemType�����������������������ߡ����⹤�ߣ�
*/
typedef struct tagBaseNotifyPlayerMsg:DISTRIBUTED_HEADER
{
	int mapID;							//��ID
	__int64 itemID;						//ID
	short itemLevel;					//����
	short itemType;						//����	
	inline void serialize(int agent,int playerid,int mapid,__int64 itemid,short level,short type,int cmd,int svrid);
}BaseNotifyPlayerMsg,*LPBaseNotifyPlayerMsg;
inline void tagBaseNotifyPlayerMsg::serialize(int agent,int playerid,int mapid,__int64 itemid,short level,short type,int cmd,int svrid)
{
	this->mapID = mapid;
	this->itemID = itemid;
	this->itemLevel = level;
	this->itemType = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,svrid,playerid,cmd);
}
typedef struct tagBaseBroadMsg:DISTRIBUTED_HEADER
{
	short countryID;					//��Ӫ
	int playerID;						//���ID
	char playerName[SIZE_24];			//�������
	int mapID;							//��ID
	__int64 itemID;						//ID
	short itemLevel;					//����
	short itemType;						//����	
	inline void serialize(int agent,short country,int playerid,const char *name,int mapid,__int64 itemid,short level,short type,int cmd,int svrid);
}BaseBroadMsg,*LPBaseBroadMsg;
inline void tagBaseBroadMsg::serialize(int agent,short country,int playerid,const char *name,int mapid,__int64 itemid,short level,short type,int cmd,int svrid)
{
	this->countryID = country;
	this->playerID = playerid;
	if(name)
	{
		strcpy(playerName,name);
	}
	else
	{
		playerName[0] = 0;
	}
	this->mapID = mapid;
	this->itemID = itemid;
	this->itemLevel = level;
	this->itemType = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,svrid,playerid,cmd);
}
//���ͼ֪ͨ������������Ƿ�����EN_S_NOTIFYPLAYERONLINE
typedef struct tagNotifyPlayerOnline:DISTRIBUTED_HEADER
{
	int playerID;					//���ID����Ϣͷ���agent>0�����ߣ���������
	inline void serialize(int playerid,int agent,int toSvrID);
}NotifyPlayerOnline,*LPNotifyPlayerOnline;
inline void tagNotifyPlayerOnline::serialize(int playerid,int agent,int toSvrID)
{
	playerID = playerid;//SERVER_BOUND_MAP_OTHER
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,toSvrID,EN_S_NOTIFYPLAYERONLINE);
}
//��Ҵ���ɵ��ճ�����֪ͨ������֪ͨս��EN_C_NOTIFY_DAYTASKOFPLAYER,EN_S_NOTIFY_DAYTASKOFPLAYER,
typedef struct tagNotify_DayTaskOfPlayer_Request:DISTRIBUTED_HEADER
{
	int playerID;
	int taskID;
	short kindType;//����  //TASKCOMPLETETARGETTYPE
	short itemType;//Ŀ������
	int itemID;//Ŀ��ID
	short number;//����
	short itemLevel;//����
	__int64 endTime;//����ʱ��
	inline void serialize(int playerid,int taskid,short kindtype,short itemtype,int itemid,short number,short level,__int64 endtime,int from,int to);
}Notify_DayTaskOfPlayer_Request,*LPNotify_DayTaskOfPlayer_Request;
inline void tagNotify_DayTaskOfPlayer_Request::serialize(int playerid,int taskid,short kindtype,short itemtype,int itemid,short num,short level,__int64 endtime,int from,int to)
{
	this->playerID = playerid;
	this->taskID = taskid;
	this->kindType = kindtype;
	this->itemType = itemtype;
	this->itemID = itemid;
	this->number = num;
	this->itemLevel = level;
	this->endTime = endtime;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,EN_C_NOTIFY_DAYTASKOFPLAYER);
}

typedef struct tagNotify_DayTaskOfPlayer_Response:DISTRIBUTED_HEADER
{
	int playerID;
	int taskID;
	inline void serialize(int playerid,int taskid,int from,int to);
}Notify_DayTaskOfPlayer_Response,*LPNotify_DayTaskOfPlayer_Response;
inline void tagNotify_DayTaskOfPlayer_Response::serialize(int playerid,int taskid,int from,int to)
{
	this->playerID = playerid;
	this->taskID = taskid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,EN_S_NOTIFY_DAYTASKOFPLAYER);
}

//�ͷ�Ӣ��EN_C_RELEASECAPTIVE,EN_S_RELEASECAPTIVE,
typedef struct tagReleaseCaptive_Request:DISTRIBUTED_HEADER
{
	int captPlayerID;			//�������ID
	int captOfficerID;			//����Ӣ��ID
	inline void serialize(int playerid,int captplayerid,int captofficerid);
}ReleaseCaptive_Request,*LPReleaseCaptive_Request;
inline void tagReleaseCaptive_Request::serialize(int playerid,int captplayerid,int captofficerid)
{
	captPlayerID = captplayerid;
	captOfficerID = captofficerid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_RELEASECAPTIVE);
}

typedef struct tagReleaseCaptive_Response:DISTRIBUTED_HEADER
{
	short error_Code;				//0:�ɹ���1������Ӣ�۲����ڣ�111������ʧ��
	inline void serialize(int agent,int playerid,short code);
}ReleaseCaptive_Response,*LPReleaseCaptive_Response;
inline void tagReleaseCaptive_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_RELEASECAPTIVE);
}
//Ӣ�ۻ�ȡ�������������֪ͨEN_C_NOTIFY_EXPORCREDIT,
typedef struct tagNotify_ExpOrCredit_Request:DISTRIBUTED_HEADER
{
	int playerID;
	int exp;				//����
	int credit;				//������
	inline void serialize(int playerid,int expval,int creditval);
}Notify_ExpOrCredit_Request,*LPNotify_ExpOrCredit_Request;
inline void tagNotify_ExpOrCredit_Request::serialize(int playerid,int expval,int creditval)
{
	playerID = playerid;
	exp = expval;
	credit = creditval;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,SERVER_BOUND_MAP_OTHER,MDM_GP_MAP_CONSORTIA,EN_C_NOTIFY_EXPORCREDIT);
}

// ˫�������Ż�ȡ�������Ӣ�۵Ĺһ�������ߵ�ĳһ��Ӣ�۲��ӽ��������ŵĸ�Ӧ���򷢴�����
struct ReqHerosHangSys : DISTRIBUTED_HEADER
{
	unsigned long doorId;        // ������ID. 
	inline void serialize(unsigned long doorId, int from, int to);
};

// �һ�ϵͳ���ڸ��һ�Ӣ����Ϣ����ĳһ��Ӣ�۲��Ӹս��������ŵĸ�Ӧ�����Ӣ����Ϣ��
struct tagHangingItemList
{
	unsigned long heroId;                 // Ӣ��ID
	char szName[SIZE_24];				  // Ӣ������
	unsigned int  hangSysType;            // �һ����� 1:��ͨ  2:����
	int iCompletetime;		              // ����Ҫ����ʱ�����ʱ��  ��
	unsigned long expVal;                 // �ܻ�ȡ�ľ���ֵ
	unsigned long moneyVal;               // ��Ҫ���ѵĽ�Ǯ
//	unsigned int heroState;               // Ӣ��״̬ 	0:����  1:����  2:����  3:����  4:��²  5.�һ�
};

// ������������ڹһ�״̬�µ�Ӣ�ۻ���Ӣ�۲��Ӹս����Ӧ����Ӱ�������Ϣ
struct RspHerosHangSys : DISTRIBUTED_HEADER
{
	unsigned int count; 
	inline void serialize(unsigned int count, int agent, int from, int to);
	inline tagHangingItemList & operator [](unsigned int index);
};

// ����һ�һ��Ӣ��
struct ReqStartHangSystemOneHero : DISTRIBUTED_HEADER
{
	unsigned long heroID;        // Ӣ��ID
	unsigned int hangSysType;    // �һ����� 1:��ͨ  2:����
	unsigned int trainingTime;   // ѵ��ʱ�� ��λ��Сʱ
	
	inline void serialize(unsigned long heroID, unsigned int hangSysType, unsigned int trainingTime, int from, int to);
};

// �һ�Ӣ�۵ķ���
struct RspStartHangSystemOneHero : DISTRIBUTED_HEADER
{
	unsigned int ret;					  // ���ؽ�� 0:�ɹ� 1:Ӣ��״̬�����Ϲһ��������޷��һ� 2: ��Ǯ����   3:û�ж�Ӧ����ĵ��ߣ�ǿ����  4���һ�ʧ�ܣ������쳣
	unsigned long heroId;                 // Ӣ��ID
	unsigned int completeTime;		      // ����Ҫ����ʱ�����ʱ��  ��
	unsigned long expVal;                 // �ܻ�ȡ�ľ���ֵ
	unsigned long moneyVal;               // ��Ҫ���ѵĽ�Ǯ

	inline void serialize(unsigned int ret, unsigned long heroId, unsigned int completeTime, unsigned long expVal, unsigned long moneyVal, int agent, int from, int to);
};

// ȡ��ĳ��Ӣ�۵Ĺһ�
struct ReqCancelHangOneHero : DISTRIBUTED_HEADER
{
	unsigned long heroID;        // Ӣ��ID

	inline void serialize(unsigned long heroID, int from, int to);
};

// ȡ��ĳ��Ӣ�۵Ĺһ� ����
struct RspCancelHangOneHero : DISTRIBUTED_HEADER
{
	unsigned int ret;	 // ���ؽ�� 0:�ɹ� 1:ȡ���һ�ʧ��
	inline void serialize(unsigned int ret, int agent, int from, int to);
};

// ĳ��Ӣ�۹һ����֪ͨ
struct RspHangOneHeroFinish : DISTRIBUTED_HEADER
{
	unsigned long heroID;        // Ӣ��ID
	inline void serialize(unsigned long heroID, int agent, int from, int to);
};


#pragma pack(pop)

/**********************************************************************************************
//���ͼ�������ṹ��
**********************************************************************************************/

//��Կ��֤
inline void
MAP_SERVER_VALIDATE::serialize(char const szAccounts[], char const szValidate[], int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_VALIDATE);
	strcpy(this->szAccounts, szAccounts);
	strcpy(this->szValidate, szValidate);
}
//������Ϣ
inline void 
PLAYER_BASE_INFO::serialize(unsigned long lPlayerID, char Name[], int iFace, unsigned long lConsortiaID, long lExp, int iRaceID, 
		int iJobID, int iCredit, int iLevel, int iMilitaryRank, unsigned long lGameTime,int iCountry, long lScience, long lMoney,
		int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_PLAYER_BASE_INFO);
	this->lPlayerID = lPlayerID;
	this->iFace = iFace;
	this->lConsortiaID = lConsortiaID;
	this->lExp = lExp;
	this->iRaceID = iRaceID;
	this->iJobID = iJobID;
	this->iCredit = iCredit;
	this->iLevel = iLevel;
	this->iMilitaryRank = iMilitaryRank;
	this->lGameTime = lGameTime;
	this->iCountry = iCountry;
	this->lScience = lScience;
	this->lMoney = lMoney;
	strcpy(this->szNick, Name);
}
//��ҵ���Դ��
inline void 
PALEYER_RESOURCE::serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,int iRedStoneCount,
		int iYellowStoneCount,int iGreenStoneCount,int iBackIronCount,int iScience,int iMoney,int iContribute,
		int content,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_PLAYER_RESOURCE_COUNT);
	this->iFoodCount = iFoodCount;
	this->iWoodCount = iWoodCount;
	this->iIronCount = iIronCount;
	this->iSkinCount = iSkinCount;
	this->iRedStoneCount = iRedStoneCount;
	this->iYellowStoneCount = iYellowStoneCount;
	this->iGreenStoneCount = iGreenStoneCount;
	this->iBackIronCount = iBackIronCount;
	this->iScience = iScience;
	this->iMoney = iMoney;
	this->iContribute = iContribute;
	this->iContent = content;
}
//��ҵĻ�����Դ
inline void 
PALEYER_RESOURCE_LIST::serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,int iRedStoneCount,
		int iYellowStoneCount,int iGreenStoneCount,int iBackIronCount,int content,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_BASE_RESOURCE_LIST);
	this->iFoodCount = iFoodCount;
	this->iWoodCount = iWoodCount;
	this->iIronCount = iIronCount;
	this->iSkinCount = iSkinCount;
	this->iRedStoneCount = iRedStoneCount;
	this->iYellowStoneCount = iYellowStoneCount;
	this->iGreenStoneCount = iGreenStoneCount;
	this->iBackIronCount = iBackIronCount;
	this->iContent = content;
}
//������Ϣ
inline void 
CITY_INFO::serialize(unsigned long lMapID, unsigned long lCityID,int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
}
//���뿤
inline void 
COMEIN_MAP::serialize(unsigned long lMapID, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_COMEIN_MAP);
	this->lMapID = lMapID;
}

//�����б���Ϣ
inline void 
CITY_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagCityList), 0, from, to, EN_S_CITY_LIST_INFO);
	this->iCount = count;
}

inline tagCityList & 
CITY_LIST::operator [](unsigned int index)
{
	return ((tagCityList *)((unsigned char *)this + sizeof(*this)))[index];
}
//��״̬��Ϣ
inline void 
MAP_STATE_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagMapState), 0, from, to, command);
	this->iCount = count;
}

inline tagMapState & 
MAP_STATE_LIST::operator [](unsigned int index)
{
	return ((tagMapState *)((unsigned char *)this + sizeof(*this)))[index];
}
//��ҳ����б���Ϣ
inline void 
PLAYER_CITY_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagPlayerCityList), 0, from, to, EN_S_PLAYER_CITY_LIST);
	this->iCount = count;
}

inline tagPlayerCityList & 
PLAYER_CITY_LIST::operator [](unsigned int index)
{
	return ((tagPlayerCityList *)((unsigned char *)this + sizeof(*this)))[index];
}
//���ڲ����б���Ϣ
inline void 
MAP_ARMY_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagArmysList), 0, from, to, command);
	this->iCount = count;
}

inline tagArmysList & 
MAP_ARMY_LIST::operator [](unsigned int index)
{
	return ((tagArmysList *)((unsigned char *)this + sizeof(*this)))[index];
}
//�������������� 
inline void 
CITY_ATTRIBUTE_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagCityAttributeList), 0, from, to, EN_S_CITY_ATTRIBUTE_LIST);
	this->iCount = count;
}

inline tagCityAttributeList & 
CITY_ATTRIBUTE_LIST::operator [](unsigned int index)
{
	return ((tagCityAttributeList *)((unsigned char *)this + sizeof(*this)))[index];
}
//�������
inline void 
CONSTRUCTION_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagConstructionList), 0, from, to, EN_S_CONSTRUCTION_COUNT);
	this->iCount = count;
}

inline tagConstructionList & 
CONSTRUCTION_LIST::operator [](unsigned int index)
{
	return ((tagConstructionList *)((unsigned char *)this + sizeof(*this)))[index];
}
//����ѽ����õĽ�����
inline void 
PLAYER_CONSTR_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagPlayerConstrList), 0, from, to, EN_S_CONSTRUCTION_COUNT);
	this->iCount = count;
}
inline tagPlayerConstrList & 
PLAYER_CONSTR_LIST::operator [](unsigned int index)
{
	return ((tagPlayerConstrList *)((unsigned char *)this + sizeof(*this)))[index];
}
//������ڽ����еĽ�����
inline void 
BUILDING_CONSTR_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagBuildingConstrList), 0, from, to, EN_S_BUILDING_CONSTR);
	this->iCount = count;
}
inline tagBuildingConstrList & 
BUILDING_CONSTR_LIST::operator [](unsigned int index)
{
	return ((tagBuildingConstrList *)((unsigned char *)this + sizeof(*this)))[index];
}
//���������еĵ���
inline void 
PRODUCEING_ITEM_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagProduceingItemList), 0, from, to, command);
	this->iCount = count;
}

inline tagProduceingItemList & 
PRODUCEING_ITEM_LIST::operator [](unsigned int index)
{
	return ((tagProduceingItemList *)((unsigned char *)this + sizeof(*this)))[index];
}
//�����������б�
inline void 
PRODUCTION_WEAPON_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagProduction), 0, from, to, EN_S_PRODUCE_WEAPON_COUNT);
	this->iCount = count;
}

inline tagProduction & 
PRODUCTION_WEAPON_LIST::operator [](unsigned int index)
{
	return ((tagProduction *)((unsigned char *)this + sizeof(*this)))[index];
}
//���������б�
inline void 
PRODUCTION_DEFEND_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagProduction), 0, from, to, EN_S_PRODUCE_DEFEND_COUNT);
	this->iCount = count;
}

inline tagProduction & 
PRODUCTION_DEFEND_LIST::operator [](unsigned int index)
{
	return ((tagProduction *)((unsigned char *)this + sizeof(*this)))[index];
}
//ѱ�޼�������
inline void 
PRODUCTION_ANIMAL_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagProduction), 0, from, to, EN_S_PRODUCE_ANIMAL_COUNT);
	this->iCount = count;
}

inline tagProduction & 
PRODUCTION_ANIMAL_LIST::operator [](unsigned int index)
{
	return ((tagProduction *)((unsigned char *)this + sizeof(*this)))[index];
}
//���⼼������
inline void 
PRODUCTION_SPECIAL_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagProduction), 0, from, to, EN_S_PRODUCE_SPECIAL_COUNT);
	this->iCount = count;
}

inline tagProduction & 
PRODUCTION_SPECIAL_LIST::operator [](unsigned int index)
{
	return ((tagProduction *)((unsigned char *)this + sizeof(*this)))[index];
}
//�Ѿ��о��˵ĵ����б�
inline void 
ITEM_RESEARCH_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItemResearch), 0, from, to, command);
	this->iCount = count;
}

inline tagItemResearch & 
ITEM_RESEARCH_LIST::operator [](unsigned int index)
{
	return ((tagItemResearch *)((unsigned char *)this + sizeof(*this)))[index];
}
//�����о��еĵ���
inline void 
RESEARCHING_ITEM_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagResearchingItemList), 0, from, to, command);
	this->iCount = count;
}

inline tagResearchingItemList & 
RESEARCHING_ITEM_LIST::operator [](unsigned int index)
{
	return ((tagResearchingItemList *)((unsigned char *)this + sizeof(*this)))[index];
}
//�о��������б�
inline void 
RESEARCH_WEAPON_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItemResearch), 0, from, to, EN_S_WEAPON_RESEARCH);
	this->iCount = count;
}

inline tagItemResearch & 
RESEARCH_WEAPON_LIST::operator [](unsigned int index)
{
	return ((tagItemResearch *)((unsigned char *)this + sizeof(*this)))[index];
}
//�о������б�
inline void 
RESEARCH_DEFEND_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItemResearch), 0, from, to, EN_S_DEFEND_RESEARCH);
	this->iCount = count;
}

inline tagItemResearch & 
RESEARCH_DEFEND_LIST::operator [](unsigned int index)
{
	return ((tagItemResearch *)((unsigned char *)this + sizeof(*this)))[index];
}
//�о�ѱ�޼����б�
inline void 
RESEARCH_ANIMAL_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItemResearch), 0, from, to, EN_S_ANIMAL_RESEARCH);
	this->iCount = count;
}

inline tagItemResearch & 
RESEARCH_ANIMAL_LIST::operator [](unsigned int index)
{
	return ((tagItemResearch *)((unsigned char *)this + sizeof(*this)))[index];
}
//�о����⼼���б�
inline void 
RESEARCH_SPECIAL_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItemResearch), 0, from, to, EN_C_SPECIAL_RESEARCH);
	this->iCount = count;
}

inline tagItemResearch & 
RESEARCH_SPECIAL_LIST::operator [](unsigned int index)
{
	return ((tagItemResearch *)((unsigned char *)this + sizeof(*this)))[index];
}
//��������
inline void 
LEVY_SOLDIER_PARAM::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagLevy_Soldier_Param), 0, from, to, EN_S_LEVY_SOLDIER_PARAM);
	this->iCount = count;
}

inline tagLevy_Soldier_Param & 
LEVY_SOLDIER_PARAM::operator [](unsigned int index)
{
	return ((tagLevy_Soldier_Param *)((unsigned char *)this + sizeof(*this)))[index];
}
//�������б���...
inline void 
LEVYING_SOLDIER::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagLevying_Soldier), 0, from, to, EN_S_LEVYING_SOLDIER);
	this->iCount = count;
}

inline tagLevying_Soldier & 
LEVYING_SOLDIER::operator [](unsigned int index)
{
	return ((tagLevying_Soldier *)((unsigned char *)this + sizeof(*this)))[index];
}
//������Ϣ
inline void
ARMY_INFO::serialize(int iCount,int iDefendID, int iWeaponLevel,int iDefendLevel, int iExp,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_SEND_ARMY_INFO);
	
	this->iCount = iCount;
	this->iDefendID = iDefendID;
	this->iWeaponLevel = iWeaponLevel;
	this->iDefendLevel = iDefendLevel;
	this->iExp = iExp;

}
//���ָ�ӹ��б���Ϣ
inline void 
PLAYER_OFFICER_LIST::serialize(int count, int from, int to,int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagPlayer_Officer_List), 0, from, to, command);
	this->iCount = count;
}
inline tagPlayer_Officer_List & 
PLAYER_OFFICER_LIST::operator [](unsigned int index)
{
	return ((tagPlayer_Officer_List *)((unsigned char *)this + sizeof(*this)))[index];
}

//�����б�
inline void 
WEAPON_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItem_List), 0, from, to, command);
	this->iCount = count;
}

inline tagItem_List & 
WEAPON_LIST::operator [](unsigned int index)
{
	return ((tagItem_List *)((unsigned char *)this + sizeof(*this)))[index];
}

//�����б�
inline void 
DEFEND_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItem_List), 0, from, to, command);
	this->iCount = count;
}

inline tagItem_List & 
DEFEND_LIST::operator [](unsigned int index)
{
	return ((tagItem_List *)((unsigned char *)this + sizeof(*this)))[index];
}
//ѱ�޼����б�
inline void 
ANIMAL_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItem_List), 0, from, to, command);
	this->iCount = count;
}

inline tagItem_List & 
ANIMAL_LIST::operator [](unsigned int index)
{
	return ((tagItem_List *)((unsigned char *)this + sizeof(*this)))[index];
}
//���⹤���б�
inline void 
SPECIAL_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItem_List), 0, from, to, command);
	this->iCount = count;
}

inline tagItem_List & 
SPECIAL_LIST::operator [](unsigned int index)
{
	return ((tagItem_List *)((unsigned char *)this + sizeof(*this)))[index];
}
//�����б�
inline void 
ARMY_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagArmyList), 0, from, to, EN_S_ARMY_LIST);
	this->iCount = count;
}

inline tagArmyList & 
ARMY_LIST::operator [](unsigned int index)
{
	return ((tagArmyList *)((unsigned char *)this + sizeof(*this)))[index];
}
//��Ʒ�б�
inline void 
GOODS_LIST::serialize(int count, int iGoodsType, int iPage, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagGoodsList), 0, from, to, EN_S_MARKET_GOODS_LIST);
	this->iCount = count;
	this->iGoodsType = iGoodsType;
	this->iPage = iPage;
}

inline tagGoodsList & 
GOODS_LIST::operator [](unsigned int index)
{
	return ((tagGoodsList *)((unsigned char *)this + sizeof(*this)))[index];
}



//�����ڳ��е�ͼ���ƶ�
inline void
ARMY_MOVE_MAP::serialize(unsigned long lMapID, unsigned long lArmyID, int iStartX, int iStartY,  int iEndX, int iEndY, int iflag, int from, int to, int command)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	 this->lMapID = lMapID;
	 this->lArmyID = lArmyID;
	 this->iStartX = iStartX;
	 this->iStartY = iStartY;
	  this->iEndX = iEndX;
	 this->iEndY = iEndY;
	 this->iflag = iflag;
}
//��������
inline void
LEVY_SOLDIER_COUNT::serialize(unsigned long lMapID,unsigned long lCityID,int iRace,int iCount,int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_LEVY_SOLDIER_COUNT);
	 this->lMapID = lMapID;
	 this->lCityID = lCityID;
	 this->iRace = iRace;
	 this->iCount = iCount;
}
//��������״̬��
inline void
LEVY_SOLDIER_INFO::serialize(int iRace,int iCount, int iState, int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_LEVY_SOLDIER_INFO);
	 this->iRace = iRace;
	 this->iCount = iCount;
	 this->iState = iState;
}
//��������
inline void
CREATE_ARMY::serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lOfficerID, char szName[],int iRaceID, int iSoldiertype, int iFSoldierCount, 
						int iFWeaponID,int iWeaponLevel, int iDefendID, int iDefendLevel, int iX, int iY, int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_CREATE_ARMY);
	 this->lMapID = lMapID;
	 this->lCityID = lCityID;
	 this->lOfficerID = lOfficerID;
	 this->iRaceID = iRaceID;
	 this->iSoldiertype = iSoldiertype;
	 this->iFSoldierCount = iFSoldierCount;
	 this->iFWeaponID = iFWeaponID;
	  this->iWeaponLevel = iWeaponLevel;
	 this->iDefendID = iDefendID;
	 this->iDefendLevel = iDefendLevel;
	 this->iX = iX;
	 this->iY = iY;
	 strcpy(this->szName, szName);
}
//��ಿ��
inline void
EDIT_ARMY::serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lOfficerID, 
		int iArmyID, int iDefendID, int iCount, int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_EDIT_ARMY);
	 this->lMapID = lMapID;
	 this->lCityID = lCityID;
	 this->lOfficerID = lOfficerID;
	 this->iArmyID = iArmyID;
	 this->iDefendID = iDefendID;
	 this->iCount = iCount;
}
//��������ʧ��
inline void 
CREATE_ARMY_FAILD::serialize(int iError, int from, int to, int command)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_CREATE_ARMY_FAILD);
	 this->iError = iError;
}

//��ɢ����EN_C_UNLAYARMY,EN_S_UNLAYARMY,
inline void tagUnlayArmy_Request::serialize(int playerid,int mapid,int armyid)
{
	//MapID = mapid;
	ArmyID = armyid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,mapid,EN_C_UNLAYARMY);
}

inline void tagUnlayArmy_Response::serialize(int agent,int mapid,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,mapid,playerid,EN_S_UNLAYARMY);
}

//ɾ������EN_C_DELETEARMY,EN_S_DELETEARMY,
inline void tagDeleteArmy_Request::serialize(int playerid,int mapid,int armyid)
{
	//MapID = mapid;
	ArmyID = armyid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,mapid,EN_C_DELETEARMY);
}

inline void tagDeleteArmy_Response::serialize(int agent,int mapid,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,mapid,playerid,EN_S_DELETEARMY);
}
//Ӣ��ϵͳ��Ϣ
inline void
HERO_SYSTEM_INFO::serialize(int iCode, int from, int to,  int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->iCode = iCode;
}
//����ָ�ӹ�
inline void
CREATE_OFFICER::serialize(char szNick[], int iFace, int iCountry, int iRaceID, int iJobID, int from, int to )
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_CREATE_OFFICER);
	strcpy(this->szNick, szNick);
	this->iFace = iFace;
	this->iCountry = iCountry;
	this->iRaceID = iRaceID;
	this->iJobID = iJobID;
}
//����ָ�ӹ�ʧ��
inline void 
CREATE_OFFICER_FAILD::serialize(int iError, int from, int to, int command)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_CREATE_OFFICER_FAILD);
	 this->iError = iError;
}
//����Ӣ��
inline void
DISMISS_HERO::serialize(unsigned long lHeroID, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_DISMISS_HERO);
	this->lHeroID = lHeroID;
}
//����Ӣ����Ϣ
inline void
DISMISS_HERO_INFO::serialize(int iCode, int from, int to,  int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->iCode = iCode;
}
//��ȡ������Ϣ
inline void
GET_ARMY_INFO::serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lArmyID,int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_GET_ARMY_INFO);
	 this->lMapID = lMapID;
	 this->lCityID = lCityID;
	 this->lArmyID = lArmyID;
}
//����������
inline void
BUILD_CONSTRUCTION::serialize(unsigned long lMapID,unsigned long lCityID,unsigned long lSortID,unsigned long lConstructionID,int iLevel,int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lSortID = lSortID;
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->lConstructionID = lConstructionID;
	this->iLevel = iLevel;
	
}
//��ٽ�����
inline void
DEMOLISH_CONSTRUCTION::serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_DEMOLISH_CONSTRUCTION);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->lPlayerID = lPlayerID;
}
//ȡ�������ｨ��
inline void
CANCEL_BUILD::serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lSortID, unsigned long lConID,int iLevel, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_CANCEL_BUILD);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->lSortID = lSortID;
	this->lConID = lConID;
	this->iLevel = iLevel;
}
//ȡ����������
inline void
CANCEL_PRODUCE::serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lSortID,int iType, int iItemID,int iLevel, int iCount, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_CANCEL_PRODUCE);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->lSortID = lSortID;
	this->iType = iType;
	this->iItemID = iItemID;
	this->iLevel = iLevel;
	this->iCount = iCount;
}
//ȡ�����е���
inline void 
CANCEL_RESEARCH::serialize(int iType, int iItemID,int iLevel,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_CANCEL_RESEARCH);
	this->iType = iType;
	this->iItemID = iItemID;
	this->iLevel = iLevel;
}
//��������
inline void
PRODUCT_ITEM::serialize(unsigned long lMapID,unsigned long lCityID, int iType, int iItem, int iLevel, int iCount,int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iType = iType;
	this->iItem = iItem;
	this->iLevel = iLevel;
	this->iCount = iCount;
	
}
//�о�
inline void
ITEM_RESEARCH::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
}
//�о�����
inline void
RESEARCH_ITEM::serialize(int iType, int iItemID, int iLevel, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->iType = iType;
	this->iItemID = iItemID;
	this->iLevel = iLevel;
}
//�����г���Ϣ
inline void
COUNTRY_MARKET_INFO::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
}
//�����г���Ʒ����
inline void
COUNTRY_MARKET_BUSINESS::serialize(unsigned long lMapID, unsigned long lCityID, int iType, int iSaleID, int iLevel, int iCount,  int iMode, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iType = iType;
	this->iSaleID = iSaleID;
	this->iLevel = iLevel;
	this->iCount = iCount;
	this->iMode = iMode;
}
//����г���Ϣ
inline void
PEOPLE_MARKET::serialize(unsigned long lSMapID, unsigned long lSCityID, int iType, int iMarketType, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lSMapID = lSMapID;
	this->lSCityID = lSCityID;
	this->iType = iType;
	this->iMarketType = iMarketType;
}
//��Ʒ����
inline void PUT_UP::serialize(unsigned long lMapID, unsigned long lCityID, int iType, int iSaleID, unsigned long lSortID, 
		int iCount, int iMinPrice, int iOncePrice, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iType = iType;
	this->iSaleID = iSaleID;
	this->lSortID = lSortID;
	this->iCount = iCount;
	this->iMinPrice = iMinPrice;
	this->iOncePrice = iOncePrice;
}
//����
inline void GOODS_BIDDEN::serialize(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lSortID, int iCurrentPrice, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iType = iType;
	this->lSortID = lSortID;
	this->iCurrentPrice = iCurrentPrice;
}
//������Ʒ
inline void BUY_GOODS::serialize(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lSortID, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iType = iType;
	this->lSortID = lSortID;
//	this->iLevel = iLevel;
}
//�㲥������Ϣ
inline void BROADCAST_CONTRIBUTE::serialize(char szPlayerName[], unsigned long lMapID,unsigned long lCityID, int iLevel, int iType, int iMoney,int iCredit, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_BROADCAST_CONTRIBUTE);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iLevel = iLevel;
	this->iType = iType;
	this->iMoney = iMoney;
	this->iCredit = iCredit;
	strcpy(this->szPlayerName, szPlayerName);
}
//����
inline void PLAYER_CONTRIBUTE::serialize(unsigned long lMapID,unsigned long lCityID, int iMoney, int iType, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_CONTRIBUTE);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	//this->lConID = lConID;
	this->iType = iType;
	this->iMoney = iMoney;
}
//������Ϣ
inline void MAP_CITY_INFO::serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,
		int iHuman,int iMannikin,int iFairy,int iDandiprat,int iHalfAnimal,int iCattle,int iBlackFairy,int iDecPerson,
		int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_SEND_CITY_INFO);
	this->iFoodCount = iFoodCount;
	this->iWoodCount = iWoodCount;
	this->iSkinCount = iSkinCount;
	this->iIronCount = iIronCount;
	this->iHuman = iHuman;
	this->iMannikin = iMannikin;
	this->iFairy = iFairy;
	this->iDandiprat = iDandiprat;
	this->iHalfAnimal = iHalfAnimal;
	this->iCattle = iCattle;
	this->iBlackFairy = iBlackFairy;
	this->iDecPerson = iDecPerson;
}
//����ת��
inline void
ARMY_TRANSFER::serialize(unsigned long lRMapID, unsigned long lTargetMapID, unsigned long  lArmyID, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_ARMYS_TRANSFER);
	this->lRMapID = lRMapID;
	this->lTargetMapID = lTargetMapID;
	this->lArmyID = lArmyID;
}
//��ҵı�����
inline void
PLAYER_SOLDIER_LIST::serialize(int iHuman, int iFairy, int iMannikin, int iDandiprat, int iHalfAnimal, int iCattle, int iBlackFairy, int iDecPerson, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_ALL_SOLDIERS);
	this->iHuman = iHuman;
	this->iFairy = iFairy;
	this->iMannikin = iMannikin;
	this->iDandiprat = iDandiprat;
	this->iHalfAnimal = iHalfAnimal;
	this->iCattle = iCattle;
	this->iBlackFairy = iBlackFairy;
	this->iDecPerson = iDecPerson;
}

//���ӳ��е�ͼ�Ͽ��ƶ��������б�
inline void 
ARMY_CAN_MOVE_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagArmyMoveList), 0, from, to, EN_S_ARMY_MOVE);
	this->iCount = count;
}

inline tagArmyMoveList & 
ARMY_CAN_MOVE_LIST::operator [](unsigned int index)
{
	return ((tagArmyMoveList *)((unsigned char *)this + sizeof(*this)))[index];
}
//�޸�ָ�ӹ�����
inline void
MODIFY_OFFICER_NAME::serialize(unsigned long lOfficerID, char szOfficerName[],int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_MODIFY_OFFICER_NAME);
	this->lOfficerID = lOfficerID;
	strcpy(this->szOfficerName, szOfficerName);
}
//�ɹ���Ϣ��
inline void 
SUCCEED_INFO::serialize(unsigned long lCityID, unsigned long lSortID, int iEventID, int iLevel, int iCount, int iCode, int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_SUCCEED_INFO);
	 this->lCityID = lCityID;
	 this->lSortID = lSortID;
	 this->iEventID = iEventID;
	 this->iLevel = iLevel;
	 this->iCount = iCount;
	 this->iCode = iCode;
}
//�г���Ϣ��
inline void 
MARKET_INFO::serialize(int iTypeID, int iGoodsID, int iCount, int iCode, int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_MARKET_INFO);
	 this->iTypeID = iTypeID;
	 this->iGoodsID = iGoodsID;
	 this->iCount = iCount;
	 this->iCode = iCode;
}
//������Ϣ
inline void 
ERROR_INFO::serialize(int iCode, int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_ERROR_INFO);
	 this->iCode = iCode;
}
//�˳���Ϸ
inline void
EXIT_MAP_SERVER::serialize(int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_EXIT_MAP_SERVER);
}
//�˳���Ϸ
inline void
EXIT_GAME::serialize(unsigned long lPlayerID, unsigned long lMapID, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_EXIT_MAP_SERVER);
	this->lPlayerID = lPlayerID;
	this->lMapID = lMapID;
}
//ָ�ӹټ���
inline void 
OFFICER_TECH_LIST::serialize(int officerid,int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagOfficerTechList), 0, from, to, command);
	this->lOfficerID = officerid;
	this->iCount = count;
}

inline tagOfficerTechList & 
OFFICER_TECH_LIST::operator [](unsigned int index)
{
	return ((tagOfficerTechList *)((unsigned char *)this + sizeof(*this)))[index];
}

//ѧϰָ�ӹټ���
inline void 
STUDY_OFFICER_TECH::serialize(unsigned long lOfficerID, int iTechID, int iType, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lOfficerID = lOfficerID;
	this->iTechID = iTechID;
	this->iType = iType;
}
//ָ�ӹ���Ϣ
inline void
GET_OFFICER_INFO:: serialize(unsigned long lOfficerID,int from, int to, int command)	
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lOfficerID = lOfficerID;
}
//ָ�ӹ�����
inline void
OFFICER_ATTRIBUTE::serialize(int officerid,int iCredit, int iStamina, int iPower, int iCelerity, int iWit, int iAttack,
		int iPhysicRecovery, int iMagicRecovery,  int iLevel, int iMilitaryRank_, int iExp,int techdot,int iActionDot, 
		int iEquipStamina,int iEquipPower,int iEquipCelerity,int iEquipWit,int iHeroHP,int iMagicAttack,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_OFFICERO_ATTRIBUTE);
	this->OfficerID = officerid;
	this->iCredit = iCredit;
	this->iStamina = iStamina;
	this->iPower = iPower;
	this->iCelerity = iCelerity;
	this->iWit = iWit;
	this->iAttack = iAttack;
	this->iPhysicRecovery = iPhysicRecovery;
	this->iMagicRecovery  = iMagicRecovery;
	this->iLevel = iLevel;
	this->iMilitaryRank_ = iMilitaryRank_;
	this->Exp = iExp;
	this->TechDot = techdot;
	this->ActionDot = iActionDot;
	this->EquipStamina = iEquipStamina;
	this->EquipPower = iEquipPower;
	this->EquipCelerity = iEquipCelerity;
	this->EquipWit = iEquipWit;
	this->HeroHP = iHeroHP;
	this->MagicAttack = iMagicAttack;
}
//Ӣ��װ���б�
inline void 
HERO_EQUIP_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagHeroEquipList), 0, from, to, command);
	this->iCount = count;
}

inline tagHeroEquipList & 
HERO_EQUIP_LIST::operator [](unsigned int index)
{
	return ((tagHeroEquipList *)((unsigned char *)this + sizeof(*this)))[index];
}
//���������е�װ��
inline void 
EXCHANGE_EQUIP_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagExchangeEquipList), 0, from, to, EN_C_PACK_EXCHANGE);
	this->iCount = count;
}

inline tagExchangeEquipList & 
EXCHANGE_EQUIP_LIST::operator [](unsigned int index)
{
	return ((tagExchangeEquipList *)((unsigned char *)this + sizeof(*this)))[index];
}
//���Ӣ��
inline void
REDEEM_HERO:: serialize(unsigned long lHeroID, int iMoney, int from, int to)	
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_REDEEM_HERO);
	this->lHeroID = lHeroID;
	this->iMoney = iMoney;
}
//����װ��
inline void
EXCHANGE_EQUIP_INFO::serialize(int iCode, int iSortID, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_EXCHANGE_EQUIP);
	this->iCode = iCode;
	this->iSortID = iSortID;
//	this->iItemID = iItemID;
}
//����װ��
inline void
DESTROY_EQUIP::serialize(unsigned long lHeroID, int iSortID, int count,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_DESTROY_EQUIP);
	this->lHeroID = lHeroID;
	this->iSortID = iSortID;
	this->Count = count;
}

//Ӣ��ս����¼
inline void 
HERO_WAR_RECORD_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagHeroWarRecord), 0, from, to, EN_S_HERO_WAR_RECORD);
	this->iCount = count;
}

inline tagHeroWarRecord & 
HERO_WAR_RECORD_LIST::operator [](unsigned int index)
{
	return ((tagHeroWarRecord *)((unsigned char *)this + sizeof(*this)))[index];
}
//Ӣ�ۼ�ɳ���¼
inline void 
HERO_GROWUP_RECORD_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagHeroGrowUpRecord), 0, from, to, EN_S_HERO_GROWUP_RECORD);
	this->iCount = count;
}

inline tagHeroGrowUpRecord & 
HERO_GROWUP_RECORD_LIST::operator [](unsigned int index)
{
	return ((tagHeroGrowUpRecord *)((unsigned char *)this + sizeof(*this)))[index];
}
//��Ϸ�е�ʱ��
inline void
GAME_TIME::serialize(long long lGameTime, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lGameTime = lGameTime;
}
//װ��/ж������
inline void 
MATERIAL_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagMaterials), 0, from, to, command);
	this->iCount = count;
	this->lArmyID = lArmyID;
	this->lMapID = lMapID;
	this->lCityID = lCityID;
}
inline tagMaterials & 
MATERIAL_LIST::operator [](unsigned int index)
{
	return ((tagMaterials *)((unsigned char *)this + sizeof(*this)))[index];
}
//ֳ�����
inline void
COLONIZE_CITY::serialize(unsigned long lArmyID, unsigned long lMapID, unsigned long lCityID, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lArmyID = lArmyID;
	this->lMapID = lMapID;
	this->lCityID = lCityID;
}
//���ʲ鿴
inline void
LOOK_MATERIALS::serialize(unsigned long lArmyID, unsigned long lMapID, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lArmyID = lArmyID;
	this->lMapID = lMapID;
}
//���ν���
inline void
MILITARY_RANK::serialize(char szName[], int iRank, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	strcpy(this->szName, szName);
	this->iRank = iRank;
}
//Ӣ�۱���
inline void
CAPTIVE_HERO::serialize(char szByPlayerName[], char szByHeroName[], char szPlayerName[], int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	strcpy(this->szByPlayerName, szByPlayerName);
	strcpy(this->szByHeroName, szByHeroName);
	strcpy(this->szPlayerName, szPlayerName);
}
//�����ͼ
inline void
COMEIN_EXIT_MAP::serialize(char szName[], unsigned long lMapID, int iType, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	strcpy(this->szName, szName);
	this->lMapID = lMapID;
	this->iType = iType;
}
//���ӴﵽԤ��
inline void
ARMY_REACH_WARM ::serialize( unsigned long lMapID,int from, int to,int unioncnt,int clancnt,int aboriginalcnt)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_BROADCAST_ARMY_WARM);
	this->lMapID = lMapID;
	this->UnionCount = unioncnt;
	this->ClanCount = clancnt;
	this->AboriginalCount = aboriginalcnt;
}
inline void
CONTRIBUTE_INFO::serialize(unsigned long lCityID, int iLevel, int iCurMoney, int iTolMoney, int iAttack, int iHP, int iAttackScop, int iVisualRange, long lCMoney, long lPMoney, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_CONTRIBUTE_INFO);
	this->lCityID = lCityID;
	this->iLevel = iLevel;
	this->iCurMoney = iCurMoney;
	this->iTolMoney = iTolMoney;
	this->iAttack = iAttack;
	this->iHP = iHP;
	this->iAttackScop = iAttackScop;
	this->iVisualRange = iVisualRange;
	this->lCMoney = lCMoney;
	this->lPMoney = lPMoney;
}
//��������·��
inline void 
ARMY_MOVE_PATH::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagOutingArmysList), 0, from, to, command);
	this->iCount = count;
}
inline tagOutingArmysList & 
ARMY_MOVE_PATH::operator [](unsigned int index)
{
	return ((tagOutingArmysList *)((unsigned char *)this + sizeof(*this)))[index];
}
//���Ӳ��ܾ���
inline void 
ARMY_CANT_MOVE ::serialize(int iCode, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_ARMY_CANT_MOVE);
	this->iCode = iCode;			//��Ϣ��
}
//��Դ�ɼ���
inline void
RESOURCE_RATE::serialize(int iFoodRate, int iWoodRate, int iSkinRate, int iIronRate, int iScience, int from, int to,int cmd)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, cmd);
	this->iFoodRate = iFoodRate;
	this->iWoodRate = iWoodRate;
	this->iSkinRate = iSkinRate;
	this->iIronRate = iIronRate;
	this->iScience = iScience;
}


//����һ���µı��䵽Ӣ�۱���EN_C_NEWTREASUREPACK,EN_S_NEWTREASUREPACK,
inline void tagNewTreasurePack_Request::serialize(int playerid,int officerid,int itemid,int type)
{
	OfficerID = officerid;
	ItemID = itemid;
	ItemType = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_NEWTREASUREPACK);
}

inline void tagNewTreasurePack_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	if (code == 0)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_NEWTREASUREPACK);
	}
	else
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) - sizeof(tagHeroEquipList),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_NEWTREASUREPACK);
	}
}

//�򿪱���EN_C_OPENTREASUREPACK,EN_S_OPENTREASUREPACK,
inline void tagOpenTreasurePack_Request::serialize(int playerid,int mapid,int cityid,int officerid,int sortid)
{
	MapID = mapid;
	CityID = cityid;
	OfficerID = officerid;
	SortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_OPENTREASUREPACK);
}

inline void tagOpenTreasurePack_Response::serialize(int agent,int playerid,int code,int money,int itemid,int randcount,
													int equipid,int sortid,int level,int pos,int showpos,int type,int equipcount)
{
	Error_Code = code;
	Money = money;
	ItemID = itemid;
	Count = randcount;
	//EquipItem.iItemID = equipid;
	EquipItem.InlayItem.NewItemID = equipid;
	EquipItem.iSortID = sortid;
	EquipItem.iLevel = level;
	EquipItem.iPos = pos;
	EquipItem.iShowPos = showpos;
	EquipItem.iType = type;
	EquipItem.iCount = equipcount;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_OPENTREASUREPACK);
}

//Ӣ������֪ͨEN_C_HEROGBH_NOTIFY
inline void tagHeroGBH_Notify::serialize(int playerid,int officerid)
{
	OfficerID = officerid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_HEROGBH_NOTIFY);
}

inline void tagHeroGBH_Broadcast::serialize(const char *playername,const char *officername)
{
	if(playername)
	{
		strcpy(PlayerName,playername);
	}
	if(officername)
	{
		strcpy(OfficerName,officername);
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,0,0,EN_S_HEROGBH_NOTIFY);
}

inline void ReqHerosHangSys::serialize(unsigned long doorId, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_REQ_HEROS_HANGINGSYS);
	this->doorId = doorId;
}

inline void RspHerosHangSys::serialize(unsigned int count, int agent, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagHangingItemList), agent, from, to, EN_RSP_HEROS_HANGINGSYS);
	this->count = count;
}

inline tagHangingItemList &RspHerosHangSys::operator [](unsigned int index)
{
	return ((tagHangingItemList *)((unsigned char *)this + sizeof(*this)))[index];
}

inline void ReqStartHangSystemOneHero::serialize(unsigned long heroID, unsigned int hangSysType, unsigned int trainingTime, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_REQ_HANG_ONEHERO);
	this->heroID = heroID;
	this->hangSysType = hangSysType;
	this->trainingTime = trainingTime;
}

inline void RspStartHangSystemOneHero::serialize(unsigned int ret, unsigned long heroId, unsigned int completeTime, unsigned long expVal, unsigned long moneyVal, int agent, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agent, from, to, EN_RSP_HANG_ONEHERO);
	this->ret = ret;
	this->heroId = heroId;
	this->completeTime = completeTime;
	this->expVal = expVal;
	this->moneyVal = moneyVal;
}

inline void ReqCancelHangOneHero::serialize(unsigned long heroID, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_REQ_CANCEL_HANG_ONEHERO);
	this->heroID = heroID;
}

inline void RspCancelHangOneHero::serialize(unsigned int ret, int agent, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agent, from, to, EN_RSP_CANCEL_HANG_ONEHERO);
	this->ret = ret;
}

inline void RspHangOneHeroFinish::serialize(unsigned long heroID, int agent, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agent, from, to, EN_RSP_TIMEOUT_HANG_ONEHERO);
	this->heroID = heroID;
}


#endif

