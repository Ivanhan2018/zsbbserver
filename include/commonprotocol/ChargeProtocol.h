#ifndef CHARGEPROTOCOL_H__
#define CHARGEPROTOCOL_H__
#pragma warning(disable:4244)
#include <string.h>
#include "Distributed_Header.h"
#include "Global_Macro.h"
#pragma pack(push,1)
#define DEFAULT_AGENT		4000
#define MAX_PACKNUM			60

enum ExchangeType
{
	GO2Money = 0,					//Go��һ����
	Money2GO						//��Ҷһ�Go��
};
enum LogType
{
	BuyCard_Log = 0,				//����־��ʽ
	UseCard_Log,					//ʹ�ÿ���־��ʽ
	UseAutoCard_Log,				//ʹ���йܿ���־��ʽ
	ImmedComplete_Log,				//���������־��ʽ
	Election_Log,					//�μӾ�ѡ��־��ʽ
	AddGODian_Log,					//����Go����־��ʽ
	ImmedResourceRate_Log,			//��������ʵ���־��ʽ
	UpdateConsortia_Log,			//�������ſ۳�GO��
};


//��ѯGo��CGC_C_QUERYMONEY,CGC_S_QUERYMONEY
typedef struct tagQueryMoney_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}QueryMoney_Request,*LPQueryMoney_Request;
inline void tagQueryMoney_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_QUERYMONEY);
}

typedef struct tagQueryMoney_Response:DISTRIBUTED_HEADER
{
	int Money;					//��ҵ�Go�㣬<0ʱ��ʾʧ�ܣ�>=0ΪGo��
	inline void serialize(int agent,int playerid,int money);
}QueryMoney_Response,*LPQueryMoney_Response;
inline void tagQueryMoney_Response::serialize(int agent,int playerid,int money)
{
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_QUERYMONEY);
}

//ʹ�ÿ��Ļ�����Ϣ�ṹ
typedef struct tagBaseUseCard_Request:DISTRIBUTED_HEADER
{
	int OfficerID;					//�������ڱ�����Ӣ��ID������������Ϊ1
	int SortID;						//��ˮ��
	//int ItemID;						//����ID
	inline void serialize(int length,int playerid,int officerid,int sortid,int cmd);
}BaseUseCard_Request,*LPBaseUseCard_Request;
inline void tagBaseUseCard_Request::serialize(int length,int playerid,int officerid,int sortid,int cmd)
{
	OfficerID = officerid;
	SortID = sortid;
	//ItemID = itemid;
	DISTRIBUTED_HEADER::serialize(length,0,playerid,MDM_GP_MAP_CHARGE,cmd);
}

//����CGC_C_BUYCARD/CGC_S_BUYCARD,	
typedef struct tagBuyCard_Request:DISTRIBUTED_HEADER
{
	int ItemID;						//����ID����ID��
	inline void serialize(int playerid,int itemid);
}BuyCard_Request,*LPBuyCard_Request;
inline void tagBuyCard_Request::serialize(int playerid,int itemid)
{
	ItemID = itemid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_BUYCARD);
}

typedef struct tagBuyCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;
	inline void serialize(int agent,int playerid,int code);
}BuyCard_Response,*LPBuyCard_Response;
inline void tagBuyCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;					//���������0���ɹ���1��Go�㲻�㣻2������������111������
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_BUYCARD);
}

//ʹ�û�����Դ�������������ԴCGC_C_RESOURCECARD��CGC_S_RESOURCECARD
typedef struct tagUseResourceCard_Request:tagBaseUseCard_Request
{
	int MapID;
	int CityID;
	inline void serialize(int playerid,int officerid,int MapID,int CityID,int sortid);
}UseResourceCard_Request,*LPUseResourceCard_Request;
inline void tagUseResourceCard_Request::serialize(int playerid,int officerid,int mapid,int cityid,int sortid)
{
	MapID = mapid;
	CityID = cityid;
	//DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_RESOURCECARD);
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_RESOURCECARD);
}

typedef struct tagUseResourceCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//���������0���ɹ���1:���û�иõ��ߣ�2:�ֿ�����������5:����ʹ�ã�111������
	inline void serialize(int agent,int playerid,int code);
}UseResourceCard_Response,*LPUseResourceCard_Response;
inline void tagUseResourceCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_RESOURCECARD);
}

//�������ٿ�CGC_C_RESOURCESPEEDCARD,CGC_S_RESOURCESPEEDCARD,
typedef struct tagUseResourceSpeedCard_Request:tagBaseUseCard_Request
{
	inline void serialize(int playerid,int officerid,int sortid);
}UseResourceSpeedCard_Request,*LPUseResourceSpeedCard_Request;
inline void tagUseResourceSpeedCard_Request::serialize(int playerid,int officerid,int sortid)
{
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_RESOURCESPEEDCARD);
}

typedef struct tagUseResourceSpeedCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//���������0���ɹ���1:���û�иõ��ߣ�5:����ʹ�ã�111������
	inline void serialize(int agent,int playerid,int code);
}UseResourceSpeedCard_Response,*LPUseResourceSpeedCard_Response;
inline void tagUseResourceSpeedCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_RESOURCESPEEDCARD);
}

//��ļӢ�ۿ�CGC_C_RESRUITHEROCARD,CGC_S_RESRUITHEROCARD,
typedef struct tagUseResruitHeroCard_Request:tagBaseUseCard_Request
{
	inline void serialize(int playerid,int officerid,int sortid);
}UseResruitHeroCard_Request,*LPUseResruitHeroCard_Request;
inline void tagUseResruitHeroCard_Request::serialize(int playerid,int officerid,int sortid)
{
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_RESRUITHEROCARD);
}

typedef struct tagUseResruitHeroCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//���������0���ɹ���1:���û�иõ��ߣ�5:����ʹ�ã�111������
	inline void serialize(int agent,int playerid,int code);
}UseResruitHeroCard_Response,*LPUseResruitHeroCard_Response;
inline void tagUseResruitHeroCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_RESRUITHEROCARD);
}

//���⼼�ܿ�CGC_C_SPECIALSKILLCARD,	CGC_S_SPECIALSKILLCARD,
typedef struct tagUseSpecialSkillCard_Request:tagBaseUseCard_Request
{
	int HeroID;					//ѧϰ���⼼�ܵ�Ӣ��ID
	inline void serialize(int playerid,int officerid,int sortid,int heroid);
}UseSpecialSkillCard_Request,*LPUseSpecialSkillCard_Request;
inline void tagUseSpecialSkillCard_Request::serialize(int playerid,int officerid,int sortid,int heroid)
{
	HeroID = heroid;
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_SPECIALSKILLCARD);
}

typedef struct tagUseSpecialSkillCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//���������0���ɹ���1:���û�иõ��ߣ�2:Ӣ�ۼ���������5:����ʹ�ã�111������
	inline void serialize(int agent,int playerid,int code);
}UseSpecialSkillCard_Response,*LPUseSpecialSkillCard_Response;
inline void tagUseSpecialSkillCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_SPECIALSKILLCARD);
}

//���졢���С��������ٿ�CGC_C_OTHERSPEEDCARD,CGC_S_OTHERSPEEDCARD,
typedef struct tagUseOtherSpeedCard_Request:tagBaseUseCard_Request
{
	int MapID;
	int CityID;
	inline void serialize(int playerid,int officerid,int sortid,int mapid,int cityid);
}UseOtherSpeedCard_Request,*LPUseOtherSpeedCard_Request;
inline void tagUseOtherSpeedCard_Request::serialize(int playerid,int officerid,int sortid,int mapid,int cityid)
{
	MapID = mapid;
	CityID = cityid;
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_OTHERSPEEDCARD);
}

typedef struct tagUseOtherSpeedCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//���������0���ɹ���1:���û�иõ��ߣ�5:����ʹ�ã�111������
	inline void serialize(int agent,int playerid,int code);
}UseOtherSpeedCard_Response,*LPUseOtherSpeedCard_Response;
inline void tagUseOtherSpeedCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_OTHERSPEEDCARD);
}

//ʹ���йܿ�CGC_C_AUTOCARD,	CGC_S_AUTOCARD,
typedef struct tagAutoConstruction
{
	int MapID;							//����ͼID
	int CityID;							//����ID
	int ConstructionID;					//������ID
	int SortID;							//��Ϊ0ʱ����ʾ���콨��������Ϊ�����������SortID
	int Level;							//����
	inline __int64 GetKey();
}AutoConstruction,*LPAutoConstruction;	
inline __int64 tagAutoConstruction::GetKey()
{
	__int64 key;
	key = MapID + CityID + SortID;
	return key;
}
typedef struct tagUseAutoCard_Request:tagBaseUseCard_Request
{
	int Count;						//�йܽ�����������ýṹ�����Count��AutoConstruction
	inline void serialize(int length,int playerid,int officerid,int sortid,int count);//�������йܿ�
}UseAutoCard_Request,*LPUseAutoCard_Request;
inline void tagUseAutoCard_Request::serialize(int length,int playerid,int officerid,int sortid,int count)
{
	OfficerID = officerid;
	SortID = sortid;
	Count = count;
	DISTRIBUTED_HEADER::serialize(length,0,playerid,MDM_GP_MAP_CHARGE,CGC_C_AUTOCARD);
}

typedef struct tagUseAutoCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//���������0���ɹ���1:���û�иõ��ߣ�2:�й��������ܲ����ÿ������������3���й��У�4:�޸��й�״̬ʧ�ܣ�5������ʹ�ã�������ʧ��
	inline void serialize(int agent,int playerid,int code);
}UseAutoCard_Response,*LPUseAutoCard_Response;
inline void tagUseAutoCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_AUTOCARD);
}
//ȡ���й�CGC_C_CANCELAUTO,CGC_S_CANCELAUTO
typedef struct tagCancelAuto_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}CancelAuto_Request,*LPCancelAuto_Request;
inline void tagCancelAuto_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_CANCELAUTO);
}
	
typedef struct tagCancelAuto_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//���������0���ɹ���1:δ�йܣ�111������
	inline void serialize(int agent,int playerid,int code);
}CancelAuto_Response,*LPCancelAuto_Response;
inline void tagCancelAuto_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_CANCELAUTO);
}

//��ѯ�й��б�CGC_C_QUERYAUTOLIST,	CGC_S_QUERYAUTOLIST,
typedef struct tagQueryAutoList_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}QueryAutoList_Request,*LPQueryAutoList_Request;
inline void tagQueryAutoList_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_QUERYAUTOLIST);
}

//���صĽṹ��AutoConstruction
typedef struct tagQueryAutoList_Response:DISTRIBUTED_HEADER
{
	int Count;							//����������-1��δ�й�
	inline void serialize(int length,int agent,int playerid,int count);
}QueryAutoList_Response,*LPQueryAutoList_Response;
inline void tagQueryAutoList_Response::serialize(int length,int agent,int playerid,int count)
{
	Count = count;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_QUERYAUTOLIST);
}

//װ����CGC_C_HEROEQUIPCARD,CGC_S_HEROEQUIPCARD,
typedef struct tagUseHeroEquipCard_Request:tagBaseUseCard_Request
{
	inline void serialize(int playerid,int officerid,int sortid);
}UseHeroEquipCard_Request,*LPUseHeroEquipCard_Request;
inline void tagUseHeroEquipCard_Request::serialize(int playerid,int officerid,int sortid)
{
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_HEROEQUIPCARD);
}

typedef struct tagUseHeroEquipCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//���������0���ɹ���1:���û�иõ��ߣ�2:�����ڶ�Ӧ��װ����5:����ʹ�ã�111������
	inline void serialize(int agent,int playerid,int code);
}UseHeroEquipCard_Response,*LPUseHeroEquipCard_Response;
inline void tagUseHeroEquipCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_HEROEQUIPCARD);
}

//��ѯ��ǰĳ��������CGC_C_QUERYRESOURCERATE,CGC_S_QUERYRESOURCERATE	
typedef struct tagQueryResourceRate_Request:DISTRIBUTED_HEADER
{
	CardType Type;
	inline void serialize(int playerid,CardType type);
}QueryResourceRate_Request,*LPQueryResourceRate_Request;
inline void tagQueryResourceRate_Request::serialize(int playerid,CardType type)
{
	Type = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_QUERYRESOURCERATE);
}

typedef struct tagQueryResourceRate_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//���������>=0:��ǰֵ��<0ʧ��
	inline void serialize(int agent,int playerid,int code);
}QueryResourceRate_Response,*LPQueryResourceRate_Response;
inline void tagQueryResourceRate_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_QUERYRESOURCERATE);
}

//ʹ�ñ�����չ��CGC_C_EXPANDPACKCARD,CGC_S_EXPANDPACKCARD,	
typedef struct tagUseExpandPackCard_Request:tagBaseUseCard_Request
{
	int PackID;						//����չ�ı������ͣ�1������������Ӣ��IDΪӢ�۱���
	inline void serialize(int playerid,int officerid,int sortid,int packid = 1);
}UseExpandPackCard_Request,*LPUseExpandPackCard_Request;
inline void tagUseExpandPackCard_Request::serialize(int playerid,int officerid,int sortid,int packid)
{
	PackID = packid;
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_EXPANDPACKCARD);
}

typedef struct tagUseExpandPackCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//0���ɹ���1:���û�иõ��ߣ�2:�����ѵ����������5:����ʹ�ã�111������
	int PackNum;					//����������Error_CodeΪ0ʱ��Ч
	inline void serialize(int agent,int playerid,int code,int num);
}UseExpandPackCard_Response,*LPUseExpandPackCard_Response;
inline void tagUseExpandPackCard_Response::serialize(int agent,int playerid,int code,int num)
{
	Error_Code = code;
	PackNum = num;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_EXPANDPACKCARD);
}

//�������֪ͨ��ϢCGC_C_NOTIFY_IMMEDCOMPLETE,CGC_S_NOTIFY_IMMEDCOMPLETE������Ϣ�ɸ�ҵ����������͹���
typedef struct tagImmedComplete_Param
{
	int Type;						//ҵ�����ͣ��磺����������ɡ�����������ɡ�����������ɡ�����������ɡ���ļӢ���������
	int MapID;
	int CityID;
	int ProductionType;				//��Ҫ��������࣬��ʾ��������Ʒ���ͣ������������ߡ����⹤�ߵȣ�����ο�GOODS_TYPEö��
	int ItemID;
	int SortID;
	int Level;
	int GOCount;					//��Ҫ���ĵ�GO����
}ImmedComplete_Param,*LPImmedComplete_Param;

typedef struct tagNotify_ImmedComplete_Request:DISTRIBUTED_HEADER
{//from Ϊ������ID
	int PlayerID;
	ImmedComplete_Param Param;
	inline void serialize(int agent,int playerid,int type,int mapid,int cityid,int itemid,int sortid,int level,int count,int protype);
}Notify_ImmedComplete_Request,*LPNotify_ImmedComplete_Request;
inline void tagNotify_ImmedComplete_Request::serialize(int agent,int playerid,int type,int mapid,int cityid,int itemid,int sortid,int level,int count,int protype)
{
	PlayerID = playerid;
	Param.Type = type;
	Param.MapID = mapid;
	Param.CityID = cityid;
	Param.ProductionType = protype;
	Param.ItemID = itemid;
	Param.SortID = sortid;
	Param.Level = level;
	Param.GOCount = count;
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
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,svrid,MDM_GP_MAP_CHARGE,CGC_C_NOTIFY_IMMEDCOMPLETE);
}
//����ҵ�������
typedef struct tagNotify_ImmedComplete_Response:DISTRIBUTED_HEADER
{
	int Error_Code;				//0���ɹ���1��GO�㲻�㣻111������ʧ��
	ImmedComplete_Param Param;
	inline void serialize(int agent,int playerid,int type,int mapid,int cityid,int itemid,int sortid,int level,int code,int count,int protype);
}Notify_ImmedComplete_Response,*LPNotify_ImmedComplete_Response;
inline void tagNotify_ImmedComplete_Response::serialize(int agent,int playerid,int type,int mapid,int cityid,int itemid,int sortid,int level,int code,int count,int protype)
{
	Error_Code = code;
	Param.Type = type;
	Param.MapID = mapid;
	Param.CityID = cityid;
	Param.ProductionType = protype;
	Param.ItemID = itemid;
	Param.SortID = sortid;
	Param.Level = level;
	Param.GOCount = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CHARGE,CGC_S_NOTIFY_IMMEDCOMPLETE);
}
//�μӾ�ѡ�۷�CGC_C_NOTIFY_ELECTION,CGC_S_NOTIFY_ELECTION
typedef struct tagNotify_Election_Request:DISTRIBUTED_HEADER
{
	int CountryID;				//��Ӫ
	short Dept;					//��ѡְλ��������������������ο�Dept_Type
	char Creed[100];
	int Money;
	int GOMoney;
	inline void serialize(int agent,int playerid,int countryid,short type,const char *pCreed,int money,int gomoney);
}Notify_Election_Request,*LPNotify_Election_Request;
inline void tagNotify_Election_Request::serialize(int agent,int playerid,int countryid,short type,const char *pCreed,int money,int gomoney)
{
	CountryID = countryid;
	Dept = type;
	if(pCreed)
	{
		strcpy(Creed,pCreed);
	}
	Money = money;
	GOMoney = gomoney;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CHARGE,CGC_C_NOTIFY_ELECTION);
}

typedef struct tagNotify_Election_Response:DISTRIBUTED_HEADER
{
	short Error_Code;			//0�ɹ���1��GO�㲻�㣻111��ʧ��
	int CountryID;				//��Ӫ
	short Dept;					//��ѡְλ��������������������ο�Dept_Type
	char Creed[100];
	int Money;
	inline void serialize(int agent,int playerid,int code,int countryid,short type,const char *pCreed,int money);
}Notify_Election_Response,*LPNotify_Election_Response;
inline void tagNotify_Election_Response::serialize(int agent,int playerid,int code,int countryid,short type,const char *pCreed,int money)
{
	Error_Code = static_cast<short> (code);
	CountryID = countryid;
	Dept =type;
	if(pCreed)
	{
		strcpy(Creed,pCreed);
	}
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CHARGE,CGC_S_NOTIFY_ELECTION);
}
//�һ�CGC_C_EXCHANGE,CGC_S_EXCHANGE,
typedef struct tagExchange_Request:DISTRIBUTED_HEADER
{
	int Type;					//����ExchangeType��Ŀǰֻ֧��Go��һ���ǮGO2Money
	int Count;					//���һ�������������С�ڵ���0
	inline void serialize(int playerid,int type,int count);
}Exchange_Request,*LPExchange_Request;
inline void tagExchange_Request::serialize(int playerid,int type,int count)
{
	Type = type;
	Count = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_EXCHANGE);
}

typedef struct tagExchange_Response:DISTRIBUTED_HEADER
{
	int Type;
	int Count;					//���ضһ��Ľ�Ǯ�������ͻ������Ӹ�ֵ����,С�ڵ���0ʱ�д���-1�����㣻-2��������
	inline void serialize(int agent,int playerid,int type,int count);
}Exchange_Response,*LPExchange_Response;
inline void tagExchange_Response::serialize(int agent,int playerid,int type,int count)
{
	Type = type;
	Count = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_EXCHANGE);
}
//��������ʣ���ʹ�ÿ�CGC_C_INCREASERECOURCERATE,CGC_S_INCREASERECOURCERATE,
typedef struct tagIncreaeRecourceRate_Request:DISTRIBUTED_HEADER
{
	int Type;					//���ͣ��ο�������
	int Rate;					//���ٰٷֱ�
	inline void serialize(int playerid,int type,int rate);
}IncreaeRecourceRate_Request,*LPIncreaeRecourceRate_Request;
inline void tagIncreaeRecourceRate_Request::serialize(int playerid,int type,int rate)
{
	Type = type;
	Rate = rate;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_INCREASERECOURCERATE);
}

typedef struct tagIncreaeRecourceRate_Response:DISTRIBUTED_HEADER
{
	int Count;					//����0ʱΪ���ٳɹ����ĵ�GO�㣬-1��GO�㲻�㣻-111��ʧ��
	int Type;					//���ͣ��ο�������
	int Rate;					//���ٰٷֱ�
	inline void serialize(int agent,int playerid,int count,int type,int rate);
}IncreaeRecourceRate_Response,*LPIncreaeRecourceRate_Response;
inline void tagIncreaeRecourceRate_Response::serialize(int agent,int playerid,int count,int type,int rate)
{
	Count = count;
	Type = type;
	Rate = rate;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_INCREASERECOURCERATE);
}

//ʹ�ûָ���CGC_C_UEEHERORESUME,CGC_S_USEHERORESUME,
typedef struct tagUseHeroResume_Request:tagBaseUseCard_Request
{
	int HeroID;			//����Ӣ��ID
	inline void serialize(int playerid,int officerid,int sortid,int heroid);
}UseHeroResume_Request,*LPUseHeroResume_Request;
inline void tagUseHeroResume_Request::serialize(int playerid,int officerid,int sortid,int heroid)
{
	HeroID = heroid;
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_UEEHERORESUME);
}

typedef struct tagUseHeroResume_Response:DISTRIBUTED_HEADER
{
	int Error_Code;			//���������0���ɹ���1:���û�иõ��ߣ�2���ǻָ�����3:Ӣ��δ���ˣ�111������
	int HeroID;				//Ӣ��ID
	inline void serialize(int agent,int playerid,int code,int heroid);
}UseHeroResume_Response,*LPUseHeroResume_Response;
inline void tagUseHeroResume_Response::serialize(int agent,int playerid,int code,int heroid)
{
	Error_Code = code;
	HeroID = heroid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_USEHERORESUME);
}

//֪ͨ���콨���������й���CGC_C_NOTITY_BUILDCONSTRUCTION,CGC_S_NOTITY_BUILDCONSTRUCTION
typedef struct tagNotify_BuildConstruction_Request:DISTRIBUTED_HEADER
{
	int PlayerID;
	AutoConstruction ConstructionInfo;
	inline void serialize(int playerid,LPAutoConstruction info);
}Nodify_BuildConstruction_Request,*LPNodify_BuildConstruction_Request;
inline void tagNotify_BuildConstruction_Request::serialize(int playerid,LPAutoConstruction info =0)
{
	PlayerID = playerid;
	if(info)
	{
		ConstructionInfo = *info;
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_C_NOTITY_BUILDCONSTRUCTION);
}

typedef struct tagNodify_BuildConstruction_Response:DISTRIBUTED_HEADER
{
	int PlayerID;
	int Error_Code;
	inline void serialize(int playerid,int code);
}Nodify_BuildConstruction_Response,*LPNodify_BuildConstruction_Response;
inline void tagNodify_BuildConstruction_Response::serialize(int playerid,int code)
{
	PlayerID = playerid;					//���ID
	Error_Code = code;						//���������0���ɹ���������ʧ��
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_S_NOTITY_BUILDCONSTRUCTION);
}

//����֪ͨCGC_C_NOTIFY_INCREASESPEED,CGC_S_NOTIFY_INCREASESPEED��
typedef struct tagNotify_IncreaseSpeed_Request:DISTRIBUTED_HEADER
{
	int			PlayerID;				//���ID
	CardType	Type;					//��������
	int			Value;					//���ٰٷֱȣ���50�������50%
	inline void serialize(int playerid,CardType type,int value);
}Notify_IncreaseSpeed_Request,*LPNotify_IncreaseSpeed_Request;
inline void tagNotify_IncreaseSpeed_Request::serialize(int playerid,CardType type,int value)
{
	PlayerID = playerid;
	Type = type;
	Value = value;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_C_NOTIFY_INCREASESPEED);
}

typedef struct tagNotify_IncreaseSpeed_Response:DISTRIBUTED_HEADER
{
	int			PlayerID;
	CardType	Type;
	int			Error_Code;
	inline void serialize(int playerid,CardType type,int code);
}Notify_IncreaseSpeed_Response,*LPNotify_IncreaseSpeed_Response;
inline void tagNotify_IncreaseSpeed_Response::serialize(int playerid,CardType type,int code)
{
	PlayerID = playerid;
	Type = type;
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_S_NOTIFY_INCREASESPEED);
}

typedef struct tagChargeSvr_TimerParam
{
	int TimerID;
	int PlayerID;
	CardType TimerType;
	int MapID;
	int CityID;
}ChargeSvr_TimerParam,*LPChargeSvr_TimerParam;
//ʱ��֪ͨ����ʱ����CGC_C_TIMERNOTIFY	
typedef struct tagNotify_Timer:DISTRIBUTED_HEADER
{
	ChargeSvr_TimerParam Param;
	inline void serialize(int playerid,CardType type,int mapid,int cityid,int timerid = -1);
}Notify_Timer,*LPNotify_Timer;	
inline void tagNotify_Timer::serialize(int playerid,CardType type,int mapid,int cityid,int timerid)
{
	Param.PlayerID = playerid;
	Param.TimerType = type;
	Param.TimerID = timerid;
	Param.MapID = mapid;
	Param.CityID = cityid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_C_TIMERNOTIFY);
}

//�ָ������ٶ�CGC_C_NOTIFY_NORMALSPEED,	CGC_S_NOTIFY_NORMALSPEED,
typedef struct tagNotify_NormalSpeed_Request:DISTRIBUTED_HEADER
{
	int PlayerID;
	CardType	Type;					//��������
	inline void serialize(int playerid,CardType type);
}Notify_NormalSpeed,*LPNotify_NormalSpeed;
inline void tagNotify_NormalSpeed_Request::serialize(int playerid,CardType type)
{
	PlayerID = playerid;
	Type = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_C_NOTIFY_NORMALSPEED);
}

typedef struct tagNotify_NormalSpeed_Response:DISTRIBUTED_HEADER
{
	int PlayerID;
	int Error_Code;
	inline void serialize(int playerid,int code);
}Notify_NormalSpeed_Response,*LPNotify_NormalSpeed_Response;
inline void tagNotify_NormalSpeed_Response::serialize(int playerid,int code)
{
	PlayerID = playerid;
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_S_NOTIFY_NORMALSPEED);
}

//һ���Լ��٣��������졢���С��������ٿ�����ļӢ��CGC_C_NOTIFY_INCREASEOTHERSPEED,		CGC_S_NOTIFY_INCREASEOTHERSPEED,
typedef struct tagNotify_IncreaseOtherSpeed_Request:DISTRIBUTED_HEADER
{
	int			PlayerID;				//���ID
	int			MapID;					//
	int			CityID;					//
	CardType	Type;					//��������
	int			Value;					//���ٰٷֱȣ���50�������50%
	int			SortID;					//��Ϊ���졢���С���������ʱ��Ϊ�����ٵ�����ID,����������Ϊ0����Ч��
	inline void serialize(int playerid,int mapid,int cityid,CardType type,int value,int sortid);
}Notify_IncreaseOtherSpeed_Request,*LPNotify_IncreaseOtherSpeed_Request;
inline void tagNotify_IncreaseOtherSpeed_Request::serialize(int playerid,int mapid,int cityid,CardType type,int value,int sortid)
{
	PlayerID = playerid;
	MapID = mapid;
	CityID = cityid;
	Type = type;
	Value = value;
	SortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_C_NOTIFY_INCREASEOTHERSPEED);
}

typedef struct tagNotify_IncreaseOtherSpeed_Response:DISTRIBUTED_HEADER
{
	int			PlayerID;
	CardType	Type;
	int			Error_Code;
	inline void serialize(int playerid,CardType type,int code);
}Notify_IncreaseOtherSpeed_Response,*LPNotify_IncreaseOtherSpeed_Response;
inline void tagNotify_IncreaseOtherSpeed_Response::serialize(int playerid,CardType type,int code)
{
	PlayerID = playerid;
	Type = type;
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_S_NOTIFY_INCREASEOTHERSPEED);
}

//�ɼ���Դ��֪ͨCGC_C_NOTIFY_COLLECTRESOURCE,	CGC_S_NOTIFY_COLLECTRESOURCE,	
typedef struct tagNotify_CollectResource_Request:DISTRIBUTED_HEADER
{
	int PlayerID;
	int MapID;
	int CityID;
	inline void serialize(int playerid,int mapid,int cityid);
}Notify_CollectResource_Request,*LPNotify_CollectResource_Request;
inline void tagNotify_CollectResource_Request::serialize(int playerid,int mapid,int cityid)
{
	PlayerID = playerid;
	MapID = mapid;
	CityID = cityid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_C_NOTIFY_COLLECTRESOURCE);
}

typedef struct tagNotify_CollectResource_Response:DISTRIBUTED_HEADER
{
	int Error_Code;						//0:�ɹ���������ʧ��
	int PlayerID;						//���ID��
	int MapID;							//����ͼID
	int CityID;							//����ID
	inline void serialize(int playerid,int mapid,int cityid,int code);
}Notify_CollectResource_Response,*LPNotify_CollectResource_Response;
inline void tagNotify_CollectResource_Response::serialize(int playerid,int mapid,int cityid,int code)
{
	PlayerID = playerid;
	MapID = mapid;
	CityID = cityid;
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_S_NOTIFY_COLLECTRESOURCE);
}

//�������֪ͨ���п���ͼ�������Զ��·�֪ͨCGC_S_NOTIFY_BUILDCOMPLETE	
typedef struct tagNotify_BuildComplete_Response:DISTRIBUTED_HEADER
{
	int PlayerID;
	int MapID;						//��ɽ���Ŀ�������ID
	int CityID;						//����ID
	int Error_Code;					//���������0������������1����Դ������������ʧ��
	inline void serialize(int playerid,int mapid,int cityid,int code);
}Notify_BuildComplete_Response,*LPNotify_BuildComplete_Response;
inline void tagNotify_BuildComplete_Response::serialize(int playerid,int mapid,int cityid,int code)
{
	PlayerID = playerid;
	MapID = mapid;
	CityID = cityid;
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_S_NOTIFY_BUILDCOMPLETE);
}
//����ϵͳ��ѯGO��CGC_C_NOTIFY_QUERYMONEY,CGC_S_NOTIFY_QUERYMONEY
typedef struct tagNotify_QueryMoney_Request:DISTRIBUTED_HEADER
{
	int PlayerID;
	int TaskID;//����ID
	inline void serialize(int agent,int playerid,int taskid,int from);
}Notify_QueryMoney_Request,*LPNotify_QueryMoney_Request;
inline void tagNotify_QueryMoney_Request::serialize(int agent,int playerid,int taskid,int from)
{
	PlayerID = playerid;
	TaskID = taskid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,MDM_GP_MAP_CHARGE,CGC_C_NOTIFY_QUERYMONEY);
}

typedef struct tagNotify_QueryMoney_Response:DISTRIBUTED_HEADER
{
	int PlayerID;
	int TaskID;//����ID
	int Money;//GO����
	inline void serialize(int agent,int playerid,int taskid,int money,int to);
}Notify_QueryMoney_Response,*LPNotify_QueryMoney_Response;
inline void tagNotify_QueryMoney_Response::serialize(int agent,int playerid,int taskid,int money,int to)
{
	PlayerID = playerid;
	TaskID = taskid;
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,to,CGC_S_NOTIFY_QUERYMONEY);
}

//�۳�/���GO��CGC_C_NOTIFY_ADDMONEY,CGC_S_NOTIFY_ADDMONEY
typedef struct tagNotify_AddMoney_Request:DISTRIBUTED_HEADER
{
	int PlayerID;		//���ID
	int Money;			//Ϊ��ʱ���GO�㣬Ϊ����ʱ����GO��
	inline void serialize(int agent,int playerid,int money,int from);
}Notify_AddMoney_Request,*LPNotify_AddMoney_Request;
inline void tagNotify_AddMoney_Request::serialize(int agent,int playerid,int money,int from)
{
	PlayerID = playerid;
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,MDM_GP_MAP_CHARGE,CGC_C_NOTIFY_ADDMONEY);
}

typedef struct tagNotify_AddMoney_Response:DISTRIBUTED_HEADER
{
	int PlayerID;
	int Error_Code;			//0�ɹ���1:GO�㲻�㣻111��ʧ��
	inline void serialize(int agent,int playerid,int code,int to);
}Notify_AddMoney_Response,*LPNotify_AddMoney_Response;
inline void tagNotify_AddMoney_Response::serialize(int agent,int playerid,int code,int to)
{
	PlayerID = playerid;
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,to,CGC_S_NOTIFY_ADDMONEY);
}

//ʹ�ü��ܿ�֪ͨ���ͼCGC_C_NOTIFY_ADDOFFICERSKILL,CGC_S_NOTIFY_ADDOFFICERSKILL
typedef struct tagNotify_AddOfficerSkill_Request:DISTRIBUTED_HEADER
{
	int PlayerID;				//���ID
	int PackID;					//����ID
	int OfficerID;				//ѧϰ���ܵ�Ӣ��ID
	int SortID;
	int SkillID;				//����ID
	inline void serialize(int agent,int playerid,int packid,int officerid,int sortid,int skillid);
}Notify_AddOfficerSkill_Request,*LPNotify_AddOfficerSkill_Request;
inline void tagNotify_AddOfficerSkill_Request::serialize(int agent,int playerid,int packid,int officerid,int sortid,int skillid)
{
	PlayerID = playerid;
	PackID = packid;
	OfficerID = officerid;
	SortID = sortid;
	SkillID = skillid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,SERVER_BOUND_MAP_OTHER,CGC_C_NOTIFY_ADDOFFICERSKILL);
}
typedef struct tagNotify_AddOfficerSkill_Response:DISTRIBUTED_HEADER
{
	short Error_Code;			//���������0���ɹ���1������
	int PlayerID;				//���ID
	int PackID;					//����ID
	int OfficerID;				//ѧϰ���ܵ�Ӣ��ID
	int SortID;
	inline void serialize(int agent,int playerid,int packid,int officerid,int sortid,int code);
}Notify_AddOfficerSkill_Response,*LPNotify_AddOfficerSkill_Response;
inline void tagNotify_AddOfficerSkill_Response::serialize(int agent,int playerid,int packid,int officerid,int sortid,int code)
{
	Error_Code = code;
	PlayerID = playerid;
	PackID = packid;
	OfficerID = officerid;
	SortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,MDM_GP_MAP_CHARGE,CGC_S_NOTIFY_ADDOFFICERSKILL);
}
//ʹ��һ�ֿ��������������͹�����֪ͨCGC_C_NOTIFY_USEONECARD,CGC_S_NOTIFY_USEONECARD,
typedef struct tagNotify_UseOneCard_Request:DISTRIBUTED_HEADER
{
	int playerID;						//���ID
	int cardID;							//����ID
	inline void serialize(int agent,int playerid,int cardid,int svrid);
}Notify_UseOneCard_Request,*LPNotify_UseOneCard_Request;
inline void tagNotify_UseOneCard_Request::serialize(int agent,int playerid,int cardid,int svrid)
{
	playerID = playerid;
	cardID = cardid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,svrid,MDM_GP_MAP_CHARGE,CGC_C_NOTIFY_USEONECARD);
}
typedef struct tagNotify_UseOneCard_Response:DISTRIBUTED_HEADER
{
	int playerID;
	int cardID;
	int error_Code;					//0:�ɹ�
	inline void serialize(int agent,int playerid,int cardid,int code,int svrid);
}Notify_UseOneCard_Response,*LPNotify_UseOneCard_Response;
inline void tagNotify_UseOneCard_Response::serialize(int agent,int playerid,int cardid,int code,int svrid)
{
	playerID = playerid;
	cardID = cardid;
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,svrid,CGC_S_NOTIFY_USEONECARD);
}

//��֪ͨ���շѷ�����֪ͨ���ͼCGC_C_NOTIFY_BUYCARD,CGC_S_NOTIFY_BUYCARD
typedef struct tagNotify_BuyCard_Request:DISTRIBUTED_HEADER
{
	int playerID;			//���ID
	int cardID;				//��ID
	int postion;			//
	inline void serialize(int agent,int playerid,int cardid,int pos);
}Notify_BuyCard_Request,*LPNotify_BuyCard_Request;
inline void tagNotify_BuyCard_Request::serialize(int agent,int playerid,int cardid,int pos)
{
	playerID = playerid;
	cardID = cardid;
	postion = pos;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,SERVER_BOUND_MAP_OTHER,CGC_C_NOTIFY_BUYCARD);
}

typedef struct tagNotify_BuyCard_Response:DISTRIBUTED_HEADER
{
	int playerID;
	int cardID;
	short error_Code;
	inline void serialize(int agent,int playerid,int cardid,short code);
}Notify_BuyCard_Response,*LPNotify_BuyCard_Response;
inline void tagNotify_BuyCard_Response::serialize(int agent,int playerid,int cardid,short code)
{
	playerID = playerid;
	cardID = cardid;
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,MDM_GP_MAP_CHARGE,CGC_S_NOTIFY_BUYCARD);
}

#pragma pack(pop)

#endif
