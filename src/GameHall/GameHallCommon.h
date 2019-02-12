#ifndef _GAME_HALL_COMMON_H
#define _GAME_HALL_COMMON_H

#include <map>
#include <boost/shared_ptr.hpp>

struct PlayerInfo
{
	unsigned int playerId;		 // 玩家ID
	GAME_HALL_TYPE gameHallType;
	unsigned int faceID;		 // 头像ID
	unsigned int agent;			 // 代理
	
	unsigned int level;			 // 等级
	unsigned int rank;			 // 军衔

	unsigned int campId;         // 阵营ID
	string playerName;			 // 玩家名

	PlayerInfo() : playerId(0), gameHallType(HALL_TYPE_INVALID), faceID(0), agent(0), level(0), rank(0), campId(0)
	{
	}
};

typedef boost::shared_ptr<PlayerInfo> PlayerInfoPtr;

typedef std::map<unsigned int /*玩家ID*/, PlayerInfoPtr /*玩家信息*/> PlayerID2Info;

#endif