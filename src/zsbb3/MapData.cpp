
#include "MapData.h"

#define SCENE_PIC_INFO_FILE      "info.ini"

#include "Global_Macro.h"

map<string, ARCH_TYPES>   g_mapStringToType;

CMapData::CMapData(void)
{
	g_mapStringToType["Ĭ��"] = ARCH_DEFAULT;
	g_mapStringToType["����"] = ARCH_TOWNHOUSE;
	g_mapStringToType["����"] = ARCH_CITY;
	g_mapStringToType["������"] = ARCH_SENTRY;
	g_mapStringToType["������"] = ARCH_EC_DOOR;
	g_mapStringToType["��¼��"] = ARCH_LOGIN;
	g_mapStringToType["ľ��"] = ARCH_LUMBERYARD;
	g_mapStringToType["�ɿ�"] = ARCH_STOPE;
	g_mapStringToType["ĥ��"] = ARCH_GRINDERY;
	g_mapStringToType["Ƥ�ϳ�"] = ARCH_SKIN;
	g_mapStringToType["����"] = ARCH_LOW_LEVEL1;
	g_mapStringToType["�ػ�"] = ARCH_BUILD_TOFT;
	g_mapStringToType["����"] = ARCH_TRAP;
	g_mapStringToType["դ��"] = ARCH_BARRIER;
	g_mapStringToType["ս��������"] = ARCH_WAR_ENTER;
	g_mapStringToType["Ұ�⸱����"] = ARCH_FIELD_COUNTRY;
	g_mapStringToType["��Դ��"] = ARCH_RES_BUILDING;
	
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

	//�����ͼ��Ϣ
	fileRead >> strLine;
	//assert(strLine == "[MapInfo]=Version,Name,CellW,CellH,CellRowCount,CellColCount,GridSize");

	//�汾
	
	fileRead >> strLine;
	m_strVersion = strLine;

	//��ͼ�ڲ���
	
	fileRead >> strLine;
	m_strInnerName = strLine;

	//�ر���Ϣ

	fileRead >> m_sizeCell.cx;     //���ε�Ԫ���
	fileRead >> m_sizeCell.cy;	 //���ε�Ԫ���
	fileRead >> m_nRows;			 //���ε�Ԫ��������
	fileRead >> m_nCols;			 //���ε�Ԫ��������
	fileRead >> m_nRectangleGridSize;  //���θ��С


	//�ر��赲����Ϣ
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

	//�ر��ϲ���Ϣ
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


	//������Ϣ
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
		//����
		fileRead >> sii.posItem.x;
		fileRead >> sii.posItem.y;
		fileRead >> sii.nRow;
		fileRead >> sii.nCol;
		//�Ƿ�Ϊ��֡
		fileRead >> sii.bMultiFrame;
		//����֡ʱΪͼƬ������֡ʱΪĿ¼��
		fileRead >> strPicOrFolder;
		//��������
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


		//�����赲����Ϣ
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
