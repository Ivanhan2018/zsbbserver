#ifndef LOAD_MAPINFO_H
#define LOAD_MAPINFO_H
#pragma once
#pragma warning(push)
#pragma warning(disable:4100)
#include "ProFightSystem.h"
#pragma warning(pop)
#include <list>
#include <map>
#define CITY_NUM		4
#include "vector"
using namespace std;

class CLoadMapInfo
{
public:
	
	CLoadMapInfo(unsigned int mapId);
	~CLoadMapInfo(void);
	unsigned getMapID()
	{
		return m_MapID ;
	}
	void ReadMapFile(char *filePath );
	void ReadMapConnectFile(unsigned int mapid);
	void SetCityPos();
	void SetGovPos();
	void SetEncampentPos();
	void ReadMapInfo(unsigned mapid);

	void SetMapState(mapstate state);
	mapstate GetMapState();
	void SetMapOwer(int ower );
	int  GetMapOwer();
	//获得地图阻挡信息
	inline bool IsBlockoff(int x, int y) const{
		/*if (IsValidPos(x , y) == false){
			return true;
		}*/
		return MapInfo[x][y] == 3;
	}
	//验证位置是否有效
	inline bool IsValidPos(int x , int y){
		if (x < 0 || y < 0 || x >= m_iMaxX || y >= m_iMaxY){
			return false;
		}
		return true;
	}
	//获得当前地形
	LAND_FORM getLandForm(int x, int y) const
	{
		return static_cast<LAND_FORM>(MapInfo[x][y]);
	}
	POS_STRUCT getPropagatePos(int propaGateID){
		if (propagate_list.size() == 0){
			return POS_STRUCT();
		}
		std::list<Propagate>::iterator iter;
		for (iter = propagate_list.begin() ; iter != propagate_list.end(); ++iter)
		{
			if (iter->uPropagateID == static_cast<unsigned int>(propaGateID)){
				return iter->Pos[0];
			}
		}
		return propagate_list.front().Pos[0];
	}
	
public:
	vector<vector<int> > MapInfo;
	int m_iMaxX;
	int m_iMaxY;
	unsigned int m_MapID;//
	list<CityInfo> m_CityInfo;//城市位置
	GovInfo GovPos;//行政中心
	list<EncampmentInfo> EncampmentPos; //登陆点
	list<SentryInfo> sentry_list;//哨所列表
	list<Propagate>  propagate_list; //副本传送门
	list<ResBuild> resList;//哨所列表
	vector<SpecialBuild> specialBuild_; //特殊建筑
	map<unsigned long ,unsigned long> m_MapToDoorList;
private:
	mapstate m_state ;
	int m_ower ; //归属性
};
extern CLoadMapInfo* g_MapInfo;
#endif