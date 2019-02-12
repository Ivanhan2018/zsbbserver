// PlayerOfficer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataManage.h"
#include "PlayerOfficer.h"
#include ".\datamanagedlg.h"
#include ".\playerofficer.h"


// CPlayerOfficer �Ի���

IMPLEMENT_DYNAMIC(CPlayerOfficer, CDialog)
CPlayerOfficer::CPlayerOfficer(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerOfficer::IDD, pParent)
{
}

CPlayerOfficer::~CPlayerOfficer()
{
}

void CPlayerOfficer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_HINT, m_stHint);
	DDX_Control(pDX, IDC_EDIT_LEVEL, m_edLevel);
	DDX_Control(pDX, IDC_EDIT_STAMINA, m_edStamina);
	DDX_Control(pDX, IDC_EDIT_POWER, m_edPower);
	DDX_Control(pDX, IDC_EDIT_CELERITY, m_edCelerity);
	DDX_Control(pDX, IDC_EDIT_WIT, m_edWit);
	DDX_Control(pDX, IDC_EDIT_TECHDOT, m_edTechDot);
	DDX_Control(pDX, IDC_EDIT_HEROID, m_edHeroID);
	DDX_Control(pDX, IDC_COMBO1, m_cbStateList);
	DDX_Control(pDX, IDC_EDIT_STATE, m_edHeroState);
	DDX_Control(pDX, IDC_EDIT_MONEY, m_edMoney);
	DDX_Control(pDX, IDC_EDIT_SCIENCE, m_edScience);
	DDX_Control(pDX, ID_MODIFYHERO, m_btnModify);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edName);
	DDX_Control(pDX, IDC_EDIT_EXP, m_edExp);
	DDX_Control(pDX, IDC_EDIT_CELERITY2, m_edCredit);
	DDX_Control(pDX, IDC_EDIT_CELERITY3, m_edActionDot);
}


BEGIN_MESSAGE_MAP(CPlayerOfficer, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_MODIFYHERO, OnBnClickedModifyhero)
	ON_EN_CHANGE(IDC_EDIT_HEROID, OnEnChangeEditHeroid)
	ON_BN_CLICKED(IDC_BUTTON_STATE, OnBnClickedButtonState)
	ON_BN_CLICKED(IDC_ADDPLAYER, OnBnClickedAddplayer)
	ON_EN_CHANGE(IDC_EDIT_LEVEL, OnEnChangeEditLevel)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CPlayerOfficer ��Ϣ�������

void CPlayerOfficer::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	m_edLevel.SetWindowText("0");
	m_edStamina.SetWindowText("0");
	m_edPower.SetWindowText("0");
	m_edCelerity.SetWindowText("0");
	m_edWit.SetWindowText("0");
	m_edTechDot.SetWindowText("0");
	m_edMoney.SetWindowText("0");
	m_edScience.SetWindowText("0");
	m_edExp.SetWindowText("0");
	m_edCredit.SetWindowText("0");
	/*m_edTechDot.EnableWindow(FALSE);
	
	EN_LEISURE=0,										//0,���� 
	EN_HAVE_ARMY,										//1,����
	EN_GBH,												//2,���� 
	EN_BY_CAPTIVE,										//3,���� 
	EN_CAPTIVE											//4 ��²
	*/
	m_edHeroState.EnableWindow(FALSE);
	int index = 0;
	index = m_cbStateList.AddString("����");
	m_cbStateList.SetItemData(index,EN_LEISURE);
	index = m_cbStateList.AddString("����");
	m_cbStateList.SetItemData(index,EN_GBH);
	index = m_cbStateList.AddString("����");
	m_cbStateList.SetItemData(index,EN_BY_CAPTIVE);
	/*
	index = m_cbStateList.AddString("����");
	m_cbStateList.SetItemData(index,EN_HAVE_ARMY);
	index = m_cbStateList.AddString("����");
	m_cbStateList.SetItemData(index,EN_BY_CAPTIVE);
	index = m_cbStateList.AddString("��²");
	m_cbStateList.SetItemData(index,EN_CAPTIVE);
	*/
	m_cbStateList.SetCurSel(0);
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strPlayerID;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edHeroID.SetWindowText(strPlayerID);
	//m_btnModify.EnableWindow(FALSE);
}
int CPlayerOfficer::HeroCredit(const int lCredit)
{
	if (lCredit <= 0) return 0;
	int iRank = 0;

	if(lCredit <10)
		iRank = 0;
	else if(lCredit <77)
		iRank = 1;
	else if(lCredit <207)
		iRank = 2;
	else if(lCredit <410)
		iRank = 3;
	else if(lCredit <705)
		iRank = 4;
	else if(lCredit <1126)
		iRank = 5;
	else if(lCredit <1733)
		iRank = 6;
	else if(lCredit <2635)
		iRank = 7;
	else if(lCredit <4028)
		iRank = 8;
	else if(lCredit <6264)
		iRank = 9;
	else if(lCredit <9978)
		iRank = 10;
	else if(lCredit <14532)
		iRank = 11;
	else if(lCredit <20957)
		iRank = 12;
	else if(lCredit <30733)
		iRank = 13;
	else
		iRank = 14;
	return iRank;
}
void CPlayerOfficer::OnBnClickedModifyhero()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strPlayerID,strHeroID;
	CString strLevel,strStamina,strPower,strCelerity,strWit,strTechDot,strExp,strCredit,strActionDot;
	int iPlayerID,iHeroID;
	int iLevel,iStamina,iPower,iCelerity,iWit,iTechDot,iExp,iCredit,iActionDot;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edHeroID.GetWindowText(strHeroID);

	m_edLevel.GetWindowText(strLevel);
	m_edStamina.GetWindowText(strStamina);
	m_edPower.GetWindowText(strPower);
	m_edCelerity.GetWindowText(strCelerity);
	m_edWit.GetWindowText(strWit);
	m_edTechDot.GetWindowText(strTechDot);
	m_edExp.GetWindowText(strExp);
	m_edCredit.GetWindowText(strCredit);
	m_edActionDot.GetWindowText(strActionDot);
	iPlayerID = atoi(strPlayerID);
	iHeroID = atoi(strHeroID);
	iLevel = atoi(strLevel);
	iStamina = atoi(strStamina);
	iPower = atoi(strPower);
	iCelerity = atoi(strCelerity);
	iWit = atoi(strWit);
	iTechDot = atoi(strTechDot);
	iExp = atoi(strExp);
	iCredit = atoi(strCredit);
	iActionDot = atoi(strActionDot);
	m_stHint.SetWindowText("");
	if(iLevel ==0 && iStamina==0 && iPower==0 && iCelerity ==0 && iWit ==0 && iTechDot ==0 && iExp==0 && iCredit == 0 && iActionDot==0)
	{
		m_stHint.SetWindowText("��������Ϊ0�����������롣");
		return;
	}
	PlayerOfficerTable offtb,updateofftb;
	offtb.setmasterid(iPlayerID);
	offtb.setdetailid(iHeroID);
	if(pMainDlg->m_DBProxy.db_select(offtb) != DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("��ѯӢ��ʧ�ܡ�");
		return;
	}
	if (offtb.level_ + iLevel >=70)
	{
		m_stHint.SetWindowText("Ӣ�ۼ�����ߣ��޷���ӡ�");
		return;
	}
	int iTotalStamina = offtb.stamina_ + offtb.levelstamina_ + offtb.equipstamina_ + iStamina;
	if (iTotalStamina >= 300 || iTotalStamina <=0)
	{
		m_stHint.SetWindowText("Ӣ���������ߣ��޷���ӡ�");
		return;
	}
	int iTotalPower = offtb.power_ + offtb.levelpower_ + offtb.equippower_ + iPower;
	if (iTotalPower >= 300 || iTotalPower<=0)
	{
		m_stHint.SetWindowText("Ӣ���������ߣ��޷���ӡ�");
		return;
	}
	int iTotalCelerity = offtb.celerity_ + offtb.levelcelerity_ + offtb.equipcelerity_ + iCelerity;
	if (iTotalCelerity >= 300 || iTotalCelerity<=0)
	{
		m_stHint.SetWindowText("Ӣ�����ݹ��ߣ��޷���ӡ�");
		return;
	}
	int iTotalWit = offtb.wit_ + offtb.levelwit_ + offtb.equipwit_ + iWit;
	if (iTotalWit >= 300 || iTotalWit<=0)
	{
		m_stHint.SetWindowText("Ӣ���������ߣ��޷���ӡ�");
		return;
	}
	if (iCredit >0)
	{
		int iRank = HeroCredit(offtb.credit_ + iCredit);
		updateofftb.credit_ = iCredit;
		updateofftb.militaryrank_ = iRank - offtb.militaryrank_;
	}
	else
	{
		iCredit = 0;
	}
	if (iExp <0)
	{
		iExp = 0;
	}
	updateofftb.setmasterid(iPlayerID);
	updateofftb.setdetailid(iHeroID);
	updateofftb.level_ = iLevel;
	//updateofftb.levelstamina_ = iStamina;
	//updateofftb.levelpower_ = iPower;
	//updateofftb.levelcelerity_ = iCelerity;
	//updateofftb.levelwit_ = iWit;
	updateofftb.techdot_ = iTechDot;
	updateofftb.exp_ = iExp;
	updateofftb.actiondot_ = iActionDot;
	//updateofftb.updatetime_ = -offtb.updatetime_;
	updateofftb.jobid_ = offtb.jobid_;
	pMainDlg->heroUpgrade(iLevel,&offtb,&updateofftb);
	pMainDlg->ProcHeroAttribute(updateofftb);
	updateofftb.jobid_ = 0;

	if(pMainDlg->m_DBProxy.db_increase(updateofftb) == DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("�޸�Ӣ�����Գɹ���");
	}
	else
	{
		m_stHint.SetWindowText("�޸�Ӣ������ʧ�ܡ�");
	}
}

void CPlayerOfficer::OnEnChangeEditHeroid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strPlayerID,strHeroID;
	int iPlayerID,iHeroID;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edHeroID.GetWindowText(strHeroID);
	iPlayerID = atoi(strPlayerID);
	iHeroID = atoi(strHeroID);

	offtb.setmasterid(iPlayerID);
	offtb.setdetailid(iHeroID);
	
	m_edHeroState.SetWindowText("");
	m_edName.SetWindowText("");
	m_stHint.SetWindowText("");
	if(pMainDlg->m_DBProxy.db_select(offtb) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	CString strTmp;
	m_edName.SetWindowText(offtb.officername_);
	/*
	strTmp.Format("%d",offtb.celerity_);
	m_edCelerity.SetWindowText(strTmp);
	strTmp.Format("%d",offtb.level_);
	m_edLevel.SetWindowText(strTmp);
	strTmp.Format("%d",offtb.stamina_);
	m_edStamina.SetWindowText(strTmp);
	strTmp.Format("%d",offtb.power_);
	m_edPower.SetWindowText(strTmp);
	strTmp.Format("%d",offtb.wit_);
	m_edWit.SetWindowText(strTmp);
	strTmp.Format("%d",offtb.techdot_);
	m_edTechDot.SetWindowText(strTmp);
	*/
	strTmp.Format("�ȼ���%d,���Σ�%d,���ݣ�%d,����:%d,����:%d,����:%d",offtb.level_,offtb.militaryrank_,offtb.celerity_,offtb.power_,offtb.wit_,offtb.stamina_);
	
	m_stHint.SetWindowText(strTmp);
	/*
	EN_LEISURE=0,										//0,���� 
	EN_HAVE_ARMY,										//1,����
	EN_GBH,												//2,���� 
	EN_BY_CAPTIVE,										//3,���� 
	EN_CAPTIVE											//4 ��²
	*/
	switch(offtb.state_)
	{
	case EN_LEISURE:
		{
			m_edHeroState.SetWindowText("����");
		}
		break;
	case EN_HAVE_ARMY:
		{
			m_edHeroState.SetWindowText("����");
		}
		break;
	case EN_GBH:
		{
			m_edHeroState.SetWindowText("����");
		}
		break;
	case EN_BY_CAPTIVE:
		{
			m_edHeroState.SetWindowText("����");
		}
		break;
	case EN_CAPTIVE:
		{
			m_edHeroState.SetWindowText("��²");
		}
		break;
	default:
		{
			m_edHeroState.SetWindowText("δ֪");
		}
		break;
	}
}

void CPlayerOfficer::OnBnClickedButtonState()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strPlayerID,strHeroID;
	int iPlayerID,iHeroID,iMapID,iCityID,iArmyID;
	int iNewState  = 0;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edHeroID.GetWindowText(strHeroID);
	iPlayerID = atoi(strPlayerID);
	iHeroID = atoi(strHeroID);
	iNewState = m_cbStateList.GetItemData(m_cbStateList.GetCurSel());
	PlayerOfficerTable offtb;
	offtb.setmasterid(iPlayerID);
	offtb.setdetailid(iHeroID);

	int iOldState = 0;
	if(pMainDlg->m_DBProxy.db_select(offtb) != DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("��ѯָ�ӹٱ�ʧ��");
		return;
	}
	if(iNewState == EN_GBH || iNewState == EN_BY_CAPTIVE)
	{
		if(offtb.state_ != EN_LEISURE)
		{
			m_stHint.SetWindowText("Ӣ��״̬�ǿ��У�����ˢΪ����");
			return;
		}
		offtb.state_ = iNewState;
		if(pMainDlg->m_DBProxy.db_update(offtb,offtb) == DB_Proxy::DB_SUCCEED)
		{
			m_stHint.SetWindowText("ˢΪ���˳ɹ�");
		}
		else
		{
			m_stHint.SetWindowText("ˢΪ����ʧ��");
		}
	}
	else if(iNewState == EN_LEISURE)
	{
		if(offtb.state_ != EN_GBH && offtb.state_ != EN_HAVE_ARMY && offtb.state_ >=0)
		{
			m_stHint.SetWindowText("Ӱ��״̬�����˻���в��ӣ������޸ġ�");
			return;
		}
		iOldState = offtb.state_;
		iMapID = offtb.mapid_;
		iCityID = offtb.cityid_;
		iArmyID = offtb.armyid_;
		memset(&offtb,0,sizeof(offtb));
		offtb.setmasterid(iPlayerID);
		offtb.setdetailid(iHeroID);
		offtb.state_ = iNewState - iOldState;
		offtb.armyid_ = -iArmyID;
		if(pMainDlg->m_DBProxy.db_increase(offtb) != DB_Proxy::DB_SUCCEED)
		{
			m_stHint.SetWindowText("�޸�Ӣ��״̬ʧ��");
		}
		else
		{
			if(iOldState == EN_HAVE_ARMY)
			{//ɾ������
				ArmyTable armytb;
				armytb.setmasterid(iMapID);
				armytb.setdetailid(iArmyID);
				pMainDlg->m_DBProxy.db_select(armytb);
				if(armytb.state_ ==2)
				{//�ѳ���
					ArmyOutTable outarmytb;
					outarmytb.setmasterid(iPlayerID);
					outarmytb.setdetailid(iArmyID);
					pMainDlg->m_DBProxy.db_delete(outarmytb);
				}
				pMainDlg->m_DBProxy.db_delete(armytb);
			}
			m_stHint.SetWindowText("�޸�Ӣ��״̬�ɹ�");
		}
	}

}

void CPlayerOfficer::OnBnClickedAddplayer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strPlayerID,strMoney,strScience;
	int iPlayerID,iMoney,iScience;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edMoney.GetWindowText(strMoney);
	m_edScience.GetWindowText(strScience);
	iPlayerID = atoi(strPlayerID);
	iMoney = atoi(strMoney);
	iScience = atoi(strScience);

	m_stHint.SetWindowText("");
	if(iMoney ==0 && iScience ==0)
	{
		m_stHint.SetWindowText("�Ƽ��㡢��ǮȫΪ0������������");
		return;
	}

	if(iMoney >100000000 || iScience >1000000)
	{
		m_stHint.SetWindowText("�Ƽ��㡢��ǮȫΪ���ܳ���100������������");
		return;
	}
	PlayerChatRoomTable playertb;
	playertb.setid(iPlayerID);
	playertb.science_ = iScience;
	playertb.money_ = iMoney;
	if(pMainDlg->m_DBProxy.db_increase(playertb) != DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("���ӿƼ��㡢��Ǯʧ��");
	}
	else
	{
		m_stHint.SetWindowText("���ӿƼ��㡢��Ǯ�ɹ�");
	}
}

void CPlayerOfficer::OnEnChangeEditLevel()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strTmp;
	m_edLevel.GetWindowText(strTmp);
	strTmp.Format("%d",atoi(strTmp) * 5);
	m_edTechDot.SetWindowText(strTmp);
}


void CPlayerOfficer::OnBnClickedButtonReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strPlayerID,strHeroID;
	int iPlayerID,iHeroID;

	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edHeroID.GetWindowText(strHeroID);
	iPlayerID = atoi(strPlayerID);
	iHeroID = atoi(strHeroID);

	PlayerOfficerTable offtb;
	offtb.setmasterid(iPlayerID);
	offtb.setdetailid(iHeroID);
	if(pMainDlg->m_DBProxy.db_select(offtb) != DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("��ѯָ�ӹٱ�ʧ��");
		return;
	}
	int flag = 0;
	if(offtb.stamina_ > 500)
	{
		offtb.stamina_ = 30;
		flag = 1;
	}
	if(offtb.power_ > 500)
	{
		offtb.power_  = 30;
		flag = 1;
	}
	if(offtb.celerity_ >500)
	{
		offtb.celerity_  = 30;
		flag = 1;
	}
	if(offtb.wit_ >500)
	{
		offtb.wit_ = 30;
		flag = 1;
	}
	if(offtb.level_ < offtb.techdot_)
	{
		offtb.techdot_ = offtb.level_ ;
		flag = 1;
	}
	if(flag)
	{
		
		pMainDlg->ProcHeroAttribute(offtb);
		pMainDlg->m_DBProxy.db_update(offtb,offtb);
	}
	
	CString strTmp;
	strTmp.Format("%d",offtb.level_);
	m_edLevel.SetWindowText(strTmp);
	strTmp.Format("%d",offtb.stamina_);
	m_edStamina.SetWindowText(strTmp);
	strTmp.Format("%d",offtb.power_);
	m_edPower.SetWindowText(strTmp);
	strTmp.Format("%d",offtb.celerity_);
	m_edCelerity.SetWindowText(strTmp);
	strTmp.Format("%d",offtb.wit_);
	m_edWit.SetWindowText(strTmp);
	strTmp.Format("%d",offtb.techdot_);
	m_edTechDot.SetWindowText(strTmp);
	m_stHint.SetWindowText("���óɹ�");
}

void CPlayerOfficer::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CPlayerOfficer::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strPlayerID,strOfficerID;
	int iPlayerID,iOfficerID;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edHeroID.GetWindowText(strOfficerID);
	iPlayerID = atoi(strPlayerID);
	iOfficerID = atoi(strOfficerID);
	if (iPlayerID != iOfficerID)
	{
		m_stHint.SetWindowText("ֻ���޸���Ӣ������");
		return;
	}
	PlayerOfficerTable offtb;
	offtb.setmasterid(iPlayerID);
	offtb.setdetailid(iPlayerID);
	if (pMainDlg->m_DBProxy.db_select(offtb) != DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("��Ӣ�۲�����");
		return;
	}
	CString strNewName;
	m_edName.GetWindowText(strNewName);
	if (strcmp(strNewName,offtb.officername_) == 0)
	{
		m_stHint.SetWindowText("Ӣ������û�иı䡣��");
		return;
	}
	strcpy(offtb.officername_,strNewName);
	if (pMainDlg->m_DBProxy.db_update(offtb,offtb) != DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("�޸�Ӣ������ʧ�ܡ�");
	}
	else
	{
		m_stHint.SetWindowText("�޸�Ӣ�����ֳɹ���");
	}
}
