#include "Consortia.h"
#include <stdio.h>

CConsortia::CConsortia()
{	
	init();
}
CConsortia::CConsortia(int id,const char *name,int mapid,int cityid,int playerid,__int64 time,int countryid)
{
	init();

	ConsortiaID = id;
	if(name)
	{
		strncpy(Name,name,SIZE_32);
	}
	MapID = mapid;
	CityID = cityid;
	ChairmanID = playerid;
	CreateTime = time;
	CountryID = countryid;
}

void CConsortia::init()
{
	Count = 0;
	OfficalCount = 0;
	FrtNoFullTime = 0;
	MapID = 0;
	CityID = 0;
	ConsortiaID = 0;
	memset(Name,0,SIZE_32);
	CreateTime = 0;

	ChairmanID = 0;
	TechID = 0;
	Level = 0;
	memset(Placard, 0,SIZE_256);
	memset(Tenet, 0,SIZE_128);
	FaceID = 0;
	memset(FaceBuffer,0,MAX_LOGO_SIZE);
	CountryID = 0;

	MaxUserNum = 30;
	Money = 0;
	Exp = 0;
	Credit = 0;
	SignID = 0;
	memset(ShortName,0,5);
}
CConsortia::~CConsortia()
{
	ConUserList.clear();
	ConUnionList.clear();
	ConBankInfo.clear();
	ConFightList.clear();
}

//公会操作
int CConsortia::SetConsortiaID(int id)
{
	return (ConsortiaID = id);
}

char* CConsortia::SetName(const char *name)
{
	if(name)
	{
		strncpy(Name,name,SIZE_32);
	}
	return Name;
}

int CConsortia::SetMapID(int mapid)
{
	return (MapID = mapid);
}

int CConsortia::SetCityID(int cityid)
{
	return (CityID = cityid);
}

int CConsortia::SetChairmanID(int playerid)
{
	return (ChairmanID = playerid);
}

int CConsortia::SetUserCount(int count)
{
	return (Count = count);
}

int CConsortia::SetTeachID(int teachid)
{
	return (TechID = teachid);
}

short CConsortia::SetLevel(short level)
{
	return (Level = level);
}

int CConsortia::SetFaceID(int faceid)
{
	return (FaceID = faceid);
}

char* CConsortia::SetFaceBuff(const char *buffer)
{
	if(buffer)
	{
		memcpy(FaceBuffer,buffer,MAX_LOGO_SIZE);
	}
	return FaceBuffer;
}

char* CConsortia::SetPlacard(const char *buffer)
{
	if(buffer)
	{
		strncpy(Placard,buffer,256);
	}
	return Placard;
}

char* CConsortia::SetTenet(const char *buffer)
{
	if(buffer)
	{
		strncpy(Tenet,buffer,SIZE_128);
	}
	else
	{
		Tenet[0] = 0;
	}
	return Tenet;
}

__int64 CConsortia::SetCreateTime(__int64 time)
{
	return (CreateTime = time);
}
__int64 CConsortia::SetFrtNoFullTime(__int64 time)
{
	return (FrtNoFullTime = time);
}
int CConsortia::SetCountryID(int countryid)
{
	return (CountryID = countryid);
}
int CConsortia::GetConsortiaID()
{
	return ConsortiaID;
}

char* CConsortia::GetName()
{
	return Name;
}

int CConsortia::GetMapID()
{
	return MapID;
}

int CConsortia::GetCityID()
{
	return CityID;
}

int CConsortia::GetChairmanID()
{
	return ChairmanID;
}

int CConsortia::GetUserCount()
{
	return Count;
}
int CConsortia::GetTeachID()
{
	return TechID;
}

short CConsortia::GetLevel()
{
	return Level;
}

int CConsortia::GetFaceID()
{
	return FaceID;
}

char* CConsortia::GetFaceBuff()
{
	return FaceBuffer;
}

char* CConsortia::GetPlacard()
{
	return Placard;
}

char* CConsortia::GetTenet()
{
	return Tenet;
}

__int64 CConsortia::GetCreateTime()
{
	return CreateTime;
}

__int64 CConsortia::GetFrtNoFullTime()
{
	return FrtNoFullTime;
}
int CConsortia::GetCountryID()
{
	return CountryID;
}
/*成员操作
*/
int CConsortia::AddUser(int playerid,int jobid,int purview,int type)
{
	ConsortiaUser_Ptr user_ptr(new CConsortiaUser(playerid,ConsortiaID,jobid,purview,type));
	ConUserList[playerid] = user_ptr;
	++Count;
	return 0;
}

int CConsortia::DelUser(int playerid)
{
	ConUserList.erase(playerid);
	--Count;
	return 0;
}

/**************************************************************************
*说明：添加一个成员
*输入参数：	user_ptr：成员对象
*返回值：0：成功
**************************************************************************/
int CConsortia::AddUser(ConsortiaUser_Ptr &user_ptr)
{
//	m_RWUserMutex.writeLock();
	ConUserList[user_ptr->GetPlayerID()] = user_ptr;
	if (user_ptr->GetConsortiaJob() == OFFICIAL_JOB)
	{
		OfficalCount++;
	}
	++Count;
	//m_RWUserMutex.unlock();
	return 0;
}

int CConsortia::GetUserJob(int playerid)
{
	int job = 0;
	//m_RWUserMutex.readLock();
	ConsortiaUser_Iter iter = ConUserList.find(playerid);//查找成员
	if(iter != ConUserList.end())
	{
		job = iter->second->GetConsortiaJob();
	}
	//m_RWUserMutex.unlock();
	return job;
}
int CConsortia::SetUserJob(int playerid,int jobid)
{
	ConsortiaUser_Iter iter = ConUserList.find(playerid);//查找成员
	if(iter != ConUserList.end())
	{
		iter->second->SetConsortiaJob(jobid);
		return 0;
	}
	return 1;
}
int CConsortia::GetUserPurview(int playerid)
{
	int purview = 0;
	//m_RWUserMutex.readLock();
	ConsortiaUser_Iter iter = ConUserList.find(playerid);//查找成员
	if(iter != ConUserList.end())
	{
		purview = iter->second->GetPurview();
	}
	//m_RWUserMutex.unlock();
	return purview;
}
int CConsortia::SetUserPurview(int playerid,int purview)
{
	ConsortiaUser_Iter iter = ConUserList.find(playerid);//查找成员
	if(iter != ConUserList.end())
	{
		iter->second->SetPurview(purview);
		return 0;
	}
	return 1;
}
int CConsortia::GetUserType(int playerid)
{
	int type = 0;
	//m_RWUserMutex.readLock();
	ConsortiaUser_Iter iter = ConUserList.find(playerid);//查找成员
	if(iter != ConUserList.end())
	{
		type = iter->second->GetType();
	}
	//m_RWUserMutex.unlock();
	return type;
}

int CConsortia::GetUserInfo(int playerid,int &jobid,int &purview,int &type)
{
	int iRet = 1;
	//m_RWUserMutex.readLock();
	ConsortiaUser_Iter iter = ConUserList.find(playerid);//查找成员
	if(iter != ConUserList.end())
	{
		iRet = 0;
		jobid = iter->second->GetConsortiaJob();
		purview = iter->second->GetPurview();
		type = iter->second->GetType();
	}
	//m_RWUserMutex.unlock();
	return iRet;
}

CConsortiaUser* CConsortia::getConsortiaUser(int playerid)
{
	ConsortiaUser_Iter iter = ConUserList.find(playerid);//查找成员
	if (iter == ConUserList.end())
	{
		return NULL;
	}
	return iter->second.get();
}
int CConsortia::SetUserInfo(int playerid,int jobid,int purview,int type)
{
	ConsortiaUser_Iter iter = ConUserList.find(playerid);//查找成员
	if(iter != ConUserList.end())
	{
		if (iter->second->GetConsortiaJob() == OFFICIAL_JOB && jobid != OFFICIAL_JOB)
		{
			OfficalCount--;
		}
		else if(iter->second->GetConsortiaJob() != OFFICIAL_JOB && jobid == OFFICIAL_JOB)
		{
			OfficalCount++;
		}
		iter->second->SetConsortiaJob(jobid);
		iter->second->SetPurview(purview);
		iter->second->SetType(type);
	}
	return 1;
}
/*同盟操作*/
int CConsortia::AddUnion(int id,int conid)
{
	ConsortiaUnion_Ptr union_ptr(new CConsortiaUnion(id,ConsortiaID,conid));
	AddUnion(union_ptr);
	return 0;
}

int CConsortia::AddUnion(ConsortiaUnion_Ptr &union_ptr)
{
	//m_RWConUnionMutex.writeLock();
	if(union_ptr->GetConsortia1ID() == ConsortiaID)
	{
		ConUnionList[union_ptr->GetConsortia2ID()] = union_ptr;
	}
	else if(union_ptr->GetConsortia2ID() == ConsortiaID)
	{
		ConUnionList[union_ptr->GetConsortia1ID()] = union_ptr;
	}	
	else
	{
		return 1;
	}
	//m_RWConUnionMutex.unlock();
	return 0;
}

int CConsortia::DelUnion(int conid)
{
	//m_RWConUnionMutex.writeLock();	
	ConUnionList.erase(conid);
	//m_RWConUnionMutex.unlock();
	return 0;
}

bool CConsortia::IsUnionConsortia(int consortiaid)
{
	bool bIsUnion = false;
	//m_RWConUnionMutex.readLock();
	bIsUnion = (ConUnionList.find(consortiaid) != ConUnionList.end())?true:false;
	//m_RWConUnionMutex.unlock();
	return bIsUnion;
}

/*银行操作*/
int CConsortia::AddBankItem(int itemid,int itemcount)
{
	ConsortiaBank_Ptr bank_ptr(new CConsortiaBank(ConsortiaID,itemid,itemcount));
	//m_RWConBankMutex.writeLock();
	ConBankInfo[itemid] = bank_ptr;
	//m_RWConBankMutex.unlock();
	return 0;
}
int CConsortia::DelBankItem(int itemid)
{
	//m_RWConBankMutex.writeLock();
	ConBankInfo.erase(itemid);
	//m_RWConBankMutex.unlock();
	return 0;
}

int  CConsortia::SetItemCount(int itemid,int itemcount)
{
	//m_RWConBankMutex.writeLock();
	ConsortiaBank_Iter iter = ConBankInfo.find(itemid);
	if(iter != ConBankInfo.end())
	{
		iter->second->SetItemCount(itemcount);
	}
	else
	{
		AddBankItem(itemid,itemcount);
	}
	//m_RWConBankMutex.unlock();
	return 0;
}

int CConsortia::GetItemCount(int itemid)
{
	int count = 0;
	//m_RWConBankMutex.readLock();
	ConsortiaBank_Iter iter = ConBankInfo.find(itemid);
	if(iter != ConBankInfo.end())
	{
		count = iter->second->GetItemCount();
	}
	//m_RWConBankMutex.unlock();
	return count;
}

int CConsortia::GetBankCount()
{
	return (int)ConBankInfo.size();
}

int CConsortia::GetBankList(LPBank_ItemInfo itemlist,int start,int &count)
{
	ConsortiaBank_Iter iter;
	int index = 0;
	int pass = 0;
	for(iter = ConBankInfo.begin();iter != ConBankInfo.end();iter++)
	{
		if((pass++) <start)
		{
			continue;
		}
		itemlist[index].ItemID = iter->second->GetItemID();
		itemlist[index].ItemCount = iter->second->GetItemCount();
		if((++index) >= count)
		{
			break;
		}
	}
	count = index;
	return count;
}

/*约战*/
int CConsortia::AddFight(int id,int conid,int mapid,int cityid,__int64 time)
{
	ConsortiaFight_Ptr fight_ptr(new CConsortiaFight(id,ConsortiaID,conid,mapid,cityid,time));
	AddFight(fight_ptr);
	return 0;
}

int CConsortia::AddFight(ConsortiaFight_Ptr &fight_ptr)
{
	//m_RWConFightMutex.writeLock();
	if(fight_ptr->GetConsortia1ID() == ConsortiaID)
	{
		ConFightList[fight_ptr->GetConsortia2ID()] = fight_ptr;
	}
	else if(fight_ptr->GetConsortia2ID() == ConsortiaID)
	{
		ConFightList[fight_ptr->GetConsortia1ID()] = fight_ptr;
	}	
	//m_RWConFightMutex.unlock();
	return 0;
}

int CConsortia::DelFight(int conid)
{
	//m_RWConFightMutex.writeLock();
	ConFightList.erase(conid);
	//m_RWConFightMutex.unlock();
	return 0;
}

bool CConsortia::IsFightConsortia(int consortiaid)
{
	bool bIsFight = false;
	//m_RWConFightMutex.readLock();
	bIsFight = (ConFightList.find(consortiaid)!=ConFightList.end())?true:false;
	//m_RWConFightMutex.unlock();
	return bIsFight;
}

/**************************************************************************
*说明：获取公会成员列表
*输入参数：userlist：成员列表
           start:从第几个开始返回；
		   count:输出输入参数，输入为：最大的个数，返回实际的个数
*返回值：公会资源编号
**************************************************************************/
int CConsortia::GetUserList(LPCon_UserInfo userlist,int start,int &count)
{
	/*只获取了成员编号，其他信息另外实时获取
	*/
	ConsortiaUser_Iter iter;
	int index = 0;
	int pass = 0;
	for(iter = ConUserList.begin();iter != ConUserList.end();iter++)
	{
		if((pass++) <start)
		{
			continue;
		}
		userlist[index].PlayerID = iter->second->GetPlayerID();
		userlist[index].ConJob = iter->second->GetConsortiaJob();//职位编号
		userlist[index].LegionTitle = iter->second->getLegionTitle();//军团称号
		userlist[index].Contribute = iter->second->getContribute();//贡献度
		userlist[index].Purview = iter->second->GetPurview();		//权限
		if((++index) >= count)
		{
			break;
		}
	}
	count = index;
	return count;
}
int CConsortia::setMaxUserNum(int num)
{
	return (MaxUserNum = num);
}
int CConsortia::getMaxUserNum()
{
	return MaxUserNum;
}
int CConsortia::setMoney(int money)
{
	return (Money = money);
}
int CConsortia::getMoney()
{
	return Money;
}
int CConsortia::addMoney(int money)
{
	Money += money;
	return Money;
}
int CConsortia::setExp(int exp)
{
	return (Exp = exp);
}
int CConsortia::getExp()
{
	return Exp;
}
int CConsortia::addExp(int exp)
{
	Exp += exp;
	return Exp;
}


int CConsortia::setHonors(int val)
{
	return (Credit = val);
}
int CConsortia::getHonors()
{
	return Credit;
}
int CConsortia::addHonors(int val)
{
	Credit += val;
	return Credit;
}

int CConsortia::setSignID(int id)
{
	return (SignID = id);
}
int CConsortia::getSignID()
{
	return SignID;
}

void CConsortia::setShortName(const char *name)
{
	if (name)
	{
		strncpy(ShortName,name,4);
	}
}
const char *CConsortia::getShortName()
{
	return ShortName;
}
bool CConsortia::isRoyalConsortia()
{
	return false;
}
//获取军团占据的城市所在郡
int CConsortia::getMapID()
{
	return 0;
}
int CConsortia::addOfficalCount(int num)
{
	return (OfficalCount += num);
}
int CConsortia::getOfficalCount()//获取官员数量
{
	return OfficalCount;
};

//向有批准加入军团权限的会员发送邮件
void CConsortia::sendEmail2Offical(const int playerid,const char *name,Notify_Send_Mail *email)
{
	ConsortiaUser_Iter iter;
	for (iter = ConUserList.begin();iter != ConUserList.end();++iter)
	{
		if ((iter->second->GetPurview() &PURVIEW_AGRESS_JOIN) == PURVIEW_AGRESS_JOIN)
		{
			email->ulRecieverPlayerID = iter->second->GetPlayerID();
			email->serialize();
			event_notify(MDM_GP_MAP_CHAT,(char*)email,email->length);	
		}
	}
}
//修改到数据库
int CConsortia::update2DBG(DB_Proxy *pDBProxy)
{
	ConsortiaInfo consInfo;
	consInfo.setid(ConsortiaID);
	strcpy(consInfo.name_,Name);
	consInfo.mapid_ = MapID;
	consInfo.cityid_ = CityID;
	consInfo.level_ = Level;
	consInfo.chairmanid_ = ChairmanID;
	consInfo.techid_ = TechID;
	consInfo.count_ = Count;
	consInfo.faceid_ = FaceID;
	strcpy(consInfo.placard_,Placard);
	strcpy(consInfo.tenet_,Tenet);

	consInfo.createtime_ = CreateTime;
	consInfo.maxusernum_ = MaxUserNum;
	consInfo.money_ = Money;
	consInfo.exp_ = Exp;
	consInfo.honors_ = Credit;
	consInfo.signid_ = SignID;
	strcpy(consInfo.shortname_,ShortName);
	return pDBProxy->db_update(consInfo,consInfo);
}
