// @brief ��Ҫ����: Ѱ·�㷨��
// @author ����:

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
		//�����������к������
		m_nRowMax = nRowMax;
		m_nColMax = nColMax;

		//��������к�����з��������н����Ϣ�Ķ�ά����
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
		//�ͷŽ���ά����
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

		//��ʼ�����з��赲����״̬Ϊδ��
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

		//�����ܷ��ʵĵ�ͼ��Ե���Ҳ��Ϊ����
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
			assert(0 && "���õĶ�̬�赲Ϊ��");
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
		//ȷ����ʼ��ͽ����㲻���赲��
		if (m_ppNodeDataArray[posStart.xPos][posStart.yPos].nState == NODE_BLOCK ||
			m_ppNodeDataArray[posEnd.xPos][posEnd.yPos].nState == NODE_BLOCK)
		{
			//���OpenList��CloseList��
			Reset();

			return false;
		}

		//1)����ʼ�㵽OpenList
		m_ppNodeDataArray[posStart.xPos][posStart.yPos].nGValue = 0;
		m_ppNodeDataArray[posStart.xPos][posStart.yPos].nFValue = 0;
		m_ppNodeDataArray[posStart.xPos][posStart.yPos].nHValue = 0;
		InsertOpenListByOrder(posStart);
		//2)������ʼ��������ڽ�㵽OpenList���ų��赲��
		FindNeighbourAndInsertIntoOpenList(posStart, posEnd);
		//3)��OpenList��ɾ����ʼ�㣬 �����뵽CloseTable
		PosXYStru posNode;
		GetNextNodeInOpenList(posNode, posEnd);
		DeleteNodeInOpenList(posNode);
		InsertCloseList(posNode);

		//4)��OpenTable��ѡ��Fֵ��С�Ľ��,��ӵ�CloseTable. ��ΪOpenTable�Ѿ���Fֵ��С�������ֱ��ȡ��һ���ڵ㼴��
		while (m_nOpenListNodeNum != 0 && !IsInCloseTable(posEnd))
		{
			GetNextNodeInOpenList(posNode, posEnd);
			DeleteNodeInOpenList(posNode);
			InsertCloseList(posNode);
			//5)�ҵ���ǰ���������ڽ�㣬 ������Ƿ��Ѵ�����OpenTable��
			FindNeighbourAndInsertIntoOpenList(posNode, posEnd);
		}

		//WCHAR wstrTip[128];
		//wsprintf(wstrTip, L"Ѱ·����ʱ��(����)%d\r\n", kt.Stop());
		//OutputDebugString(wstrTip);

		//��������㲻��CloseList�У�����ʧ��
		if (!IsInCloseTable(posEnd))
		{
			//���OpenList��CloseList��
			Reset();

			return false;
		}

		////���ƽ�������
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


		//���ƽ�������
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

		//���OpenList��CloseList��
		Reset();

		return true;
	}

	void ISeekPath::FindNeighbourAndInsertIntoOpenList(const PosXYStru &posFind, const PosXYStru &nodeEnd)
	{
		//�����ڽ�㣬�ų��赲��,�ų�����CloseTable�еĽ��,��дG,H,F,parentNodeֵ

		//���ݵ�ǰ��ͼ��һ��������ĸ��ڽ��
		PosXYStru node[4];

		if (posFind.xPos & 1)
		{
			//�����������
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
			//�����ż����
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

			//�����赲��
			if (m_ppNodeDataArray[node[i].xPos][node[i].yPos].nState == NODE_BLOCK || m_ppNodeDataArray[node[i].xPos][node[i].yPos].nState == NODE_BLOCK_DYNAMIC)
			{
				continue;
			}
			//�����Ѿ���CloseList�еĽ��
			if (m_ppNodeDataArray[node[i].xPos][node[i].yPos].nState == NODE_IN_CLOSE)
			{
				continue;
			}

			//��������ڽ���G,H,Fֵ
			nNewG = m_ppNodeDataArray[posFind.xPos][posFind.yPos].nGValue + getLandFormWeightVal(node[i].xPos, node[i].yPos);
			nNewH = static_cast<unsigned short>(CalcDistanceBetweenTwoNode(node[i], nodeEnd));
			nNewF = nNewG + nNewH;
			if (m_ppNodeDataArray[node[i].xPos][node[i].yPos].nState == NODE_UNUSED)
			{
				//���ý�㲻��OpenList�У�ֱ�Ӽ��뵽OpenList��
				m_ppNodeDataArray[node[i].xPos][node[i].yPos].nGValue = nNewG;
				m_ppNodeDataArray[node[i].xPos][node[i].yPos].nHValue = nNewH;
				m_ppNodeDataArray[node[i].xPos][node[i].yPos].nFValue = nNewF;
				m_ppNodeDataArray[node[i].xPos][node[i].yPos].posParentNode.xPos = posFind.xPos;
				m_ppNodeDataArray[node[i].xPos][node[i].yPos].posParentNode.yPos = posFind.yPos;

				InsertOpenListByOrder(node[i]);
			}
			else
			{
				//���ý���Ѿ���OpenList��,����Gֵ�ж��Ƿ���Ҫ����ԭ���Ľ��G,H,Fֵ
				if (nNewG < m_ppNodeDataArray[node[i].xPos][node[i].yPos].nGValue)
				{
					//Ϊ��·��ʱ������OpenTable�иý��ԭ����ֵ
					m_ppNodeDataArray[node[i].xPos][node[i].yPos].nGValue = nNewG;
					m_ppNodeDataArray[node[i].xPos][node[i].yPos].nFValue = nNewF;
					m_ppNodeDataArray[node[i].xPos][node[i].yPos].posParentNode.xPos = posFind.xPos;
					m_ppNodeDataArray[node[i].xPos][node[i].yPos].posParentNode.yPos = posFind.yPos;

					//�ȴ�OpenList��ɾ���ý�㣬�����²��뵽OpenList���У���ʹOpenList����Ȼ�ǰ�Fֵ�����
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
		//����OpenList��Ľ����
		m_nCloseListNodeNum++;
	}

	void ISeekPath::InsertOpenListByOrder(const PosXYStru &posNode)
	{
		int i;
		//�ҵ�OpenList�д��ڵ�posNode���Fֵ��λ��
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
					//Ҫ�������OpenListͷ
					posTemp = m_posHeadOpenList;
					m_posHeadOpenList = posNode;
					m_ppNodeDataArray[posNode.xPos][posNode.yPos].posLinkNode = posTemp;
				}
				else
				{
					//�������posFindAhead��posFind֮��
					m_ppNodeDataArray[posFindAhead.xPos][posFindAhead.yPos].posLinkNode = posNode;
					m_ppNodeDataArray[posNode.xPos][posNode.yPos].posLinkNode = posFind;
				}
				m_ppNodeDataArray[posNode.xPos][posNode.yPos].nState = NODE_IN_OPEN;
				//����OpenList��Ľ����
				m_nOpenListNodeNum++;
				return;
			}
			else
			{
				//����������һ�����
				posFindAhead = posFind;
				posFind = m_ppNodeDataArray[posFind.xPos][posFind.yPos].posLinkNode;
			}
		}
		if (m_nOpenListNodeNum == 0)
		{
			//OpenList��Ϊ��
			m_posHeadOpenList = posNode;
			m_ppNodeDataArray[m_posHeadOpenList.xPos][m_posHeadOpenList.yPos].nState = NODE_IN_OPEN;	
		}
		else
		{
			//û���ҵ����ڵ�ǰ���Fֵ�Ľ�㣬�ѽ����뵽OpenListβ
			m_ppNodeDataArray[posFindAhead.xPos][posFindAhead.yPos].posLinkNode = posNode;
			m_ppNodeDataArray[posNode.xPos][posNode.yPos].nState = NODE_IN_OPEN;			
		}

		//����OpenList��Ľ����
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
					//Ҫɾ���Ľ����OpenList��ͷ
					m_posHeadOpenList = m_ppNodeDataArray[m_posHeadOpenList.xPos][m_posHeadOpenList.yPos].posLinkNode;
				}
				else
				{
					//Ҫɾ���Ľ����OpenList���м�
					m_ppNodeDataArray[posFindAhead.xPos][posFindAhead.yPos].posLinkNode = m_ppNodeDataArray[posFind.xPos][posFind.yPos].posLinkNode;
				}
				m_ppNodeDataArray[posNode.xPos][posNode.yPos].nState = NODE_UNUSED;
				m_nOpenListNodeNum--;
				return;
			}
			else
			{
				//����������һ�����
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
			//�鿴�Ƿ��2�����͵�1������Fֵ��Hֵһ��
			node2 = m_ppNodeDataArray[node1.xPos][node1.yPos].posLinkNode;
			if (m_ppNodeDataArray[node1.xPos][node1.yPos].nFValue == m_ppNodeDataArray[node2.xPos][node2.yPos].nFValue && 
				m_ppNodeDataArray[node1.xPos][node1.yPos].nHValue == m_ppNodeDataArray[node2.xPos][node2.yPos].nHValue)
			{
				if (abs(node2.xPos - posEnd.xPos) + abs(node2.yPos - posEnd.yPos) < 
					abs(node1.xPos - posEnd.xPos) + abs(node1.yPos - posEnd.yPos))
				{
					//�Ƚ�node1��node2��һ����ֱ�߾�������ĩ������, ����2������������ѡ���2��
					posSel = node2;
					return;
				}
			}
		}
		//ֱ��ѡ���1�����
		posSel = node1;
	}
}