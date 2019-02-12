#include "ChargeTask.h"
#include "ProcChargeMsg.h"
#pragma warning(disable:4100)
#pragma warning(disable:4244)
#pragma warning(disable:4706)
CChargeTask::CChargeTask()
{
	m_bRuning = false;
	m_CurMonth = -1;
}

CChargeTask::~CChargeTask()
{
	ClearParamData();
	if (m_hFile)
	{
		fclose(m_hFile);
		m_hFile = 0;
	}
}

int CChargeTask::InitServer(LPServerParam param)
{
	//读取数据库
	m_SvrParam = *param;

	DB_Proxy m_DBProxy;
	if(!m_DBProxy.init(m_SvrParam.DBGIP,m_SvrParam.DBGPort,MDM_GP_MAP_CHARGE))
	{
		printf("Connect DBG(IP:%s,Port:%d) fault.\n",m_SvrParam.DBGIP,m_SvrParam.DBGPort);
		return -1;
	}
	//读取卡配置表
	if(GetCardParamTable(&m_DBProxy))
	{
		printf("GetCardParamTable fault\n");
	}
	//读取装备配置表
	if(ReadEquipAttribute())
	{
		printf("GetEquipParamTable fault.\n");
	}
	//读取加速参数表
	InitSpeedParam();
	//读取背包是否重叠参数
	InitPackStoreParam();
	readConstructionConfigure();
	//读取托管玩家
	m_DBProxy.fini();
//#endif
	return 0;
}

bool CChargeTask::startTask(unsigned int num_threads, 
		DATA_RECEIVER receiver_func, 
		EVENT_NOTIFY notify_func,
		TIMER_NOTIFY timer_func,
		SOCKET_DISCONNECT disconn_func,
		LOGIC_EXCEPTION exception_func)
{
	if(!init(m_SvrParam.AgentIP,m_SvrParam.AgentPort,m_SvrParam.LogSvrIP,m_SvrParam.LogSvrPort,MDM_GP_MAP_CHARGE,5,receiver_func,notify_func,timer_func,disconn_func,exception_func))
	{
		return false;
	}

	if(StartWebClient(m_SvrParam.WebIP,m_SvrParam.WebPort,m_SvrParam.Timeout,500) <=0)
	{
		fini();
		return false;
	}

	return startTask(num_threads);
}

bool CChargeTask::startTask(std::size_t nThreads)
{
	m_bRuning = true;
	if(!IceUtil::Task<char*,MAX_MSG_COUNT>::startTask(nThreads))
	{
		m_bRuning = false;
	}
	if(m_bRuning)
	{
		DB_Proxy m_DBProxy;
		if(m_DBProxy.init(m_SvrParam.DBGIP,m_SvrParam.DBGPort,MDM_GP_MAP_CHARGE))
		{
			//定时器
			GetAutoPlayerTable(&m_DBProxy);
			GetTimerTable(&m_DBProxy);
		}
		//读取托管玩家
		m_DBProxy.fini();
	}
	return m_bRuning;
}
void CChargeTask::stopTask()
{
	m_bRuning = false;
	/*
	char *msg;
	while (size())
	{
		getQueue(msg);
		exit(msg);
	}
	*/
	IceUtil::Task<char*,MAX_MSG_COUNT>::stopTask();
	StopWebClient();
	fini();
}
void CChargeTask::run()
{
	char *msg = 0;
	CProcChargeMsg m_ProcMsg(this,m_SvrParam.DBGIP,m_SvrParam.DBGPort);

	while(m_bRuning)
	{
		if(getQueue(msg))
		{
			m_ProcMsg.ProcMsg(msg);
			delete[] msg;
			msg = 0;
		}
	}
}
void CChargeTask::exit(char *msg)
{
	delete[] msg;
}

void CChargeTask::ClearParamData()
{
	//删除卡信息
	CardParam_Map_Iterator iter;
	LPCardParam pDelCardParam = 0;
	for(iter = m_CardParamList.begin();iter!=m_CardParamList.end();iter++)
	{
		if((pDelCardParam = iter->second))
		{
			delete pDelCardParam;
			pDelCardParam = 0;
		}
		
	}
	m_CardParamList.clear();
	//删除装备信息
	ClearEquipAttribute();

	//删除定时器参数列表
	IceUtil::Mutex::Lock lock(m_TimerParamMutex);
	TimerParam_Iterator paramIter;
	LPNotify_Timer pDelTimerParam;
	for(paramIter = m_TimerParamList.begin();paramIter != m_TimerParamList.end();paramIter++)
	{
		if((pDelTimerParam = paramIter->second))
		{
			delete pDelTimerParam;
			pDelTimerParam = 0;
		}
	}
	m_TimerParamList.clear();

	//加速参数表
	SpeedParam_Map_Iterator speedIter;
	LPSpeedParam_Node pDelSpeedParam = 0;
	for(speedIter = m_SpeedParamList.begin();speedIter != m_SpeedParamList.end();speedIter++)
	{
		if((pDelSpeedParam = speedIter->second))
		{
			delete pDelSpeedParam;
			pDelSpeedParam = 0;
		}
	}
	m_SpeedParamList.clear();

	//背包是否重叠参数
	PackStoreParam* pPackStoreParam = 0;
	PackStoreParam_Map_Iterator packiter;
	for (packiter = m_PackStoreParamMap.begin();packiter != m_PackStoreParamMap.end();packiter++)
	{
		pPackStoreParam = packiter->second;
		if (pPackStoreParam)
		{
			delete pPackStoreParam;
			pPackStoreParam =0;
		}
	}
	m_PackStoreParamMap.clear();
	if (m_hFile)
	{
		fclose(m_hFile);
		m_hFile = 0;
	}
}

void CChargeTask::AddCardParam(LPCardParam pCardParam)
{
	m_CardParamList[pCardParam->CardID] = pCardParam;
}

LPCardParam CChargeTask::GetCardParam(int cardid)
{
	CardParam_Map_Iterator iter;
	iter = m_CardParamList.find(cardid);
	if(iter == m_CardParamList.end())
	{
		return NULL;
	}
	return iter->second;
}

HeroEquipAttribute* CChargeTask::GetEquipAttribute(int itemid)
{
	HeroEquipAttribute_Map_Iterator iter = m_EquipAttributeList.find(itemid);
	if(iter == m_EquipAttributeList.end())
	{
		return NULL;;
	}
	return iter->second;
}

int CChargeTask::GetCardParamTable(DB_Proxy *pDBProxy)
{
	FILE *fp = 0;
	fp = fopen("configtable/CardConfig.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/CardConfig.csv失败。\n");
		return -1;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	LPCardParam pCardParamItem = 0;
	//
	int iPicture = 0;
	char efcetBuf[128]={0};
	int iOpenEnable = 0;
	int iKind = 0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pCardParamItem = new CardParam;
		memset(pCardParamItem,0,sizeof(CardParam));
		if(sscanf(buf,"%d,%[^,],%[^,],%d,%d,%d,%d,%d,%d,%d,%d",
			&pCardParamItem->CardID,&pCardParamItem->CardName,&efcetBuf,&iPicture,&pCardParamItem->Type,&iKind,
			&pCardParamItem->Price,&pCardParamItem->Value,&pCardParamItem->Time,&pCardParamItem->Postion,&iOpenEnable)<11)
		{
			delete pCardParamItem;
			pCardParamItem = 0;
			continue;
		}
		pCardParamItem->OpenEnabled = (iOpenEnable >0)?1:0;
		AddCardParam(pCardParamItem);
	}
	fclose(fp);
	return 0;
}

int CChargeTask::ReadEquipAttribute()
{
	FILE *fp = 0;
	fp = fopen("configtable/EquipAttribute.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/EquipAttribute.csv失败。\n");
		return -1;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	HeroEquipAttribute *pItem = 0;
	//m_EquipAttributeList
	int iSale = 0;
	int iPicture = 0;
	char jobs[32]={0};
	int jobIDs[6] = {0};
	int jobCount = 0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new HeroEquipAttribute;
		memset(pItem,0,sizeof(HeroEquipAttribute));
		if(sscanf(buf,"%d,%[^,],%d,%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->itemID,&pItem->name,&pItem->level,&iPicture,&jobs,&pItem->postion,
			&pItem->needStamina,&pItem->needPower,&pItem->needCelerity,&pItem->needWit,&pItem->stamina,&pItem->power,
			&pItem->celerity,&pItem->wit,&pItem->physicAttack,&pItem->physicRecovery,&pItem->magicAttack,&pItem->magicRecovery,
			&pItem->attackSpeed,&pItem->heroHP,&pItem->injureRate,&pItem->captureRate,&pItem->beInjureRate,&pItem->beCaptureRate,
			&pItem->forceRate,&pItem->setID,&pItem->skill,&pItem->skillLevel,&pItem->quality,&pItem->inlayNums,&iSale,&pItem->price)<31)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		pItem->isSale = (iSale >0)?1:0;
		if (jobs[0] == '0')
		{
			memset(pItem->jobIDs,1,6);
		}
		else
		{
			jobCount = sscanf(jobs,"%d|%d|%d|%d|%d|%d",&jobIDs[0],&jobIDs[1],&jobIDs[2],&jobIDs[3],&jobIDs[4],&jobIDs[5]);
			for (int i=0;i<jobCount;i++)
			{
				pItem->jobIDs[jobIDs[i]] = 1;
			}
		}
		m_EquipAttributeList[pItem->itemID] = pItem;
	}
	fclose(fp);
	return 0;
}

void CChargeTask::ClearEquipAttribute()
{
	HeroEquipAttribute_Map_Iterator iter;
	HeroEquipAttribute *pDelItem = 0;
	for (iter = m_EquipAttributeList.begin();iter != m_EquipAttributeList.end();iter++)
	{
		if((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_EquipAttributeList.clear();
}
int CChargeTask::GetAutoPlayerTable(DB_Proxy *pDBProxy)
{
	int iRet = -1;
	PlayerProduceState playerstate;
	playerstate.setmasterid(Auto_Card);
	PlayerProduceState *pPlayerList = 0;
	char *retbuf = 0;
	int count;
	if(pDBProxy->db_multi_select(playerstate,retbuf,count) == DB_Proxy::DB_SUCCEED)
	{
		iRet = 0;
		AutoPlayer_Ptr player_ptr;
		count = count /sizeof(PlayerProduceState);
		pPlayerList = (PlayerProduceState*)retbuf;
		for(int i=0;i<count;i++)
		{
			if(GetAutoPlayer(pPlayerList[i].playerid_,player_ptr))
			{
				player_ptr = new CAutoPlayer(pPlayerList[i].playerid_);
			}
			//增加托管列表,从文件中读取
			if(player_ptr.get())
			{
				player_ptr->ReadFile();
				AddAutoPlayer(player_ptr);
				//发送一个采集资源结束的消息，便于开始托管
				SendCollectComplete(pDBProxy,pPlayerList[i].playerid_);
#ifdef PRINT_HINTINFO
				printf("PlayerID:%d\n",pPlayerList[i].playerid_);
#endif
			}
		}
	}
	else
	{
	}
#ifdef PRINT_HINTINFO
	printf("==========================GetAutoPlayerTable finished=====================================\n");
#endif
	if(retbuf)
	{
		delete[] retbuf;
		retbuf = 0;
	}

	return iRet;
}
int CChargeTask::GetTimerTable(DB_Proxy *pDBProxy)
{
	int iRet = -1;
	ChargeSvrTimer timertb;
	timertb.setmasterid(MDM_GP_MAP_CHARGE);
	ChargeSvrTimer *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	LPChargeSvr_TimerParam pTimerParam = 0;
	if(pDBProxy->db_multi_select(timertb,retbuf,count) == DB_Proxy::DB_SUCCEED)
	{
		iRet = 0;
		count = count/sizeof(ChargeSvrTimer);
		pRetSet = (ChargeSvrTimer*)retbuf;
		unsigned long timer_val = 0;
		for(int i=0;i<count;i++)
		{
			pTimerParam = (LPChargeSvr_TimerParam)pRetSet[i].param_;

#ifdef PRINT_HINTINFO
			printf("Old Timer id = %d,time:%d\n",pTimerParam->TimerID,pRetSet[i].time_);
#endif
			//删除表中的记录
			timertb.setdetailid(pTimerParam->TimerID);
			if(pDBProxy->db_delete(timertb) != DB_Proxy::DB_SUCCEED)
			{
				printf("Delete timer param fault(%d)\n",pTimerParam->TimerID);
			}
			timer_val = (unsigned long)(pRetSet[i].time_ - IceUtil::Time::now().toSeconds());
			if(timer_val <=0)
				timer_val = 1;
			
			//增加一个定时器
			LPNotify_Timer nTimer = new Notify_Timer;
			nTimer->serialize(pTimerParam->PlayerID,pTimerParam->TimerType,0,0);
			nTimer->Param.TimerID = set_timer(timer_val,nTimer);
			if(nTimer->Param.TimerID >=0)
			{
				AddTimerNotifyMsg(pTimerParam->PlayerID,pTimerParam->TimerType,nTimer);

#ifdef PRINT_HINTINFO
				printf("Timer id = %d(%d)\n",nTimer->Param.TimerID,timer_val);
#endif
				//加入数据库
				ChargeSvrTimer svrTimer;
				svrTimer.setmasterid(MDM_GP_MAP_CHARGE);
				svrTimer.setdetailid(nTimer->Param.TimerID);
				svrTimer.time_ = pRetSet[i].time_;
				memcpy(svrTimer.param_,&(nTimer->Param),sizeof(ChargeSvr_TimerParam));
				if(pDBProxy->db_insert(svrTimer) != DB_Proxy::DB_SUCCEED)
				{
					printf("Insert timer param to db fault.\n");
				}
			}
			else
			{
				delete nTimer;
				nTimer = 0;
			}
		}
	}
#ifdef PRINT_HINTINFO
	printf("==========================GetTimerTable finished=====================================\n");
#endif
	if(retbuf)
	{
		delete[] retbuf;
		retbuf = 0;
	}
	
	return iRet;
}

void CChargeTask::SendCollectComplete(DB_Proxy *pDBProxy,int playerid)
{
	PlayerCity playercity;
	playercity.setmasterid(playerid);
	PlayerCity *pRetSet = 0;
	char *retbuf;
	int count = 0;
	if(pDBProxy->db_multi_select(playercity,retbuf,count) == DB_Proxy::DB_SUCCEED)
	{
		LPNotify_CollectResource_Response rep_msg;
		count = count/sizeof(PlayerCity);
		pRetSet = (PlayerCity*)retbuf;
		for(int i=0;i<count;i++)
		{
			rep_msg = new Notify_CollectResource_Response;
			rep_msg->serialize(playerid,pRetSet[i].mapid_,pRetSet[i].cityid_,0);
			putQueue((char*)rep_msg);
		}
	}		

	if(retbuf)
	{
		delete[] retbuf;
		retbuf = 0;
	}

}

int CChargeTask::InitSpeedParam()
{
	FILE *fp = 0;
	char buf[1024] = {0};
	fp = fopen("configtable/SpeedParam.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/SpeedParam.csv失败。\n");
		return 1;
	}

	fgets(buf,1024,fp);
	LPSpeedParam_Node pNewNode = 0;
	while(!feof(fp))
	{
		pNewNode = new SpeedParam_Node;
		fgets(buf,1024,fp);
		if(sscanf(buf,"%d,%d,%d,%d",&pNewNode->TypeID,&pNewNode->Value,
			&pNewNode->Price,&pNewNode->Times) <4)
		{
			delete pNewNode;
			pNewNode = 0;
			continue;
		}
#ifdef PRINT_HINTINFO
		printf("SpeedParam:Type:%d,Value:%d,Price:%d,Times:%d\n",pNewNode->TypeID,pNewNode->Value,pNewNode->Price,pNewNode->Times);
#endif
		m_SpeedParamList[pNewNode->TypeID*1000 + pNewNode->Value] = pNewNode;
	}
	fclose(fp);
	return 0;
}

int CChargeTask::InitPackStoreParam()
{
	FILE *fp = 0;
	char buf[1024] = {0};
	fp = fopen("configtable/PackStoreParam.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/PackStoreParam.csv失败。\n");
		return 1;
	}

	fgets(buf,1024,fp);
	PackStoreParam* pNewNode = 0;
	int iOverlap = 0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;

		pNewNode = new PackStoreParam;
		if(sscanf(buf,"%d,%d",&pNewNode->Postion,&iOverlap) <2)
		{
			delete pNewNode;
			pNewNode = 0;
			continue;
		}
		pNewNode->IsOverlap = iOverlap;
		m_PackStoreParamMap[pNewNode->Postion] = pNewNode;
	}
	fclose(fp);
	return 0;
}
int CChargeTask::ProcTimer(const void * param)
{
	return 0;
}


char *CChargeTask::SetLogFormat(LogType logtype,const char *strFormat)
{
	if(strFormat)
	{
		strncpy(m_strLogFormat[logtype],strFormat,SIZE_128);
	}
	return m_strLogFormat[logtype];
}
char *CChargeTask::GetLogFormat(LogType logtype)
{
	return m_strLogFormat[logtype];
}

char *CChargeTask::SetLogPath(const char *path)
{
	if(path)
	{
		strncpy(m_LogPath,path,SIZE_128);
	}
	return m_LogPath;
}
char *CChargeTask::GetLogPath()
{
	return m_LogPath;
}
int CChargeTask::AddTimerNotifyMsg(int playerid,int type,LPNotify_Timer param)
{
	IceUtil::Mutex::Lock lock(m_TimerParamMutex);
	long key = playerid * 1000 + type;
	m_TimerParamList[key] = param;
	return 0;
}

int CChargeTask::DelTimerNotifyMsg(int playerid,int type)
{
	IceUtil::Mutex::Lock lock(m_TimerParamMutex);
	long key = playerid * 1000 + type;
	m_TimerParamList.erase(key);
	return 0;
}
LPNotify_Timer CChargeTask::GetTimerID(int playerid,int type)
{
	LPNotify_Timer pTimerParam = 0;
	IceUtil::Mutex::Lock lock(m_TimerParamMutex);
	long key= playerid * 1000 + type;
	TimerParam_Iterator iter = m_TimerParamList.find(key);
	if(iter != m_TimerParamList.end())
	{
		pTimerParam = iter->second;
	}
	return pTimerParam;
}
int CChargeTask::AddAutoPlayer(AutoPlayer_Ptr &player_ptr)
{
	if(player_ptr.get() ==0)
		return -1;

	IceUtil::RWRecMutex::WLock wlock(m_AutoPlayerMutex);
	m_AutoPlayerList[player_ptr->GetPlayerID()] = player_ptr;
	return 0;
}
int CChargeTask::GetAutoPlayer(int playerid,AutoPlayer_Ptr &player_ptr)
{
	IceUtil::RWRecMutex::RLock rlock(m_AutoPlayerMutex);
	AutoPlayer_Iterator iter;
	iter = m_AutoPlayerList.find(playerid);
	if(iter == m_AutoPlayerList.end())
	{
		return -1;
	}
	player_ptr = iter->second;
	return 0;
}
int CChargeTask::DelAutoPlayer(int playerid)
{
	IceUtil::RWRecMutex::WLock wlock(m_AutoPlayerMutex);
	m_AutoPlayerList.erase(playerid);
	//删除对应的本地文件
	char filename[128] = {0};
	sprintf(filename,"%s\\%d.dat",SAVEDATAPATH,playerid);
	remove(filename);
	printf("Remove file name:%s\n",filename);
	return 0;
}
//0:成功；-1：资源不足；-2:该城市托管已结束;-3:未托管
int CChargeTask::GetConstruction(int playerid,int mapid,int cityid,AutoConstruction &autocons,DB_Proxy *pDBProxy,int &count)
{
	IceUtil::RWRecMutex::RLock rlock(m_AutoPlayerMutex);
	AutoPlayer_Iterator iter;
	iter = m_AutoPlayerList.find(playerid);
	if(iter == m_AutoPlayerList.end())
	{
		return -3;
	}
	return iter->second->GetConstruction(this,mapid,cityid,autocons,pDBProxy,count);
}
int CChargeTask::SetCheckResourceTime(int time)
{
	return (m_CheckTime = time);
}
int CChargeTask::GetCheckResourceTime()
{
	return m_CheckTime;
}
//Go点兑换参数
int CChargeTask::SetExchageRate(int type,int rate)
{
	if(type == GO2Money)
	{
		m_GO2Money_Rate = rate;
	}
	else if(type == Money2GO)
	{
		m_Money2GO_Rate = rate;
	}
	else
	{
		rate = 0;
	}
	return rate;
}
int CChargeTask::GetExchageRate(int type)
{
	int rate = 0;
	if(type == GO2Money)
	{
		rate = m_GO2Money_Rate;
	}
	else if(type == Money2GO)
	{
		rate = m_Money2GO_Rate;
	}
	return rate;
}
//加速参数
LPSpeedParam_Node CChargeTask::GetSpeedParam(int type,int value)
{
	LPSpeedParam_Node pParam_Node = 0;
	SpeedParam_Map_Iterator iter = m_SpeedParamList.find(type * 1000+value);
	if(iter != m_SpeedParamList.end())
	{
		pParam_Node = iter->second;
	}
	return pParam_Node;
}
int CChargeTask::ReconnectWebServer()
{
	/*IceUtil::Mutex::Lock lock(m_ConnectWebSvrMutex);
	StopWebClient();
	if(StartWebClient(m_SvrParam.WebIP,m_SvrParam.WebPort,m_SvrParam.Timeout) <=0)
		return -1;
	*/
	return 0;
}
//背包重叠参数
PackStoreParam *CChargeTask::GetPackStoreParam(int postion)
{
	PackStoreParam_Map_Iterator iter = m_PackStoreParamMap.find(postion);
	if (iter != m_PackStoreParamMap.end())
	{
		return iter->second;
	}
	return NULL;
}
void CChargeTask::WriteLog(DB_Proxy *pDBProxy,int svrid,int playerid,const char *format,...)
{
	IceUtil::Mutex::Lock wlock(m_LogMutex);
	IceUtil::DateTime curdate;
	IceUtil::Time::now().toDateTime(curdate);
	if (curdate.mon != m_CurMonth)
	{
		if (m_hFile)
		{
			fclose(m_hFile);
			m_hFile =0;
		}
		char filename[128] = {0};
		sprintf(filename,"%s\\consumenote%d%d.csv",GetLogPath(),curdate.year,curdate.mon);
		if((m_hFile = fopen(filename,"a+")) == 0)
		{
			m_CurMonth = -1;
			return ;
		}
		m_CurMonth = curdate.mon; 
	}

	PlayerOfficerTable officertb;
	officertb.setmasterid(playerid);
	officertb.setdetailid(playerid);
	pDBProxy->db_select(officertb);
	fprintf(m_hFile,"%d,%s,%d-%.2d-%.2d %.2d:%.2d:%.2d,%d,",
		playerid,officertb.officername_,
		curdate.year,curdate.mon,curdate.day,curdate.hour,curdate.min,curdate.sec,svrid);
	va_list arglist;
	va_start(arglist,format);
	vfprintf(m_hFile,format,arglist);
	va_end(arglist);
	fprintf(m_hFile,"\n");
	fflush(m_hFile);
}
//读取建筑物参数表
ConstructionConfigure * CChargeTask::getConstructionConfigure(int constructionid,int level)
{
	ConstructionConfigure* pConstrParam = NULL;
	ConstructionConfigure_Multimap_Iterator lowIter,upperIter;
	lowIter = m_ConstructionConfigureMultimap.lower_bound(constructionid);
	upperIter = m_ConstructionConfigureMultimap.upper_bound(constructionid);

	for(; lowIter != upperIter; lowIter++)
	{
		if (lowIter->second->level_ == level)
		{
			pConstrParam = lowIter->second;
			break;
		}
	}

	return pConstrParam;
}
//读取建筑配置表
void CChargeTask::readConstructionConfigure()
{
	FILE *fp = 0;
	fp = fopen("configtable/ConstructionConfigure.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/ConstructionConfigure.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	ConstructionConfigure* pItem = 0;
	//m_ConstructionConfigureMultimap
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new ConstructionConfigure;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->constructionid_,&pItem->level_,&pItem->completetimes_,&pItem->money_,&pItem->foodcount_,
			&pItem->woodcount_,&pItem->ironcount_,&pItem->skincount_,&pItem->science_,&pItem->modify_)<10)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_ConstructionConfigureMultimap.insert(ConstructionConfigure_MultiMap_Pair(pItem->constructionid_,pItem));
	}
	fclose(fp);
}
void CChargeTask::clearConstructionConfigure()
{
	ConstructionConfigure_Multimap_Iterator iter;
	ConstructionConfigure *pDelItem = 0;
	for (iter = m_ConstructionConfigureMultimap.begin();iter != m_ConstructionConfigureMultimap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_ConstructionConfigureMultimap.clear();
}
