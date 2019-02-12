// DlgExportLogTool.cpp : 实现文件
//

#include "stdafx.h"
#include <cassert>
#include <process.h> 
#include <sstream>


#include "LogExportTool.h"
#include "DlgExportLogTool.h"

#include "DlgProgressBar.h"

// CDlgExportLogTool 对话框

enum
{
	Unknown_Type = 0,
	HeroAttribute_Type,      // 英雄属性表				    1
	Experience_Type,         // 经验表						2
	HeroUpgrade_Type,		 // 升级表						3
	State_Type,				 // 状态表						4
	HeroSkillDot_Type,       // 英雄技能点表				5
	HeroSkillLog_Type,       // 英雄技能日志表				6
	MilitaryCredit_Type,	 // 军衔、荣誉表				7
	TechnologicalDot_Type,   // 科技点表					8
	PlayerCityNumLog_Type,   // 玩家城市数量日志表			9
	PlayerHeroNumLog_Type,   // 玩家英雄数日志表			10
	MoneyLog_Type,			 // 金钱日志表					11
	BaseResource_Type,       // 资源收集：基本资源表		12
	SoldierLog_Type,         // 士兵日志表					13
	WeaponDefender_Type,     // 士兵装备日志：武器防具表	14
	HeroEquipLog_Type,       // 英雄装备日志表				15
	ProficiencyEquipLog_Type,// 精练装备日志表				16
	TaskLog_Type,			 // 任务日志表					17
	PlayerBuildingLog_Type,  // 玩家建筑日志表				18
};

enum
{
	FIVE = 5,
	TEN = 10,
};

bool g_isCancelProgressBar = false;

CDlgProgressBar g_ProgressBar(g_isCancelProgressBar, false);

void dealWithEndWork(CString &sHeroAttributeCSVPath, CString &sExperienceCSVPath, CString &sHeroUpgradeCSVPath, CString &sStateCSVPath, CString &sHeroSkillDotCSVPath, 
					 CString &sHeroSkillLogCSVPath, CString &sMilitaryCreditCSVPath, CString &sTechnologicalDotCSVPath, CString &sPlayerCityNumLogCSVPath, 
					 CString &sPlayerHeroNumLogCSVPath, CString &sMoneyLogCSVPath, CString &sBaseResourceCSVPath, CString &sSoldierLogCSVPath, 
					 CString &sWeaponDefenderCSVPath, CString &sHeroEquipLogCSVPath, CString &sProficiencyEquipLogCSVPath, CString &sTaskLogCSVPath, CString &sPlayerBuildingLogCSVPath,
					 CDlgExportLogTool *pDlgExportLogTool, unsigned int iTime, bool isCancel)
{
	// 删除创建的临时CSV文件
	if (!sHeroAttributeCSVPath.IsEmpty())
	{
		::DeleteFile(sHeroAttributeCSVPath.GetBuffer());
	}

	if (!sExperienceCSVPath.IsEmpty())
	{
		::DeleteFile(sExperienceCSVPath.GetBuffer());
	}

	if (!sHeroUpgradeCSVPath.IsEmpty())
	{
		::DeleteFile(sHeroUpgradeCSVPath.GetBuffer());
	}

	if (!sStateCSVPath.IsEmpty())
	{
		::DeleteFile(sStateCSVPath.GetBuffer());
	}

	if (!sHeroSkillDotCSVPath.IsEmpty())
	{
		::DeleteFile(sHeroSkillDotCSVPath.GetBuffer());
	}

	if (!sHeroSkillLogCSVPath.IsEmpty())
	{
		::DeleteFile(sHeroSkillLogCSVPath.GetBuffer());
	}

	if (!sMilitaryCreditCSVPath.IsEmpty())
	{
		::DeleteFile(sMilitaryCreditCSVPath.GetBuffer());
	}

	if (!sTechnologicalDotCSVPath.IsEmpty())
	{
		::DeleteFile(sTechnologicalDotCSVPath.GetBuffer());
	}

	if (!sPlayerCityNumLogCSVPath.IsEmpty())
	{
		::DeleteFile(sPlayerCityNumLogCSVPath.GetBuffer());
	}

	if (!sPlayerHeroNumLogCSVPath.IsEmpty())
	{
		::DeleteFile(sPlayerHeroNumLogCSVPath.GetBuffer());
	}

	if (!sMoneyLogCSVPath.IsEmpty())
	{
		::DeleteFile(sMoneyLogCSVPath.GetBuffer());
	}

	if (!sBaseResourceCSVPath.IsEmpty())
	{
		::DeleteFile(sBaseResourceCSVPath.GetBuffer());
	}

	if (!sSoldierLogCSVPath.IsEmpty())
	{
		::DeleteFile(sSoldierLogCSVPath.GetBuffer());
	}

	if (!sWeaponDefenderCSVPath.IsEmpty())
	{
		::DeleteFile(sWeaponDefenderCSVPath.GetBuffer());
	}

	if (!sHeroEquipLogCSVPath.IsEmpty())
	{
		::DeleteFile(sHeroEquipLogCSVPath.GetBuffer());
	}

	if (!sProficiencyEquipLogCSVPath.IsEmpty())
	{
		::DeleteFile(sProficiencyEquipLogCSVPath.GetBuffer());
	}

	if (!sTaskLogCSVPath.IsEmpty())
	{
		::DeleteFile(sTaskLogCSVPath.GetBuffer());
	}

	if (!sPlayerBuildingLogCSVPath.IsEmpty())
	{
		::DeleteFile(sPlayerBuildingLogCSVPath.GetBuffer());
	}

	if (!isCancel)
	{
		AfxMessageBox("导入日志文件内容到数据库执行完成！！！");
	}


	if (NULL != g_ProgressBar.m_hWnd)
	{
		::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
	}

	::CloseHandle(pDlgExportLogTool->m_hThread);
	pDlgExportLogTool->m_hThread = NULL;

#ifdef _DEBUG
	iTime = GetTickCount() - iTime;

	CString sTem;
	sTem.Format("%d", iTime);

	AfxMessageBox(sTem);
#endif
}

unsigned int __stdcall ExportLogToDB(void * ptr)
{
	unsigned int iTime = GetTickCount();
	CDlgExportLogTool *pDlgExportLogTool = (CDlgExportLogTool *)ptr;

	if (NULL == pDlgExportLogTool)
	{
		return 0;
	}

	otl_connect &db_ = pDlgExportLogTool->db_;
	std::list<CString> &lstFilePath = pDlgExportLogTool->m_lstFilePath;
	std::list<CString>::iterator itBeg = lstFilePath.begin();
	std::list<CString>::iterator itEnd = lstFilePath.end();
	CStdioFile file;
	BOOL isOpenSucess = true;
	CString sRowText,sTmp;
	int iIndex = 0;
	unsigned int iType = 0;
	unsigned int iCount = 0;
	unsigned int iTotalCount = 0;

	CString sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
		sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
		sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath;

	std::string sHeroAttribute, sExperience, sHeroUpgrade, sState, sHeroSkillDot, sHeroSkillLog, sMilitaryCredit, sTechnologicalDot, sPlayerCityNumLog, 
		sPlayerHeroNumLog, sMoneyLog, sBaseResource, sSoldierLog, sWeaponDefender, sHeroEquipLog, sProficiencyEquipLog, sTaskLog, sPlayerBuildingLog;
	
	std::string sSql;

	FILE * fp = NULL;

	unsigned int iSize = static_cast<unsigned int>(lstFilePath.size());
	
	for (; itBeg!=itEnd; ++itBeg)
	{
		if (g_isCancelProgressBar)
		{
			dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, 
				sTechnologicalDotCSVPath, sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, 
				sHeroEquipLogCSVPath, sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
			return 0;
		}

		sRowText.Empty();
		isOpenSucess = file.Open(itBeg->GetBuffer(), CFile::modeRead /*| CFile::typeText*/);

		if (!isOpenSucess)
		{
			continue;
		}

		while(true) 
		{
			if (g_isCancelProgressBar)
			{
				file.Close();

				dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
					sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
					sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);

				return 0;
			}

			// 文件没有结束 
			if(file.ReadString(sRowText)) 
			{ 
				iIndex = -1;

				for (int i=0; i<FIVE; i++)
				{	
					iIndex = sRowText.Find(',', iIndex + 1);

					if (-1 == iIndex)
					{
						break;
					}
				}

				if (-1 == iIndex)
				{
					break;
				}

				sTmp = sRowText.Left(iIndex);

				sRowText = sRowText.Right(sRowText.GetLength() - iIndex - 1);

				iIndex = sRowText.Find(',');
				iType = static_cast<unsigned int>(atoi(sRowText.Left(iIndex)));
				sRowText = sRowText.Right(sRowText.GetLength() - iIndex);

				sRowText = sTmp + sRowText;
			
				switch (iType)
				{
				case HeroAttribute_Type:
					
					if (sHeroAttributeCSVPath.IsEmpty())
					{
						sHeroAttributeCSVPath = *itBeg;
						sHeroAttributeCSVPath.Replace(".log", "_HeroAttribute.csv");
					}
					sHeroAttribute += sRowText + "\r\n";
					break;
				case Experience_Type:

					if (sExperienceCSVPath.IsEmpty())
					{
						sExperienceCSVPath = *itBeg;
						sExperienceCSVPath.Replace(".log", "_experience.csv");
					}

					sExperience += sRowText + "\r\n";
					break;
				case HeroUpgrade_Type:

					if (sHeroUpgradeCSVPath.IsEmpty())
					{
						sHeroUpgradeCSVPath = *itBeg;
						sHeroUpgradeCSVPath.Replace(".log", "_HeroUpgrade.csv");
					}

					sHeroUpgrade += sRowText + "\r\n";
					break;
				case State_Type:

					if (sStateCSVPath.IsEmpty())
					{
						sStateCSVPath = *itBeg;
						sStateCSVPath.Replace(".log", "_state.csv");
					}

					sState += sRowText + "\r\n";
					break;
				case HeroSkillDot_Type:

					if (sHeroSkillDotCSVPath.IsEmpty())
					{
						sHeroSkillDotCSVPath = *itBeg;
						sHeroSkillDotCSVPath.Replace(".log", "_heroSkillDot.csv");
					}

					sHeroSkillDot += sRowText + "\r\n";
					break;
				case HeroSkillLog_Type:
					if (sHeroSkillLogCSVPath.IsEmpty())
					{
						sHeroSkillLogCSVPath = *itBeg;
						sHeroSkillLogCSVPath.Replace(".log", "_heroSkillLog.csv");
					}

					sHeroSkillLog += sRowText + "\r\n";
					break;
				case MilitaryCredit_Type:
					
					if (sMilitaryCreditCSVPath.IsEmpty())
					{
						sMilitaryCreditCSVPath = *itBeg;
						sMilitaryCreditCSVPath.Replace(".log", "_militaryCredit.csv");
					}

					sMilitaryCredit += sRowText + "\r\n";
					break;
				case TechnologicalDot_Type:
					
					if (sTechnologicalDotCSVPath.IsEmpty())
					{
						sTechnologicalDotCSVPath = *itBeg;
						sTechnologicalDotCSVPath.Replace(".log", "_technologicalDot.csv");
					}

					sTechnologicalDot += sRowText + "\r\n";
					break;
				case PlayerCityNumLog_Type:
					
					if (sPlayerCityNumLogCSVPath.IsEmpty())
					{
						sPlayerCityNumLogCSVPath = *itBeg;
						sPlayerCityNumLogCSVPath.Replace(".log", "_playerCityNumLog.csv");
					}

					sPlayerCityNumLog += sRowText + "\r\n";
					break;
				case PlayerHeroNumLog_Type:

					if (sPlayerHeroNumLogCSVPath.IsEmpty())
					{
						sPlayerHeroNumLogCSVPath = *itBeg;
						sPlayerHeroNumLogCSVPath.Replace(".log", "_playerHeroNumLog.csv");
					}

					sPlayerHeroNumLog += sRowText + "\r\n";
					break;
				case MoneyLog_Type:

					if (sMoneyLogCSVPath.IsEmpty())
					{
						sMoneyLogCSVPath = *itBeg;
						sMoneyLogCSVPath.Replace(".log", "_moneyLog.csv");
					}

					sMoneyLog += sRowText + "\r\n";
					break;
				case BaseResource_Type:
					if (sBaseResourceCSVPath.IsEmpty())
					{
						sBaseResourceCSVPath = *itBeg;
						sBaseResourceCSVPath.Replace(".log", "_baseResource.csv");
					}

					sBaseResource += sRowText + "\r\n";
					break;
				case SoldierLog_Type:
					if (sSoldierLogCSVPath.IsEmpty())
					{
						sSoldierLogCSVPath = *itBeg;
						sSoldierLogCSVPath.Replace(".log", "_soldierLog.csv");
					}

					sSoldierLog += sRowText + "\r\n";
					break;
				case WeaponDefender_Type:
					if (sWeaponDefenderCSVPath.IsEmpty())
					{
						sWeaponDefenderCSVPath = *itBeg;
						sWeaponDefenderCSVPath.Replace(".log", "_weaponDefender.csv");
					}

					sWeaponDefender += sRowText + "\r\n";
					break;
				case HeroEquipLog_Type:
					if (sHeroEquipLogCSVPath.IsEmpty())
					{
						sHeroEquipLogCSVPath = *itBeg;
						sHeroEquipLogCSVPath.Replace(".log", "_heroEquipLog.csv");
					}

					sHeroEquipLog += sRowText + "\r\n";
					break;
				case ProficiencyEquipLog_Type:
					if (sProficiencyEquipLogCSVPath.IsEmpty())
					{
						sProficiencyEquipLogCSVPath = *itBeg;
						sProficiencyEquipLogCSVPath.Replace(".log", "_proficiencyEquipLog.csv");
					}

					sProficiencyEquipLog += sRowText + "\r\n";
					break;
				case TaskLog_Type:
					if (sTaskLogCSVPath.IsEmpty())
					{
						sTaskLogCSVPath = *itBeg;
						sTaskLogCSVPath.Replace(".log", "_taskLog.csv");
					}

					sTaskLog += sRowText + "\r\n";
					break;
				case PlayerBuildingLog_Type:
					if (sPlayerBuildingLogCSVPath.IsEmpty())
					{
						sPlayerBuildingLogCSVPath = *itBeg;
						sPlayerBuildingLogCSVPath.Replace(".log", "_playerBuildingLog.csv");
					}

					sPlayerBuildingLog += sRowText + "\r\n";
					break;
				default:
					break;
				}

			} 
			//文件结束 
			else
			{
				break; 
			}
		} 

		if (-1 == iIndex)
		{
			continue;
		}
		
		iCount++;
		iTotalCount++;
		file.Close();

		// 写文件到CSV
		if (iCount > TEN || iTotalCount == iSize)
		{
			iCount = 0;

			if (!sHeroAttributeCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sHeroAttributeCSVPath.GetBuffer(), "w+");
				
				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sHeroAttribute.c_str(), sHeroAttribute.length(), 1, fp);
					fclose(fp);
				}

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				sHeroAttribute.clear();

				sSql = "bulk   insert HeroAttribute from '" + sHeroAttributeCSVPath + "' with (FIELDTERMINATOR=',')";
				
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件HeroAttribute出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
					
				}

			}

			if (!sExperienceCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				//streamCorps << std::endl;
				//std::string &sTmp = streamCorps.str();
		
				fp = ::fopen(sExperienceCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sExperience.c_str(), sExperience.length(), 1, fp);
					fclose(fp);
				}

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				sExperience.clear();

				// 把CSV文件导入到数据库中
				sSql = "bulk   insert experience from '" + sExperienceCSVPath + "' with (FIELDTERMINATOR=',')";
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件experience出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sHeroUpgradeCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				//streamCountryOnTheMarch << std::endl;
				//std::string &sTmp = streamCountryOnTheMarch.str();
		
				fp = ::fopen(sHeroUpgradeCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sHeroUpgrade.c_str(), sHeroUpgrade.length(), 1, fp);
					fclose(fp);
				}

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				sHeroUpgrade.clear();

				// 把CSV文件导入到数据库中
				sSql = "bulk   insert HeroUpgrade from '" + sHeroUpgradeCSVPath + "' with (FIELDTERMINATOR=',')";
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件HeroUpgrade出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sStateCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sStateCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sState.c_str(), sState.length(), 1, fp);
					fclose(fp);
				}


				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				sState.clear();

				// 把CSV文件导入到数据库中
				sSql = "bulk   insert state from '" + sStateCSVPath + "' with (FIELDTERMINATOR=',')";
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件state出现异常！！！", MB_OK | MB_ICONERROR);
#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sHeroSkillDotCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sHeroSkillDotCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sHeroSkillDot.c_str(), sHeroSkillDot.length(), 1, fp);
					fclose(fp);
				}

			
				sHeroSkillDot.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert heroSkillDot from '" + sHeroSkillDotCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件heroSkillDot出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sHeroSkillLogCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sHeroSkillLogCSVPath.GetBuffer(), "w+");
			
				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sHeroSkillLog.c_str(), sHeroSkillLog.length(), 1, fp);
					fclose(fp);
				}


				sHeroSkillLog.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert heroSkillLog from '" + sHeroSkillLogCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件heroSkillLog出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sMilitaryCreditCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sMilitaryCreditCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sMilitaryCredit.c_str(), sMilitaryCredit.length(), 1, fp);
					fclose(fp);
				}


				sMilitaryCredit.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert militaryCredit from '" + sMilitaryCreditCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件militaryCredit出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sTechnologicalDotCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sTechnologicalDotCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sTechnologicalDot.c_str(), sTechnologicalDot.length(), 1, fp);
					fclose(fp);
				}


				sTechnologicalDot.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert technologicalDot from '" + sTechnologicalDotCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件technologicalDot出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sPlayerCityNumLogCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sPlayerCityNumLogCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sPlayerCityNumLog.c_str(), sPlayerCityNumLog.length(), 1, fp);
					fclose(fp);
				}


				sPlayerCityNumLog.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert playerCityNumLog from '" + sPlayerCityNumLogCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件playerCityNumLog出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sPlayerHeroNumLogCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sPlayerHeroNumLogCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sPlayerHeroNumLog.c_str(), sPlayerHeroNumLog.length(), 1, fp);
					fclose(fp);
				}

				sPlayerHeroNumLog.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert playerHeroNumLog from '" + sPlayerHeroNumLogCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件playerHeroNumLog出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}


			if (!sMoneyLogCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sMoneyLogCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sMoneyLog.c_str(), sMoneyLog.length(), 1, fp);
					fclose(fp);
				}


				sMoneyLog.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert moneyLog from '" + sMoneyLogCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件moneyLog出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sBaseResourceCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sBaseResourceCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sBaseResource.c_str(), sBaseResource.length(), 1, fp);
					fclose(fp);
				}


				sBaseResource.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert baseResource from '" + sBaseResourceCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件baseResource出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sSoldierLogCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sSoldierLogCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sSoldierLog.c_str(), sSoldierLog.length(), 1, fp);
					fclose(fp);
				}


				sSoldierLog.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert soldierLog from '" + sSoldierLogCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件soldierLog出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sWeaponDefenderCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sWeaponDefenderCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sWeaponDefender.c_str(), sWeaponDefender.length(), 1, fp);
					fclose(fp);
				}
	

				sWeaponDefender.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert weaponDefender from '" + sWeaponDefenderCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件weaponDefender出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sHeroEquipLogCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sHeroEquipLogCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sHeroEquipLog.c_str(), sHeroEquipLog.length(), 1, fp);
					fclose(fp);
				}


				sHeroEquipLog.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert heroEquipLog from '" + sHeroEquipLogCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件heroEquipLog出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sProficiencyEquipLogCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sProficiencyEquipLogCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sProficiencyEquipLog.c_str(), sProficiencyEquipLog.length(), 1, fp);
					fclose(fp);
				}


				sProficiencyEquipLog.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert proficiencyEquipLog from '" + sProficiencyEquipLogCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件proficiencyEquipLog出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}


			if (!sTaskLogCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sTaskLogCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sTaskLog.c_str(), sTaskLog.length(), 1, fp);
					fclose(fp);
				}


				sTaskLog.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert taskLog from '" + sTaskLogCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件taskLog出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}

			if (!sPlayerBuildingLogCSVPath.IsEmpty())
			{	
				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				fp = ::fopen(sPlayerBuildingLogCSVPath.GetBuffer(), "w+");

				if (NULL == fp)
				{
					assert(0 && "打开文件失败！");
				}
				else
				{
					::fwrite(sPlayerBuildingLog.c_str(), sPlayerBuildingLog.length(), 1, fp);
					fclose(fp);
				}


				sPlayerBuildingLog.clear();

				if (g_isCancelProgressBar)
				{
					dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
						sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
						sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, true);
					return 0;
				}

				// 把CSV文件导入到数据库中;
				sSql = "bulk   insert playerBuildingLog from '" + sPlayerBuildingLogCSVPath + "' with (FIELDTERMINATOR=',')" ;
				try
				{
					otl_cursor::direct_exec(db_, sSql.c_str());
					db_.commit();
				}
				catch(otl_exception &exp)
				{ 
					if (NULL != g_ProgressBar.m_hWnd)
					{
						::PostMessage(g_ProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
					}
					AfxMessageBox("导入CSV文件playerBuildingLog出现异常！！！", MB_OK | MB_ICONERROR);

#ifdef _DEBUG
					CString sMsg(exp.msg);
					sMsg += "	";
					sMsg += exp.stm_text;
					sMsg += "	";
					sMsg += exp.var_info;
					sMsg += "\n";
					AfxOutputDebugString(sMsg);
#endif
					return 0;
				}
			}
		}

		if (NULL != g_ProgressBar.m_hWnd)
		{
			g_ProgressBar.setProgressBarPos(static_cast<int>(iTotalCount * 100 / iSize));
		}
	}

	//sSql.clear();
	//sSql.str("");

	dealWithEndWork(sHeroAttributeCSVPath, sExperienceCSVPath, sHeroUpgradeCSVPath, sStateCSVPath, sHeroSkillDotCSVPath, sHeroSkillLogCSVPath, sMilitaryCreditCSVPath, sTechnologicalDotCSVPath, 
		sPlayerCityNumLogCSVPath, sPlayerHeroNumLogCSVPath, sMoneyLogCSVPath, sBaseResourceCSVPath, sSoldierLogCSVPath, sWeaponDefenderCSVPath, sHeroEquipLogCSVPath, 
		sProficiencyEquipLogCSVPath, sTaskLogCSVPath, sPlayerBuildingLogCSVPath, pDlgExportLogTool, iTime, false);

	//streamIndividualRole.clear();
	//streamIndividualRole.str("");

	//streamCorps.clear();
	//streamCorps.str("");
	
	//streamCountryOnTheMarch.clear();
	//streamCountryOnTheMarch.str("");

	//streamTaskSystem.clear();
	//streamTaskSystem.str("");

	//streamBattleSystem.clear();
	//streamBattleSystem.str("");

	return 0;
}



IMPLEMENT_DYNAMIC(CDlgExportLogTool, CDialog)

CDlgExportLogTool::CDlgExportLogTool(const CString &sDBDSN, const CString &sDBUser, const CString &sDBPassword, CWnd* pParent /*=NULL*/)
: CDialog(CDlgExportLogTool::IDD, pParent), m_hThread(NULL), m_sDBDSN(sDBDSN), m_sDBUser(sDBUser), m_sDBPassword(sDBPassword)
{

}

CDlgExportLogTool::~CDlgExportLogTool()
{
	if (NULL != m_hThread)
	{
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	db_.logoff();
}

void CDlgExportLogTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgExportLogTool, CDialog)
	ON_BN_CLICKED(IDC_BTN_EXPORT_FILES, &CDlgExportLogTool::OnBnClickedBtnExportFiles)
	ON_BN_CLICKED(IDC_BTN_EXPORT_FOLDER, &CDlgExportLogTool::OnBnClickedBtnExportFolder)
END_MESSAGE_MAP()

BOOL CDlgExportLogTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	otl_connect::otl_initialize();

	std::string con_str = "UID=";
	con_str += m_sDBUser;
	con_str += ";PWD=";
	con_str += m_sDBPassword;
	con_str += ";DSN=";
	con_str += m_sDBDSN;

	try 
	{ 
		db_.rlogon(con_str.c_str());
	}
	catch(otl_exception&)
	{ 
		db_.logoff();
		MessageBox(_T("连接数据库失败，请检查配置文件"), _T("错误"), MB_OK | MB_ICONERROR);
		
		EndDialog(0);
		return FALSE;
	}

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// CDlgExportLogTool 消息处理程序

void CDlgExportLogTool::OnBnClickedBtnExportFiles()
{
	m_lstFilePath.clear();
	TCHAR szFile[MAX_PATH * MAX_PATH] = {0};

	CFileDialog dlg(TRUE,
		NULL, 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT | OFN_ENABLESIZING ,
		_T("Log Files(*.log)|*.log|All Files(*.*)|*.*||"),
		NULL);

	dlg.m_ofn.lpstrFile = szFile;
	dlg.m_ofn.nMaxFile = MAX_PATH * MAX_PATH;

	if (IDOK == dlg.DoModal())
	{
		POSITION pos = dlg.GetStartPosition();

		while (NULL != pos)
		{
			CString filePath = dlg.GetNextPathName(pos);
			m_lstFilePath.push_back(filePath);
		}

		if (!m_lstFilePath.empty())
		{
			createWorkThreadTToExportLogToDB();
		}
	}
}

void CDlgExportLogTool::OnBnClickedBtnExportFolder()
{
	m_lstFilePath.clear();

	TCHAR szDir[MAX_PATH] = {0};
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;
	bi.lpszTitle = _T("选择需要导出日志的所在文件夹");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	ITEMIDLIST *pidl  = SHBrowseForFolder(&bi);

	if(NULL == pidl) 
	{
		return;
	}

	SHGetPathFromIDList(pidl, szDir);

	Recurse(szDir);

	if (!m_lstFilePath.empty())
	{
		createWorkThreadTToExportLogToDB();
	}
}

void CDlgExportLogTool::Recurse(LPCTSTR szDir)   
{   
	if (NULL == szDir)
	{
		return;
	}

	CString  strDir(szDir);   
	strDir += _T("\\*.*");

	CFileFind  finder; 
	BOOL isFind = finder.FindFile(strDir);   

	while (isFind)   
	{   
		isFind = finder.FindNextFile();   

		if (finder.IsDots())  
		{
			continue;   
		}	

		CString strFilePath = finder.GetFilePath();

		m_lstFilePath.push_back(strFilePath);

		if (finder.IsDirectory())   
		{   
			CString str = finder.GetFilePath();   

			Recurse(str);   
		}   
	}  
}

void CDlgExportLogTool::createWorkThreadTToExportLogToDB()
{
	if (NULL != m_hThread)
	{
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	unsigned int tid = 0;
	m_hThread = (HANDLE)::_beginthreadex(0, 0, 
		ExportLogToDB, (void *)this, 0, &tid);

	if (NULL == m_hThread)
	{
		MessageBox(_T("创建工作线程失败，请重试"), _T("错误"), MB_OK | MB_ICONERROR);	

		return;
	}

	g_isCancelProgressBar = false;
	g_ProgressBar.DoModal();
}