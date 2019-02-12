#ifndef __SERVER_PROTOCOL__
#define __SERVER_PROTOCOL__

#include "Global_Macro.h"
#include "DBTableHeader.h"
#include "ProChatSystem.h"
#pragma warning(disable:4100)

#pragma pack(push,1)

//GM����
//�޸����������¼�(-�������������)
struct MODIFY_NOTICEFORCES_EVENT:DISTRIBUTED_HEADER
{
	int NoticeID;	//�����NoticeType
	int ForcesID;  //����ChannelType
	int DataLen;
	char cData[GM_DATA_SIZE+1];
	inline void serialize(int nid,int fid,const char * data);
};
inline 
void MODIFY_NOTICEFORCES_EVENT::serialize(int nid,int fid,const char * data)
{
	this->ForcesID = fid;
	this->NoticeID = nid;
	if (data)
	{
		DataLen = (int)strlen(data);
		if (DataLen>GM_DATA_SIZE) DataLen=GM_DATA_SIZE;
		strncpy(cData,data,DataLen);
	}
	else
	{
		DataLen = 0;
	}
	cData[DataLen]='\0';
	DISTRIBUTED_HEADER::serialize(sizeof(*this)-(GM_DATA_SIZE+1)+DataLen,0,MDM_GP_GAME_MANAGE_SERVER,MDM_GP_MAP_CHAT,GC_NOTICEFORCES_EVENT);
}
/*
//GM˽���¼���-���������)��������
struct GMCHAT_EVENT : DISTRIBUTED_HEADER
{
	int fromGmID;			//������
	int toGmID;				//��������
	inline void serialize(int fid,int tid);
};
inline 
void GMCHAT_EVENT::serialize(int fid,int tid)   
{
	this->fromGmID = fid;
	this->toGmID	= tid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,MDM_GP_GAME_MANAGE_SERVER,MDM_GP_MAP_CHAT,GC_CHAT_EVENT);
}
*/

/*   begin ����ϵͳ�¼�  */
/* ��ҵȼ������α���¼� */

struct USER_PLAYERLEVEL_PLAYERRANK_EVENT:DISTRIBUTED_HEADER
{
	unsigned long	PlayerID;
	int				PalyerLevel;
	int				PlayerRank;
    inline void serialize(int,unsigned long, int, int);
};
inline void
USER_PLAYERLEVEL_PLAYERRANK_EVENT::serialize(int agentid, unsigned long playerid, int playerlevel, int playerrank)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agentid,0,MDM_GP_MAP_TASK,TC_USER_PLAYERLEVEL_PLAYERRANK_EVENT);
	this->PlayerID		=	playerid;
	this->PalyerLevel	=	playerlevel;
	this->PlayerRank	=	playerrank;
}


/* ���������ߴ����¼� */
struct USER_TASKITEM_EVENT:DISTRIBUTED_HEADER
{
	unsigned long	PlayerID;
	int				PlayerLevel;
	int				PlayerRank;
	unsigned int	TaksItem;
	inline void serialize(int,unsigned long, int, int, unsigned int);
};
inline void
USER_TASKITEM_EVENT::serialize(int agentid, unsigned long playerid, int playerlevel, int playerrank, unsigned int taskitem)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agentid,0,MDM_GP_MAP_TASK,TC_USER_TASKITEM_EVENT);
	this->PlayerID		=	playerid;
	this->PlayerLevel	=	playerlevel;
	this->PlayerRank	=	playerrank;
	this->TaksItem		=	taskitem;
}


/* �����������¼� */
struct USER_TASK_COMPLETE_EVENT:DISTRIBUTED_HEADER
{
	unsigned long	PlayerID;
	unsigned int	TaskID;
	inline void serialize(int,unsigned long, unsigned int); 
};
inline void
USER_TASK_COMPLETE_EVENT::serialize(int agendtid, unsigned long playerid, unsigned int taskid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agendtid,0,MDM_GP_MAP_TASK,TC_USER_TASK_COMPLETE_EVENT);
	this->PlayerID		=	playerid;
	this->TaskID		=	taskid;
}

/* ������������¼�(���ID������ID��ɱ��BOSS��ʶ��ɱ���������ٽ�����) */
struct ECTYPE_EVENT :DISTRIBUTED_HEADER
{
	unsigned long	PlayerID;
	unsigned int	EctypeID;
	unsigned char	Boss;
	unsigned int	NpcNums;
	unsigned int	BuildNums;
	inline void	serialize(int, unsigned long, unsigned int, unsigned char, unsigned int, unsigned int);
};
inline void 
ECTYPE_EVENT::serialize(int agentid, unsigned long playerid, unsigned int ectypeid, unsigned char boss, unsigned int npcnums, unsigned int buildnums)
{
	this->PlayerID	=	playerid;
	this->EctypeID	=	ectypeid;
	this->Boss		=	boss;
	this->NpcNums	=	npcnums;
	this->BuildNums	=	buildnums;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agentid,0,MDM_GP_MAP_TASK,TC_ECTYPE_EVENT);
}
/*    end ����ϵͳ�¼� */


////evet struct
struct startFight
{
	int cmd;
	unsigned long MapID;
};
struct ArmyArrive: DISTRIBUTED_HEADER
{
	int cmd ;
	unsigned long uArmyID;
	unsigned long uFromMapID;
	unsigned long uToMapID;
	unsigned long uMidMapID; //����Ŀ�꿤��ǰһ�����ı��
	inline void serialize(int length, int agent, int from, int to, int command = CMD_ARMY_ARRIVED);
};

inline void ArmyArrive::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,4000,from,to,cmd);
}

struct EndFight:DISTRIBUTED_HEADER
{
	int cmd;
	unsigned long uMapID;
	int winer; // 1 = ���� 2 ,����

	inline void serialize(int length, int agent, int from, int to, int command = CMD_END_FIGHT);
};

inline void EndFight::serialize(int length, int agent, int from, int to, int command )
{
	DISTRIBUTED_HEADER::serialize(length,4000,from,to,command);
}

struct tagArmy_Reach : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lPlayerID,unsigned long lFromMapID,unsigned long uMidMapID, unsigned long lToMapID,unsigned long lArmyID, int sortid,int command/* = CMD_ARMY_REACH*/);
	unsigned long							lPlayerID;					//������ҵ�ID
	unsigned long							lFromMapID;
	unsigned long							lToMapID;					//����Ŀ�꿤ID
	unsigned long							uMidMapID;					//����Ŀ�꿤��ǰһ�����ı��
	unsigned long							lArmyID;
	int										sortID;						//�Ƿ�ʹ�ü��ٿ���0��ʹ�ã���0ʹ��
};

struct tagBackArmy_Reach:DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	unsigned long ulFromMapID;
	unsigned long lArmyID;
	int				ret ; // ret = 1 �ɹ� 0 �� ʧ��
	inline void serialize(unsigned long playerid,unsigned long frommapid,unsigned long armyid,int ret)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,0,0,CMD_BACK_ARMY_REACH);
		this->lArmyID =  armyid;
		this->ulFromMapID = frommapid;
		this->ulPlayerID = playerid;
		this->ret = ret;
	}
};
inline void
tagArmy_Reach::serialize(unsigned long lPlayerID,unsigned long lFromMapID,unsigned long uMidMapID,unsigned long lToMapID,unsigned long lArmyID, int sortid,int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);//CMD_ARMY_REACH / CMD_ARMY_ARRIVED
	//this->cmd = cmd;
	this->lPlayerID = lPlayerID;
	this->lFromMapID = lFromMapID;
	this->uMidMapID = uMidMapID;
	this->lToMapID = lToMapID;
	this->lArmyID = lArmyID;
	this->sortID = sortid;
}
struct tagHeroParam : DISTRIBUTED_HEADER
{
	inline void serialize(long long lStartFight, unsigned long lPlayerID,unsigned long lHeroID,unsigned long lMapID,char szArmyName[],int iExp,int iCredit,
		int iEnemyCount,int iGeneralCount,int iState);
	long long			lStartFight;		//��ʼս��ʱ��
	unsigned long		lPlayerID;			//���ID
	unsigned long		lHeroID;			//Ӣ��ID
	unsigned long		lMapID;				//��ID
	char				szArmyName[SIZE_32];//��������
	int					iExp;				//����ֵ
	int					iCredit;			//������
	int					iEnemyCount;		//ɱ����
	int					iGeneralCount;		//��²����
	int					iState;				//״̬ //0,���� 1,���� 2,���� 3,���� 4 ��² ������ HERO_STATE
};

struct tagCaptiveHero : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lPlayerID,unsigned long lHeroID,unsigned long lByPlayerID,unsigned long lByHeroID);
	unsigned long		lPlayerID;			//���ID
	unsigned long		lHeroID;			//Ӣ��ID
	unsigned long		lByPlayerID;		//����Ӣ�� ���ID
	unsigned long		lByHeroID;			//����Ӣ��ID
};
inline void
tagCaptiveHero::serialize(unsigned long lPlayerID,unsigned long lHeroID,unsigned long lByPlayerID,unsigned long lByHeroID)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, 0);
	this->lPlayerID = lPlayerID;
	this->lHeroID = lHeroID;
	this->lByPlayerID = lByPlayerID;
	this->lByHeroID = lByHeroID;
}
//��ҵ���Դ��
struct tagPlayerResource : DISTRIBUTED_HEADER
{
	inline void serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,int iRedStoneCount,int iYellowStoneCount,
		int iGreenStoneCount,int iBackIronCount,int iMoney,int iScience,int iContribute);
	int								iFoodCount;				//��ʳ����
	int								iWoodCount;				//ľ������
	int								iIronCount;				//������
	int								iSkinCount;				//Ƥ����
	int								iRedStoneCount;			//�챦ʯ
	int								iYellowStoneCount;		//�Ʊ�ʯ
	int								iGreenStoneCount;		//�̱�ʯ
	int								iBackIronCount;			//����
	int								iMoney;					//Ǯ
	int								iScience;				//�Ƽ���
	int								iContribute;			//���׶�
};
inline void
tagHeroParam::serialize(long long lStartFight, unsigned long lPlayerID,unsigned long lHeroID,unsigned long lMapID,char szArmyName[],int iExp,int iCredit,
							int iEnemyCount,int iGeneralCount,int iState)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, CMD_HERO_PARAM);
	this->lStartFight = lStartFight;
	this->lPlayerID = lPlayerID;
	this->lHeroID = lHeroID;
	this->lMapID = lMapID;
	this->iExp = iExp;
	this->iCredit = iCredit;
	this->iEnemyCount = iEnemyCount;
	this->iGeneralCount = iGeneralCount;
	this->iState = iState;
	strcpy(this->szArmyName, szArmyName);
}
inline void
tagPlayerResource::serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,int iRedStoneCount,int iYellowStoneCount,
							int iGreenStoneCount,int iBackIronCount,int iMoney,int iScience,int iContribute)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, CMD_PLAYER_UPDATA_RESOURCE);
	this->iFoodCount = iFoodCount;
	this->iWoodCount = iWoodCount;
	this->iIronCount = iIronCount;
	this->iSkinCount = iSkinCount;
	this->iRedStoneCount = iRedStoneCount;
	this->iYellowStoneCount = iYellowStoneCount;
	this->iGreenStoneCount = iGreenStoneCount;
	this->iBackIronCount = iBackIronCount;
	this->iMoney = iMoney;
	this->iScience = iScience;
	this->iContribute =iContribute ;
}

struct ArmyId2MapId
{
	unsigned long armyId;
	unsigned long mapId;
};

// Ұ�⸱�������¼�
struct EventEctypeOver :DISTRIBUTED_HEADER{
	unsigned int entraceID; 
	inline void serialize(int from, int to)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,from,to, EVENT_ECTYPE_OVER);
	}
};
struct Notify_Enter_Ectype :DISTRIBUTED_HEADER
{
	//int				cmd;
	unsigned int	uEctypeID;
	unsigned int    entranceID;
	unsigned int    uEctypeRoom[2];
	unsigned int    playerCount;
	unsigned int	iArmyCount;

	inline unsigned long& getPlayerId(int iIndex)
	{
		return ((unsigned long*)((unsigned char *)this + sizeof(Notify_Enter_Ectype)))[iIndex];
	}

	inline ArmyId2MapId& getArmyInfo(int iIndex)
	{
		return ((ArmyId2MapId*)((unsigned char *)this + sizeof(Notify_Enter_Ectype) + sizeof(unsigned long) * playerCount))[iIndex];
	}

	inline void serialize(int length, int agent, int from, int to, int command = CMD_ENTER_ECTYPE)
	{
		DISTRIBUTED_HEADER::serialize(length,4000,from,to,command);
	}
};

struct Notify_Back_Enter_Ectype :DISTRIBUTED_HEADER // ���ص�֪ͨ
{
	//int cmd ;
	int ret ;//1 ��ʾ�ɹ�,0��ʾ�������� �ȴ�
	int RoomID;
	int uEctypeRoomID[2];
	int uEctypID;
	int mapid;
	unsigned int entranceID;


	inline void serialize(int length, int agent, int from, int to, int command = CMD_BACK_ENTER_ECTYPE);
};

inline void Notify_Back_Enter_Ectype::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,4000,from,to,command);
}

struct Notify_Back_Army_Ectype:DISTRIBUTED_HEADER
{
	int cmd ;
	int uEctypeID;
	int iArmyCount;
	int entranceID; //���ID
	unsigned long uArmyID[20*5];
	inline void serialize(int length, int agent, int from, int to, int command = CMD_BACK_ARMY_ECTYPE);
};
inline void Notify_Back_Army_Ectype::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,4000,from,to,command);
}

struct Notify_Add_Main_Hero:DISTRIBUTED_HEADER  // t֪ͨ����һ����Ӣ���¼�
{
	int		iOfficerID;
	char	szOfficerName[SIZE_24];
	inline void serialize(int cmd = CMD_ADD_MAIN_HERO);
};
inline void Notify_Add_Main_Hero::serialize(int cmd /* = CMD_ADD_MAIN_HERO */)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,0,0,cmd);
}

struct Notify_Send_Mail :DISTRIBUTED_HEADER // ֪ͨ�ʼ�ϵͳ���͵��ʼ�
{
	MAIL_TYPE		type;					//�ʼ�����
	unsigned long	ulRecieverPlayerID;		//�ռ��˱��
	char			szSendPlayerName[SIZE_16];//����������
	char			sztitle[SIZE_16];		//�ʼ�����
	char			szbody[SIZE_256];		//�ʼ�����
	char			szPertain[SIZE_64];    //����

	inline void serialize(int cmd = CMD_ADD_MAIl_NOTIFY)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,0,0,cmd);
	}
};

struct Notify_BroadMail:DISTRIBUTED_HEADER//֪ͨ����ӪȺ���ʼ�
{
	int				CountryID;			//��Ӫ��0:ȫ��;1�����ˣ�2������;
	char			szSendPlayerName[SIZE_16];//����������
	char			sztitle[SIZE_16];		//�ʼ�����
	char			szbody[SIZE_256];		//�ʼ�����
	inline void serialize()
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,0,0,CMD_BROADMAIL_NOTIFY);
	}
};
struct Notify_Add_Consortia :DISTRIBUTED_HEADER //֪ͨ���˼��繫��
{
	unsigned long		ulPlayerID;
	unsigned long		ulConsortiaID;
	inline void serialize(int cmd = CMD_ADD_CONSORTIA_NOTIFY)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,0,0,cmd);
	}
};

struct NewsFightReprotEvent_1
{
	int64 time;			// ʱ��
	unsigned int	  MapID;		// ����ͼ
	unsigned int	  type;			//��ռ������Ӫ
};


struct NewsFightReprotEvent_2
{
	int64 time;			//ʱ��
	unsigned int MapID;			//�����
};

struct HeroNewsReportEvent_1
{
	int64 time;
	char  MailHero1[SIZE_16];		//������Ӣ�۵���������
	char  Hero[SIZE_16];			//������Ӣ�۵�����
	char  MailHero2[SIZE_16];		//���񷽵�����Ӣ������
};

struct HeroNewsReportEvnet_2
{
	int64 time;
	char  MailHero[SIZE_16]; //����Ӣ�۵�����
	char  Hero[SIZE_16];		//����Ӣ������
};

typedef HeroNewsReportEvent_1 HeroNewsReportEvent_3;
typedef HeroNewsReportEvent_1 HeroNewsReportEvent_4;

struct Map_NewsReportEvent3
{
	unsigned int   AttackRole;  // 1�������������� 2 �� �������ǲ���
	int64 StartTime;  // ս����ʼʱ��
	unsigned int mapid;//�ص�
	int64 EndTime;//����ʱ�䣬û�н���Ϊ0
	short warstate;// 1 = ��ȻԡѪ��ս 2 =�ɹ�ռ��õ��� 3= ��ʱս�Գ���
	bool  bJoin;   //1= ����  0 �� û��
	unsigned int   ADealSolderNum;//����������
	unsigned int   RDealSolderNum;//���ط�����
	unsigned int   DestorySNum;//�ݻ���������
	unsigned int   DestoryCNum;//�ݻٳ�������
	unsigned int   ACatchOfficerNum; //������������������
	unsigned int   RCatchOfficerNum; //���ط�������������
	unsigned int   ADealOfficerNum;  //������ս����������
	unsigned int   RDealOfficerNum;  //����ս����������
};

struct HeroNewsReportEvent_5  //��ս���з�����ɱ
{
	int64 time;       //ʱ��
	int   MapID;      //�ص�
	char  szAPlayerName[SIZE_16]; // ���a
	char  szOfficerName[SIZE_16]; // ��������
	char  szBPlayerName[SIZE_16]; // ���B
	int   ret ;						// 1= ���� 2 �� ��ɱ
};

struct HeroNewsReportEvent_6 //�ͷŻ��ɱ����
{
	char   szAPlayerName[SIZE_16]; // ���A
	bool   bRedeem;				   //�Ƿ����
	char   szOfficerName[SIZE_16]; //��������
	char   szBPlayerName[SIZE_16]; // ���B
	int    ret ;					// 1��  �ͷ�  0 �� ��ɱ
};
struct Notify_Add_News_Report:DISTRIBUTED_HEADER
{
	NEWS_TYPE		type; // ս������
	char			szConent[SIZE_128];
	inline void serialize( int agent, int command = CMD_ADD_NEWS_REPORT_NOTIFY)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,0,0,command);
	}
};

struct HeroStudySkillEvent:DISTRIBUTED_HEADER
{
	unsigned long ulArmyID; // ���ӱ��
	unsigned long ulofficerID;//ָ�ӹٱ��
	unsigned int ulskillID; //���ܱ��
	int				iLevel;
	inline void serialize(unsigned long ulArmyID,unsigned long officerid,unsigned int skill,int level,int cmd= CMD_HERO_STUDY_SKILL)
	{
		this->ulArmyID = ulArmyID;
		this->ulofficerID = officerid;
		this->ulskillID = skill;
		this->iLevel = level;
		DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,0,0,cmd);
	}
};
struct heroModifyParamEvent:DISTRIBUTED_HEADER
{
	unsigned long PlayerID;
	unsigned long OfficerID;
	inline void serialize(int playerid,int officerid)
	{
		PlayerID = playerid;
		OfficerID = officerid;
		DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,SERVER_BOUND_MAP_OTHER,0,CMD_UPDATE_HERO_PARAM);
	}
};


struct MsgEctypeDelPlayer :DISTRIBUTED_HEADER{
	int ectypeID; // ����ID
	int playerNum; //�������
	inline void serialize(int playerNum){
		this->playerNum = playerNum;
		DISTRIBUTED_HEADER::serialize(sizeof(MsgEctypeDelPlayer) + sizeof(unsigned int) * playerNum ,4000,0,0,CMD_DEL_ECTYPE_PLAYER);
	}
	inline unsigned int & operator[] (int index){
		return ((unsigned int*)((unsigned char *)this + sizeof(MsgEctypeDelPlayer)))[index];
	}
};
struct MsgEctypeFightOver :DISTRIBUTED_HEADER{
	int roomID; //����ID
	inline void serialize(int roomID){
		this->roomID = roomID;
		DISTRIBUTED_HEADER::serialize(sizeof(MsgEctypeFightOver) ,4000,0,0,CMD_ECTYPE_FIGHT_OVER);
	}
};
#pragma pack(pop)

#endif