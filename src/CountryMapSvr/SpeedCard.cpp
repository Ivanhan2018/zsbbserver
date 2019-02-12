#include "SpeedCard.h"
#include "log.hpp"
#pragma warning(disable:4127)
CSpeedCard								g_SpeedCard;
CSpeedCard::CSpeedCard()
{
	m_mapPlayerCard.clear();
}
CSpeedCard::~CSpeedCard()
{
}

//����֪ͨ
void CSpeedCard::IncreaseSpeed(void * pDataBuffer, int iDataSize)
{
	if (iDataSize != sizeof(tagNotify_IncreaseSpeed_Request)) 
	{
		LOG_TRACE_EX(false,("CSpeedCard::IncreaseSpeed-->��Ϣ���Ƿ�\n"));
		return;
	}

	tagNotify_IncreaseSpeed_Request *pInSpeed = (tagNotify_IncreaseSpeed_Request *)pDataBuffer;

	int ret = FindPlayer(pInSpeed->PlayerID);
	if (ret == 0)
		ret = AddPlayer(pInSpeed->PlayerID);
	if (ret == 0) return;
	int iValue = (pInSpeed->Value+100);///100;//modify by xujincai 20090107 iValue=(pInSpeed->Value+100)/100,iValue��pInSpeed����[0,100����һֱΪ1
	switch(pInSpeed->Type)
	{
	case AllResource_Speed_Card:		//�������ٿ���������Դ����������
		//SetProduceValue(pInSpeed->PlayerID, iValue);
		SetFoodValue(pInSpeed->PlayerID, iValue);
		SetWoodValue(pInSpeed->PlayerID, iValue);
		SetSkinValue(pInSpeed->PlayerID, iValue);
		SetIronValue(pInSpeed->PlayerID, iValue);
		break;
	case Food_Speed_Card:				//��ʳ���ٿ�
		SetFoodValue(pInSpeed->PlayerID, iValue);
		break;
	case Wood_Speed_Card:				//ľ�����ٿ�
		SetWoodValue(pInSpeed->PlayerID, iValue);
		break;
	case Skin_Speed_Card:				//Ƥ�����ٿ�
		SetSkinValue(pInSpeed->PlayerID, iValue);
		break;
	case Iron_Speed_Card:				//�������ٿ�
		SetIronValue(pInSpeed->PlayerID, iValue);
		break;
	case Construction_Speed_Card:		//�������ٿ�
		SetConstrValue(pInSpeed->PlayerID, iValue);
		break;
	case Conscription_Speed_Card:		//�������ٿ�
		SetConscrValue(pInSpeed->PlayerID, iValue);
		break;
	case Research_Speed_Card:			//�������ٿ�
		SetResearchValue(pInSpeed->PlayerID, iValue);
		break;
	case Recruit_Card:					//��ļӢ�ۿ�
		SetRecruitValue(pInSpeed->PlayerID, iValue);
		break;
	default:
		LOG_TRACE_EX(false,("CSpeedCard::IncreaseSpeed-->���%dδ֪�ļ�����Ϣ%d,���٣�%d\n",pInSpeed->PlayerID,pInSpeed->Type,pInSpeed->Value));
		break;
	};
}
//�ָ������ٶ�
void CSpeedCard::NormalSpeed(void * pDataBuffer, int iDataSize)
{
	if (iDataSize != sizeof(tagNotify_NormalSpeed_Request)) 
	{
		LOG_TRACE_EX(false,("CSpeedCard::NormalSpeed-->��Ϣ���Ƿ�\n"));
		return;
	}

	tagNotify_NormalSpeed_Request *pNorSpeed = (tagNotify_NormalSpeed_Request *)pDataBuffer;
	int iValue = 100;//modify by xujincai 20090107 ��������ʹ�ðٷֱȣ�1-��100
	switch(pNorSpeed->Type)
	{
	case AllResource_Speed_Card:		//�������ٿ���������Դ����������
		//SetProduceValue(pNorSpeed->PlayerID, iValue);
		SetFoodValue(pNorSpeed->PlayerID, iValue);
		SetWoodValue(pNorSpeed->PlayerID, iValue);
		SetSkinValue(pNorSpeed->PlayerID, iValue);
		SetIronValue(pNorSpeed->PlayerID, iValue);
		break;
	case Food_Speed_Card:				//��ʳ���ٿ�
		SetFoodValue(pNorSpeed->PlayerID, iValue);
		break;
	case Wood_Speed_Card:				//ľ�����ٿ�
		SetWoodValue(pNorSpeed->PlayerID, iValue);
		break;
	case Skin_Speed_Card:				//Ƥ�����ٿ�
		SetSkinValue(pNorSpeed->PlayerID, iValue);
		break;
	case Iron_Speed_Card:				//�������ٿ�
		SetIronValue(pNorSpeed->PlayerID, iValue);
		break;
	case Construction_Speed_Card:		//�������ٿ�
		SetConstrValue(pNorSpeed->PlayerID, iValue);
		break;
	case Conscription_Speed_Card:		//�������ٿ�
		SetConscrValue(pNorSpeed->PlayerID, iValue);
		break;
	case Research_Speed_Card:			//�������ٿ�
		SetResearchValue(pNorSpeed->PlayerID, iValue);
		break;
	case Recruit_Card:					//��ļӢ�ۿ�
		SetRecruitValue(pNorSpeed->PlayerID, iValue);
		break;
	};
}
int CSpeedCard::AddPlayer(unsigned long lPlayerID)
{
	tagSpeedCard * pSpeedCard = new tagSpeedCard;
	if (pSpeedCard == NULL) return 0;
	memset(pSpeedCard,0,sizeof(tagSpeedCard));
	m_mapPlayerCard.insert(make_pair(lPlayerID, pSpeedCard));
	return 1;
}
int CSpeedCard::DeletePlayer(unsigned long lPlayerID)
{
	tagSpeedCard * pSpeedCard = NULL;
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
	{
		pSpeedCard = pIt->second;
		delete pSpeedCard ;
		pSpeedCard = NULL;
		m_mapPlayerCard.erase(lPlayerID);
		return 1;
	}
	return 0;
}
int CSpeedCard::FindPlayer(unsigned long lPlayerID)
{
	/*del by xujincai 20090124
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt->second == NULL || pIt->first <= 0) return 0;
	*/
	
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if(pIt == m_mapPlayerCard.end())
		return 0;
	return 1;
}
void CSpeedCard::ClearPlayer()
{
	map<unsigned long, tagSpeedCard*>::iterator SpeedCard;
	tagSpeedCard * pSpeedCard = NULL;
	for (SpeedCard = m_mapPlayerCard.begin() ; SpeedCard != m_mapPlayerCard.end() ; SpeedCard++ )
	{
		pSpeedCard = SpeedCard->second;
		delete pSpeedCard;
		pSpeedCard = NULL;
	}
	m_mapPlayerCard.clear();
}
tagSpeedCard * CSpeedCard::GetSpeedCardInfo(unsigned long lPlayerID)
{
	tagSpeedCard * pSpeedCard = NULL;
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		pSpeedCard = pIt->second;

	return pSpeedCard;
}
void CSpeedCard::SetProduceValue(unsigned long lPlayerID, int iValue)
{
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		pIt->second->iProduceValue = iValue;
}
void CSpeedCard::SetFoodValue(unsigned long lPlayerID, int iValue)
{
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		pIt->second->iFoodValue = iValue;
}
void CSpeedCard::SetWoodValue(unsigned long lPlayerID, int iValue)
{
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		pIt->second->iWoodValue = iValue;
}
void CSpeedCard::SetSkinValue(unsigned long lPlayerID, int iValue)
{
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		pIt->second->iSkinValue = iValue;
}
void CSpeedCard::SetIronValue(unsigned long lPlayerID, int iValue)
{
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
if (pIt != m_mapPlayerCard.end())
		pIt->second->iIronValue = iValue;
}
void CSpeedCard::SetConstrValue(unsigned long lPlayerID, int iValue)
{
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		pIt->second->iConstrValue = iValue;
}
void CSpeedCard::SetConscrValue(unsigned long lPlayerID, int iValue)
{
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		pIt->second->iConscrValue = iValue;
}
void CSpeedCard::SetResearchValue(unsigned long lPlayerID, int iValue)
{
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		pIt->second->iResearchValue = iValue;
}
void CSpeedCard::SetRecruitValue(unsigned long lPlayerID, int iValue)
{
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		pIt->second->iRecruitValue = iValue;
}
int CSpeedCard::GetProduceValue(unsigned long lPlayerID)
{
	int iValue = 100;
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		iValue = pIt->second->iProduceValue;//iConscrValue;
	if (iValue < 100) iValue = 100;//modify by xujincai 20090107 ����ʹ�ðٷֱ�iValue=1->iValue =100
	return iValue;
}
int CSpeedCard::GetFoodValue(unsigned long lPlayerID)
{
	int iValue = 100;
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		iValue = pIt->second->iFoodValue;//iConscrValue;
	if (iValue < 100) iValue = 100;//modify by xujincai 20090107 ����ʹ�ðٷֱ�iValue=1->iValue =100
	return iValue;
}
int CSpeedCard::GetWoodValue(unsigned long lPlayerID)
{
	int iValue = 100;
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		iValue = pIt->second->iWoodValue;//iConscrValue;
	if (iValue < 100) iValue = 100;//modify by xujincai 20090107 ����ʹ�ðٷֱ�iValue=1->iValue =100
	return iValue;
}
int CSpeedCard::GetSkinValue(unsigned long lPlayerID)
{
	int iValue = 100;
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		iValue = pIt->second->iSkinValue;//iConscrValue;
	if (iValue < 100) iValue = 100;//modify by xujincai 20090107 ����ʹ�ðٷֱ�iValue=1->iValue =100
	return iValue;
}
int CSpeedCard::GetIronValue(unsigned long lPlayerID)
{
	int iValue = 100;
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		iValue = pIt->second->iIronValue;//iConscrValue;
	if (iValue < 100) iValue = 100;//modify by xujincai 20090107 ����ʹ�ðٷֱ�iValue=1->iValue =100
	return iValue;
}
int CSpeedCard::GetConstrValue(unsigned long lPlayerID)
{
	int iValue = 100;
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		iValue = pIt->second->iConstrValue;//iConscrValue;
	if (iValue < 100) iValue = 100;//modify by xujincai 20090107 ����ʹ�ðٷֱ�iValue=1->iValue =100
	return iValue;
}
int CSpeedCard::GetConscrValue(unsigned long lPlayerID)
{
	int iValue = 100;
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		iValue = pIt->second->iConscrValue;
	if (iValue < 100) iValue = 100;//modify by xujincai 20090107 ����ʹ�ðٷֱ�iValue=1->iValue =100
	return iValue;
}
int CSpeedCard::GetResearchValue(unsigned long lPlayerID)
{
	int iValue = 100;
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		iValue = pIt->second->iResearchValue;
	if (iValue < 100) iValue = 100;//modify by xujincai 20090107 ����ʹ�ðٷֱ�iValue=1->iValue =100
	return iValue;
}
int CSpeedCard::GetRecruitValue(unsigned long lPlayerID)
{
	int iValue = 100;
	map<unsigned long, tagSpeedCard*>::iterator pIt= m_mapPlayerCard.find(lPlayerID);
	if (pIt != m_mapPlayerCard.end())
		iValue = pIt->second->iRecruitValue;
	if (iValue < 100) iValue = 100;//modify by xujincai 20090107 ����ʹ�ðٷֱ�iValue=1->iValue =100
	return iValue;
}