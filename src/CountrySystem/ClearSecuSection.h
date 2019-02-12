// @brief 简要描述: 清扫安全区的实现类
// @author 作者: 黄 文 才

#ifndef _CLEARING_SECURITY_SECTION_H
#define _CLEARING_SECURITY_SECTION_H

#include <list>
#include <vector>
#include "CommonFun.h"

class ClearSecuSection
{
public:
	enum
	{
		oneCellMaxHasArmyNum = 5,
		secuSectionAreaVal = 6
	};

	ClearSecuSection(MapDataStru & mapDataAndFraise, unsigned int secuSectionArea = secuSectionAreaVal);
	virtual ~ClearSecuSection(void);

	// 函数描述:清扫安全区
	// @return 成功与否
	bool clearingSecuSection();

private:
	// 函数描述:清扫一个安全区
	// @param propagateParam 传送门结构体类型数据
	// @return 成功与否
	bool disposeOneSecuSection(const EncampmentInfo &propagateParam);

	MapDataStru mapDataAndFraise_;		// 部队，障碍物地图数据结构
	unsigned int secuSectionArea_;		// 安全区的范围，如这个值为6，则表示6圈内为安全区
};

#endif