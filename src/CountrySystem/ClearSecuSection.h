// @brief ��Ҫ����: ��ɨ��ȫ����ʵ����
// @author ����: �� �� ��

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

	// ��������:��ɨ��ȫ��
	// @return �ɹ����
	bool clearingSecuSection();

private:
	// ��������:��ɨһ����ȫ��
	// @param propagateParam �����Žṹ����������
	// @return �ɹ����
	bool disposeOneSecuSection(const EncampmentInfo &propagateParam);

	MapDataStru mapDataAndFraise_;		// ���ӣ��ϰ����ͼ���ݽṹ
	unsigned int secuSectionArea_;		// ��ȫ���ķ�Χ�������ֵΪ6�����ʾ6Ȧ��Ϊ��ȫ��
};

#endif