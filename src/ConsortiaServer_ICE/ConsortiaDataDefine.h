#ifndef CONSORTIADATADEFINE_H__
#define CONSORTIADATADEFINE_H__
#include "IceUtil.h"
#include "Global_Macro.h"
#include "PlayerAcc.h"
#include "Consortia.h"
#include "ConsortiaOfficial.h"
#include "ConsortiaProtocol.h"
#include "PlayerRanking.h"
#include "CommonFunction.h"
#include "ConsortiaRanking.h"
#include "ZSBB_DataDefine.h"

#ifndef	MAX_LOGO_SIZE
#	define MAX_LOGO_SIZE			4096	//�Զ���Logo���������
#endif

#define CFGFILE		"./Consortia.ini"
#define MAX_CONSORTIA_COUNT		500

//==================================================================
typedef enum {
	JoinApply_Email,		//���������ʼ�����
	AgreeJoin_Email,		//ͬ�����
	RefuseJoin_Email,		//�ܾ�����
	KickUser_Email,			//Kick��Ա
	InviteEmail,			//�������
	AgreeInvite_Email,		//ͬ������
	RefuseInvite_Email,		//�ܾ�����
	PoseOfficial_Email,		//��ְ֪ͨ
	DeposeOfficial_Email,	//��ְ֪ͨ
	TranChairman_Email,		//�ƽ��᳤
	AllyApply_Email,		//��������
	AgreeAlly_Email,		//ͬ�����
	RefuseAlly_Email,		//�ܾ�����
	Unally_Email			//����
}Email_Param;

typedef enum{
	Default_Notice,			//Ĭ��
	CreateCons_Notice,		//��������
	DelConsortia_Notice,	//��ɢ����
	JoinConsortia_Notice,	//���빫��
	KickUser_Notice,		//Kick��Ա
	QuitConsortia_Notice,	//�˳�����
	PoseOfficail_Notice,	//��ְ
	DeposeOfficail_Notice,	//��ְ
	TranChairman_Notice,	//�ƽ��᳤
}Notice_Param;
//���Ӳ���
typedef struct tagAgent_DB_Param
{
	char			AgentIP[SIZE_32];
	unsigned int	AgentPort;
	char			DBGIP[SIZE_32];
	unsigned int	DBGPort;
	char			LogSvrIP[SIZE_32];
	unsigned int	LogSvrPort;
}Agent_DB_Param,*LPAgent_DB_Param;

typedef struct tagEmailParam
{
	char Title[SIZE_16];
	char Body[SIZE_128];
}EmailParam,*LPEmailParam;

//��Ϸ�е��������
typedef IceUtil::Handle<CPlayerAcc> PlayerAcc_Ptr;
typedef std::map<int,PlayerAcc_Ptr> PlayerAcc_Map;
typedef std::map<int,PlayerAcc_Ptr>::iterator PlayerAcc_Iterator;

typedef std::map<std::string,int> PlayerAcc_Name_Map;
typedef std::map<std::string,int>::iterator PlayerAcc_Name_Iterator;

/*����
*/
typedef IceUtil::Handle<CConsortia> Consortia_Ptr;
typedef std::map<int,Consortia_Ptr> Consortia_Map;
typedef struct tagConsortiaNode
{
	RWMutex rwmutex_;		//������
	Consortia_Ptr con_ptr;	//����
}ConsortiaNode,*LPConsortiaNode;

//ְλ��Ϣ��
typedef IceUtil::Handle<CConsortiaOfficial> ConsortiaOfficial_Ptr;
typedef std::map<int,ConsortiaOfficial_Ptr> ConsortiaOfficial_Map;
typedef std::map<int,ConsortiaOfficial_Ptr>::iterator ConsortiaOfficial_Iteraotr;


//���������������
typedef struct tagCreateConsortiaLimit
{
	int			ResID;					//ID
	int			FoodCount;				//��ʳ����
	int			WoodCount;				//ľ������
	int			IronCount;				//������
	int			SkinCount;				//Ƥ����
	int			RedStoneCount;			//�챦ʯ
	int			YollowStoneCount;		//�Ʊ�ʯ
	int			GreenStoneCount;		//�̱�ʯ
	int			BackIronCount;			//����
	int			Times;					//�����೤ʱ�䣨Сʱ����Աδ��Personnel��ʱ����ɢ����
	int			Personnel;				//��Ա��������24Сʱ�󣬳�Ա������Ҫ��
	int			Science;				//�Ƽ���
	int			Money;					//��Ǯ
	int			Level;					//�ȼ�Ҫ��
	int			JoinLevel;				//���빫��ĵȼ�Ҫ��
}CreateConsortiaLimit,*LPCreateConsortiaLimit;

//���빫�������
//typedef IceUtil::Handle


//����
typedef	IceUtil::Handle<CPlayerRanking> PlayerRanking_Ptr;

typedef	IceUtil::Handle<CConsortiaRanking> ConsortiaRanking_Ptr;

typedef struct tagTirrierParam
{
	int TirrierType;//0:սʿ��1�����֣�2����ʦ
	int SoldierType;	//ʿ������
	int WeaponType;		//��������
	int WeaponLevel;	//��������
	int DefendType;		//��������
	int DefendLevel;	//���߼���
	int Money;			//����
}TirrierParam,*LPTirrierParam;

typedef std::map<int,LPTirrierParam> TirrierParam_Map;
typedef TirrierParam_Map::iterator TirrierParam_Map_Iterator;

#endif
