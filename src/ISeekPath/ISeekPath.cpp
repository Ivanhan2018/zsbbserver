// @brief 简要描述: 寻路算法类
// @author 作者:

#include "windows.h"
#include "ISeekPath.h"

namespace SEEK_PATH_NAME_SPACE
{
	ISeekPath::ISeekPath()
	{
		m_ppNodeDataArray = NULL;
		m_nRowMax = 0;
		m_nColMax = 0;

		m_nOpenListNodeNum = 0;

		m_nCloseListNodeNum = 0;
	}

	ISeekPath::~ISeekPath(void)
	{
		if (m_ppNodeDataArray)
		{
			Release();
		}
	}

	bool ISeekPath::Init(int nRowMax, int nColMax)
	{
		//保存表格的最大行和最大列
		m_nRowMax = nRowMax;
		m_nColMax = nColMax;

		//根据最大行和最大列分配存放所有结点信息的二维数组
		m_ppNodeDataArray = new NodeData*[m_nRowMax];
		assert(m_ppNodeDataArray);
		int i;
		for (i = 0; i < m_nRowMax; i++)
		{
			m_ppNodeDataArray[i] = new NodeData[m_nColMax];

			memset(m_ppNodeDataArray[i], 0, sizeof(NodeData) * m_nColMax);

			//	assert(m_ppNodeDataArray[i]);
		}

		Reset();

		SetBlockInfo();

		return true;
	}

	void ISeekPath::Release()
	{
		int i;
		//释放结点二维数组
		if (m_ppNodeDataArray != NULL)
		{
			for (i = 0; i < m_nRowMax; i++)
			{
				delete [] m_ppNodeDataArray[i];
			}
			delete [] m_ppNodeDataArray;
			m_ppNodeDataArray = NULL;
		}
	}

	void ISeekPath::Reset()
	{
		m_nOpenListNodeNum = 0;
		m_nCloseListNodeNum = 0;

		//初始化所有非阻挡格结点状态为未用
		int i, j;
		for (i = 0; i < m_nRowMax; i++)
		{
			for (j = 0; j < m_nColMax; j++)
			{
				if (m_ppNodeDataArray[i][j].nState != NODE_BLOCK)
				{
					m_ppNodeDataArray[i][j].nState = NODE_UNUSED;
				}
			}
		}
	}

	void ISeekPath::SetBlockInfo()
	{
#ifdef _DEBUG
		assert(m_nRowMax != 0 && m_nColMax != 0);
#endif

		for (int i=0; i<m_nRowMax; i++)
		{
			for (int j=0; j<m_nColMax; j++)
			{
				if (IsBlockoff(i, j))
				{
					m_ppNodeDataArray[i][j].nState = NODE_BLOCK;
				}
			}
		}

		//将不能访问的地图边缘结点也设为阻拦
		for (int i = 0; i < m_nColMax; i++)
		{
			m_ppNodeDataArray[0][i].nState = NODE_BLOCK;
			m_ppNodeDataArray[m_nRowMax - 1][i].nState = NODE_BLOCK;
		}

		for (int i = 0; i < m_nRowMax; i++)
		{
			if (i & 1)
			{
				m_ppNodeDataArray[i][0].nState = NODE_BLOCK;
				m_ppNodeDataArray[i][m_nColMax - 1].nState = NODE_BLOCK;
			}
		}

		Reset();
	}

	void ISeekPath::SetDynamicBlockInfo(const std::list<PosXYStru> &posDynamicBlock)
	{
#ifdef _DEBUG
		assert(m_nRowMax != 0 && m_nColMax != 0);

		if (posDynamicBlock.empty())
		{
			assert(0 && "设置的动态阻挡为空");
		}
#endif
		std::list<PosXYStru>::const_iterator itBeg = posDynamicBlock.begin();
		std::list<PosXYStru>::const_iterator itEnd = posDynamicBlock.end();

		for (; itBeg!=itEnd; itBeg++)
		{
			m_ppNodeDataArray[itBeg->xPos][itBeg->yPos].nState = NODE_BLOCK_DYNAMIC;
		}
	}

	bool ISeekPath::findPath(const PosXYStru &posStart, const PosXYStru &posEnd, Vect_Pos & outVectPos)
	{
		//确保开始点和结束点不是阻挡格
		if (m_ppNodeDataArray[posStart.xPos][posStart.yPos].nState == NODE_BLOCK ||
			m_ppNodeDataArray[posEnd.xPos][posEnd.yPos].nState == NODE_BLOCK)
		{
			//清空OpenList和CloseList表
			Reset();

			return false;
		}

		//1)加起始点到OpenList
		m_ppNodeDataArray[posStart.xPos][posStart.yPos].nGValue = 0;
		m_ppNodeDataArray[posStart.xPos][posStart.yPos].nFValue = 0;
		m_ppNodeDataArray[posStart.xPos][posStart.yPos].nHValue = 0;
		InsertOpenListByOrder(posStart);
		//2)加入起始点的所有邻结点到OpenList，排除阻挡格
		FindNeighbourAndInsertIntoOpenList(posStart, posEnd);
		//3)从OpenList中删除起始点， 并加入到CloseTable
		PosXYStru posNode;
		GetNextNodeInOpenList(posNode, posEnd);
		DeleteNodeInOpenList(posNode);
		InsertCloseList(posNode);

		//4)从OpenTable中选择F值最小的结点,添加到CloseTable. 因为OpenTable已经按F值大小排序过，直接取第一个节点即可
		while (m_nOpenListNodeNum != 0 && !IsInCloseTable(posEnd))
		{
			GetNextNodeInOpenList(posNode, posEnd);
			DeleteNodeInOpenList(posNode);
			InsertCloseList(posNode);
			//5)找到当前结点的所有邻结点， 并检查是否已存在于OpenTable中
			FindNeighbourAndInsertIntoOpenList(posNode, posEnd);
		}

		//WCHAR wstrTip[128];
		//wsprintf(wstrTip, L"寻路花费时间(纳秒)%d\r\n", kt.Stop());
		//OutputDebugString(wstrTip);

		//若结束结点不在CloseList中，查找失败
		if (!IsInCloseTable(posEnd))
		{
			//清空OpenList和CloseList表
			Reset();

			return false;
		}

		////复制结果到输出
		//int i;
		//for (i = 0; i < m_nCloseListNodeNum; i++)
		//{
		//	if (i >= nResultPosNum)
		//	{
		//		nResultPosNum = i + 1;
		//		return false;
		//	}
		//	if (i == 0)
		//	{
		//		posNode = m_posHeadCloseList;
		//	}
		//	else
		//	{
		//		posNode = m_ppNodeDataArray[posNode.nRow][posNode.nCol].posParentNode;
		//	}
		//	pResultPos[i] = posNode;
		//	if (posNode == posEnd)
		//	{
		//		break;
		//	}
		//}
		//nResultPosNum = i;


		//复制结果到输出
		int i;
		for (i = 0; i < m_nCloseListNodeNum; i++)
		{
			if (i == 0)
			{
				posNode = m_posHeadCloseList;
			}
			else
			{
				posNode = m_ppNodeDataArray[posNode.xPos][posNode.yPos].posParentNode;
			}

			outVectPos.insert(outVectPos.begin(), posNode);

			if (posNode == posStart)
			{
				break;
			}
		}

		outVectPos.erase(outVectPos.begin());

		//清空OpenList和CloseList表
		Reset();

		return true;
	}

	void ISeekPath::FindNeighbourAndInsertIntoOpenList(const PosXYStru &posFind, const PosXYStru &nodeEnd)
	{
		//查找邻结点，排除阻挡格,排除已在CloseTable中的结点,填写G,H,F,parentNode值

		//根据当前地图，一个结点有四个邻结点
		PosXYStru node[4];

		if (posFind.xPos & 1)
		{
			//结点在奇数行
			node[0].xPos = posFind.xPos - 1;
			node[0].yPos = posFind.yPos - 1;
			node[1].xPos = posFind.xPos - 1;
			node[1].yPos = posFind.yPos;
			node[2].xPos = posFind.xPos + 1;
			node[2].yPos = posFind.yPos - 1;
			node[3].xPos = posFind.xPos + 1;
			node[3].yPos = posFind.yPos;
		}
		else
		{
			//结点在偶数行
			node[0].xPos = posFind.xPos - 1;
			node[0].yPos = posFind.yPos;
			node[1].xPos = posFind.xPos - 1;
			node[1].yPos = posFind.yPos + 1;
			node[2].xPos = posFind.xPos + 1;
			node[2].yPos = posFind.yPos;
			node[3].xPos = posFind.xPos + 1;
			node[3].yPos = posFind.yPos + 1;
		}

		int i;
		unsigned short nNewG, nNewH, nNewF;
		for (i = 0; i < 4; i++)
		{
			if (node[i].xPos < 0 || node[i].yPos < 0 
				|| node[i].xPos >= m_nRowMax || node[i].yPos >= m_nColMax)
			{
				continue;
			}

			//忽略阻挡格
			if (m_ppNodeDataArray[node[i].xPos][node[i].yPos].nState == NODE_BLOCK || m_ppNodeDataArray[node[i].xPos][node[i].yPos].nState == NODE_BLOCK_DYNAMIC)
			{
				continue;
			}
			//忽略已经在CloseList中的结点
			if (m_ppNodeDataArray[node[i].xPos][node[i].yPos].nState == NODE_IN_CLOSE)
			{
				continue;
			}

			//计算这个邻结点的G,H,F值
			nNewG = m_ppNodeDataArray[posFind.xPos][posFind.yPos].nGValue + getLandFormWeightVal(node[i].xPos, node[i].yPos);
			nNewH = static_cast<unsigned short>(CalcDistanceBetweenTwoNode(node[i], nodeEnd));
			nNewF = nNewG + nNewH;
			if (m_ppNodeDataArray[node[i].xPos][node[i].yPos].nState == NODE_UNUSED)
			{
				//当该结点不在OpenList中，直接加入到OpenList中
				m_ppNodeDataArray[node[i].xPos][node[i].yPos].nGValue = nNewG;
				m_ppNodeDataArray[node[i].xPos][node[i].yPos].nHValue = nNewH;
				m_ppNodeDataArray[node[i].xPos][node[i].yPos].nFValue = nNewF;
				m_ppNodeDataArray[node[i].xPos][node[i].yPos].posParentNode.xPos = posFind.xPos;
				m_ppNodeDataArray[node[i].xPos][node[i].yPos].posParentNode.yPos = posFind.yPos;

				InsertOpenListByOrder(node[i]);
			}
			else
			{
				//当该结点已经在OpenList中,根据G值判断是否需要更新原来的结点G,H,F值
				if (nNewG < m_ppNodeDataArray[node[i].xPos][node[i].yPos].nGValue)
				{
					//为新路径时，更新OpenTable中该结点原来的值
					m_ppNodeDataArray[node[i].xPos][node[i].yPos].nGValue = nNewG;
					m_ppNodeDataArray[node[i].xPos][node[i].yPos].nFValue = nNewF;
					m_ppNodeDataArray[node[i].xPos][node[i].yPos].posParentNode.xPos = posFind.xPos;
					m_ppNodeDataArray[node[i].xPos][node[i].yPos].posParentNode.yPos = posFind.yPos;

					//先从OpenList中删除该结点，再重新插入到OpenList表中，以使OpenList表仍然是按F值排序的
					DeleteNodeInOpenList(node[i]);
					InsertOpenListByOrder(node[i]);
				}
			}
		}
	}

	void ISeekPath::InsertCloseList(const PosXYStru &posNode)
	{
		PosXYStru posTemp;
		posTemp = m_posHeadCloseList;
		m_posHeadCloseList = posNode;
		m_ppNodeDataArray[posNode.xPos][posNode.yPos].posLinkNode = posTemp;
		m_ppNodeDataArray[posNode.xPos][posNode.yPos].nState = NODE_IN_CLOSE;
		//增加OpenList表的结点数
		m_nCloseListNodeNum++;
	}

	void ISeekPath::InsertOpenListByOrder(const PosXYStru &posNode)
	{
		int i;
		//找到OpenList中大于的posNode结点F值的位置
		int nFValue, nGValue;
		nFValue = m_ppNodeDataArray[posNode.xPos][posNode.yPos].nFValue;
		nGValue = m_ppNodeDataArray[posNode.xPos][posNode.yPos].nGValue;
		PosXYStru posFind, posFindAhead, posTemp;
		posFind = m_posHeadOpenList;
		posFindAhead = m_posHeadOpenList;
		for (i = 0; i < m_nOpenListNodeNum; i++)
		{
			if (m_ppNodeDataArray[posFind.xPos][posFind.yPos].nFValue > nFValue)
			{
				if (posFind == m_posHeadOpenList)
				{
					//要插入点在OpenList头
					posTemp = m_posHeadOpenList;
					m_posHeadOpenList = posNode;
					m_ppNodeDataArray[posNode.xPos][posNode.yPos].posLinkNode = posTemp;
				}
				else
				{
					//插入点在posFindAhead和posFind之间
					m_ppNodeDataArray[posFindAhead.xPos][posFindAhead.yPos].posLinkNode = posNode;
					m_ppNodeDataArray[posNode.xPos][posNode.yPos].posLinkNode = posFind;
				}
				m_ppNodeDataArray[posNode.xPos][posNode.yPos].nState = NODE_IN_OPEN;
				//增加OpenList表的结点数
				m_nOpenListNodeNum++;
				return;
			}
			else
			{
				//继续查找下一个结点
				posFindAhead = posFind;
				posFind = m_ppNodeDataArray[posFind.xPos][posFind.yPos].posLinkNode;
			}
		}
		if (m_nOpenListNodeNum == 0)
		{
			//OpenList表为空
			m_posHeadOpenList = posNode;
			m_ppNodeDataArray[m_posHeadOpenList.xPos][m_posHeadOpenList.yPos].nState = NODE_IN_OPEN;	
		}
		else
		{
			//没有找到大于当前结点F值的结点，把结点插入到OpenList尾
			m_ppNodeDataArray[posFindAhead.xPos][posFindAhead.yPos].posLinkNode = posNode;
			m_ppNodeDataArray[posNode.xPos][posNode.yPos].nState = NODE_IN_OPEN;			
		}

		//增加OpenList表的结点数
		m_nOpenListNodeNum++;
	}

	void ISeekPath::DeleteNodeInOpenList(const PosXYStru &posNode)
	{
		PosXYStru posFind, posFindAhead;
		posFind = m_posHeadOpenList;
		posFindAhead = m_posHeadOpenList;
		int i;
		for (i = 0; i < m_nOpenListNodeNum; i++)
		{
			if (posFind == posNode)
			{
				if (posFind == m_posHeadOpenList)
				{
					//要删除的结点在OpenList表头
					m_posHeadOpenList = m_ppNodeDataArray[m_posHeadOpenList.xPos][m_posHeadOpenList.yPos].posLinkNode;
				}
				else
				{
					//要删除的结点在OpenList表中间
					m_ppNodeDataArray[posFindAhead.xPos][posFindAhead.yPos].posLinkNode = m_ppNodeDataArray[posFind.xPos][posFind.yPos].posLinkNode;
				}
				m_ppNodeDataArray[posNode.xPos][posNode.yPos].nState = NODE_UNUSED;
				m_nOpenListNodeNum--;
				return;
			}
			else
			{
				//继续查找下一个结点
				posFindAhead = posFind;
				posFind = m_ppNodeDataArray[posFind.xPos][posFind.yPos].posLinkNode;
			}
		}
	}

	bool ISeekPath::IsInCloseTable(const PosXYStru &nodeTest)
	{
		if (m_ppNodeDataArray[nodeTest.xPos][nodeTest.yPos].nState == NODE_IN_CLOSE)
			return true;

		return false;
	}

	int ISeekPath::CalcDistanceBetweenTwoNode(const PosXYStru &pos1, const PosXYStru &pos2)
	{
		PosXYStru posStart, posEnd;
		posStart = pos1;
		posEnd = pos2;
		coordinateTransition(posStart.xPos , posStart.yPos);
		coordinateTransition(posEnd.xPos , posEnd.yPos);
		return abs(posEnd.yPos - posStart.yPos) > abs(posEnd.xPos - posStart.xPos) ? abs(posEnd.yPos - posStart.yPos) : abs(posEnd.xPos - posStart.xPos);
	}

	void ISeekPath::GetNextNodeInOpenList(PosXYStru &posSel, const PosXYStru &posEnd)
	{
		PosXYStru node1, node2;
		node1 = m_posHeadOpenList;
		if (m_nOpenListNodeNum > 2)
		{
			//查看是否第2个结点和第1个结点的F值和H值一样
			node2 = m_ppNodeDataArray[node1.xPos][node1.yPos].posLinkNode;
			if (m_ppNodeDataArray[node1.xPos][node1.yPos].nFValue == m_ppNodeDataArray[node2.xPos][node2.yPos].nFValue && 
				m_ppNodeDataArray[node1.xPos][node1.yPos].nHValue == m_ppNodeDataArray[node2.xPos][node2.yPos].nHValue)
			{
				if (abs(node2.xPos - posEnd.xPos) + abs(node2.yPos - posEnd.yPos) < 
					abs(node1.xPos - posEnd.xPos) + abs(node1.yPos - posEnd.yPos))
				{
					//比较node1和node2哪一个在直线距离上离末结点更近, 若第2个结点更近，则选择第2个
					posSel = node2;
					return;
				}
			}
		}
		//直接选择第1个结点
		posSel = node1;
	}
}