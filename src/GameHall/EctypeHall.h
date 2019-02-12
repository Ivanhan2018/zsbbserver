// @brief 简要描述: 副本大厅实体类  
// @author 作者: 黄 文 才

#ifndef ECTYPE_HALL_H
#define ECTYPE_HALL_H

#include "FightManager.h"
#include "EctypeRoom.h"
#include "LoadDBInfo.h"
#include "GameHallCommon.h"
	
typedef std::map<unsigned int /* 玩家ID */, unsigned int /* 副本ID */> PlayerID2EctypeId;
typedef std::map<unsigned int, EctypeRoom *> RoomID2Info;		// <房间ID 房间>
typedef std::map<unsigned int, RoomID2Info> Ectype2RoomInfo;    // <副本ID <房间ID，房间> >
typedef std::map<unsigned int /* entranceID */, std::pair<unsigned int /* faceRoomId */, unsigned int /* ectypeId */ > > EntranceId2RoomId;

typedef std::map<unsigned int /* 部队ID */, unsigned int /* 副本ID */> ArmyId2EctypeId;

typedef std::map<unsigned int /* 部队ID */, unsigned int /* EntranceId */> ArmyId2EntranceId;

typedef std::map<unsigned int /* 阵营ID */, RoomID2Info /* 房间ID, 房间*/> CampId2RoomInfo; 

typedef std::map<unsigned int /* entranceID */, CampId2RoomInfo> EntranceId2CampIdRoomInfo;

typedef std::map<unsigned int /* 阵营ID */, std::set<unsigned int /* 玩家ID */> > CampId2PlayerIds;

typedef std::map<unsigned int /* entranceID */, CampId2PlayerIds> EntranceId2CampIdPlayerIds;

typedef std::map<unsigned int /* 房间ID */, bool /* 房间内的所有玩家是否准备好 */> Room2IsReady;
								
class CLoadDBInfo;
class DB_Proxy;

// 部队基本信息
struct ArmyBaseInfo
{
	ArmyBaseInfo(unsigned int playerIdParam, const char *szArmyName) : playerId(playerIdParam), armyName(szArmyName)
	{
	}

	unsigned int playerId;     // 部队归属玩家ID
	string armyName;           // 部队名
};

// 玩家基本信息
struct PlayerBaseInfo
{
	PlayerBaseInfo(unsigned int playerIdParam, unsigned int faceIdParam, const char *szPlayerName, unsigned int levelParam, unsigned int rankParam, unsigned int campIdParam) 
		: playerId(playerIdParam), faceID(faceIdParam), playerName(szPlayerName), level(levelParam), rank(rankParam), campId(campIdParam)
	{
	}

	unsigned int playerId;	 // 玩家ID
	unsigned int faceID;     // 头像ID
	string playerName;       // 玩家名
	unsigned int level;		 // 等级
	unsigned int rank;       // 军衔
	unsigned int campId;     // 阵营ID
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

	// 函数描述：用户双击进入副本时的处理函数
	// @param msgPtr 发送请求的结构体
	// @return 返回函数是否成功执行
	bool onEnterEctypeHall(const ReqHallInfo *msgPtr);

	bool onEnterEctypeHallFromArmyList(const ReqHallInfoFromArmyList *msgPtr);

	// 函数描述：副本服务器判断是否在副本中，然后回调到此
	// 其中Notify_Back_Is_In_Ectype 中的ret的字段在副本服务器中赋的值，其意思是否在副本中
	// @param lpEvent 副本服务器返回的结构体
	// @return 返回函数是否成功执行
	bool eventBackIsInEctypeInfo(const Notify_Back_Is_In_Ectype *lpEvent);

	// 函数描述：创建副本房间
	// @param msg 发送请求的结构体
	// @return 返回函数是否成功执行
	bool onCreateEctypeRoom(const ReqCreateEctypeRoom *msg);

	// 函数描述：加入房间
	// @param msg 发送请求的结构体
	// @return 返回函数是否成功执行
	bool onAddToEctypeRoom(const ReqAddToEctypeRoom *msg);

	// 函数描述：用户点击添加部队面板上 确定 按钮时的处理函数
	// @param msg 发送请求的结构体
	// @return 返回函数是否成功执行
	bool onAddArmyToRoom(ReqAddArmyToEctype  *msg);

	// 函数描述：用户点击大厅面板上 踢出房间 按钮时的处理函数
	// @param msg 发送请求的结构体
	// @return 返回函数是否成功执行
	bool onKickOutOneMember(const ReqKickOutEctypeRoom *msg);

	// 函数描述：用户点击大厅面板上 玩家列表 时的处理函数
	// @param msg 发送请求的结构体
	// @return 返回函数是否成功执行
	bool onGetRoomInfo(const ReqEctypeRoomInfo *msg);

	// 函数描述：用户点击 房间面板上 修改公告 按钮时的处理函数
	// @param msg 发送请求的结构体
	// @return 返回函数是否成功执行
	bool onRoomNotice(const ReqEctypeRoomNote* msg);

	// 函数描述：用户点击大厅面板上 进入副本 按钮时的处理函数
	// @param msg 发送请求的结构体
	// @return 返回函数是否成功执行
	bool onEnterEctypeSystem(const ReqEnterEctypeSystem *msg);

	// 函数描述：用户点击大厅面板上 退出房间 按钮时的处理函数
	// @param msg 发送请求的结构体
	// @return 返回函数是否成功执行
	bool onExitEctypeHall(const ReqExitEctypeHall *msg);

	// 函数描述：用户点击房间面板上 返回大厅 按钮时的处理函数
	// @param msg 发送请求的结构体
	// @return 返回函数是否成功执行
	bool onBackEctypeHall(const ReqBackToHall *msg);

	// 函数描述：用户点击房间面板上 添加完成 按钮时的处理函数
	// @param msg 发送请求的结构体
	// @return 返回函数是否成功执行
	bool onEctypeReady(const ReqEctypeReady *msg);	

	// 创建副本,副本给予的事件通知
	bool eventCreateEctypeBack(const Notify_Back_Enter_Ectype *eventCreateEctype);

	// 函数描述：通过 doorID返回副本ID
	// @param doorID 结构体中的doorID
	// @return 返回副本ID
	unsigned int getEctypeID(unsigned int doorID) const;

	// 函数描述：在eventBackIsInEctypeInfo中调用，用来处理返回给客户端的大厅信息
	// @param lpReq 请求参数结构体
	// @return 返回函数是否成功执行
	bool rspEnterEctypeHall(unsigned long playerId, unsigned int ectypeID, unsigned int entranceID);

	// 函数描述：判断用户是否在副本中
	// @param playerID 玩家ID
	// @return 返回是否副本中
	bool isInEctypeRoom(unsigned int playerID, bool isSingleRoom = false);

	// 函数描述：根据副本ID自动生成一个房间ID
	// @param EctypeID 副本ID
	// @return 返回房间ID
	unsigned int getRoomID(unsigned int EctypeID);

	// 函数描述：添加一个房间
	// @param ectypeID 副本ID
	// @param roomID   房间ID
	// @param room 要增加的房间
	// @return 函数是否成功执行
	bool addRoomInEctype(unsigned int ectypeID, unsigned int roomID, const EctypeRoom* room);

	// 函数描述：删除一个房间
	// @param ectypeID 副本ID
	// @param roomID   房间ID
	// @return 函数是否成功执行
	void deleteRoom(unsigned long ectypeID, unsigned long roomID);

	// 函数描述：进入副本房间,获取房间信息
	// @return 函数是否成功执行
	bool rspEnterEctypeRoomInfo(ReqEnterEctypeRoom *buf);
	
	// 函数描述：根据副本ID,房间ID返回一个房间
	// @param ectypeID 副本ID
	// @param roomID   房间ID
	// @return 函数是否成功执行
	EctypeRoom *getRoom(unsigned int EctypeID, unsigned int RoomID);
	
	// 函数描述：通知房间中的所有玩家，该玩家退出房间了
	// @param roomTable_ 副本ID
	// @param ectypeID   副本ID
	// @param rsp RspBackToHall 结构体
	// @return 无
	void NoticeAllPlayerInRoom(int ectypeID, RspBackToHall &rsp);

	// 函数描述：房主退出房间时，通知所有的在该副本中的玩家（含同一在房间中的玩家）
	// @param roomTable_ 副本ID
	// @param ectypeID   副本ID
	// @param rsp RspBackToHall 结构体
	// @return 无
	void NoticeAllPlayerInEctype(int ectypeID, RspBackToHall &rsp);

	// 退出单人副本
	bool onExitSingleEctype(ReqExitSingleEctypeInfo *msg);

	// 判断是否是单人副本
	bool isSingleEctypeFun(unsigned int entranceId);

	// 获取部队名称
	ArmyBaseInfo getArmyBaseInfo(unsigned int armyId, unsigned int fromMapId);

	// 获取玩家信息
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

	PlayerID2Info &playerId2InfoPtr_;                               // 活动玩家信息

	CNPCInfo *npcInfo_;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 副本
	PlayerID2EctypeId playerID2EctypeId_;							// 在副本大厅中,包括在房间中的玩家ID
	Ectype2RoomInfo roomTable_;										// 副本房间信息
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

	ArmyId2EctypeId armyId2EctypeId_;								 // 部队与进入的副本ID映射
	ArmyId2EntranceId armyId2EntranceId;							 // 部队与进入的EntranceId映射
	EntranceId2RoomId entranceId2RoomId_;							 // EntranceId与房间ID的映射

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 战场
	EntranceId2CampIdPlayerIds entranceId2CampIdPlayerIds_;           // 在战场大厅中,包括在房间中的玩家ID
	EntranceId2CampIdRoomInfo entranceId2CampIdRoomInfo_;			  // 战场房间信息
	Room2IsReady room2IsReady_;                                       // 战场中准备好的房间（房间中的所有玩家都准备好）
	Room2IsReady room2IsWaiting;                                      // 战场中房间是否已进入等待状态
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

#endif