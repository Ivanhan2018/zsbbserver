// @brief ��Ҫ����: ��������ʵ����  
// @author ����: �� �� ��

#ifndef ECTYPE_HALL_H
#define ECTYPE_HALL_H

#include "FightManager.h"
#include "EctypeRoom.h"
#include "LoadDBInfo.h"
#include "GameHallCommon.h"
	
typedef std::map<unsigned int /* ���ID */, unsigned int /* ����ID */> PlayerID2EctypeId;
typedef std::map<unsigned int, EctypeRoom *> RoomID2Info;		// <����ID ����>
typedef std::map<unsigned int, RoomID2Info> Ectype2RoomInfo;    // <����ID <����ID������> >
typedef std::map<unsigned int /* entranceID */, std::pair<unsigned int /* faceRoomId */, unsigned int /* ectypeId */ > > EntranceId2RoomId;

typedef std::map<unsigned int /* ����ID */, unsigned int /* ����ID */> ArmyId2EctypeId;

typedef std::map<unsigned int /* ����ID */, unsigned int /* EntranceId */> ArmyId2EntranceId;

typedef std::map<unsigned int /* ��ӪID */, RoomID2Info /* ����ID, ����*/> CampId2RoomInfo; 

typedef std::map<unsigned int /* entranceID */, CampId2RoomInfo> EntranceId2CampIdRoomInfo;

typedef std::map<unsigned int /* ��ӪID */, std::set<unsigned int /* ���ID */> > CampId2PlayerIds;

typedef std::map<unsigned int /* entranceID */, CampId2PlayerIds> EntranceId2CampIdPlayerIds;

typedef std::map<unsigned int /* ����ID */, bool /* �����ڵ���������Ƿ�׼���� */> Room2IsReady;
								
class CLoadDBInfo;
class DB_Proxy;

// ���ӻ�����Ϣ
struct ArmyBaseInfo
{
	ArmyBaseInfo(unsigned int playerIdParam, const char *szArmyName) : playerId(playerIdParam), armyName(szArmyName)
	{
	}

	unsigned int playerId;     // ���ӹ������ID
	string armyName;           // ������
};

// ��һ�����Ϣ
struct PlayerBaseInfo
{
	PlayerBaseInfo(unsigned int playerIdParam, unsigned int faceIdParam, const char *szPlayerName, unsigned int levelParam, unsigned int rankParam, unsigned int campIdParam) 
		: playerId(playerIdParam), faceID(faceIdParam), playerName(szPlayerName), level(levelParam), rank(rankParam), campId(campIdParam)
	{
	}

	unsigned int playerId;	 // ���ID
	unsigned int faceID;     // ͷ��ID
	string playerName;       // �����
	unsigned int level;		 // �ȼ�
	unsigned int rank;       // ����
	unsigned int campId;     // ��ӪID
};

class EctypeHall
{
public:
	EctypeHall(CLoadDBInfo *dbInfo, DB_Proxy *dbPtr, PlayerID2Info &playId2InfoPtr);
	virtual ~EctypeHall(void);

	void parse(const char *buf);

	void initFieldEctypeRoom();

private:
	NO_COPYABLE_AND_ASSIGN(EctypeHall);

	// �����������û�˫�����븱��ʱ�Ĵ�����
	// @param msgPtr ��������Ľṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool onEnterEctypeHall(const ReqHallInfo *msgPtr);

	bool onEnterEctypeHallFromArmyList(const ReqHallInfoFromArmyList *msgPtr);

	// ���������������������ж��Ƿ��ڸ����У�Ȼ��ص�����
	// ����Notify_Back_Is_In_Ectype �е�ret���ֶ��ڸ����������и���ֵ������˼�Ƿ��ڸ�����
	// @param lpEvent �������������صĽṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool eventBackIsInEctypeInfo(const Notify_Back_Is_In_Ectype *lpEvent);

	// ����������������������
	// @param msg ��������Ľṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool onCreateEctypeRoom(const ReqCreateEctypeRoom *msg);

	// �������������뷿��
	// @param msg ��������Ľṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool onAddToEctypeRoom(const ReqAddToEctypeRoom *msg);

	// �����������û������Ӳ�������� ȷ�� ��ťʱ�Ĵ�����
	// @param msg ��������Ľṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool onAddArmyToRoom(ReqAddArmyToEctype  *msg);

	// �����������û������������� �߳����� ��ťʱ�Ĵ�����
	// @param msg ��������Ľṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool onKickOutOneMember(const ReqKickOutEctypeRoom *msg);

	// �����������û������������� ����б� ʱ�Ĵ�����
	// @param msg ��������Ľṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool onGetRoomInfo(const ReqEctypeRoomInfo *msg);

	// �����������û���� ��������� �޸Ĺ��� ��ťʱ�Ĵ�����
	// @param msg ��������Ľṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool onRoomNotice(const ReqEctypeRoomNote* msg);

	// �����������û������������� ���븱�� ��ťʱ�Ĵ�����
	// @param msg ��������Ľṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool onEnterEctypeSystem(const ReqEnterEctypeSystem *msg);

	// �����������û������������� �˳����� ��ťʱ�Ĵ�����
	// @param msg ��������Ľṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool onExitEctypeHall(const ReqExitEctypeHall *msg);

	// �����������û������������� ���ش��� ��ťʱ�Ĵ�����
	// @param msg ��������Ľṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool onBackEctypeHall(const ReqBackToHall *msg);

	// �����������û������������� ������ ��ťʱ�Ĵ�����
	// @param msg ��������Ľṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool onEctypeReady(const ReqEctypeReady *msg);	

	// ��������,����������¼�֪ͨ
	bool eventCreateEctypeBack(const Notify_Back_Enter_Ectype *eventCreateEctype);

	// ����������ͨ�� doorID���ظ���ID
	// @param doorID �ṹ���е�doorID
	// @return ���ظ���ID
	unsigned int getEctypeID(unsigned int doorID) const;

	// ������������eventBackIsInEctypeInfo�е��ã����������ظ��ͻ��˵Ĵ�����Ϣ
	// @param lpReq ��������ṹ��
	// @return ���غ����Ƿ�ɹ�ִ��
	bool rspEnterEctypeHall(unsigned long playerId, unsigned int ectypeID, unsigned int entranceID);

	// �����������ж��û��Ƿ��ڸ�����
	// @param playerID ���ID
	// @return �����Ƿ񸱱���
	bool isInEctypeRoom(unsigned int playerID, bool isSingleRoom = false);

	// �������������ݸ���ID�Զ�����һ������ID
	// @param EctypeID ����ID
	// @return ���ط���ID
	unsigned int getRoomID(unsigned int EctypeID);

	// �������������һ������
	// @param ectypeID ����ID
	// @param roomID   ����ID
	// @param room Ҫ���ӵķ���
	// @return �����Ƿ�ɹ�ִ��
	bool addRoomInEctype(unsigned int ectypeID, unsigned int roomID, const EctypeRoom* room);

	// ����������ɾ��һ������
	// @param ectypeID ����ID
	// @param roomID   ����ID
	// @return �����Ƿ�ɹ�ִ��
	void deleteRoom(unsigned long ectypeID, unsigned long roomID);

	// �������������븱������,��ȡ������Ϣ
	// @return �����Ƿ�ɹ�ִ��
	bool rspEnterEctypeRoomInfo(ReqEnterEctypeRoom *buf);
	
	// �������������ݸ���ID,����ID����һ������
	// @param ectypeID ����ID
	// @param roomID   ����ID
	// @return �����Ƿ�ɹ�ִ��
	EctypeRoom *getRoom(unsigned int EctypeID, unsigned int RoomID);
	
	// ����������֪ͨ�����е�������ң�������˳�������
	// @param roomTable_ ����ID
	// @param ectypeID   ����ID
	// @param rsp RspBackToHall �ṹ��
	// @return ��
	void NoticeAllPlayerInRoom(int ectypeID, RspBackToHall &rsp);

	// ���������������˳�����ʱ��֪ͨ���е��ڸø����е���ң���ͬһ�ڷ����е���ң�
	// @param roomTable_ ����ID
	// @param ectypeID   ����ID
	// @param rsp RspBackToHall �ṹ��
	// @return ��
	void NoticeAllPlayerInEctype(int ectypeID, RspBackToHall &rsp);

	// �˳����˸���
	bool onExitSingleEctype(ReqExitSingleEctypeInfo *msg);

	// �ж��Ƿ��ǵ��˸���
	bool isSingleEctypeFun(unsigned int entranceId);

	// ��ȡ��������
	ArmyBaseInfo getArmyBaseInfo(unsigned int armyId, unsigned int fromMapId);

	// ��ȡ�����Ϣ
	PlayerBaseInfo getPlayerBaseInfo(unsigned int playerId);

	bool onArmyLeaveEctype(ReqLeaveEctypeArmyInfo *buf);

	bool onEnterFieldEctype(ReqEnterFieldEctype *buf);

	void onFieldEctypeReboot(EventEctypeOver *buf);

	void initOneFieldEctypeRoom(unsigned int entraceID);

	void onGetArmyInEntranceId(ReqGetArmyEntranceID *buf);

	void GetEntranceIdRoomInfosToClient(const RoomID2Info &roomInfos, RspHallInfo &rspHallInfo, PlayerInfoPtr playerInfoPtr);

	EctypeRoom *getSpecifRoom(unsigned int entranceID, PlayerInfoPtr playerInfoPtr, unsigned int roomId);

	bool addRoomInBattlefield(unsigned int entranceId, unsigned int campId, unsigned int roomId, const EctypeRoom* room);

	EctypeRoom * getPartnershipRoom(unsigned int entranceId, unsigned int campId);

	EctypeRoom *getSpecifRoom(unsigned int entranceID, unsigned int roomId);

	void deleteBattlefieldRoom(unsigned int entranceId, unsigned int roomId);

	void noticeAllPlayerInBattelfieldRoom(unsigned int entranceId,  unsigned int campId, unsigned int roomId, RspBackToHall &rsp);

	void noticeAllPlayerInBattelfield(unsigned int entranceId, unsigned int campId,  RspBackToHall &rsp);

	unsigned int getBattlefieldRoomID(unsigned int entranceId);

	bool onCancelBattlefieldWaiting(ReqCancelBattleFieldWaitingInfo *buf);

	void notifyPlayerCancelBattlefieldFail(unsigned int agent, unsigned int playerId);

	CLoadDBInfo *dbInfo_;
	DB_Proxy  *dbPtr_;	

	PlayerID2Info &playerId2InfoPtr_;                               // ������Ϣ

	CNPCInfo *npcInfo_;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����
	PlayerID2EctypeId playerID2EctypeId_;							// �ڸ���������,�����ڷ����е����ID
	Ectype2RoomInfo roomTable_;										// ����������Ϣ
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

	ArmyId2EctypeId armyId2EctypeId_;								 // ���������ĸ���IDӳ��
	ArmyId2EntranceId armyId2EntranceId;							 // ����������EntranceIdӳ��
	EntranceId2RoomId entranceId2RoomId_;							 // EntranceId�뷿��ID��ӳ��

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ս��
	EntranceId2CampIdPlayerIds entranceId2CampIdPlayerIds_;           // ��ս��������,�����ڷ����е����ID
	EntranceId2CampIdRoomInfo entranceId2CampIdRoomInfo_;			  // ս��������Ϣ
	Room2IsReady room2IsReady_;                                       // ս����׼���õķ��䣨�����е�������Ҷ�׼���ã�
	Room2IsReady room2IsWaiting;                                      // ս���з����Ƿ��ѽ���ȴ�״̬
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

#endif