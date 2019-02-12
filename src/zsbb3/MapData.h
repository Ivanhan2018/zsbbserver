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
	string strPicfile;    //��ȫͼƬ
	string strMaskFile;	//�ɰ�ͼƬ
	POINT posItem;			//���ĵ�λ��
	int nRow;				//��Ԫ����
	int nCol;				//��Ԫ����
	int nAlpha;				//Alphaֵ
	bool bBlend;			//�Ƿ�ʹ���ɰ�
}MixItemInfo;

typedef struct tagSceneItemInfo
{
	string strID;      //ID
	string strName;	//����
	POINT posItem;		//���ĵ�����
	int nRow;			//��Ԫ����
	int nCol;			//��Ԫ����
	bool bMultiFrame;   //�Ƿ�Ϊ��֡
	string strPicOrFolder;//����֡ʱΪͼƬ������֡ʱΪĿ¼��
	int nObjectType;   //��������

	vector<ObstacleCellInfo> sceneItemObstacleArray;	//�����赲����Ϣ
}SceneItemInfo;




class CMapData
{
public:
	CMapData(void);
	~CMapData(void);

	bool LoadFile(string wstrFile);

private:
	void AjustItemPoint(POINT &posItem);	//������ת��Ϊ��������Ͻǵ�ֵ
	void AjustCellRowCol(int &nRow, int &nCol); //����Ԫ�����е���Ϊ���Ͻ�Ϊ��1��1��
	void AjustCellRowCol(long &nRow, long &nCol);
public:
	string m_strVersion;					//��ͼ�汾
	string m_strInnerName;					//��ͼ�ڲ���
	SIZE m_sizeCell;						//���ε�Ԫ����
	int m_nRows;							//���ε�Ԫ������
	int m_nCols;							//���ε�Ԫ������
	int m_nRectangleGridSize;				//���θ��С

	vector<ObstacleCellInfo> m_obstacleItemArray;    //��ͼ�е��赲����Ϣ
	vector<MixItemInfo> m_miiArray;					 //�ر��ϲ���Ϣ
	vector<SceneItemInfo> m_siiArray;				 //��������Ϣ
};
