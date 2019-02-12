#include "FixTaskTable.h"
#include "NetTask.h"

bool CFixTaskTable::LoadData(CNetTask *pNetTask,const char * addr, short port)
{
	UnLoadData();
	loadMailInfoParam();//读取邮件参数
	loadPackStoreParam();
	//InsertTask(101160);

	//装载EquipAttribute表(任务奖励--奖励装备引用)
	loadEquipAttribute();
	loadCardConfigTable();
	//静态任务表
	return loadTaskBaseTable(pNetTask);
}

void CFixTaskTable::UnLoadData()
{
	m_BaseTaskMap.clear();
	m_HortationMap.clear();
	m_EctypeTaslContainerMap.clear();
	clearMailInfoParam();
	clearCardConfig();
	clearPackStoreParam();
	clearEquipAttribute();
}
//读取任务参数
bool CFixTaskTable::loadTaskBaseTable(CNetTask *pNetTask)
{
	FILE *fp = 0;
	fp = fopen("configtable/TaskBaseTable.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/TaskBaseTable.csv失败。\n");
		return false;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	TaskBaseTable taskParam;
	tagEctypeTask eta;
	tagEctypeTask etb;
	int i1 =0,j1=0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		if(sscanf(buf,"%d,%[^,],%d,%d,%d,%d,%d,%d,%[^,],%[^,],%d,%d",
			&taskParam.taskid_,&taskParam.taskname_,&taskParam.tasktype_,&taskParam.tasklevel_,&taskParam.playerlevel_,
			&taskParam.playerrank_,&taskParam.taskitem_,&taskParam.prvtaskid_,&taskParam.bonus_,&taskParam.contest_,
			&taskParam.mailtype_,&taskParam.days)<11)
		{
			printf("任务参数配置有误：任务ID：%d\n",taskParam.taskid_);
			LOG_ERROR(("任务参数配置有误：任务ID：%d\n",taskParam.taskid_));
			continue;
		}
		m_BaseTaskMap[taskParam.taskid_] = taskParam;
		if (taskParam.tasktype_==ttFb)
		{
			//副本内容格式
			//副本ID:boss:npc:build:副本ID:boss:npc:build
			if (sscanf(taskParam.contest_,"%d:%d:%d:%d:%d:%d:%d:%d",&i1,&eta.boos,&eta.npc,&eta.build,\
				&j1,&etb.boos,&etb.npc,&etb.build)!=8)
			{
				printf("副本配置有误：任务ID：%d\n",taskParam.taskid_);
				LOG_ERROR(("副本配置有误：任务ID：%d\n",taskParam.taskid_));
				return false;  //副本任务数据错误
			}
			eta.taskid=taskParam.taskid_;
			eta.taskType = taskParam.tasktype_;
			etb.taskid=taskParam.taskid_;
			etb.taskType = taskParam.tasktype_;
			m_EctypeTaslContainerMap[i1]=eta;
			m_EctypeTaslContainerMap[j1]=etb;
		}
		else if(taskParam.tasktype_ == ttDayTask || taskParam.tasktype_ == ttConsortiaTask)
		{
			//日常任务,种类:类型:ItemID:Number:Level:联盟MAPID:部落MAPID
			DayTask_DetailInfo dayTaskInfo;
			if (sscanf(taskParam.contest_,"%d:%d:%d:%d:%d:%d:%d",
				&dayTaskInfo.kindType,&dayTaskInfo.itemType,&dayTaskInfo.itemID,&dayTaskInfo.number,
				&dayTaskInfo.itemLevel,&dayTaskInfo.maps[1],&dayTaskInfo.maps[2]) <7)
			{
				printf("日常任务配置有误：任务ID：%d\n",taskParam.taskid_);
				LOG_ERROR(("日常任务配置有误：任务ID：%d\n",taskParam.taskid_));
				return false;  //日常任务数据错误
			}
			dayTaskInfo.days = taskParam.days;
			dayTaskInfo.endTime = 0;
			dayTaskInfo.taskType = taskParam.tasktype_;
			dayTaskInfo.taskLevel = taskParam.tasklevel_;
			m_DayTaskMap[taskParam.taskid_] = dayTaskInfo;
			if (dayTaskInfo.kindType == TASKTARGET_BATTLEFIELD)//战场日常任务
			{
				eta.taskid=taskParam.taskid_;
				eta.taskType = taskParam.tasktype_;

				etb.taskid=taskParam.taskid_;
				etb.taskType = taskParam.tasktype_;

				m_EctypeTaslContainerMap[dayTaskInfo.maps[1]]=eta;
				if (dayTaskInfo.maps[2] != dayTaskInfo.maps[1])
				{
					m_EctypeTaslContainerMap[dayTaskInfo.maps[2]]=etb;
				}
			}
		}
	}
	fclose(fp);
	__int64 curtimes = IceUtil::Time::now().toSeconds();
	pNetTask->loadTaskEndTime(curtimes);
	pNetTask->reloadDayTaskTargetMap(curtimes);
	//解析任务奖励
	TaskBaseTable_Map_ConstIterator mit;
	char hdata[5][SIZE_128];
	HORTATION_Plus hDataList;

	for (mit = m_BaseTaskMap.begin();mit!=m_BaseTaskMap.end();++mit)
	{
		memset(hdata,0,5*SIZE_128);
		memset(&hDataList,0,sizeof(hDataList));
		int rcount = sscanf(mit->second.bonus_,"%[^;];%[^;];%[^;];%[^;];%s",hdata[0],hdata[1],hdata[2],hdata[3],hdata[4]);
		int itype = 0;
		for (int i=0;i<rcount;i++)
		{
			if (sscanf(hdata[i],"%d:",&itype) > 0) 
			{
				switch (itype)
				{
				case 0:		//道具
					{
						int counts=sscanf(hdata[i],"%d:%d:%d:%d:%d:%d:%d:%d:%d:%d",&itype,&hDataList.h0.hlist[0].hid,\
							&hDataList.h0.hlist[0].htype,&hDataList.h0.hlist[0].num,&hDataList.h0.hlist[1].hid,&hDataList.h0.hlist[1].htype, \
							&hDataList.h0.hlist[1].num,&hDataList.h0.hlist[2].hid,&hDataList.h0.hlist[2].htype,&hDataList.h0.hlist[2].num);
						if (counts>=4) 
						{
							hDataList.b0=true;
							hDataList.h0.count=(counts-1)/3;
						}
						break;
					}
				case 1:		//装备
					{
						int counts = sscanf(hdata[i],"%d:%d:%d:%d:%d:%d:%d",\
							&itype,&hDataList.h1.hlist[0].hid,&hDataList.h1.hlist[0].num,\
							&hDataList.h1.hlist[1].hid,&hDataList.h1.hlist[1].num,\
							&hDataList.h1.hlist[2].hid,&hDataList.h1.hlist[2].num);
						if (counts>=3) 
						{
							hDataList.b1=true;
							hDataList.h1.count=(counts-1)/2;

						}
						break;
					}
				case 2:		//荣誉点，经验值
					{
						hDataList.b2 = (sscanf(hdata[i],"%d:%d:%d",&itype,&hDataList.h2.i1,&hDataList.h2.i2)>=3);
						break;
					}
				case 3:		//金钱
					{
						hDataList.b3 = (sscanf(hdata[i],"%d:%d",&itype,	&hDataList.money)>=2);
						break;
					}
				case 4:		//粮食，木材，铁，皮料
					{
						hDataList.b4 = (sscanf(hdata[i],"%d:%d:%d:%d:%d",&itype,&hDataList.h4.nums[0],\
							&hDataList.h4.nums[1],&hDataList.h4.nums[2],&hDataList.h4.nums[3])>=5);
						break;
					}
					//5：奖励卡
				case 5:
					{
						hDataList.CardCount = sscanf(hdata[i],"%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d",&itype,
							&hDataList.CardItem[0].CardItemID,&hDataList.CardItem[0].Count,
							&hDataList.CardItem[1].CardItemID,&hDataList.CardItem[1].Count,
							&hDataList.CardItem[2].CardItemID,&hDataList.CardItem[2].Count,
							&hDataList.CardItem[3].CardItemID,&hDataList.CardItem[3].Count,
							&hDataList.CardItem[4].CardItemID,&hDataList.CardItem[4].Count);
						hDataList.CardCount = (hDataList.CardCount -1)/2;
						if (hDataList.CardCount <0)
						{
							hDataList.CardCount = 0;
						}
					}
					break;
					//6：奖励GO点
				case 6:
					{
						hDataList.GoDian = 0;
						if (sscanf(hdata[i],"%d:%d",&itype,&hDataList.GoDian) <2)
						{
							hDataList.GoDian = 0;
						}
					}
					break;
				case 7://奖励士兵
					{
						if (sscanf(hdata[i],"%d:%d",&itype,&hDataList.SoldierCount) <2)
						{
							hDataList.SoldierCount = 0;
						}
					}
					break;
				case 8://奖励技能点
					{
						if (sscanf(hdata[i],"%d:%d",&itype,&hDataList.TechDot) <2)
						{
							hDataList.TechDot = 0;
						}
					}
					break;
				default:
					{
						break;
					}
				}
			}
		}
		m_HortationMap[mit->first] = hDataList;
	}
	return true;
}


LPMailInfoParam_Item CFixTaskTable::getMailInfoParam(int countryid,int type)
{
	int key = type * 10 + countryid;
	MailInfoParam_Map_Iterator iter;
	iter = m_MailInfoParamMap.find(key);
	if(iter == m_MailInfoParamMap.end())
		return NULL;
	return iter->second;
}
//读取邮件参数表
void CFixTaskTable::loadMailInfoParam()
{
	FILE *fp = 0;
	fp = fopen("configtable/MailInfoParam.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/MailInfoParam.csv失败。\n");
		return;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);
	char content[256] = {0};

	LPMailInfoParam_Item pItem = 0;
	int iLen = 0;
	//m_QuantityByRankList
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new MailInfoParam_Item;
		if(sscanf(buf,"%d,%d,%[^,],%[^,]",&pItem->CounryID,&pItem->Type,&pItem->Title,&content)<4)
		{
			delete pItem;
			pItem = 0;
			continue;
		}

		iLen = strlen(content) -1;
		if(content[iLen] == '\n')
			content[iLen] = 0;
		StrReplace(content,pItem->Content,"\\n","\r\n");
		m_MailInfoParamMap[pItem->Type * 10 + pItem->CounryID] = pItem;
		//printf("countryid:%d,type:%d,title:%s,content:%s\n",pItem->CounryID,pItem->Type,pItem->Title,pItem->Content);
	}
	fclose(fp);
}
void CFixTaskTable::clearMailInfoParam()
{
	MailInfoParam_Map_Iterator iter;
	LPMailInfoParam_Item pDelItem = 0;
	for(iter = m_MailInfoParamMap.begin();iter != m_MailInfoParamMap.end();iter++)
	{
		if(pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_MailInfoParamMap.clear();
}
LPCardParam CFixTaskTable::getCardConfig(int itemid)
{
	CardParam_Map_Iterator iter;
	//m_CardConfigMap
	iter = m_CardConfigMap.find(itemid);
	if (iter == m_CardConfigMap.end())
	{
		return NULL;
	}
	return iter->second;
}
int CFixTaskTable::loadCardConfigTable()
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
		m_CardConfigMap[pCardParamItem->CardID] = pCardParamItem;
	}
	fclose(fp);
	return 0;
}
int CFixTaskTable::clearCardConfig()
{
	CardParam_Map_Iterator iter;
	LPCardParam pDelItem = 0;
	for (iter = m_CardConfigMap.begin();iter != m_CardConfigMap.end();iter++)
	{
		if (pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_CardConfigMap.clear();
	return 0;
}
PackStoreParam *CFixTaskTable::getPackStoreParam(int postion)
{
	PackStoreParam_Map_Iterator iter = m_PackStoreParamMap.find(postion);
	if (iter != m_PackStoreParamMap.end())
	{
		return iter->second;
	}
	return NULL;
}
int CFixTaskTable::loadPackStoreParam()
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
void CFixTaskTable::clearPackStoreParam()
{
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
}

HeroEquipAttribute *CFixTaskTable::getHeroEquipAttribute(int itemid)
{
	HeroEquipAttribute_Map_Iterator iter = m_HeroEquipAttributeList.find(itemid);
	if (iter == m_HeroEquipAttributeList.end())
	{
		return NULL;
	}
	return iter->second;
}
//读取装备属性表
void CFixTaskTable::loadEquipAttribute()
{
	FILE *fp = 0;
	fp = fopen("configtable/EquipAttribute.csv","r");
	if(fp == NULL)
	{
		printf("打开configtable/EquipAttribute.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	HeroEquipAttribute *pItem = 0;
	//m_HeroEquipAttributeList
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
		m_HeroEquipAttributeList[pItem->itemID] = pItem;
	}
	fclose(fp);
}
void CFixTaskTable::clearEquipAttribute()
{
	HeroEquipAttribute_Map_Iterator iter;
	HeroEquipAttribute *pDelItem = 0;
	for (iter = m_HeroEquipAttributeList.begin();iter != m_HeroEquipAttributeList.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_HeroEquipAttributeList.clear();
}
