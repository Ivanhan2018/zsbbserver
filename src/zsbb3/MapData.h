#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <assert.h>
using namespace std;


typedef struct argPOINT 
{
	int x;
	int y;
}POINT;

typedef struct argSIZE
{
	int cx;
	int cy;
}SIZE;

typedef struct tagObstacleCellInfo
{
	POINT posRowCol;
	int   nType;
}ObstacleCellInfo;

typedef struct tagMixItemInfo
{
	string strPicfile;    //混全图片
	string strMaskFile;	//蒙板图片
	POINT posItem;			//中心点位置
	int nRow;				//单元格行
	int nCol;				//单元格列
	int nAlpha;				//Alpha值
	bool bBlend;			//是否使用蒙板
}MixItemInfo;

typedef struct tagSceneItemInfo
{
	string strID;      //ID
	string strName;	//名称
	POINT posItem;		//中心点坐标
	int nRow;			//单元格行
	int nCol;			//单元格列
	bool bMultiFrame;   //是否为多帧
	string strPicOrFolder;//当单帧时为图片名，多帧时为目录名
	int nObjectType;   //对象类型

	vector<ObstacleCellInfo> sceneItemObstacleArray;	//场景阻挡格信息
}SceneItemInfo;




class CMapData
{
public:
	CMapData(void);
	~CMapData(void);

	bool LoadFile(string wstrFile);

private:
	void AjustItemPoint(POINT &posItem);	//将坐标转换为相对于左上角的值
	void AjustCellRowCol(int &nRow, int &nCol); //将单元格行列调整为左上角为（1，1）
	void AjustCellRowCol(long &nRow, long &nCol);
public:
	string m_strVersion;					//地图版本
	string m_strInnerName;					//地图内部名
	SIZE m_sizeCell;						//菱形单元格宽高
	int m_nRows;							//菱形单元格行数
	int m_nCols;							//菱形单元格列数
	int m_nRectangleGridSize;				//矩形格大小

	vector<ObstacleCellInfo> m_obstacleItemArray;    //地图中的阻挡格信息
	vector<MixItemInfo> m_miiArray;					 //地表混合层信息
	vector<SceneItemInfo> m_siiArray;				 //场景层信息
};
