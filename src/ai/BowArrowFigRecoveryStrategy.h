// @brief ��Ҫ����: �����ַ�������
// @author ����: �� �� ��

#ifndef _BOW_ARROW_FIGRECOVERYSTRATEGY
#define _BOW_ARROW_FIGRECOVERYSTRATEGY

#include "ibowarrowfigstragegy.h"

class BowArrowFigRecoveryStrategy :
	public IBowArrowFigStrategy
{
public:
	BowArrowFigRecoveryStrategy(void);
public:
	virtual ~BowArrowFigRecoveryStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, IManager *strategyMgr, bool isEven);


private:
	// �����������жϵ�ǰ�����Ƿ����㳷������
	// @param mapDataAndFraise ��ͼ�ϰ���ṹ
	// @param armyObjParam AI���ҵľ���
	// @return ���㷵���ǣ����򷵻ط�
	bool isSatisfyCondiOfCountermarch(IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj) const;


	bool callFriendArmyToReinforce( std::list<IArmyPtr> &lstFriendGroup, bool &isSucess, IArmyPtrvectInvect * mapDataParamPtr, IArmyPtr armyObj, IManager * strategyMgr, IArmyPtr &armyPtrTmp, IArmyPtr EnemyInGunshotPtr ) const;
};

#endif
