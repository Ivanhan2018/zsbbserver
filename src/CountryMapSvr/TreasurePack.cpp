#include "TreasurePack.h"
#include <string.h>
#include <stdlib.h>
#include "LoadDBinfo.h"
CTreasurePack::CTreasurePack(CLoadDBInfo *loadDBInfo,int id,int level):
loadDBInfo_(loadDBInfo),ID(id),level_(level)
{
	memset(&SureFlopParam,0,sizeof(FlopParam));
	memset(Randomflop,0,sizeof(FlopParam)*100);
	memset(Equipflop,0,sizeof(FlopParam)*100);
}
CTreasurePack::~CTreasurePack()
{
}
void CTreasurePack::SetID(int id)
{
	ID = id;
}
void CTreasurePack::InitParam(const char *buf,int type)
{
	char strBuf[64] = {0};
	char TmpBuf[16] = {0};
	char TmpBuf2[16] = {0};
	if(type == SUREFLOP)
	{//必掉的只有一个
		if(GetFieldValue(buf,0,strBuf,':') ==0)
		{
			SureFlopParam.ItemID = atoi(strBuf);
		}
		else
		{
			SureFlopParam.ItemID = 0;
		}
		if(GetFieldValue(buf,1,strBuf,':') ==0)
		{
			SureFlopParam.Rate = atoi(strBuf);
		}
		else
		{
			SureFlopParam.Rate = 100;
		}
		if(GetFieldValue(buf,2,strBuf,':') ==0)
		{
			if(GetFieldValue(strBuf,0,TmpBuf,'-') ==0)
			{
				SureFlopParam.MinCount = atoi(TmpBuf);
			}
			else
			{
				SureFlopParam.MinCount = 0;
			}
			if(GetFieldValue(strBuf,1,TmpBuf,'-') ==0)
			{
				SureFlopParam.MaxCount = atoi(TmpBuf);
			}
			else
			{
				SureFlopParam.MaxCount = SureFlopParam.MinCount ;
			}
			if(SureFlopParam.MaxCount <SureFlopParam.MinCount)
			{
				SureFlopParam.MaxCount = SureFlopParam.MinCount;
			}
		}
		else
		{
			SureFlopParam.MinCount = SureFlopParam.MaxCount = 0;
		}
	}
	else //if(type == RANDOMFLOP)
	{//随机掉，
		int index = 0;
		FlopParam tmpParam;
		while(GetFieldValue(buf,index++,strBuf,';') == 0)
		{
			//ItemID
			tmpParam.ItemID  = 0;
			if(GetFieldValue(strBuf,0,TmpBuf,':') ==0)
			{
				tmpParam.ItemID = atoi(TmpBuf);
			}
			if(tmpParam.ItemID == 0)
				continue;
			//Rate
			if(GetFieldValue(strBuf,1,TmpBuf,':') ==0)
			{
				tmpParam.Rate = atoi(TmpBuf);
			}
			else
			{
				tmpParam.Rate = 0;
			}
			//Count
			if(GetFieldValue(strBuf,2,TmpBuf,':') ==0)
			{
				if(GetFieldValue(TmpBuf,0,TmpBuf2,'-') ==0)
				{
					tmpParam.MinCount = atoi(TmpBuf2);
				}
				else
				{
					tmpParam.MinCount = 1;
				}
				if(GetFieldValue(TmpBuf,1,TmpBuf2,'-') ==0)
				{
					tmpParam.MaxCount = atoi(TmpBuf2);
				}
				else
				{
					tmpParam.MaxCount = tmpParam.MinCount;
				}
				if(tmpParam.MaxCount <tmpParam.MinCount)
				{
					tmpParam.MaxCount = tmpParam.MinCount;
				}
			}
			else
			{
				tmpParam.MinCount = tmpParam.MaxCount = 1;
			}

			if(type ==RANDOMFLOP)
			{
				InitRandom(&tmpParam,Randomflop);
			}
			else if(type == EQUIPFLOP)
			{
				InitRandom(&tmpParam,Equipflop);
			}
		}
	}
	//else if(type == EQUIPFLOP)
}
void CTreasurePack::GetItems(LPTreasurePackItem sureflop,LPTreasurePackItem randomflop,LPTreasurePackItem equipflop)
{
	//srand(IceUtil::Time::now().toSeconds());
	int index = 0;
	//必掉落
	sureflop->ItemID = SureFlopParam.ItemID;
	if(SureFlopParam.MinCount >= SureFlopParam.MaxCount)
	{
		sureflop->Count = SureFlopParam.MinCount;
	}
	else
	{
		sureflop->Count = rand()%(SureFlopParam.MaxCount - SureFlopParam.MinCount) + SureFlopParam.MinCount;
	}
	//特殊资源Randomflop
	index = rand()%100;
	if(Randomflop[index].ItemID ==0)
	{
		randomflop->ItemID = 0;
		randomflop->Count = 0;
	}
	else
	{//随机数量
		randomflop->ItemID = Randomflop[index].ItemID;
		if(Randomflop[index].MinCount >= Randomflop[index].MaxCount)
		{
			randomflop->Count = Randomflop[index].MinCount;
		}
		else
		{
			randomflop->Count = rand()%(Randomflop[index].MaxCount - Randomflop[index].MinCount) + Randomflop[index].MinCount;
		}
	}
	//装备类Equipflop
	equipflop->ItemID = 0;
	equipflop->Count = 0;
	int scale = 0;
	int postion = 0;
	scale = loadDBInfo_->getEquipScaleByProbability();
	if (scale <=0)
	{
		return ;
	}
	postion = loadDBInfo_->getEquipPostionByProbability();
	if (postion <=0)
	{
		return ;
	}
	HeroEquipAttribute *heroEquipAttri = loadDBInfo_->getOneEquip(scale,postion,level_);
	if (heroEquipAttri)
	{
		equipflop->ItemID = heroEquipAttri->itemID;
		equipflop->Count = 1;
	}
	/*装备类Equipflop
	index = rand()%100;
	if(Equipflop[index].ItemID ==0)
	{
		equipflop->ItemID = 0;
		equipflop->Count = 0;
	}
	else
	{//随机数量
		equipflop->ItemID = Equipflop[index].ItemID;
		if(Equipflop[index].MinCount >= Equipflop[index].MaxCount)
		{
			equipflop->Count = Equipflop[index].MinCount;
		}
		else
		{
			equipflop->Count = rand()%(Equipflop[index].MaxCount - Equipflop[index].MinCount) + Equipflop[index].MinCount;
		}
	}
	*/
}

int CTreasurePack::GetFieldValue(const char *srcbuf,int index,char *destbuf,char sepchar)
{
	char *pPrePoint = (char*)srcbuf;
	char *pNextPoint = 0;
	int i = 0;
	while(i <index)
	{
		if((pNextPoint = strchr(pPrePoint,sepchar)) ==0)
			break;
		i++;
		pPrePoint = pNextPoint + 1;
	}
	if(i < index)
		return -1;

	pNextPoint = strchr(pPrePoint,sepchar);
	if(pNextPoint)
	{
		int len = int(pNextPoint - pPrePoint);
		strncpy(destbuf,pPrePoint,len);
		destbuf[len] = 0;
	}
	else
	{
		strcpy(destbuf,pPrePoint);
	}
	return 0;
}

//随机放入数组中，5秒超时返回
int CTreasurePack::InitRandom(LPFlopParam param,LPFlopParam paramArray)
{
	int count = 0;
	//srand((unsigned int)IceUtil::Time::now().toSeconds());
	int index = 0;
	long start = GetTickCount();
	long end = start;
	while(count <param->Rate && (end - start) <5000)
	{
		index = rand()%100;
		if(paramArray[index].ItemID ==0)
		{
			paramArray[index] = *param;
			count++;
		}
		end = GetTickCount();
	}
	return 0;
}

void CTreasurePack::ShowData(int type)
{
	if(type == SUREFLOP)
	{
		printf("%d:%d:%d:%d\n",SureFlopParam.ItemID,SureFlopParam.Rate,SureFlopParam.MinCount,SureFlopParam.MaxCount);
	}
	else
	{
		LPFlopParam paramArray = 0;
		if(type == RANDOMFLOP)
		{
			paramArray = Randomflop;
		}
		else
		{
			paramArray = Equipflop;
		}
		int count = 0;
		for(int i=0;i<100;i++)
		{
			if(paramArray[i].ItemID !=0)
			{
				count++;
				printf("%d:%d:%d:%d\n",paramArray[i].ItemID,paramArray[i].Rate,paramArray[i].MinCount,paramArray[i].MaxCount);
			}
		}
	}
}