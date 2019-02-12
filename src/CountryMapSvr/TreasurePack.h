#ifndef TREASUREPACK_H__
#define TREASUREPACK_H__
#include "IceUtil.h"
//宝箱类
class CLoadDBInfo;
class CTreasurePack
{
private:
	typedef struct tagFlopParam
	{
		int ItemID;
		int Rate;
		int MinCount;
		int MaxCount;
	}FlopParam,*LPFlopParam;
public:
	typedef struct tagTreasurePackItem
	{
		int ItemID;
		int Count;
	}TreasurePackItem,*LPTreasurePackItem;

	enum {SUREFLOP,RANDOMFLOP,EQUIPFLOP};
	CTreasurePack(CLoadDBInfo *loadDBInfo,int id,int level);
	~CTreasurePack();
	void InitParam(const char *buf,int type);
	void GetItems(LPTreasurePackItem sureflop,LPTreasurePackItem randomflop,LPTreasurePackItem equipflop);
	void ShowData(int type);
private:
	void SetID(int id);
	int GetFieldValue(const char *srcbuf,int index,char *destbuf,char sepchar);
	int InitRandom(LPFlopParam param,LPFlopParam paramArray);
private:
	int ID;
	int level_;		//宝箱级别
	CLoadDBInfo * loadDBInfo_;//全局参数
	FlopParam SureFlopParam;
	FlopParam Randomflop[100];
	FlopParam Equipflop[100]; 
};

#endif