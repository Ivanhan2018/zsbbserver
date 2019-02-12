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
	int			OfficialID;				//职位编号
	char		OfficialName[16];		//职位名称
	int			Purview;				//权限值(按位异或)
	short		Level;					//职位级别（0：级别最高，会长）
};
#endif
