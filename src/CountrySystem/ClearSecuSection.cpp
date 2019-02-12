// @brief 简要描述: 清扫安全区的实现类
// @author 作者: 黄 文 才

#include "ClearSecuSection.h"

ClearSecuSection::ClearSecuSection(MapDataStru & mapDataAndFraise, unsigned int secuSectionArea)
: mapDataAndFraise_(mapDataAndFraise), secuSectionArea_(secuSectionArea)
{
}

ClearSecuSection::~ClearSecuSection(void)
{
}

bool ClearSecuSection::clearingSecuSection()
{
	bool isSucess = false;

	std::list<EncampmentInfo>  &encampmentInfolst = (mapDataAndFraise_.ClassImpedient)->EncampmentPos;
	std::list<EncampmentInfo>::iterator itBeg = encampmentInfolst.begin();
	std::list<EncampmentInfo>::iterator itEnd = encampmentInfolst.end();

	for (; itBeg!=itEnd; itBeg++)
	{
		isSucess = disposeOneSecuSection(*itBeg);
		
		if (!isSucess)
		{
			return false;
		}
	}	

	return true;
}

bool ClearSecuSection::disposeOneSecuSection(const EncampmentInfo &propagateParam)
{
	IArmyPtrvectInvect * armyPtrvectInvect = mapDataAndFraise_.MapData;

	CLoadMapInfo *ClassImpedient = mapDataAndFraise_.ClassImpedient;

	if (NULL == armyPtrvectInvect || NULL == ClassImpedient)
	{
		return false;
	}
	
	const POS_STRUCT &posStru = propagateParam.EncapentPos;
	PosXYStru posCenter(posStru.PosX, posStru.PosY);
	bool isSucess = false;
	std::list<IArmyPtr> lstArmyPtr;
	std::vector<PosXYStru> vectPos, vectTmp;
	std::list<IArmyPtr>::iterator itArmyPtrBeg, itArmyPtrEnd;
	unsigned int  canAriseArmyNum = 0;

	// 收集安全区上的所有点坐标
	for (unsigned int i=1; i<=secuSectionArea_; ++i)
	{
		vectTmp.clear(); 
		isSucess = CommonFunctionSingleton::getInstance()->GetAllSpecifyPos(posCenter, i, vectTmp, armyPtrvectInvect);

		if (!isSucess)
		{
			return false;
		}
		
		vectPos.insert(vectPos.end(), vectTmp.begin(), vectTmp.end());
	}

	// 统计安全区上的所有部队
	std::vector<PosXYStru>::iterator itBeg = vectPos.begin();
	std::vector<PosXYStru>::iterator itEnd = vectPos.end();
	unsigned int lstSize = 0;
	vector<POS_STRUCT> moveTrack;

	for (; itBeg!=itEnd; ++itBeg)
	{
		if (ClassImpedient->IsBlockoff(itBeg->xPos, itBeg->yPos))
		{
			continue;
		}

		const list<IArmyPtr> &lstIArmyPtr = (*armyPtrvectInvect)[itBeg->xPos][itBeg->yPos];
	
		lstArmyPtr.insert(lstArmyPtr.end(), lstIArmyPtr.begin(), lstIArmyPtr.end());
	}

	// 把安全区内的部队踢出安全区
	for (unsigned int i=secuSectionArea_+1; !lstArmyPtr.empty(); ++i)
	{
		vectTmp.clear();
		isSucess = CommonFunctionSingleton::getInstance()->GetAllSpecifyPos(posCenter, i, vectTmp, armyPtrvectInvect);

		if (!isSucess)
		{
			return false;
		}

		itBeg = vectTmp.begin();
		itEnd = vectTmp.end();

		for (; itBeg!=itEnd && !lstArmyPtr.empty(); ++itBeg)
		{
			if (ClassImpedient->IsBlockoff(itBeg->xPos, itBeg->yPos))
			{
				continue;
			}

			const list<IArmyPtr> &lstIArmyPtr = (*armyPtrvectInvect)[itBeg->xPos][itBeg->yPos];	
			lstSize = static_cast<unsigned int>(lstIArmyPtr.size());

			if (lstSize >= oneCellMaxHasArmyNum)
			{
				continue;
			}
			else
			{
				itArmyPtrBeg = lstArmyPtr.begin();
				canAriseArmyNum = oneCellMaxHasArmyNum - lstSize;

				while (canAriseArmyNum-- && itArmyPtrBeg != lstArmyPtr.end())
				{
					moveTrack.clear();
					POS_STRUCT posXY;
					posXY.PosX = itBeg->xPos;
					posXY.PosY = itBeg->yPos;

					moveTrack.push_back(posXY);

					(*itArmyPtrBeg)->Move(moveTrack, *itArmyPtrBeg);

					itArmyPtrBeg = lstArmyPtr.erase(itArmyPtrBeg);
				}
			}
		}
	}

	return true;
}