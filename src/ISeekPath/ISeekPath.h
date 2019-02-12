// @brief 简要描述: 寻路算法抽象类
// @author 作者: 

#ifndef _ISEEK_PATH_ARITHM_H_H_H
#define _ISEEK_PATH_ARITHM_H_H_H

#include <vector>
#include <list>
#include <assert.h>

namespace SEEK_PATH_NAME_SPACE
{
	enum NodeState
	{
		NODE_UNUSED = 0,
		NODE_BLOCK,
		NODE_BLOCK_DYNAMIC,
		NODE_IN_OPEN,
		NODE_IN_CLOSE
	};

	enum
	{
		MAX_NEIGHBOUR_NODE = 4
	};

	/*
	enum
	{
		SPACE_VALUE_BETWEEN_NODE = 1
	};
	*/
	enum
	{
		LAND_PLAIN_WEIGHT_VALUE = 1,		// 平原的权值
		LAND_FOREST_WEIGHT_VALUE = 3,		// 森林的权值 
		LAND_RIVERWAY_WEIGHT_VALUE = 8		// 河流的权值
	};

	// 地图格子的坐标
	struct PosXYStru
	{
		PosXYStru(int _xPos, int _yPos) : xPos(_xPos), yPos(_yPos)
		{
		}
		PosXYStru() : xPos(0), yPos(0)
		{
		}
		~PosXYStru()
		{

		}

		bool operator==(const PosXYStru &posParam)
		{
			return (xPos == posParam.xPos && yPos == posParam.yPos);
		}

		bool operator!=(const PosXYStru &posParam)
		{
			return !(operator==(posParam));
		}

		PosXYStru & operator = (const PosXYStru &posParam)
		{
			if (this == &posParam)
			{
				return *this;
			}

			xPos = posParam.xPos;
			yPos = posParam.yPos;

			return *this;
		}

		bool operator < (const PosXYStru &posParam) const 
		{
			return xPos < posParam.yPos && yPos < posParam.yPos;
		}

		int xPos;
		int yPos;
	};

	typedef std::vector<PosXYStru> Vect_Pos;

	//寻路结点结构
	typedef struct tagNodeData
	{
		unsigned short nFValue;			//F值 = G + H
		unsigned short nGValue;			//G值，距开始点的距离
		unsigned short nHValue;			//H值，距结束点的距离
		PosXYStru posParentNode;			//父结点坐标
		PosXYStru posLinkNode;				//当为OpenList表中的结点时，指向OpenList表中的下一个结点
		NodeState nState;				//结点状态
	}NodeData;

	inline void coordinateTransition(const int x, int &y)
	{
		if(x & 1) 
		{	
			y = (y << 1) - 2; 
		}
		else
		{
			y = (y << 1) - 1;
		}
	}

	class ISeekPath
	{
	public:
		ISeekPath::ISeekPath();
		virtual ~ISeekPath(void);

		bool Init(int nRowMax, int nColMax);
		void Release();
		void Reset();
		void SetBlockInfo();
		void SetDynamicBlockInfo(const std::list<PosXYStru> &posDynamicBlock);

		bool findPath(const PosXYStru &posStart, const PosXYStru &posEnd, Vect_Pos & outVectPos);// 寻路主函数

		void FindNodeInSameDistance(const PosXYStru posCenter, const int nDistance, PosXYStru *pResultPos, int &nResultPosNum);			//寻找相同距离结点（算法1）

		// 判断指定点是否为静态阻挡
		virtual bool IsBlockoff(int x, int y) const  = 0;

		virtual int getLandFormWeightVal(int , int ) const
		{
			return LAND_PLAIN_WEIGHT_VALUE;
		}

	private:
		// 找到相邻单元格
		void FindNeighbourAndInsertIntoOpenList(const PosXYStru &posFind, const PosXYStru &nodeEnd);

		void InsertOpenListByOrder(const PosXYStru &posNode);

		void DeleteNodeInOpenList(const PosXYStru &posNode);

		bool IsInCloseTable(const PosXYStru &nodeTest);		
		// 是否在Close表里
		void InsertCloseList(const PosXYStru &posNode);
		// 计算两单元格间的距离
		int  CalcDistanceBetweenTwoNode(const PosXYStru &pos1, const PosXYStru &pos2);							
		// 得到下一个在Open表中的结点
		void GetNextNodeInOpenList(PosXYStru &posSel, const PosXYStru &posEnd);								

	private:
		NodeData	**m_ppNodeDataArray;

		int			m_nRowMax;
		int			m_nColMax;

		PosXYStru	m_posHeadOpenList;
		int			m_nOpenListNodeNum;

		PosXYStru	    m_posHeadCloseList;
		int			m_nCloseListNodeNum;
	};

}

#endif