#ifndef CONSORTIAOFFICIAL_H__
#define CONSORTIAOFFICIAL_H__

#include "IceUtil.h"

class CConsortiaOfficial:public IceUtil::Shared
{
public:
	CConsortiaOfficial();
	CConsortiaOfficial(int id,const char *name,int purview,short level);
	~CConsortiaOfficial();
	int SetOfficial(int id,const char *name,int purview,short level);
	int SetOfficialID(int id);
	char* SetOfficialName(const char *name);
	int SetPurview(int purview);
	short SetLevel(int level);

	int GetOfficialID();
	char* GetOfficialName();
	int GetPurview();
	short GetLevel();
private:
	int			OfficialID;				//ְλ���
	char		OfficialName[16];		//ְλ����
	int			Purview;				//Ȩ��ֵ(��λ���)
	short		Level;					//ְλ����0��������ߣ��᳤��
};
#endif
