#include "ConsortiaUnion.h"


CConsortiaUnion::CConsortiaUnion()
:UnionID(0),Consortia1ID(0),Consortia2ID(0)
{
}
CConsortiaUnion::CConsortiaUnion(int id,int con1id,int con2id):
UnionID(id),Consortia1ID(con1id),Consortia2ID(con2id)
{
}
CConsortiaUnion::~CConsortiaUnion()
{
}
/**************************************************************************
*说明：设置公会同盟信息
*输入参数：	
*返回值：0:成功；其他:失败
**************************************************************************/
int CConsortiaUnion::SetUnion(int id,int con1id,int con2id)
{
	UnionID = id;
	Consortia1ID = con1id;
	Consortia2ID = con2id;
	return 0;
}
/**************************************************************************
*说明：设置公会同盟编号
*输入参数：	
*返回值：同盟编号
**************************************************************************/
int CConsortiaUnion::SetUnionID(int id)
{
	return (UnionID = id);
}
/**************************************************************************
*说明：设置公会1
*输入参数：	
*返回值：公会1编号
**************************************************************************/
int CConsortiaUnion::SetConsortia1ID(int consortiaid)
{
	return (Consortia1ID = consortiaid);
}
/**************************************************************************
*说明：设置公会2
*输入参数：	
*返回值：公会2编号
**************************************************************************/
int CConsortiaUnion::SetConsortia2ID(int consortiaid)
{
	return (Consortia2ID = consortiaid);
}
/**************************************************************************
*说明：获取同盟编号
*输入参数：	
*返回值：同盟编号
**************************************************************************/

int CConsortiaUnion::GetUnionID()
{
	return UnionID;
}
/**************************************************************************
*说明：同盟的公会编号
*输入参数：	
*返回值：同盟的公会编号
**************************************************************************/
int CConsortiaUnion::GetConsortia1ID()
{
	return Consortia1ID;
}
/**************************************************************************
*说明：获取同盟的公会编号
*输入参数：	
*返回值：同盟的公会编号
**************************************************************************/
int CConsortiaUnion::GetConsortia2ID()
{
	return Consortia2ID;
}