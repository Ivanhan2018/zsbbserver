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
	//��õ�ͼ�赲��Ϣ
	inline bool IsBlockoff(int x, int y) const{
		/*if (IsValidPos(x , y) == false){
			return true;
		}*/
		return MapInfo[x][y] == 3;
	}
	//��֤λ���Ƿ���Ч
	inline bool IsValidPos(int x , int y){
		if (x < 0 || y < 0 || x >= m_iMaxX || y >= m_iMaxY){
			return false;
		}
		return true;
	}
	//��õ�ǰ����
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
	list<CityInfo> m_CityInfo;//����λ��
	GovInfo GovPos;//��������
	list<EncampmentInfo> EncampmentPos; //��½��
	list<SentryInfo> sentry_list;//�����б�
	list<Propagate>  propagate_list; //����������
	list<ResBuild> resList;//�����б�
	vector<SpecialBuild> specialBuild_; //���⽨��
	map<unsigned long ,unsigned long> m_MapToDoorList;
private:
	mapstate m_state ;
	int m_ower ; //������
};
extern CLoadMapInfo* g_MapInfo;
#endif