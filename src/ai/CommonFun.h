// @brief ��Ҫ����: �����ӿں��� 
// @author ����: �� �� ��

#ifndef _COMMON_FUN_H_H_H
#define _COMMON_FUN_H_H_H

//#include "boost/noncopyable.hpp"
#include <vector>
#include "FactoryCreateProfessionMger.h"

using namespace aistrategy;

enum 
{ 
	ONE_CIRCLE = 1,
	FOUR_CIRCLE = 4,
	EIGHT_CIRCLE = 8
};

// �ƶ����������������
struct FunParam
{
	IArmyPtrvectInvect * mapDataParamPtr;	// ���ӵ�ͼ����ָ��
	SeekPath * seekPathPtr;					// Ѱ·�����ָ��
	ENUM_ARMY_STATUS &armyCurrentStatus;    // ��ǰ�Ĳ���״̬
	vector<PosXYStru> &movePosVector;		// �ƶ���Ŀ�ĵ�����
	PosXYStru &moveTagetPos;				// Ŀ���ƶ��ĵ�����
	unsigned int &enemyID;					// �о�����ID
	IArmyPtr armyPtr;						// ��ǰ�����Ĳ���
	IManager * strategyManagerPtr;			// ���Թ�������ָ��

	FunParam(IArmyPtrvectInvect * mapDataParamPtrTmp,
		SeekPath * seekPathPtrTmp, ENUM_ARMY_STATUS &armyCurrentStatusTmp, vector<PosXYStru> &movePosVectorTmp, PosXYStru &moveTagetPosTmp, unsigned int &enemyIDTmp,
		IArmyPtr armyPtrTmp, IManager * strategyManagerPtrTmp)
		: mapDataParamPtr(mapDataParamPtrTmp), seekPathPtr(seekPathPtrTmp), armyCurrentStatus(armyCurrentStatusTmp), movePosVector(movePosVectorTmp), 
		  moveTagetPos(moveTagetPosTmp), enemyID(enemyIDTmp), armyPtr(armyPtrTmp), strategyManagerPtr(strategyManagerPtrTmp)
	{

	}

private:
	FunParam();
	FunParam & operator = (const FunParam &attOrMoveFunParam);
};

// ��������
struct attackFunParam
{
	FunParam &funParam; 
	unsigned int attackTargetArmyID;		// �Ƿ�ִ�к�������

	attackFunParam(FunParam &funParamTmp,
		unsigned int attackTargetArmyIDParam)
		:  funParam(funParamTmp),
		attackTargetArmyID(attackTargetArmyIDParam)
	{

	}

private:
	attackFunParam();
	attackFunParam & operator = (const attackFunParam &attOrMoveFunParam);
};

// �ƶ�����
struct moveFunParam
{
	FunParam &funParam; 
	PosXYStru &posTarget;					// �ƶ�Ŀ���

	moveFunParam(FunParam &funParamTmp,
		PosXYStru &posTargetParam)
		:  funParam(funParamTmp),
		posTarget(posTargetParam)
	{

	}

private:
	moveFunParam();
	moveFunParam & operator = (const moveFunParam &attOrMoveFunParam);
};

typedef std::set<std::pair<int, int> > pairInSet;
typedef std::vector<IArmyPtr> ArmyPtrVector;
typedef std::list<IArmyPtr> listArmyPtr;
typedef const std::list<IArmyPtr>::const_iterator IArmyConstIter;

class StatFriendArmyAndEnemyStru
{
public:
	StatFriendArmyAndEnemyStru(
	IArmyPtr armyObjParam,
	IArmyPtrvectInvect * mapDataPtr,
	bool isOnlyBuilding,
	bool isOnlyTrusteeship,
	listArmyPtr & lstFriendArmy,
	listArmyPtr & lstEnemy,
	int & countCellsHasEnemy) :
	armyObjParam_(armyObjParam), mapDataPtr_(mapDataPtr), isOnlyBuilding_(isOnlyBuilding), isOnlyTrusteeship_(isOnlyTrusteeship), lstFriendArmy_(lstFriendArmy),
	lstEnemy_(lstEnemy), countCellsHasEnemy_(countCellsHasEnemy)
	{

	}
	
	bool fun(int xPos, int yPos);

private:
	IArmyPtr armyObjParam_;
	const IArmyPtrvectInvect * mapDataPtr_;
	bool isOnlyBuilding_;
	bool isOnlyTrusteeship_;
	listArmyPtr & lstFriendArmy_;
	listArmyPtr & lstEnemy_;
	int & countCellsHasEnemy_;

	StatFriendArmyAndEnemyStru();
	StatFriendArmyAndEnemyStru(const StatFriendArmyAndEnemyStru &attOrMoveFunParam);
	StatFriendArmyAndEnemyStru & operator = (const StatFriendArmyAndEnemyStru &attOrMoveFunParam);
};

class SpecifyDistAllPosHasArmy
{
public:
	SpecifyDistAllPosHasArmy(const IArmyPtrvectInvect * mapDataPtr,
	const CLoadMapInfo * staticBlockMapPtr,
	std::list<PosXYStru> & lstAllPosHasArmy) : mapDataPtr_(mapDataPtr), staticBlockMapPtr_(staticBlockMapPtr),
	lstAllPosHasArmy_(lstAllPosHasArmy)
	{

	}

	bool fun(int xPos, int yPos);

private:
	const IArmyPtrvectInvect * mapDataPtr_;
	const CLoadMapInfo * staticBlockMapPtr_;
	std::list<PosXYStru> & lstAllPosHasArmy_;

	SpecifyDistAllPosHasArmy();
	SpecifyDistAllPosHasArmy(const SpecifyDistAllPosHasArmy &attOrMoveFunParam);
	SpecifyDistAllPosHasArmy & operator = (const SpecifyDistAllPosHasArmy &attOrMoveFunParam);
};

class SecurityPosTarget
{
public:
	SecurityPosTarget(IArmyPtr armyObj,
	const IArmyPtrvectInvect * mapDataParamPtr,
	const SeekPath * seekPathArithmParamPtr,
	std::list<PosXYStru> &lstFindPos) :armyObj_(armyObj), mapDataParamPtr_(mapDataParamPtr), seekPathArithmParamPtr_(seekPathArithmParamPtr), lstFindPos_(lstFindPos)
	{

	}

	bool fun(int xPos, int yPos);

private:
	IArmyPtr armyObj_;
	const IArmyPtrvectInvect * mapDataParamPtr_;
	const SeekPath * seekPathArithmParamPtr_;
	std::list<PosXYStru> &lstFindPos_;
	SecurityPosTarget();
	SecurityPosTarget(const SecurityPosTarget &attOrMoveFunParam);
	SecurityPosTarget & operator = (const SecurityPosTarget &attOrMoveFunParam);
};

class CommonFunctionSingleton /*: private boost::noncopyable*/
{
public:	
	enum
	{
		SELF_DEFINE_ARMY_NUM = 5
	};

	enum
	{
		RAMDOM_FROM_VALUE = 1,
		RAMDOM_TO_VALUE = 100
	};

	// ������������ȡͨ�ú����൥�������ָ��
	// @return ����
	static CommonFunctionSingleton * getInstance();

	// ������������������ڵ�ĳһ���ӵ�����ָ�룬���������޵о����򷵻ص��ǿ�ָ�롣
	/*
	����
	��ͬ�����ڵ�Ŀ��ѡȡ�����ео���λ�ģ���������ʦ����ʦ�����֡�սʿ��˳�����Ŀ������ѡ��
	�������1֧���ϵı�����ѡ�б��ֲ���ʱ�����ȹ������������ߵ���ֻ���ӣ�
	*/
	// @param  mapDataPtr ���ӵ�ͼ����ָ��
	// @param  armyObjParam ��ǰ�������ӵĶ���
	// @return ��������ڵĲ���ָ��
	IArmyPtr getGunshotEnemyToAttack(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObjParam, const SOLDIER_TYPE tipTopAttackTar=PRIEST);


	// ������������ȡָ�����루Ȧ���Ĳ��ӵ�list
	// @param  mapDataPtr ���ӵ�ͼ����ָ��
	// @param armyObjParam ��ǰ�������ӵĶ���
	// @param distanceVal ָ���ľ���
	// @param isOnlyBuilding �Ƿ�ֻ��������
	// @param isOnlyTrusteeship �Ƿ�ֻ�����йܲ���
	// @param lstFriendArmy �����Ѿ����ӵ�����ָ���б�
	// @param lstEnemy ���صо����ӵ�����ָ���б�
	// @param countCellsHasEnemy ͳ���ж��ٸ����ео�
	// @return �ɹ����,ָ��Ȧ������ͼ��Χʱ������false
	bool getSpecifyDistanceEnemy(IArmyPtrvectInvect * mapDataPtr,
								 IArmyPtr armyObjParam, 
								 int distanceVal,
								 bool isOnlyBuilding,
								 bool isOnlyTrusteeship,
								 listArmyPtr &lstFriendArmy,
								 listArmyPtr &lstEnemy,
								 int &countCellsHasEnemy);

	// ������������ȡָ�����루Ȧ���������б�
	// @param  mapDataPtr ���ӵ�ͼ����ָ��
	// @param armyObjParam ��ǰ�������ӵĶ���
	// @param distanceVal ָ���ľ���
	// @param lstPos ����ָ��Ȧ�ϵ������б�
	// @return �ɹ����
	template <typename A>
	bool getSpecifyDistancePos(IArmyPtrvectInvect * mapDataPtr, const IArmyPtr armyObjParam, const int distanceVal, A &a);

	// ������������ȡָ����Χ�ڵ�Ҫ�����ĵо������ж��Ƿ��ȡ����
	/*
	����
	����A��Ұ�ڳ��ֵо�B����ϵͳͳ���Բ���AΪ���ĵ�8��Χ�ڵ����йܲ��ӣ���NPC���ӣ��Ĺ������ܺ���HP�ܺͣ��ó���ֵTFP1��THP1�����ܻ�������Ѫֵ����
	��ͳ���Բ���BΪ���ĵ�8��Χ�ڵо����ӵĹ������ܺ���HP�ܺͣ��ó���ֵTFP2��THP2����
	���TFP2+THP2���� 2����TFP1+THP1�����жϵ���̫ǿ�󣬲���Aѡ����ˣ�ת�����״̬��
	*/
	// @param  mapDataPtr ���ӵ�ͼ����ָ��
	// @param armyObjSelf ��ǰ�������ӵĶ���
	// @param disVal ָ���Ĳ���Ϊ���ĵļ���Χ��ͳ��HP�͹�����
	// @param TargetArmyPtr ���ع����Ĳ���,���û�п��Թ����Ĳ��ӣ���Ϊ��ָ�롣
	// @param isAttack �Ƿ���й���
	// @return �ɹ����
	bool getAttackArmy(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObjSelf, const int disVal,  IArmyPtr &TargetArmyPtr, bool &isAttack, std::list<IArmyPtr> &lstFriendGroup, const SOLDIER_TYPE tipTopAttackTar);

	// ������������ȡ���˵�����㡣
	/*
	����
	ѡ�������δ���ݻٵ��ҷ��ĳ���/�Ǳ�������һ��Ϊ�ƶ�Ŀ�ꣻ���ȫ�����򡢳Ǳ������ݻ٣������к��˵Ĳ�����һ���̶�����ˣ��������������3��Χ�ڣ�
	*/
	// @param  mapDataPtr ���ӵ�ͼ����ָ��
	// @param armyObjParam  AI���ҵľ���
	// @param Command ��Ҫ���ظ����ӵ�����ṹ
	// @return �ɹ����
	bool GetPosOfCountermarch(IArmyPtrvectInvect * mapDataPtr, CLoadMapInfo *ClassImpedientPtr, IArmyPtr armyObjParam, PosXYStru &posXY);

	// ������������ȡָ�������Ƿ���ڵ���ս������Ϊ�����������ȼ�:սʿ->����
	// @param  mapDataPtr ���ӵ�ͼ����ָ��
	// @param armyObj  AI���ҵľ���
	// @param iWithinScopeVal  ָ��������
	// @param followArmyObj ���ظ����Ŀ�겿��
	// @param �ɹ����
	bool getFollowObjPtr(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObj,  const int iWithinScopeVal, IArmyPtr &followArmyObj);


	// ��������:������Լ�Ϊ����(�����Լ�)��ָ����Χ�ڵ��Ѿ������йܿ��в��ӣ���NPC���ӣ���
	// @param  mapDataPtr ���ӵ�ͼ����ָ��
	// @param armyObjParam ��ǰ�������ӵĶ���
	// @param distanceVal ָ���ľ���
	// @param lstArmy  ���ؼ��ž�(�������)
	// @return �ɹ����
	bool getGroupArmy(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObjSelf, const int disVal, std::list<IArmyPtr> &lstArmy);

	// ��������:��ò�����Ѫ���ٵ���ֻ, ר������ʦ
	// @param listArmy ��������
	// @return ���ز�����Ѫ���ٵ���ֻ
	IArmyPtr getMinHPArmy(const std::list<IArmyPtr> &listArmy);


	// ��������:ִ�������ƶ���Ŀ�ĵ������ȡ��Ҫ�ߵĵ�
	// @param  attOrMoveFunParm �����ƶ� Ŀ��Ĳ���
	// @return �ɹ����
	bool setMoveTargetPos(moveFunParam &attOrMoveFunParm);

	// ��������:ִ�����ù������������ȡ��Ҫ�ߵĵ�
	// @param  attOrMoveFunParm ���ù���Ŀ��Ĳ���
	// @return �ɹ����
    bool setAttackTarget(attackFunParam &attOrMoveFunParm);

	bool MoveToSecurityPos(IArmyPtrvectInvect * mapDataParamPtr,  SeekPath * seekPathArithmParamPtr, IArmyPtr armyObj, const IArmyPtr attackMeArmy, PosXYStru &posMoveTo);

	// ������������ȡ��ĳ����Ϊ���ĵ�ָ�����루Ȧ���ϵ����е�
	// @param  mapDataPtr ���ӵ�ͼ����ָ��
	// @param posStart ���ĵ�
	// @param distanceVal ָ����Χ
	// @param lstPos ���ص��б�
	// @return  �ɹ���� 
	template <typename A>
	bool GetAllSpecifyPos(IArmyPtrvectInvect * mapDataPtr,
		const PosXYStru &posStart,
		const int distanceVal, 
		A &a);

	bool getSpecifyDistancePos(const IArmyPtr armyObjParam, 
		const int distanceVal, 
		std::vector<PosXYStru> &vectPos,
		IArmyPtrvectInvect * mapDataPtr);
	
	// ����������������Ұ�⿿���ĵо����������ȼ�����1.ְҵ   2.��̾���   3.HP���ģ�
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param armyObjParam ��ǰ�������ӵĶ���
	// @return  �����ҵ��Ĳ���ָ��
	IArmyPtr getNearestEnemyOutView(IArmyPtrvectInvect * mapDataPtr, IArmyPtr armyObjParam);

	// ������������ȡ1~100�ڵ������
	// @return  ���ػ�ȡ�������
	int GetRandomValFromOneToHundred();

	// ��������������ָ��Ȧ��ռ�в��ӵ������
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param armyObjParam ��ǰ�������ӵĶ���
	// @param lstAllPosHasArmy ���ָ��Ȧ�ϵ��в��ӵ����������
	// @param distanceVal ָ��Ȧ��
	// @param isIncludeStatBlock ͳ���Ƿ񺬾�̬�ϰ���
	// @return  �����ҵ��Ĳ���ָ��
	bool getSpecifyDistAllPosHasArmy(IArmyPtrvectInvect * mapDataPtr,
																 const CLoadMapInfo * staticBlockMapPtr,
																IArmyPtr armyObjParam,
																std::list<PosXYStru> &lstAllPosHasArmy,
																const int distanceVal);

	// ������������ȡĿ�ĵ㸽������Ŀɵ�������Լ�����ĵ�����
	// @param ClassImpedient �ϰ����ͼ����ָ��
	// @param armyPos ��ǰ�������ӵ�����
	// @param mapDataPtr ���ӵ�ͼ����ָ��
	// @param moveTagetPos ��Ҫ�����Ŀ�ĵ�
	// @return  �ɹ����
	bool GetProximateNotStaticBlockPos(const CLoadMapInfo *ClassImpedient, const PosXYStru &armyPos, IArmyPtrvectInvect * mapDataPtr, PosXYStru &moveTagetPos);
		
	// ��������������Ŀ��ѡȡ����ѡ��ָ����Χ�ڵĵо���Ϊ����Ŀ��
	// @param  mapDataPtr ���ӵ�ͼ����ָ��
	// @param armyObjParam ����AI�Ĳ���
	// @param scopeVal ָ����Χ
	// @param enemyPtr ����Ŀ��
	//@param tipTopAttackTar       ���ȹ�����ְҵ����
	// @return  �ɹ���� 
	bool getApproEnemyToAttackOnAimRule(IArmyPtrvectInvect * mapDataPtr , IArmyPtr armyObjParam, const int scopeVal, IArmyPtr &enemyPtr,  SOLDIER_TYPE tipTopAttackTar = SOLDIERTYPE_NONE);

	bool getArmysInPos(const PosXYStru &pos, const IArmyPtrvectInvect * mapDataPtr, std::vector<IArmyPtr> &Armys);

	bool enemyIsInAttackScope(const std::vector<POS_STRUCT> &armyPosVect, const std::vector<POS_STRUCT> &enemyPosVect, int attackScope);

	bool GetAllSpecifyPos(const PosXYStru &posStart,
		const int distanceVal, 
		std::vector<PosXYStru> &vectPos,
		const IArmyPtrvectInvect * mapDataPtr);

	// ��ȡ�Ա�����ը��Ĺ���Ŀ��
	bool getSelfBlastAttArmys(const PosXYStru &pos, const IArmyPtrvectInvect * mapDataPtr, std::vector<IArmyPtr> &armysCenter, std::vector<IArmyPtr> &armysInOneCircle);
private:
	// �����������ҳ�ͬ�ֲ���Ѫ������ֻ
	// @param listArmy ��������
	// @return  ͬ�ֲ���Ѫ������ֻ
	IArmyPtr getMaxHPArmy(const std::list<IArmyPtr>& listArmy);

	// ����������У���ȡ��ǰ��������
	// @param  mapDataPtr ���ӵ�ͼ����ָ��
	// @param armyObjParam ����AI�Ĳ���
	// @param Xpos ���ز��ӵ�x����
	// @param Ypos ���ز��ӵ�y����
	// @param XMaxVal ���ص�ͼ������x����
	// @param YMaxVal ���ص�ͼ����С��y����
	// @return  �ɹ���� 
	bool invalidateXYpos(IArmyPtrvectInvect * mapDataPtr,int Xpos, int Ypos);

	// ����������ͳ���Ѿ��͵о�
	// @param lstIArmyPtr ���в����б�
	// @param armyObjParam ��ǰ��AI�Ĳ���
	// @param isOnlyBuilding �Ƿ�ֻ��������
	// @param isOnlyTrusteeship �Ƿ�ֻ�����йܲ���
	// @param lstFriendArmy �����Ѿ��б�
	// @param lstEnemy ���صо��б�
	// @param countCellsHasEnemy �����ж��ٸ����ео�
	// @param setArmyId ���ص��Ҳ��ӵ�ID����
	// @return  ͬ�ֲ���Ѫ������ֻ
	/*bool StatFriendArmyAndEnemy(const list<IArmyPtr> &lstIArmyPtr,
								IArmyPtr armyObjParam,
								bool isOnlyBuilding,
								bool isOnlyTrusteeship,
								listArmyPtr &lstFriendArmy,
								listArmyPtr &lstEnemy,
								int &countCellsHasEnemy);
								*/

	

	// ��������:���ؼ��ž�(�������)��HP
	// @param listArmy ��������б�
	// @return ���ؼ��ž�(�������)��HP
	int getGroupArmyHP(const std::list<IArmyPtr>& listArmy);

	// ��������:���ؼ��ž�(�������)��FP(������)
	// @param listArmy ��������б�
	// @return ���ؼ��ž�(�������)��FP
	int getGroupArmyFP(const std::list<IArmyPtr>& listArmy);

	// �������� ���Һ��ʵĺ�������
	// @param  mapDataPtr ���ӵ�ͼ����ָ��
	// @param lstPos �����б�
	// @param setXYpos �о��Ǳ������꼯��
	// @return �ҵ����
	bool isFindCountermarchPos(IArmyPtrvectInvect * mapDataPtr, const std::vector<PosXYStru> &vectPos, const pairInSet &setXYpos, PosXYStru &posXY);

	// �������� ������Ұ�⿿���ĵо����������ȼ�����1.ְҵ   2.��̾���   3.HP���ģ�
	// @param lstPos �����б�
	// @param listArmy ��ѡ��ĵо��б�
	// @return �����ҵ��ĵо�ָ��
	IArmyPtr getMinDistanceArmy(const IArmyPtr armyObjParam, const std::list<IArmyPtr> &listArmy);

	//�����趨�����ȹ������ƶ��÷��صĲ���
	//@param enemyClergyContainer  ��ʦ����
	//@param enemyRabbiContainer   ��ʦ����
	//@param enemyBowContainer     ����������
	//@param enemyWarriorContainer սʿ����
	//@param tipTopAttackTar       ���ȹ�����ְҵ����
	//@return Ŀ�겿�ӣ�����оͷ��أ����򷵻�NULL
	IArmyPtr getArmyByPriority(const listArmyPtr &enemyClergyContainer, const listArmyPtr &enemyRabbiContainer, const listArmyPtr &enemyBowContainer, const listArmyPtr &enemyWarriorContainer, const SOLDIER_TYPE tipTopAttackTar);

private:
	CommonFunctionSingleton();

	static CommonFunctionSingleton * comFunInstance;  // Ωһʵ��
	
	class CGarbo // ����Ψһ��������������������ɾ������
	{
	public:
		~CGarbo()
		{
			 if (NULL != CommonFunctionSingleton::comFunInstance)
			 {
				 delete CommonFunctionSingleton::comFunInstance;
				 CommonFunctionSingleton::comFunInstance = NULL;
			 }
		}
	};
	
	static CGarbo Garbo; // ����һ����̬��Ա���ڳ������ʱ��ϵͳ�����������������
};

#endif
