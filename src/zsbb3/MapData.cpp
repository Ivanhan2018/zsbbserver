
#include "MapData.h"

#define SCENE_PIC_INFO_FILE      "info.ini"

#include "Global_Macro.h"

map<string, ARCH_TYPES>   g_mapStringToType;

CMapData::CMapData(void)
{
	g_mapStringToType["默认"] = ARCH_DEFAULT;
	g_mapStringToType["行政"] = ARCH_TOWNHOUSE;
	g_mapStringToType["城市"] = ARCH_CITY;
	g_mapStringToType["岗哨塔"] = ARCH_SENTRY;
	g_mapStringToType["副本门"] = ARCH_EC_DOOR;
	g_mapStringToType["登录点"] = ARCH_LOGIN;
	g_mapStringToType["木场"] = ARCH_LUMBERYARD;
	g_mapStringToType["采矿场"] = ARCH_STOPE;
	g_mapStringToType["磨房"] = ARCH_GRINDERY;
	g_mapStringToType["皮料厂"] = ARCH_SKIN;
	g_mapStringToType["特殊"] = ARCH_LOW_LEVEL1;
	g_mapStringToType["地基"] = ARCH_BUILD_TOFT;
	g_mapStringToType["陷阱"] = ARCH_TRAP;
	g_mapStringToType["栅栏"] = ARCH_BARRIER;
	g_mapStringToType["战场传送门"] = ARCH_WAR_ENTER;
	g_mapStringToType["野外副本门"] = ARCH_FIELD_COUNTRY;
	g_mapStringToType["资源点"] = ARCH_RES_BUILDING;
	
}

CMapData::~CMapData(void)
{
}

bool CMapData::LoadFile(string strFile)
{
	ifstream fileRead;
	fileRead.open(strFile.data());
	if (!fileRead.is_open())
		return false;


	string strLine;

	//读入地图信息
	fileRead >> strLine;
	//assert(strLine == "[MapInfo]=Version,Name,CellW,CellH,CellRowCount,CellColCount,GridSize");

	//版本
	
	fileRead >> strLine;
	m_strVersion = strLine;

	//地图内部名
	
	fileRead >> strLine;
	m_strInnerName = strLine;

	//地表信息

	fileRead >> m_sizeCell.cx;     //菱形单元格宽
	fileRead >> m_sizeCell.cy;	 //菱形单元格高
	fileRead >> m_nRows;			 //菱形单元格行总数
	fileRead >> m_nCols;			 //菱形单元格列总数
	fileRead >> m_nRectangleGridSize;  //矩形格大小


	//地表阻挡格信息
	fileRead >> strLine;
	//assert (strLine == "[MapObstacle]=ObstacleCount,[ObstacleType,CellRow,CellCol]...");

	int nObstacleCount, nObstacleNum;
	fileRead >> nObstacleCount;
	if (nObstacleCount > 0)
	{
		for (nObstacleNum = 0; nObstacleNum < nObstacleCount; nObstacleNum++)
		{
			ObstacleCellInfo oci;
			fileRead >> oci.nType;
			fileRead >> oci.posRowCol.x;
			fileRead >> oci.posRowCol.y;
			AjustCellRowCol(oci.posRowCol.x, oci.posRowCol.y);

			m_obstacleItemArray.push_back(oci);
		}
	}

	//地表混合层信息
	fileRead >> strLine;
	//assert(strLine == "[BackgroundMixLayer]=MixItemCount,[Pic,Mask,CeterX,CenterY,CellRow,CellCol,Alpha,Blend]...");

	string strPicfile;
	string strMaskFile;
	int nMixItemCount;
	int nMixItemIndex;

	fileRead >> nMixItemCount;
	for (nMixItemIndex = 0; nMixItemIndex < nMixItemCount; nMixItemIndex++)
	{
		MixItemInfo mii;

		fileRead >> strPicfile;
		fileRead >> strMaskFile;
		fileRead >> mii.posItem.x;
		fileRead >> mii.posItem.y;
		fileRead >> mii.nRow;
		fileRead >> mii.nCol;
		fileRead >> mii.nAlpha;
		fileRead >> mii.bBlend;

		AjustItemPoint(mii.posItem);
		AjustCellRowCol(mii.nRow, mii.nCol);

		mii.strPicfile = strPicfile;
		mii.strMaskFile = strMaskFile;

		m_miiArray.push_back(mii);
	}


	//场景信息
	fileRead >> strLine;
	//assert(strLine == "[SceneLayer]=SceneItemCount,[ID,Name,CenterX,CenterY,CellRow,CellCol,MultiFrame,Pic,Type,[ObstacleType,CellRow,CellCol]...]...");

	int nSceneItemCount, nSceneItemIndex;
	string  strID, strName, strType, strPicOrFolder;
	fileRead >> nSceneItemCount;
	for (nSceneItemIndex = 0; nSceneItemIndex < nSceneItemCount; nSceneItemIndex++)
	{
		SceneItemInfo sii;
		
		//ID
		fileRead >> strID;
		//Name
		fileRead >> strName;
		//坐标
		fileRead >> sii.posItem.x;
		fileRead >> sii.posItem.y;
		fileRead >> sii.nRow;
		fileRead >> sii.nCol;
		//是否为多帧
		fileRead >> sii.bMultiFrame;
		//当单帧时为图片名，多帧时为目录名
		fileRead >> strPicOrFolder;
		//对象类型
		fileRead >> strType;

		AjustItemPoint(sii.posItem);
		AjustCellRowCol(sii.nRow, sii.nCol);

		sii.strID = strID;
		sii.strName = strName;
		sii.strPicOrFolder = strPicOrFolder;

		if (g_mapStringToType[strType] != NULL)
		{
			sii.nObjectType = g_mapStringToType[strType];
		}
		else
		{
			sii.nObjectType = ARCH_DEFAULT;
		}


		//场景阻挡格信息
		int nSceneItemObstacleCount, nSceneItemObstacleIndex;
		fileRead >>  nSceneItemObstacleCount;
		for (nSceneItemObstacleIndex = 0; nSceneItemObstacleIndex < nSceneItemObstacleCount; nSceneItemObstacleIndex++)
		{
			ObstacleCellInfo oci;
			fileRead >> oci.nType;
			fileRead >> oci.posRowCol.x;
			fileRead >> oci.posRowCol.y;

			AjustCellRowCol(oci.posRowCol.x, oci.posRowCol.y);
			sii.sceneItemObstacleArray.push_back(oci);
		}

		m_siiArray.push_back(sii);
	}
	fileRead.close();

	return true;
}


void CMapData::AjustItemPoint(POINT &posItem)
{
	posItem.x = posItem.x + m_sizeCell.cx * m_nCols / 2;
	posItem.y = m_sizeCell.cy * m_nRows / 2 - posItem.y;
}

void CMapData::AjustCellRowCol(int &/*nRow*/, int &/*nCol*/)
{
	/*nRow = nRow + 1;
	nCol = nCol + 1;*/
}

void CMapData::AjustCellRowCol(long &/*nRow*/, long &/*nCol*/)
{
	/*nRow = nRow + 1;
	nCol = nCol + 1;*/
}
