#ifndef PRO_NPC_LOGIC_
#define PRO_NPC_LOGIC_
#include "Distributed_Header.h"
#include "ProFightSystem.h"
#define   MAX_WAIT_ROOM_NUM  1000
#define   SEND_MAX_ROOM_INFO 200
#define   MAX_ECTYPE_ROOM_MEM 5
#pragma pack(push ,1)
/////////////////////////////Npc////////////////////////////////////////
struct ReqCreateEctypeRoom :DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID; //��ұ��
	unsigned int  DoorID; //�������	
	char		  szEctypeName[SIZE_16];
	char		  szPassWord[SIZE_16];
	char		  szNotify[SIZE_24*2];
	//inline void serialize(int mapid,int player,int command);
	inline void serialize(int mapid,int player,int command = REQ_CREATE_ROOM_INFO );//20091221
};

struct RspCreateEctypeRoom :DISTRIBUTED_HEADER
{
	unsigned long playerID;
	unsigned int  iEctypeRoomID; //���ظ��������ķ�����
	unsigned char cPasswordflag;// �����־λ  1�� ������ 0 �� û������
	char		  cEctypeRoomName[SIZE_16]; //�������ƣ������ߵ�����
	char          czCreateName[SIZE_16];
	inline void serialize(int length, int agent, int from, int to, int command);
};

struct ReqAddToEctypeRoom : DISTRIBUTED_HEADER //���뷿��
{
	//unsigned long ulPlayerID;
	//unsigned int  DoorID;//uEctypeID;
	//unsigned int  ulEctypeRoomID; //����ĸ��������ķ�����
	//char		  szPassWord[SIZE_16];
	//inline void  serialize(int mapid ,int player,int command);
	unsigned long ulPlayerID;
	unsigned int  uEctypeID;
	unsigned int  ulEctypeRoomID; //����ĸ��������Ķ�����
	char		  szPassWord[SIZE_16];
	inline void  serialize(int mapid ,int player,int command = REQ_ADD_ECTYPE_ROOM_INFO);//20091221
};

struct RspAddToEctypeRoom :DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	unsigned char flag ; //1=  �ɹ���֪ͨ�÷���������ˣ�ʧ��ֻ���ظ�������� 0 =ʧ�� 2 ��ʾ ������� 3���˷����ѽ���ս���ȴ�״̬ 4.���ڸ����� 5: //��Ա�Ѵ�����
	unsigned int uEctypeRoomID;
	char Name[SIZE_24];
	unsigned int faceID;
	int			 iLevel;   //�ȼ�
	int			 iMilitaryRank; //����
	inline void serialize(int length, int agent, int from, int to, int command);
};
struct ReqKickOutEctypeRoom:DISTRIBUTED_HEADER // �߳���������
{
	unsigned long	ulPlayerID;
	unsigned long	ulOtherID;
	unsigned int	uEctypeID;
	unsigned int	uEcytypeRoomID;
	//inline void serialize( int from, int to, int command);
	inline void serialize( int from, int to, int command = REQ_KICK_OUT_OTHER_INFO);//20091221
};

struct RspKickOutEctypeRoom:DISTRIBUTED_HEADER
{
	bool			flag ;
	unsigned long	ulOtherID;
	unsigned long	uEctypeID;
	unsigned int	uEctypeRoomID;
	inline void serialize(int length, int agent, int from, int to, int command);
};


struct ReqHallInfo :DISTRIBUTED_HEADER //��ȡ������Ϣ
{
	//unsigned long plyaerID;  //��ұ��
	//unsigned long DoorID;    //�������
	//inline void serialize(int from, int to, int command);
	unsigned long plyaerID;  //��ұ��
	unsigned long EctypeID;  //����������ID
	inline void serialize(int from, int to, int command= REQ_ENTER_HALL_INFO);//20091221
};

struct ReqHallInfoFromArmyList :DISTRIBUTED_HEADER // ˫�������б�ʱ�����͵Ļ�ȡ������Ϣ
{
	unsigned long armyId;    // ����ID
	unsigned long plyaerID;  // ��ұ��

	inline void serialize(int from, int to, int command = REQ_ENTER_HALL_INFO_FROM_ARMY_LIST)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from ,to ,command);
	}
};

struct HallInfo
{
	unsigned int uEctypeRoomID; // ��������	
	int			 count; //��������
	unsigned char cPasswordflag;// �����־λ  1�� ������ 0 �� û������
	char		 cName[SIZE_24];//��������
	char		 czCreateName[SIZE_16];
};
struct RspHallInfo :DISTRIBUTED_HEADER
{
	bool flag;    //�Ƿ�ɹ�
	int iRoomCount;  //���͵ķ�������Ϣ
	HallInfo Info[SEND_MAX_ROOM_INFO]; //������Ϣ
	inline void serialize(int length, int agent, int from, int to, int command);
};

struct ReqEctypeRoomInfo:DISTRIBUTED_HEADER //���븱����ȡ����������Ϣ
{
	//unsigned int DoorID;//EctypeID;     
	//unsigned int uEctypeRoomID;
	//inline void serialize(int from, int to, int command);
	unsigned int uEctypeID;     
	unsigned int uEctypeRoomID;
	inline void serialize(int from, int to, int command = REQ_GET_ONE_ROOM_INFO);//20091221
};

struct RspEctypeRoomInfo:DISTRIBUTED_HEADER
{
	char name[5][SIZE_24];
	char RoomNote[SIZE_32];
	int count ; //�������
	inline void serialize(int agent, int from, int to, int command);
};

struct ReqEctypeRoomNote :DISTRIBUTED_HEADER // ���ͷ��乫����Ϣ
{
	unsigned long ulplayerID;
	unsigned int  uEctypID;
	unsigned int  uEctypeRoomID;
	char		  RoomNote[SIZE_32];
	//inline void serialize( int from, int to, int command);
	inline void serialize( int from, int to, int command =REQ_SEND_NOTICE_INFO);//20091221
};

struct RspEctypeRoomNote :DISTRIBUTED_HEADER
{
	unsigned int  uEctypeID;
	unsigned int  uEctypeRoomID;
	char		  RoomNote[SIZE_32];
	inline void serialize(int length ,int agent, int from, int to, int command);
};

struct ReqAddArmyToEctype :DISTRIBUTED_HEADER // ��Ӷ���
{
	bool isSigleEctype; // �Ƿ��ǵ��˸���
	unsigned long ulPlayerID;
	unsigned int fromMapId;  // ��Դ��ID
	unsigned int  DoorID;//uEctypeID;
	unsigned int  uEctypeRoomID;
	unsigned char ucArmyNum ; //��Ӳ��ӵ�����
	inline void serialize(int from, int to, int command);
	inline unsigned long& operator[](int iIndex);
};

struct ReqLeaveEctypeArmyInfo : DISTRIBUTED_HEADER
{
	unsigned char ucArmyNum ; // ���ӵ�����
	
	inline void serialize(int from, int to, int command = EVENT_ARMY_LEAVE_ECTYPE)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(unsigned long) * ucArmyNum, 0, from, to, command);
	}

	inline unsigned long& operator[](int iIndex)
	{
		return ((unsigned long*)((unsigned char *)this + sizeof(ReqLeaveEctypeArmyInfo)))[iIndex];
	}
};

// �����뿪���������򸱱�����ʱ������Ϣ
struct RspArmyLeaveEctype : DISTRIBUTED_HEADER
{
	unsigned int playerId; // ���ӹ������ID
	unsigned int roomId;   // ������������ʵ�ʵķ���ID
	unsigned int armyId;   // armyId��0��ʾ�������������ʵ�ʵĲ���ID
	inline void serialize(int length, int agent, int from, int to, int command = EVENT_ARMY_LEAVE_OR_DIE_IN_ECTYPE);
};

void RspArmyLeaveEctype::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length, 4000, from, to, command);
}

// ����exe����ʱ��ע�ᵽ��Ϸ������������������Ϣ
struct ESvcRegiInGameHall : DISTRIBUTED_HEADER
{
	unsigned int eServerId;
    inline void serialize(int length, int agent, int from, int to, int command = ECTYPE_SERVER_REGISTER_IN_GAMEHALL);
};

void ESvcRegiInGameHall::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length, 4000, from, to, command);
}

struct RspAddArmyToEctype :DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	unsigned long uEctypeID;
	int count;
	//char ArmyID[MAX_ARMY_NUM][SIZE_24];
	char ArmyName[MAX_ARMY_NUM][SIZE_24];//20091221
	inline void serialize(int length, int agent, int from, int to, int command);
};
//���������еĸ�����ս����Ұ��ȷ��������Ӳ���
struct ReqAddArmyToRoom :DISTRIBUTED_HEADER
{
	int fromMapID; //��Դ��ͼ
	unsigned long serverID; //ʵ�ʷ�����ID
	int roomID; //ʵ�ʷ���ID
	int count; //��������
	inline unsigned long& operator[](int index)//��Ҫ����Ĳ���
	{
		return ((unsigned long *)((char *)this + sizeof(ReqAddArmyToRoom)))[index]  ;
	}
	inline void serialize(int from , int to){
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(unsigned long) * count ,0,from,to,ADD_ARMY_TO_ROOM);
	}
};
struct RspInfo{
	unsigned long armyId; //���ӣɣ�
	unsigned char failReason; //ʧ��ԭ��
};
//��Ӧ������ս����Ұ��ȷ��������Ӳ���
struct RspAddArmyToRoom :DISTRIBUTED_HEADER
{
	bool state;//����״̬ 0 ����ʧ�� �� 1����ɹ�
	int count; //����ʧ�ܵĲ�������
	inline RspInfo& operator[](int index)//����ʧ�ܵĲ���
	{
		return ((RspInfo *)((char *)this + sizeof(RspAddArmyToRoom)))[index]  ;
	}
	inline void serialize(int from , int to){
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(RspInfo) * count ,0,from,to,RSP_ADD_ARMY_TO_ROOM);
	}
};
struct ReqEnterEctypeRoom :DISTRIBUTED_HEADER //���븱����������
{
	unsigned long	ulPlayerID;
	int				DoorID;//iEctypeID;
	unsigned int	uEctypeRoomID;
	inline void		serialize(int from, int to, int command);
};


struct EctypeRoomPlayerInfo  //����������Ϣ
{
	unsigned long	playerid;  //�����Ϣ
	int				faceid;  //ͷ��
	int				count ;    //��������
	unsigned char	cflag; //�Ƿ�������ɱ�־	
	int             iLevel;
	int				iMilitrayRank;
	char            cHeroName[SIZE_24];
	char			ArmyName[MAX_ARMY_NUM][SIZE_16]; //������������
};
struct RspEnterEctypeRoom :DISTRIBUTED_HEADER
{
	int							count;
	EctypeRoomPlayerInfo		RoomPlayerInfo[MAX_ECTYPE_ROOM_MEM];
	inline void serialize(int length, int agent, int from, int to, int command);
};

struct ReqEnterEctypeSystem : DISTRIBUTED_HEADER // ���븱��ϵͳ
{
	unsigned long ulPlayer;
	//unsigned int  uEctypeID;
	unsigned int  uEctypeRoomID;
	unsigned int  doorId;
	inline void serialize(int from, int to, int command = REQ_ENTER_ECTYPE_SYSTEM_INFO);
};

struct RspEnterEctypeSystem :DISTRIBUTED_HEADER
{
	bool isSigleEctype; // �Ƿ��ǵ��˸���
	unsigned char flag; //1 �� ׼���� ��0 = û�����δ׼���ã� 2 �� ���Ƿ���  3. ���븱��������ʧ��
	int  EctypeID;//�������
	int  EcServerID;           //����������ID
	unsigned int Ectypemapid;
	int  EctypeRoomID; //���ظ��������ķ�����
	int  NpcRoomID;  // ����ϵͳ������
	inline void serialize(int length, int agent, int from, int to, int command);
};

struct ReqExitEctypeHall :DISTRIBUTED_HEADER
{
	unsigned long playerID;
	unsigned int  iEctypeID;
	unsigned int  iRoomId;
	//inline void serialize(int from, int to, int command);
	inline void serialize(int from, int to, int command = REQ_LEAVE_ECTYPE_HALL_INFO);
};

inline void ReqExitEctypeHall::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}

struct RspExitEctypeHall:DISTRIBUTED_HEADER
{
	bool flag;
	bool IsHost;
	unsigned long ExitPlayerID;
	inline void serialize(int length, int agent, int from, int to, int command);
};
inline void RspExitEctypeHall::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

// ���ڵ��˸�����Ұ�⸱���������������ͻ��˵���Ϣ�ṹ��
struct RspSingleEctypeInfo :DISTRIBUTED_HEADER
{
	bool flag;    //�Ƿ�ɹ�
	unsigned int uEctypeRoomID; // ��������ID
	inline void serialize(int length, int agent, int from, int to, int command = RSP_BACK_SINGLE_ECTYPE_ROOM_INFO);
};

// ս����ʼ�ȴ�
struct RspBattleFieldWaitingInfo : DISTRIBUTED_HEADER
{
	inline void serialize(int length, int agent, int from, int to, int command = RSP_BATTLE_FIELD_WAITING_INFO)
	{
		DISTRIBUTED_HEADER::serialize(length, agent, from, to, command);
	}
};

// ȡ��ս���ȴ�
struct ReqCancelBattleFieldWaitingInfo : DISTRIBUTED_HEADER
{
	unsigned int roomId;
	unsigned int playerId;  // ���ȡ���ȴ������ID
	unsigned int campId;    // ��ӪID
	unsigned int entranceId; 

	inline void serialize(int length, int agent, int from, int to, int command = REQ_CANCEL_BATTLE_FIELD_WAITING_INFO)
	{
		DISTRIBUTED_HEADER::serialize(length, agent, from, to, command);
	}
};

// ȡ��ս���ȴ��ķ�����
struct RspCancelBattleFieldWaitingInfo : DISTRIBUTED_HEADER
{
	bool flag; // �ɹ����

	inline void serialize(int length, int agent, int from, int to, int command = RSP_CANCEL_BATTLE_FIELD_WAITING_INFO)
	{
		DISTRIBUTED_HEADER::serialize(length, agent, from, to, command);
	}
};

inline void RspSingleEctypeInfo::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length, agent, from, to, command);
}

struct ReqExitSingleEctypeInfo : DISTRIBUTED_HEADER
{
	unsigned long playerID;
	//unsigned int  iEctypeID;
	unsigned int  iRoomId;
	unsigned int  doorId;
	inline void serialize(int from, int to, int command);
};

inline void ReqExitSingleEctypeInfo::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
}

//////////////////////////////////////Ectype///////////////////////////////////////////////

struct REQ_ENTER_NPC_SYSTEM_STRUCT : DISTRIBUTED_HEADER//������븱��ϵͳ
{
	unsigned int uplayerID;
	unsigned int EctypeRoomID;
	int          EctypeID; // ����ĸ������	
	int			 msgFromMapID; //��Ϣ��Դ������  add by ljw 2009 12 03
	//inline void  serialize(int agant,int mapid ,int player , int command);
	inline void  serialize(int playerID ,int EcServerID , int command = REQ_ENTER_NPC_SYSTEM_INFO);
};


struct RSP_ENTER_NPC_SYSTEM_STRUCT : DISTRIBUTED_HEADER //����������븱����Ϣ
{
	bool			flag ;     //�����Ƿ�ɹ���־λ
	unsigned int	uPlayerID; //
	unsigned int    uEctypeID; //�������
	unsigned int	uNpcMapID; //������ͼ
	int				count;     //��������
	ARMY_POS		ArmyInfo[MAX_ARMY_NUM*10];
	inline void  serialize(int lenth,int agent ,int from ,int to , int command);
};
// ���ض�����Ϣ

struct ReqMoveEctypeArmyInfo :DISTRIBUTED_HEADER //�����ƶ�
{
	unsigned int	ulUserID;
	unsigned int    uEctypeID;
	unsigned int    uRoomID;
	unsigned int	ulArmyID;
	unsigned		uMapID ;
	POS_STRUCT		pos;
	inline void serialize(int from,int to  ,int command = REQ_MOVE_ECTYPE_ARMY_INFO );
};

inline void ReqMoveEctypeArmyInfo::serialize(int from ,int to ,int command )
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from ,to,command);
}
struct RspMoveEctypeArmyInfo :DISTRIBUTED_HEADER
{
	unsigned int ulUserID;
	unsigned int ulArmyID;
	//mapstate state ;
	POS_STRUCT POS;
	bool flag;
	inline void serialize(int length, int agent, int from, int to, int command = RSP_MOVE_ECTYPE_ARMY_INFO);
};

inline void RspMoveEctypeArmyInfo::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

struct ReqAttackTargetInfo :DISTRIBUTED_HEADER // ���ý���Ŀ��
{
	unsigned int uUserID;
	unsigned int uEctypeID;
	unsigned int uEctypeRoomID;
	unsigned int myArmyID;
	unsigned int TargetArmyID;
	int			 x;
	int			 y;
	inline void serialize(int from ,int to , int command = REQ_ATTACK_ECTYPE_TARGET_INFO);

};//ѡ�񹥻�Ŀ��
inline void ReqAttackTargetInfo::serialize(int from ,int to , int command )
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}
struct RspAttackTargetInfo : DISTRIBUTED_HEADER
{
	int flag ;
	inline void serialize(int ,int,int,int,int command = RSP_ATTACK_ECTYPE_TAGGET_INFO);
};
inline void RspAttackTargetInfo::serialize(int length ,int agent,int from,int to ,int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}
struct REQ_EXIT_NPC_SYSTEM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned int uUserID;
	inline void serialize(int agent ,int mapid,int userid,int cmd = REQ_EXIT_NPC_SYSTEM_INFO);
};
inline void REQ_EXIT_NPC_SYSTEM_STRUCT :: serialize(int agent ,int mapid,int userid,int cmd)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent ,mapid,userid,cmd);
}
struct RSP_EXIT_NPC_SYSTEM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long uUserID;
	//FIGHT_ROLE_TYPE type ;
	inline void serialize(int agent ,int mapid ,int userid,int cmd);
};

struct ReqLeaveEctypeSystem:DISTRIBUTED_HEADER  //�뿪����
{
	unsigned long uUserID;
	unsigned int  uEctypeID;
	unsigned int  uEctypeRoomID;
	inline void serialize(int from ,int to ,int command = REQ_LEAVE_ECTYPE_SYSTEM_INFO);
};

inline void ReqLeaveEctypeSystem::serialize(int from ,int to ,int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}


struct ReqEctypeGetArmyInfo : DISTRIBUTED_HEADER
{
	unsigned long ArmyID;
	unsigned long EctypeID;
	unsigned long EctypeRoomID;
	inline void serialize(int PlayerID, int EctypeServerID, int command = REQ_ECTYPE_GET_ARMY_INFO);
};

inline void ReqEctypeGetArmyInfo::serialize(int PlayerID, int EctypeServerID, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,PlayerID,EctypeServerID,command);
}
struct ReqEctypeRetreatArmyInfo : DISTRIBUTED_HEADER
{
	unsigned long ArmyID;
	unsigned long EctypeID;
	unsigned long EctypeRoomID;
	int  entranceID; //���ID
	inline void serialize(int PlayerID, int EctypeServerID, int command = REQ_ECTYPE_SYSTEM_ARMY_RETREAT);
};
inline void ReqEctypeRetreatArmyInfo::serialize(int PlayerID, int EctypeServerID, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,PlayerID,EctypeServerID,command);
}
struct RspEctypeGetArmyInfo : DISTRIBUTED_HEADER
{
	unsigned long	ArmyHostID; //������ұ��
	unsigned int	FaceID; //Ӣ��ͷ����
	ATTACK_TYPE		Attacktype; //��������(����/ħ��)
	int				AttackPower;//������
	int				DefendPyPower;//������
	int				defendMgPower;//ħ������
	int				soldiercount;//�ܵı�������
	int				totalHp;// HP����
	int				weaponID; //����ID
	int				defendID; //����ID
	bool			TrustFlag; //�йܱ�־
	RACE_TYPE	    raceID;	//����ID
	char			ArmyName[SIZE_16]; 
	char			szMainHeroName[SIZE_16];
	char            szHeroName[SIZE_16];
	unsigned char	iSodierOccupation;//����ְҵ
	unsigned short  iLevel; //�ȼ�
	unsigned short  iPower;//����
	unsigned short  iCelerity;//����
	unsigned short  iIntellect;//����
	unsigned short  iStamina;//����

	inline void serialize(int length , int agent,int from ,int to ,int cmd);

};

inline void RspEctypeGetArmyInfo::serialize(int length , int agent,int from ,int to ,int cmd)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,cmd);
}

////////////////////////////////////////////////////////////////////////////////
//test 
struct tagEnterEctype: DISTRIBUTED_HEADER
{
	unsigned long   uPlayerID;
	unsigned int    EcypeID;  
	unsigned int    DoorID;   //������ID ��3λ������
	inline void     serialize( int from, int to, int command =REQ_TEST_ENTER_ECTYPE_SYSTEM_INFO );
};
inline void tagEnterEctype::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}

struct tagRspEnterEctype:DISTRIBUTED_HEADER
{
	bool flag ;
	inline void serialize(int length, int agent, int from, int to, int command =RSP_TEST_ENTER_ECTYPE_SYSTEM_INFO);
};

inline void tagRspEnterEctype::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}
///////////////

struct ReqSetEctypeArmyTrust : DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	unsigned long ulArmyID;
	unsigned int  uEctypeID;
	unsigned int  uEctypeRoomID;
	inline void  serialize(int playerID, int ectServerID, int command = REQ_SET_ECTYPE_ARMY_TRUST_INFO);
};

inline void ReqSetEctypeArmyTrust::serialize(int playerID, int ectServerID, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerID,ectServerID,command);
}
struct RspSetEctypeArmyTrust :DISTRIBUTED_HEADER
{
	bool flag;
	bool IsTrust;
	inline void serialize(int length, int agent, int from, int to, int command);
};

inline void RspSetEctypeArmyTrust::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

/*
struct RspNoticeEndEctypeInfo :DISTRIBUTED_HEADER
{
	unsigned int EctypeID;
	unsigned int EctypeRoomID;
	int			 flag;
	int			 iKillNpcNum;  //ɱ��NPC����
	int			 iDealNum;     //��������
	int			 iGetBoxNum;	//��ȡ��������
	inline void serialize(int length, int agent, int from, int to, int command);
};
*/

struct RspNoticeEndEctypeInfo :DISTRIBUTED_HEADER
{
	unsigned int EctypeID;
	unsigned int EctypeRoomID;
	unsigned int ulFromMapID;  //���ĸ������븱��
	int			 flag;
	int			 iKillNpcNum;  //ɱ��NPC����
	int			 iDealNum;     //��������
	int			 iGetBoxNum;   //��ȡ��������
	inline void serialize(int length, int agent, int from, int to, int command);
};
inline void RspNoticeEndEctypeInfo::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

struct ReqBackToHall:DISTRIBUTED_HEADER //���ش���
{
	unsigned int ulPlayerID;
	unsigned int DoorID;
	int			 iEctypeRoomID;
	inline void serialize(int playerid, int mapid, int command = REQ_BACK_ECTYPE_HALL_INFO);

};

struct ReqEnterFieldEctype : DISTRIBUTED_HEADER // ����Ұ�⸱��
{
	unsigned int armyID;
	unsigned int doorId;

	inline void serialize(int from, int to, int command = REQ_ENTER_FIELD_ECTYPE)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to,command);
	}
};

struct RspEnterFieldEctype : DISTRIBUTED_HEADER
{
	unsigned int roomId;
	unsigned int ectypeId;
	unsigned int armyId;

	inline void serialize(int length, int agent, int from, int to, int command = RSP_ENTER_FIELD_ECTYPE)
	{
		DISTRIBUTED_HEADER::serialize(length, agent, from, to, command);
	}
};

inline void ReqBackToHall::serialize(int playerid, int mapid, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,mapid,command);
}
struct RspBackToHall:DISTRIBUTED_HEADER  // 
{
	bool IsRoomHost;   // �Ƿ��Ƿ�����־
	unsigned long playerID;
	int  iEctypeRoomID; //�˳��ķ�����ұ��
	char Name[SIZE_24];//�˳��������
	inline void serialize(int length, int agent, int from, int to, int command = RSP_BACK_ECTYPE_HALL_INFO);
};
inline void RspBackToHall::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

struct RspDeleteArmyInfo :DISTRIBUTED_HEADER
{
	int count ;
	unsigned long ArmyID[25];
	inline void serialize( int lenth, int agent, int from, int to, int command = RSP_DELETE_ARMY_INFO)
	{
		DISTRIBUTED_HEADER::serialize(lenth,agent,from,to,command);
	}
};

struct Notify_Is_In_Ectype :DISTRIBUTED_HEADER
{
	unsigned long playerID;
	unsigned int  uEctype;
	unsigned int  entranceID;
	unsigned int  uFromMapID;
	inline void serialize(int length, int command = CMD_IS_IN_ECTYPE)
	{
		DISTRIBUTED_HEADER::serialize(length,0,0,0,command);
	}
};
struct Notify_Back_Is_In_Ectype :DISTRIBUTED_HEADER
{
	unsigned long	playerID;
	bool			ret;
	int				roomid;
	int				uEctypID;
    unsigned int  entranceID;

	inline void serialize(int length, int command = CMD_BACK_IS_IN_ECTYPE)
	{
		DISTRIBUTED_HEADER::serialize(length,0,0,0,command);
	}
};
struct ReqEctypeReady:DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	int			  iDoorID;
	int			  iEctypeRoomID;
	unsigned char flag ;// =1 ׼���ţ�0 =û��׼����
	inline void serialize( int playerID, int serverID, unsigned char f,int doorid,int roomid,int command =REQ_ECTYPE_READY_INFO)
	{
		this->ulPlayerID = playerID;
		this->flag = f;
		this->iDoorID = doorid;
		this->iEctypeRoomID = roomid;
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerID,serverID,command);
	}
};
struct RspEctypeReady:DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	unsigned long flag;
	inline void serialize(int agent, int from, int to, int command =RSP_ECTYPE_READY_INFO)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this), agent, from,to,command);
	}
};

struct ReqGetArmyEntranceID : DISTRIBUTED_HEADER
{
	unsigned long armyId;

	inline void serialize(int agent, int from, int to, int command = REQ_GET_ARMY_IN_ENTRANCEID)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
	}
};

struct RspGetArmyEntranceID : DISTRIBUTED_HEADER
{
	unsigned long entranceId;

	inline void serialize(int agent, int from, int to, int command = RSP_GET_ARMY_IN_ENTRANCEID)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
	}
};

struct ReqEctypeDeleteArmy:DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	unsigned long ulArmyID;
	int           RoomID;
	inline void serialize(int playerid, int serverid, int roomid,unsigned long Armyid,int command = REQ_ECTYPE_DELETE_ARMY_INFO)
	{
		this->ulPlayerID = playerid;
		this->ulArmyID = Armyid;
		this->RoomID = roomid;
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,serverid,command);
	}
};

struct RspEctypeDeleteArmy:DISTRIBUTED_HEADER
{
	unsigned long	ulPlayerID;
	unsigned long	ulArmyID;
	int				iRoomID;
	unsigned char   cFlag;

	inline void serialize(int length, int agent, int from, int to, int command =RSP_ECTYPE_DELETE_ARMY_INFO)
	{
		DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
	}
};
struct EctypeTurnInfo:DISTRIBUTED_HEADER
{
	unsigned long	RooID;
	int				iTurnNum;
	int				CurToTalArmyNum;
	int             CurProArmyNum;
	inline void serialize(int agent,int from ,int to , int command )
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
	}

};

struct RspGetRse:DISTRIBUTED_HEADER
{
	unsigned long	ItemId;
	unsigned short  ResNum;
	inline void serialize(int agent,int from,int to,int command = RSP_ECTYPE_GET_RES)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
	}
};

//ǿ��ת������ڸ����г���ʱ�����û���Լ��Ĳ�����ǿ����ת����Դ��
struct ForceVeer :DISTRIBUTED_HEADER
{
	int fromMap; //��Դ��ID
	inline void serialize(int agent, int from, int to){
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to, EVENT_FORCE_VEER);
	}
};
#pragma pack(pop)


inline void ReqCreateEctypeRoom ::serialize(int mapid,int player,int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0 ,player,mapid,command);
}

inline void RspCreateEctypeRoom::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length ,agent,from,to,command);
}

inline void ReqAddToEctypeRoom ::serialize( int mapid ,int player,int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0 ,player,mapid,command);
}

inline void RspAddToEctypeRoom ::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length ,agent,from,to,command);
}

inline void ReqKickOutEctypeRoom ::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}

inline void RspKickOutEctypeRoom::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

inline void ReqHallInfo::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from ,to ,command);
}

inline void RspHallInfo::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

inline void ReqEctypeRoomInfo::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to ,command);
}

inline void RspEctypeRoomInfo::serialize(int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
}

inline void ReqEctypeRoomNote::serialize( int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}

inline void RspEctypeRoomNote::serialize(int length ,int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

inline void ReqAddArmyToEctype::serialize( int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(unsigned long) * ucArmyNum,0,from,to,command);
}
inline unsigned long& ReqAddArmyToEctype::operator [](int iIndex)
{
	return ((unsigned long*)((unsigned char *)this + sizeof(ReqAddArmyToEctype)))[iIndex];
}
inline void RspAddArmyToEctype::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

inline void RSP_EXIT_NPC_SYSTEM_STRUCT::serialize(int agent ,int mapid ,int userid,int cmd)
{
	this->uUserID = userid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,userid,mapid,cmd);
}

//inline void REQ_ENTER_NPC_SYSTEM_STRUCT::serialize(int agant,int mapid ,int player , int command)
//{
//	DISTRIBUTED_HEADER::serialize(sizeof(*this),agant,mapid,player,command);
//}
//20091221
inline void REQ_ENTER_NPC_SYSTEM_STRUCT::serialize(int playerID ,int EcServerID , int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerID,EcServerID,command);
}

inline void RSP_ENTER_NPC_SYSTEM_STRUCT::serialize(int lenth,int agent,int playerID ,int mapid , int command)
{
	DISTRIBUTED_HEADER::serialize(lenth,agent,mapid,playerID ,command);
}
//inline void REQ_ENTER_SEE_OTHER_FIGHT_STRUCT ::serialize(int agant , int Mapid,int PlayerID,int command)
//{
//	DISTRIBUTED_HEADER::serialize(sizeof(*this),agant,PlayerID,Mapid,command);
//}
//
//inline void RSP_ENTER_SEE_OTHER_FIGHT_STRUCT::serialize(int mapid,int playerid,int command)
//{
//	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,mapid,playerid,command);
//}

inline void ReqEnterEctypeRoom ::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from ,to,command);
}

inline void RspEnterEctypeRoom::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

inline void RspEnterEctypeSystem ::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}


inline void ReqEnterEctypeSystem::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}
#endif