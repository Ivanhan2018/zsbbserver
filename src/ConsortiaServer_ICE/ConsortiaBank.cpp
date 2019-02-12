#include "ConsortiaBank.h"

CConsortiaBank::CConsortiaBank()
:ConsortiaID(0),ItemID(0),ItemCount(0)
{
}
CConsortiaBank::CConsortiaBank(int consortiaid,int itemid,int itemcount):
ConsortiaID(consortiaid),ItemID(itemid),ItemCount(itemcount)
{
}
CConsortiaBank::~CConsortiaBank()
{
}
/**************************************************************************
*说明：设置公会信息
*输入参数：	
*返回值：0：成功
**************************************************************************/
int CConsortiaBank::SetItemInfo(int consortiaid,int itemid,int itemcount)
{
	ConsortiaID = consortiaid;
	ItemID = itemid;
	ItemCount = itemcount;
	return 0;
}
/**************************************************************************
*说明：设置公会编号
*输入参数：	
*返回值：公会编号
**************************************************************************/
int CConsortiaBank::SetConsortia(int consortiaid)
{
	return (ConsortiaID = consortiaid);
}
/**************************************************************************
*说明：设置资源ID
*输入参数：	
*返回值：资源ID
**************************************************************************/
int CConsortiaBank::SetItemID(int itemid)
{
	return (ItemID = itemid);
}
/**************************************************************************
*说明：设置资源数量
*输入参数：	
*返回值：资源数量
**************************************************************************/
int CConsortiaBank::SetItemCount(int itemcount)
{
	return (ItemCount = itemcount);
}
/**************************************************************************
*说明：获取公会ID
*输入参数：	
*返回值：获取公会ID
**************************************************************************/
int CConsortiaBank::GetConsortiaID()
{
	return ConsortiaID;
}
/**************************************************************************
*说明：公会资源编号
*输入参数：	
*返回值：公会资源编号
**************************************************************************/
int CConsortiaBank::GetItemID()
{
	return ItemID;
}
/**************************************************************************
*说明：资源数量
*输入参数：	
*返回值：资源数量
**************************************************************************/
int CConsortiaBank::GetItemCount()
{
	return ItemCount;
}
