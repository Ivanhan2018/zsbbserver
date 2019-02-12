// @brief ��Ҫ����: ����ʵ���� 
// @author ����: �� �� ��

#ifndef ECTYPE_ROOM_H
#define ECTYPE_ROOM_H

#include <map>
#include <set>
#include <string>
#include <assert.h>
#include "globalmacrodef.h"

// Ӣ����Ϣ�ṹ
struct HeroStruct 
{
	HeroStruct (const std::string &nameParam, const unsigned int levelParam, const unsigned int rankParam, unsigned int faceidParam)
		: name(nameParam), level(levelParam), rank(rankParam), faceid(faceidParam)
	{

	}

	std::string name;		 // Ӣ����
	unsigned int level;		 // �ȼ�
	unsigned int rank;       // ����
	unsigned int faceid;     // ͷ��ID
};

typedef std::map<unsigned int /* ��ID */, std::set<unsigned int /* ����ID */> > MapId2ArmyIds;

typedef std::map<unsigned int /* ���ID */, MapId2ArmyIds > PlayerId2MapId2ArmyIds;

typedef std::map<unsigned int /* ���ID */, const HeroStruct *> PlayerId2BaseInfo;	         // ��Ҷ�Ӧ����Ӣ����Ϣ	

typedef std::map<unsigned int /* ���ID */, bool /* �Ƿ�׼���� */> PlayerId2IsReady;		 // ����Ƿ�׼����Ϣ


class EctypeRoom
{
public:
	// �������������캯��
	// @param name ��������
	// @param szEctypeName ��������
	// @param szPassWord ����
	// @param mapid ��ͼID
	// @param playerID ����ID
	// @param szNotify ���乫��
	// @return ��
	EctypeRoom(const char *szHostName, const char *szRoomName, const char *szPassWord, int mapid, unsigned int hostID, const char *szNotify);

	virtual ~EctypeRoom(void);

	// ������������ȡ�������ڵĸ���ID
	// @return ���ظ���ID
	inline unsigned int getEctypeID() const 
	{
		return ectypeID_;
	}

	// �������������ø���ID
	// @param ectypeID �������ڵĸ���ID
	// @return ��
	inline void setEctypeID(unsigned int ectypeID)
	{
		ectypeID_ = ectypeID;
	}

	// ������������÷���ID
	// @return ���ط���ID
	inline int getRoomID() const
	{
		return roomID_;
	}
	
	// �������������÷���ID
	// @param roomID Ҫ���õķ���ID
	// @return ��
	inline void setRoomID(unsigned int roomID)
	{
		roomID_ = roomID;
	}

	// ������������÷���ID
	// @return ���ط���ID
	inline unsigned int getRoomHostID() const
	{
		return roomHostID_;
	}
	
	// ������������÷�����
	// @return ���ط�����
	inline const std::string &getHostName() const
	{ 
		return hostName_;
	}

	// ������������÷�����
	// @return ���ط�����
	inline const std::string &getRoomName() const
	{
		return roomName_;
	}

	// �������������÷�����
	// @param roomName Ҫ���õķ�����
	// @return ��
	inline void setRoomName(const std::string &roomName)
	{
		roomName_ = roomName;
	}
	
	// ������������÷��乫��
	// @return ���ط��乫��
	inline const std::string &getRoomNotice() const
	{
		return roomNotice_;
	}

	// ������������÷����Ա����
	// @return ���ط����Ա����
	inline unsigned int getHeroNum() const
	{
		return static_cast<unsigned int>(hero_.size());
	}

	// ������������÷��������Ϣ��
	// @return ���ط��������Ϣ��
	inline const PlayerId2MapId2ArmyIds &getPlayerInfo() const
	{
		return memberTable_;
	}

	// ����������������Ӣ����Ϣ
	// @return �������Ӣ����Ϣ
	inline const PlayerId2BaseInfo &getPlayerHeroInfo() const
	{
		return hero_;
	}

	// ����������������״̬��Ϣ
	// @return �������״̬��Ϣ
	inline PlayerId2IsReady &getPlayerStateInfo() 
	{
		return memberReadyInfo_;
	}

	// ������������÷�������
	// @return ���ط�������
	inline const std::string &getPassword() const
	{
		return roomPassword_;
	}

	// �������������ָ������Ƿ�׼������Ϣ
	// @return ����ָ������Ƿ�׼������Ϣ
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

	// �����������������׼����Ϣ
	// @param playID ��Ա�б�
	// @param isReady �Ƿ�׼����
	// @return ��
	inline void setIsReady(unsigned int playID, bool isReady)
	{
		 memberReadyInfo_[playID] = isReady;
	}

	// �������������÷��乫��
	// @param RoomNote Ҫ���ĵķ��乫��
	// @return ��
	inline void setRoomNotice(const char *szRoomNote)
	{
		roomNotice_ = szRoomNote;
	}
	
	// ��������������һ�����
	// @param playID ���ID
	// @param namePtr �������
	// @param levelParam ��ҵȼ�
	// @param rankParam ��Ҿ���
	// @return ��
	void addPlayer(unsigned int playerID, const char *szPlayerName, unsigned int levelParam, unsigned int rankParam, unsigned int faceId);

	// ����������ɾ��һ����ҵ����ò���
	// @param playerID ���ID
	// @return ��
	void DeleteAllArmy(unsigned int playerID);

	// �����������߳�(ɾ��)һ����ҵ����в���
	// @param playerID ���ID
	// @return ��
	void kickMemberOrDelMem(unsigned int playerID);

	// ������������ĳ���������һֻ����
	// @param playerID ���ID
	// @param armyID ����ID
	// @return ��
	void addArmy(unsigned int playerID, unsigned int countryId, unsigned int armyID);

	inline unsigned int getRoomHasTotalArmyNum() const 
	{
		return roomHasArmySum;
	}

private:
	NO_COPYABLE_AND_ASSIGN(EctypeRoom);

	unsigned int ectypeID_;					// ����ID
	unsigned int roomID_;					// ����ID
	unsigned int roomHostID_;				// ����ID
	std::string roomName_;					// ��������
	std::string hostName_;					// ��������
	std::string roomPassword_;				// ��������
	std::string roomNotice_;				// ���乫��
	unsigned int mapID_;					// ��ͼID
	PlayerId2MapId2ArmyIds memberTable_;	// <���ID,<��ID, ����ID��>>
	PlayerId2BaseInfo hero_;				// ���Ӣ�ۻ�����Ϣ
	PlayerId2IsReady memberReadyInfo_;      // ����Ƿ�׼����
	unsigned int roomHasArmySum;            // �����ڵ��ܲ�����
};

#endif
