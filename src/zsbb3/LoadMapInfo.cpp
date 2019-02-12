#include "LoadMapInfo.h"

#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>

#include <crtdbg.h>
#include "MapData.h"
#pragma warning(disable:4100)
#pragma warning(disable:4189)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#pragma warning(disable:4389)
#pragma warning(disable:4800)
CLoadMapInfo* g_MapInfo = NULL;
CLoadMapInfo::CLoadMapInfo(unsigned int mapId)
:m_MapID(mapId),
m_ower(3),
m_state(peace)
{
	ReadMapInfo(mapId);
}

CLoadMapInfo::~CLoadMapInfo(void)
{
}

void CLoadMapInfo::ReadMapInfo(unsigned mapid)
{
	cout<<"*************** CLoadMapInfo::ReadMapInfo()***********************"<<endl;
	char path[20];
	sprintf(path,"map\\%d.dat",mapid);
	ReadMapFile(path);
	if (mapid>1000 && mapid<=1060)
	{
		ReadMapConnectFile(mapid);
	}
	
}
void CLoadMapInfo::ReadMapFile(char *filePath)
{
	cout<<"*************** CLoadMapInfo::ReadMapFile()***********************"<<endl;
	if (filePath == NULL || strcmp(filePath,"")==0)
	{
		return ;
	}
	cout<<"Load Map   "<<filePath<<endl;
	int line = -1;
	int count = 0 ;
	int count2 = 0 ;
	int obj =0;
	int numcity = 0; //城市计数
	int numsentry =0 ;//哨所计数
	int numsod =0 ;//大本云计数
	//ifstream   ifs(filePath);   
	string   buf; 
	string   buf2;
	string   buf3;
	char tmp[5]="";
	//int buildid;
	int x =0 ;
	int y = 0 ;
	int ipointcount =0;
	POS_STRUCT Pos;
	EncampmentInfo EncapInfo;
	SentryInfo  Sentry ;
	Propagate	tagPropagate;
	CityInfo cityInfo;
	ResBuild resNuild;
	
	CMapData MapData;
	string str(filePath);
	if(!MapData.LoadFile(str)){
		MapData.m_nRows = 1;
		MapData.m_nCols = 1;
	}
	m_iMaxX = MapData.m_nRows * 2 + 1;
	m_iMaxY = MapData.m_nCols + 1;

	vector<int> mapValue;
	mapValue.resize(m_iMaxY , 0);
	MapInfo.resize(m_iMaxX, mapValue);
	
	
	//wstring path =  StringToWstring(str);
	for (int i = 0; i < m_iMaxX ; i ++)
	{
		for (int j = 0 ; j < m_iMaxY ; j ++)
		{
			if (i == 0 || j == 0 || i == m_iMaxX - 1 || j == m_iMaxY - 1)
			{
				MapInfo[i][j] = 3;
			}
		}
	}
	
	vector<ObstacleCellInfo>::iterator iter1;
	for (iter1 = MapData.m_obstacleItemArray.begin();iter1!=MapData.m_obstacleItemArray.end();iter1++){
		x = (*iter1).posRowCol.x;
		y = (*iter1).posRowCol.y;
		MapInfo[x][y] =(*iter1).nType;
	}
	vector<SceneItemInfo>::iterator iter;
	for (iter = MapData.m_siiArray.begin();iter!=MapData.m_siiArray.end();iter++)
	{

		switch((*iter).nObjectType)
		{
		case ARCH_DEFAULT:			//默认非可操作建筑，如树，山...
			for (iter1 = (*iter).sceneItemObstacleArray.begin();iter1!=(*iter).sceneItemObstacleArray.end();iter1++)
			{
				x = (*iter1).posRowCol.x;
				y = (*iter1).posRowCol.y;
				MapInfo[x][y] = (*iter1).nType;
			}
			break;
		case ARCH_TOWNHOUSE:		//城镇中心
			GovPos.uGovID = atoi((*iter).strID.c_str());
			GovPos.centryPos.PosX = (*iter).nRow;
			GovPos.centryPos.PosY = (*iter).nCol;
			for (iter1 = (*iter).sceneItemObstacleArray.begin();iter1!=(*iter).sceneItemObstacleArray.end();iter1++)
			{
				x = (*iter1).posRowCol.x;
				y = (*iter1).posRowCol.y;
				MapInfo[x][y] = (*iter1).nType;
				Pos.PosX = x;
				Pos.PosY = y;
				Pos.weight = 3;
				GovPos.GovPos.push_back(Pos);
			}			
			break;
		case ARCH_CITY:				//城市
			cityInfo.CityPos.clear();
			cityInfo.uCityID = atoi((*iter).strID.c_str());
			cityInfo.centryPos.PosX = (*iter).nRow;
			cityInfo.centryPos.PosY = (*iter).nCol;
			//m_CityInfo[numcity].uCityID = atoi((*iter).strID.c_str());
			for (iter1 = (*iter).sceneItemObstacleArray.begin();iter1!=(*iter).sceneItemObstacleArray.end();iter1++)
			{
				x = (*iter1).posRowCol.x;
				y = (*iter1).posRowCol.y;
				MapInfo[x][y] = (*iter1).nType;
				Pos.PosX = x;
				Pos.PosY = y;
				Pos.weight = 3;
				cityInfo.CityPos.push_back(Pos);
			}
			m_CityInfo.push_back(cityInfo);
			//numcity++;
			break;
		case ARCH_SENTRY:			//岗哨塔
			Sentry.SertryID= atoi((*iter).strID.c_str());
			x = (*iter).sceneItemObstacleArray.front().posRowCol.x;
			y = (*iter).sceneItemObstacleArray.front().posRowCol.y;
			Sentry.centryPos.PosX = (*iter).nRow;
			Sentry.centryPos.PosY = (*iter).nCol;
			MapInfo[x][y] = 3;
			Sentry.SerTryPos.PosX = x;
			Sentry.SerTryPos.PosY = y;
			Sentry.SerTryPos.weight = 3;
			sentry_list.push_back(Sentry);
			break;
		case ARCH_RES_BUILDING: //资源点
			resNuild.resId= atoi((*iter).strID.c_str());
			x = (*iter).sceneItemObstacleArray.front().posRowCol.x;
			y = (*iter).sceneItemObstacleArray.front().posRowCol.y;
			resNuild.centryPos.PosX = (*iter).nRow;
			resNuild.centryPos.PosY = (*iter).nCol;
			MapInfo[x][y] = 3;
			resNuild.SerTryPos.PosX = x;
			resNuild.SerTryPos.PosY = y;
			resNuild.SerTryPos.weight = 3;
			resList.push_back(resNuild);
			break;
		case ARCH_WAR_ENTER:
		case ARCH_FIELD_COUNTRY:
		case ARCH_EC_DOOR:			//副本传送门
			tagPropagate.uPropagateID = atoi((*iter).strID.c_str());
			for (iter1 = (*iter).sceneItemObstacleArray.begin();iter1!=(*iter).sceneItemObstacleArray.end();iter1++)
			{
				x = (*iter1).posRowCol.x;
				y = (*iter1).posRowCol.y;
				tagPropagate.Pos[ipointcount].PosX = x;
				tagPropagate.Pos[ipointcount].PosY = y;
				tagPropagate.Pos[ipointcount].weight = 3;
				MapInfo[x][y] = 3;
				ipointcount++;
				if (ipointcount>1)
				{
					break;
				}
			}				
			ipointcount = 0;
			propagate_list.push_back(tagPropagate);
			break;
		case ARCH_LOGIN:			//登录点(部队出征到达位置)
			EncapInfo.uEncampentID = atoi((*iter).strID.c_str());
			for (iter1 = (*iter).sceneItemObstacleArray.begin();iter1!=(*iter).sceneItemObstacleArray.end();iter1++)
			{
				x =(*iter1).posRowCol.x;
				y =(*iter1).posRowCol.y;
				EncapInfo.EncapentPos.PosX = x;
				EncapInfo.EncapentPos.PosY = y;
				EncapInfo.EncapentPos.weight = 3;
				MapInfo[x][y] = 3;
				//m_CityInfo[numcity].CityPos.push_back(Pos);
			}
			EncampmentPos.push_back(EncapInfo);
			break;
		case ARCH_LUMBERYARD:		//木场
			//break;
		case ARCH_STOPE:			//采矿场
			//break;
		case ARCH_GRINDERY:			//磨房
			//break;
		case ARCH_SKIN:				//皮料加工厂
			for (iter1 = (*iter).sceneItemObstacleArray.begin();iter1!=(*iter).sceneItemObstacleArray.end();iter1++)
			{
				x =(*iter1).posRowCol.x;
				y =(*iter1).posRowCol.y;
				MapInfo[x][y] = 3;
			}
			break;
		case ARCH_BUILD_TOFT:
		case ARCH_TRAP:
		case ARCH_BARRIER:
			SpecialBuild sBuild;
			sBuild.buildID = atoi((*iter).strID.c_str());
			sBuild.pos.PosX = -1;
			for (iter1 = (*iter).sceneItemObstacleArray.begin();iter1!=(*iter).sceneItemObstacleArray.end();iter1++)
			{
				sBuild.pos.PosX =(*iter1).posRowCol.x;
				sBuild.pos.PosY =(*iter1).posRowCol.y;
				
			}
			assert((sBuild.pos.PosX != -1) && "地图特殊建筑数据加载错误！");
			specialBuild_.push_back(sBuild);
			 break;
		case ARCH_LOW_LEVEL1:		//特殊类型的建筑，如(桥，路，河)这些做排序使用，要放在所有建筑最下层显示
		case ARCH_LOW_LEVEL2:
		case ARCH_LOW_LEVEL3:
			for (iter1 = (*iter).sceneItemObstacleArray.begin();iter1!=(*iter).sceneItemObstacleArray.end();iter1++)
			{
				x =(*iter1).posRowCol.x;
				y =(*iter1).posRowCol.y;
				MapInfo[x][y] = (*iter1).nType;
			}
			break;
		default:
			break;
		}
	}
}
void CLoadMapInfo::ReadMapConnectFile(unsigned int mapid)
{
	cout<<"************ReadMapConnectFile()**************"<<endl;
	unsigned int count = 0 ;
	int count2 = 0 ;
	int count3 =0;
	int obj =0;
	string buf;
	ifstream   ifs("configtable/MapConectTable.csv");
	char tmp[10]="";
	while(ifs)
	{
		if (getline(ifs, buf))
		{
			count ++;
			if (count -1== mapid-1000)
			{
				for (int i = 0 ; i <(int)(buf.length());i ++)
				{
					if (buf[i]!=',')
					{
						tmp[count2]= buf[i];
						count2++;
						//count ++;
					}
					if (buf[i]==','||i == (int)(buf.length())-1)
					{
						count3++;
						if (count3>1)
						{
							obj = atoi(tmp);
							if (obj>0)
							{
								m_MapToDoorList[count3-1] = obj;
							}
						}
						memset(tmp,0,10);
						count2 =0;
					}
				}
				break;
			}
		}
	}
	ifs.close();

}
void CLoadMapInfo::SetCityPos()
{

}

void CLoadMapInfo::SetGovPos()
{

}

void CLoadMapInfo::SetEncampentPos()
{

}

void CLoadMapInfo::SetMapState(mapstate state)
{
	m_state = state;
}

mapstate CLoadMapInfo::GetMapState()
{
	return m_state;
}

void CLoadMapInfo::SetMapOwer(int ower )
{
	m_ower = ower;
}

int CLoadMapInfo::GetMapOwer()
{
	return m_ower;
}