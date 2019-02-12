// @brief ��Ҫ����: ��������������
// @author ����: �� �� ��

#ifndef _BOW_ARROW_FIGSELFDEFSTRATEGY
#define _BOW_ARROW_FIGSELFDEFSTRATEGY

#include "ibowarrowfigstragegy.h"

class BowArrowFigSelfdefStrategy :
	public IBowArrowFigStrategy
{
public:
	BowArrowFigSelfdefStrategy();
public:
	virtual ~BowArrowFigSelfdefStrategy(void);

	// ����������ִ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param armyObj ��ǰ�����Ĳ���
	// @param strategyMgr ���Թ�����
	// @return ����true / false �� true: �ܹ���  false��������
	bool thinking(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, IArmyPtr armyObjParam, IManager *strategyMgr, bool isEven);
private:
	// ����������ȷ���������Ĳ����Ƿ������ѣ�������⣬������Ұ��Χ�ڣ�
	// @param armyObjParam  �������ӵĶ��󣨵�ǰAI���ƵĲ��ӣ�
	// @param LastAttackTarget �������Ĳ��ӵ�����ָ��
	// @return �Ƿ�������
	bool isAttackArmyIsOutruning(const IArmyPtr armyObjParam, const IArmyPtr LastAttackTarget) const;

	// ����������������������޵о����Ƿ񷴻��������ĵо�����
	// @param seekPathArithmParamPtr Ѱ·�����ָ��
	// @param LastAttackedSelf���һ�������Լ��ĵо�
	// @param EnemyInGunshotPtr������ڵĵо�
	// @param Command��Ҫ���ظ����ӵ�����ṹ
	// @return �ɹ����
	bool thinkingInOffset(IArmyPtrvectInvect * mapDataParamPtr, SeekPath * seekPathArithmParamPtr, const IArmyPtr LastAttackedSelf, const IArmyPtr EnemyInGunshotPtr, IArmyPtr armyObjParam, IManager *strategyMgr, bool isEven) const;
};

#endif
