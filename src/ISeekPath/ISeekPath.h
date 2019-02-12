// @brief ��Ҫ����: Ѱ·�㷨������
// @author ����: 

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
		LAND_PLAIN_WEIGHT_VALUE = 1,		// ƽԭ��Ȩֵ
		LAND_FOREST_WEIGHT_VALUE = 3,		// ɭ�ֵ�Ȩֵ 
		LAND_RIVERWAY_WEIGHT_VALUE = 8		// ������Ȩֵ
	};

	// ��ͼ���ӵ�����
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

	//Ѱ·���ṹ
	typedef struct tagNodeData
	{
		unsigned short nFValue;			//Fֵ = G + H
		unsigned short nGValue;			//Gֵ���࿪ʼ��ľ���
		unsigned short nHValue;			//Hֵ���������ľ���
		PosXYStru posParentNode;			//���������
		PosXYStru posLinkNode;				//��ΪOpenList���еĽ��ʱ��ָ��OpenList���е���һ�����
		NodeState nState;				//���״̬
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

		bool findPath(const PosXYStru &posStart, const PosXYStru &posEnd, Vect_Pos & outVectPos);// Ѱ·������

		void FindNodeInSameDistance(const PosXYStru posCenter, const int nDistance, PosXYStru *pResultPos, int &nResultPosNum);			//Ѱ����ͬ�����㣨�㷨1��

		// �ж�ָ�����Ƿ�Ϊ��̬�赲
		virtual bool IsBlockoff(int x, int y) const  = 0;

		virtual int getLandFormWeightVal(int , int ) const
		{
			return LAND_PLAIN_WEIGHT_VALUE;
		}

	private:
		// �ҵ����ڵ�Ԫ��
		void FindNeighbourAndInsertIntoOpenList(const PosXYStru &posFind, const PosXYStru &nodeEnd);

		void InsertOpenListByOrder(const PosXYStru &posNode);

		void DeleteNodeInOpenList(const PosXYStru &posNode);

		bool IsInCloseTable(const PosXYStru &nodeTest);		
		// �Ƿ���Close����
		void InsertCloseList(const PosXYStru &posNode);
		// ��������Ԫ���ľ���
		int  CalcDistanceBetweenTwoNode(const PosXYStru &pos1, const PosXYStru &pos2);							
		// �õ���һ����Open���еĽ��
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