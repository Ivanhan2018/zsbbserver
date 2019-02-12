// @brief 简要描述: 房间实体类 
// @author 作者: 黄 文 才

#ifndef ECTYPE_ROOM_H
#define ECTYPE_ROOM_H

#include <map>
#include <set>
#include <string>
#include <assert.h>
#include "globalmacrodef.h"

// 英雄信息结构
struct HeroStruct 
{
	HeroStruct (const std::string &nameParam, const unsigned int levelParam, const unsigned int rankParam, unsigned int faceidParam)
		: name(nameParam), level(levelParam), rank(rankParam), faceid(faceidParam)
	{

	}

	std::string name;		 // 英雄名
	unsigned int level;		 // 等级
	unsigned int rank;       // 军衔
	unsigned int faceid;     // 头像ID
};

typedef std::map<unsigned int /* 郡ID */, std::set<unsigned int /* 部队ID */> > MapId2ArmyIds;

typedef std::map<unsigned int /* 玩家ID */, MapId2ArmyIds > PlayerId2MapId2ArmyIds;

typedef std::map<unsigned int /* 玩家ID */, const HeroStruct *> PlayerId2BaseInfo;	         // 玩家对应的主英雄信息	

typedef std::map<unsigned int /* 玩家ID */, bool /* 是否准备好 */> PlayerId2IsReady;		 // 玩家是否准备信息


class EctypeRoom
{
public:
	// 函数描述：构造函数
	// @param name 房间名称
	// @param szEctypeName 副本名称
	// @param szPassWord 密码
	// @param mapid 地图ID
	// @param playerID 房主ID
	// @param szNotify 房间公告
	// @return 无
	EctypeRoom(const char *szHostName, const char *szRoomName, const char *szPassWord, int mapid, unsigned int hostID, const char *szNotify);

	virtual ~EctypeRoom(void);

	// 函数描述：获取房间所在的副本ID
	// @return 返回副本ID
	inline unsigned int getEctypeID() const 
	{
		return ectypeID_;
	}

	// 函数描述：设置副本ID
	// @param ectypeID 房间所在的副本ID
	// @return 无
	inline void setEctypeID(unsigned int ectypeID)
	{
		ectypeID_ = ectypeID;
	}

	// 函数描述：获得房间ID
	// @return 返回房间ID
	inline int getRoomID() const
	{
		return roomID_;
	}
	
	// 函数描述：设置房间ID
	// @param roomID 要设置的房间ID
	// @return 无
	inline void setRoomID(unsigned int roomID)
	{
		roomID_ = roomID;
	}

	// 函数描述：获得房主ID
	// @return 返回房主ID
	inline unsigned int getRoomHostID() const
	{
		return roomHostID_;
	}
	
	// 函数描述：获得房主名
	// @return 返回房主名
	inline const std::string &getHostName() const
	{ 
		return hostName_;
	}

	// 函数描述：获得房间名
	// @return 返回房间名
	inline const std::string &getRoomName() const
	{
		return roomName_;
	}

	// 函数描述：设置房间名
	// @param roomName 要设置的房间名
	// @return 无
	inline void setRoomName(const std::string &roomName)
	{
		roomName_ = roomName;
	}
	
	// 函数描述：获得房间公告
	// @return 返回房间公告
	inline const std::string &getRoomNotice() const
	{
		return roomNotice_;
	}

	// 函数描述：获得房间成员总数
	// @return 返回房间成员总数
	inline unsigned int getHeroNum() const
	{
		return static_cast<unsigned int>(hero_.size());
	}

	// 函数描述：获得房间玩家信息集
	// @return 返回房间玩家信息集
	inline const PlayerId2MapId2ArmyIds &getPlayerInfo() const
	{
		return memberTable_;
	}

	// 函数描述：获得玩家英雄信息
	// @return 返回玩家英雄信息
	inline const PlayerId2BaseInfo &getPlayerHeroInfo() const
	{
		return hero_;
	}

	// 函数描述：获得玩家状态信息
	// @return 返回玩家状态信息
	inline PlayerId2IsReady &getPlayerStateInfo() 
	{
		return memberReadyInfo_;
	}

	// 函数描述：获得房间密码
	// @return 返回房间密码
	inline const std::string &getPassword() const
	{
		return roomPassword_;
	}

	// 函数描述：获得指定玩家是否准备好信息
	// @return 返回指定玩家是否准备好信息
	inline bool getIsReady(unsigned int playID) const
	{
		PlayerId2IsReady::const_iterator it = memberReadyInfo_.find(playID);

		if (it != memberReadyInfo_.end())
		{
			return it->second;
		}
		else
		{
			assert(0);
			return false;
		}
	}

	// 函数描述：设置玩家准备信息
	// @param playID 成员列表
	// @param isReady 是否准备好
	// @return 无
	inline void setIsReady(unsigned int playID, bool isReady)
	{
		 memberReadyInfo_[playID] = isReady;
	}

	// 函数描述：设置房间公告
	// @param RoomNote 要更改的房间公告
	// @return 无
	inline void setRoomNotice(const char *szRoomNote)
	{
		roomNotice_ = szRoomNote;
	}
	
	// 函数描述：增加一个玩家
	// @param playID 玩家ID
	// @param namePtr 玩家名字
	// @param levelParam 玩家等级
	// @param rankParam 玩家军衔
	// @return 无
	void addPlayer(unsigned int playerID, const char *szPlayerName, unsigned int levelParam, unsigned int rankParam, unsigned int faceId);

	// 函数描述：删除一个玩家的所用部队
	// @param playerID 玩家ID
	// @return 无
	void DeleteAllArmy(unsigned int playerID);

	// 函数描述：踢出(删除)一个玩家的所有部队
	// @param playerID 玩家ID
	// @return 无
	void kickMemberOrDelMem(unsigned int playerID);

	// 函数描述：给某个玩家增加一只部队
	// @param playerID 玩家ID
	// @param armyID 部队ID
	// @return 无
	void addArmy(unsigned int playerID, unsigned int countryId, unsigned int armyID);

	inline unsigned int getRoomHasTotalArmyNum() const 
	{
		return roomHasArmySum;
	}

private:
	NO_COPYABLE_AND_ASSIGN(EctypeRoom);

	unsigned int ectypeID_;					// 副本ID
	unsigned int roomID_;					// 房间ID
	unsigned int roomHostID_;				// 房主ID
	std::string roomName_;					// 房间名称
	std::string hostName_;					// 房主名称
	std::string roomPassword_;				// 房间密码
	std::string roomNotice_;				// 房间公告
	unsigned int mapID_;					// 地图ID
	PlayerId2MapId2ArmyIds memberTable_;	// <玩家ID,<郡ID, 部队ID集>>
	PlayerId2BaseInfo hero_;				// 玩家英雄基本信息
	PlayerId2IsReady memberReadyInfo_;      // 玩家是否准备好
	unsigned int roomHasArmySum;            // 房间内的总部队数
};

#endif
