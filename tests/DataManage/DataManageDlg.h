// DataManageDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DB_Proxy.h"
#include "DB_Table.h"
#include "ZSBB_DataDefine.h"
#include "ProFightSystem.h"
#include <map>
#include <stdio.h>
#define  MAX_LEVEL 200
typedef struct tagRaceJobParam
{
	int FaceID;
	int IsKnight;
	int IsSoldier;
	int IsShooter;
	int IsMaster;
	int IsPastor;
}RaceJobParam,*LPRaceJobParam;

typedef struct tagSpecialHeroParam
{
	int SortID;
	char Name[24];
	int JobID;
	int RaceID;
	int FaceID;
	int Stamina;
	int Power;
	int Celerity;
	int Wit;
	int TechID;
	int TechLevel;
}SpecialHeroParam,*LPSpecialHeroParam;

typedef struct tagWeaponParam
{
	int ItemID;
	char Name[32];
	int Type;
}WeaponParam,*LPWeaponParam;

typedef struct tagTaskBF_Item
{
	int TaskID;
	int Food;
	int Wood;
	int Skin;
	int Iron;
}TaskBF_Item,*LPTaskBF_Item;
// CDataManageDlg �Ի���
class CDataManageDlg : public CDialog
{
// ����
public:
	CDataManageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DATAMANAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	typedef std::map<int,EquipAttribute*>   EquipAttribute_Map;
	typedef std::map<int,EquipAttribute*>::iterator EquipAttribute_Iterator;
	
	typedef std::map<int,LPSpecialHeroParam> SpecialHeroParam_Map;
	typedef std::map<int,LPSpecialHeroParam>::iterator SpecialHeroParam_Iterator;

	typedef std::map<int,LPWeaponParam> WeaponParam_Map;
	typedef std::map<int,LPWeaponParam>::iterator WeaponParam_Map_Iterator;
	
	typedef std::map<int,LPTaskBF_Item> TaskBF_Map;
	typedef TaskBF_Map::iterator TaskBF_Map_Iterator;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	bool m_bConnected;
	RaceJobParam m_RaceJobParam[8];
	char m_HeroName[1024][16];
	int m_LevelParam[MAX_LEVEL+1];
public:
	CIPAddressCtrl			m_ipDBGIP;
	CEdit					m_edDBGPort;
	CEdit					m_edPlayerID;
	CEdit					m_edCityID;
	CButton					m_btnHeroPack;
	CButton					m_btnResource;
	CButton					m_btnSoldier;
	CButton					m_btnWeapon;
	EquipAttribute_Map		m_EquipAttributeList;
	CardParam_Map			m_CardConfigList;
	SpecialHeroParam_Map	m_SpecialHeroParamList;
	WeaponParam_Map			m_WeaponParamList;
	HeroEquipAttribute_Map	m_HeroEquipParamList;
	TaskBF_Map				m_TaskBFList;
	OfficerInitSkill_MultiMap						officerInitSkillMultiMap;//����Ч����
	SkillDetail_Map                                 skillDetailMap_;
	//�Ƿ�����ص��Ĳ���
	PackStoreParam_Map								m_PackStoreParamMap;
	ConstructionConfigure_Multimap					m_ConstructionConfigureMultimap;
public:
	int GetUnusedPos(int playerid,int officerid,DB_Proxy *pDBProxy);		//����һ��δʹ�õı�����Ԫ��
	int GetUnusedPos(PlayerItemStorageTable &packitem,DB_Proxy *pDBProxy);
	int ReadRaceJobParam();
	int ReadHeroName();
	int ReadLevelParam();
	int ReadEquipAttribute();
	EquipAttribute* GetEquipAttribute(int id);
	int ClearEquipAttribute();

	int ReadCardConfig();
	LPCardParam GetCardConfig(int id);
	int ClearCardConfig();

	int InitCardConfigList(CComboBox *pComboBox);

	int GetExp(int level);
	LPRaceJobParam GetRaceJobParam(int raceid);
	char *GetHeroName(int id);

	int ReadSpecialHeroParam();
	LPSpecialHeroParam GetSpecialHeroParam(int id);
	int ClearSpecialHeroParam();

	int ReadWeaponParam();
	LPWeaponParam GetWeaponParam(int id);
	int ClearWeaponParam();
	int InitWeaponParamComboBox(CComboBox *pComboBox);

	int ReadHeroEquipParam();
	HeroEquipAttribute* GetHeroEquipParam(int id);
	int ClearHeroEquipParam();
	int InitHeroEquipParamComboBox(CComboBox *pComboBox);

	int ReadTaskBFParam();
	LPTaskBF_Item GetTaskBFParam(int taskid);
	int ClearTaskBFParam();

	void ProcHeroAttribute(PlayerOfficerTable &hero);


	//��ȡӢ�۳�ʼ���ܲ�����
	void readOfficerInitSkill();
	void clearOfficerInitSkill();

	//����ְҵID ��ȡ��ְҵĬ�ϵļ��ܲ���
	OfficerInitSkill_MultiMap_iterator getOfficerInitSkill_Lowerbound(int jobid);
	OfficerInitSkill_MultiMap_iterator getOfficerInitSkill_Upperbound(int jobid);
	//��ȡ����������
	void readSkillDetail(void);
	//�ͷż���������
	void clearSkillDetail(void);
	SkillDetail * getSkillDetail(SKILL_TYPE skillID , int skillLevel); //��ȡ����������Ϣ

	//��ȡ�Ƿ�����ص���ű����Ĳ���
	PackStoreParam *GetPackStoreParam(int postion);
	int ReadPackStoreParam();
	void ClearPackStoreParam();

	//��ȡ�������ñ�
	void loadConstructionConfigure();
	void clearConstructionConfigure();
	ConstructionConfigure* getConstructionConfigure(int iConID, int iLevel);

	void heroUpgrade(int level,PlayerOfficerTable *officer,PlayerOfficerTable *upofficer);
public:
	DB_Proxy m_DBProxy;
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnClose();
	CButton m_btnConnect;
	afx_msg void OnBnClickedBtnHeropack();
	afx_msg void OnBnClickedBtnResource();
	afx_msg void OnBnClickedBtnSoldier();
	afx_msg void OnBnClickedBtnWeapon();
	CComboBox m_cbMapList;
	CButton m_btnRefresh;
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnCbnSelchangeComboMap();
	afx_msg void OnBnClickedBtnAddcontent();
	CButton m_btnAddContent;
	CButton m_btnHeroManage;
	afx_msg void OnBnClickedBtnHeromanage();
	CButton m_btnAddHero;
	afx_msg void OnBnClickedButtonHero();
//	afx_msg void OnBnClickedButtonAddcity();
	CButton m_btnDataView;
	afx_msg void OnBnClickedButtonDataview();
	CButton m_btnInit;
	afx_msg void OnBnClickedButtonInit();
	CStatic m_csInfo;
	CButton m_btnResume;
	afx_msg void OnBnClickedBtnResume();
	CEdit m_edCurLevel;
	CEdit m_edExp;
	afx_msg void OnBnClickedButtonGetlevel();
	CButton m_btnDelCons;
	afx_msg void OnBnClickedButtonDelcons();
	afx_msg void OnBnClickedButton1();
	CButton m_btnInitRes;
	afx_msg void OnBnClickedButton2();
	CComboBox m_cbHeroJob;
	CButton m_btnResafeAllHero;
	CButton m_btnDelEmail;
	afx_msg void OnBnClickedButton3();
	CButton m_btnSpecialHero;
	CComboBox m_cbSpecialHero;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton4();
	CButton m_btnConsortia;
	afx_msg void OnBnClickedButton5();
	CEdit m_edUser;
	CEdit m_edPassword;
	afx_msg void OnBnClickedButton6();
	CButton m_btnModifyHeroAttribute;
	afx_msg void OnBnClickedButton7();
	CButton m_btnTaskBF;
	afx_msg void OnBnClickedButton8();
};
