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
*˵�������ù�����Ϣ
*���������	
*����ֵ��0���ɹ�
**************************************************************************/
int CConsortiaBank::SetItemInfo(int consortiaid,int itemid,int itemcount)
{
	ConsortiaID = consortiaid;
	ItemID = itemid;
	ItemCount = itemcount;
	return 0;
}
/**************************************************************************
*˵�������ù�����
*���������	
*����ֵ��������
**************************************************************************/
int CConsortiaBank::SetConsortia(int consortiaid)
{
	return (ConsortiaID = consortiaid);
}
/**************************************************************************
*˵����������ԴID
*���������	
*����ֵ����ԴID
**************************************************************************/
int CConsortiaBank::SetItemID(int itemid)
{
	return (ItemID = itemid);
}
/**************************************************************************
*˵����������Դ����
*���������	
*����ֵ����Դ����
**************************************************************************/
int CConsortiaBank::SetItemCount(int itemcount)
{
	return (ItemCount = itemcount);
}
/**************************************************************************
*˵������ȡ����ID
*���������	
*����ֵ����ȡ����ID
**************************************************************************/
int CConsortiaBank::GetConsortiaID()
{
	return ConsortiaID;
}
/**************************************************************************
*˵����������Դ���
*���������	
*����ֵ��������Դ���
**************************************************************************/
int CConsortiaBank::GetItemID()
{
	return ItemID;
}
/**************************************************************************
*˵������Դ����
*���������	
*����ֵ����Դ����
**************************************************************************/
int CConsortiaBank::GetItemCount()
{
	return ItemCount;
}
