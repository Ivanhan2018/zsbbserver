#ifndef CONSORTIABANK_H__
#define CONSORTIABANK_H__

#include "IceUtil.h"

class CConsortiaBank:public IceUtil::Shared
{
public:
	CConsortiaBank();
	CConsortiaBank(int consortiaid,int itemid,int itemcount);
	~CConsortiaBank();
	int SetItemInfo(int consortiaid,int itemid,int itemcount);
	int SetConsortia(int consortiaid);
	int SetItemID(int itemid);
	int SetItemCount(int itemcount);

	int GetConsortiaID();
	int GetItemID();
	int GetItemCount();
private:
	int		ConsortiaID;		//公会ID
	int		ItemID;				//道具ID 
	int		ItemCount;			//道具数量
};
#endif
