#ifndef CONSORTIAUNION_H__
#define CONSORTIAUNION_H__

#include "IceUtil.h"
class CConsortiaUnion:public IceUtil::Shared
{
public:
	CConsortiaUnion();
	CConsortiaUnion(int id,int con1id,int con2id);
	~CConsortiaUnion();
	int SetUnion(int id,int con1id,int con2id);
	int SetUnionID(int id);
	int SetConsortia1ID(int consortiaid);
	int SetConsortia2ID(int consortiaid);

	int GetUnionID();
	int GetConsortia1ID();
	int GetConsortia2ID();
private:
	int		UnionID;			//����ͬ��ID
	int		Consortia1ID;		//����1 ID
	int		Consortia2ID;		//����2 ID
};

#endif
