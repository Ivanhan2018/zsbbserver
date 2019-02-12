#ifndef _GAME_HALL_COMMON_H
#define _GAME_HALL_COMMON_H

#include <map>
#include <boost/shared_ptr.hpp>

struct PlayerInfo
{
	unsigned int playerId;		 // ���ID
	GAME_HALL_TYPE gameHallType;
	unsigned int faceID;		 // ͷ��ID
	unsigned int agent;			 // ����
	
	unsigned int level;			 // �ȼ�
	unsigned int rank;			 // ����

	unsigned int campId;         // ��ӪID
	string playerName;			 // �����

	PlayerInfo() : playerId(0), gameHallType(HALL_TYPE_INVALID), faceID(0), agent(0), level(0), rank(0), campId(0)
	{
	}
};

typedef boost::shared_ptr<PlayerInfo> PlayerInfoPtr;

typedef std::map<unsigned int /*���ID*/, PlayerInfoPtr /*�����Ϣ*/> PlayerID2Info;

#endif