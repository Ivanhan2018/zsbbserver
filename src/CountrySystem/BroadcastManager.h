#pragma once

struct DISTRIBUTED_HEADER;
struct FightPack;
enum PROTOCOL_FIGHT;


//����㲥����
//���ڿ��й㲥����ʱʹ��
class BroadcastManager
{
public:
	BroadcastManager();
public:
	~BroadcastManager(void);
public:
	//���
	void insertBroadData(DISTRIBUTED_HEADER * broadcastData ,PROTOCOL_FIGHT fightCmd ,int dataLen);
	//�㲥���ݵ��ͻ���
	void msgBroadcastData(void);
	//��ʼ��
	bool initialize(int curServerID);
private:
	//�㲥��
	FightPack * broadcastPackData_;
	//������ID
	int curServerID_;
};
