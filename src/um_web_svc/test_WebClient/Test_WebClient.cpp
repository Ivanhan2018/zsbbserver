#include <windows.h>

#include "um_web.h"
#include "um_web_client.h"

#include <iostream>
#include <string>

using namespace std;

void test_StartWebClient(int timeout,int reconntime)
{
	int rlt = StartWebClient("192.168.1.243",6006,timeout,reconntime);
	printf("StartWebClient Result=%d\n",rlt);
	return;
}

void test_StopWebClient()
{
	int rlt = StopWebClient();
	printf("StopWebClient Result=%d\n",rlt);
	return;
}

void test_CheckUserName()
{
	printf("testing test_CheckUserName...\n");
	char gstrname[16] = "xfsZSBB_";
	
	int rlt = CheckUserName(gstrname);
	printf("CheckUserName Result=%d\n",rlt);
	return ;
}

void test_RegUser()
{
	printf("testing test_RegUser...\n");
	int rlt = RegUser("tesaat_121","123456","192.168.1.161","laichanghe@163.com",1,"www.sina.com.cn",0,0, 99);
	printf("RegUser Result=%d\n",rlt);

	rlt = RegUser("tesaat_122","123456","192.168.1.161","laichanghe@163.com",1,"www.sina.com.cn","","",99);
	printf("RegUser Result=%d\n",rlt);

	rlt = RegUser("tesaat_123","123456","192.168.1.161","laichanghe@163.com",1,"www.sina.com.cn","www.sina.com.cn","www.sina.com.cn", 99);
	printf("RegUser Result=%d\n",rlt);
	return ;
}

void test_UserLogin()
{
	printf("testing test_UserLogin...\n");
	int rlt =  UserLogin("laich","theGameIsGood","192.168.1.161");
	printf("UserLogin Result=%d\n",rlt);
	return;
}

void test_UpdatePassword()
{
	printf("testing test_UpdatePassword...\n");
	int rlt =  UpdatePassword("laich","theGameIsGood","0123456789");
	printf("UpdatePassword Result=%d\n",rlt);
	return;
}

void test_UpdatePasswordNoCHeck()
{
	printf("testing test_UpdatePasswordNoCHeck...\n");
	int rlt =  UpdatePasswordNoCheck("laich","laich123");
	printf("UpdatePassword Result=%d\n",rlt);
	return;
}

void test_GetEmail()
{
	printf("testing test_GetEmail...\n");
	char email[100];
	int rlt = GetEmail("laich",email);
	printf("GetEmail Result=%d\n",rlt);
	if (rlt == 1)
	{
		printf("%s\n",email);
	}
}

void test_AddMoney()
{
	//printf("testing test_AddMoney...\n");
	//int rlt = AddMoney("laich",160,10);
	//printf("AddMoney Result=%d\n",rlt);
}

void test_GetAnswer()
{
	printf("testing test_GetAnswer...\n");
	char answer[200];
	int rlt=GetAnswer("laich",answer);
	printf("GetAnswer Result=%d\n",rlt);
	if (rlt==1) printf("Answer=%s\n",answer);
}

void test_UpdateEmail()
{
	printf("testing test_UpdateEmail...\n");
	int rlt = UpdateEmail("laich","laichanghe@163.com","laichanghe@sina.com");
	printf("UpdateEmail Result=%d\n",rlt);
}


void test_SubMoney()
{
	printf("testing test_SubMoney...\n");
	int rlt = SubGoDian(140007,50, 0);
	printf("SubMoney Result=%d\n",rlt);

	rlt = SubGoDian(140007,50, "");
	printf("SubMoney Result=%d\n",rlt);

	rlt = SubGoDian(140007,50, "日你妈1次");
	printf("SubMoney Result=%d\n",rlt);

	rlt = SubGoDian(140007,50, "日你妈2次");
	printf("SubMoney Result=%d\n",rlt);

	rlt = SubGoDian(140007,50, "日你妈3次");
	printf("SubMoney Result=%d\n",rlt);

	rlt = SubGoDian(140007,50, "日你妈4次");
	printf("SubMoney Result=%d\n",rlt);
}

void test_GetMoney()
{
	//printf("testing test_GetMoney...\n");
	//int money_(0);
//	int rlt = GetMoney(100000,money_);
//	printf("GetMoney Result=%d\n",rlt);
	//if (rlt > 0) printf("money=%d \n",money_);
}

void test_GetPlayerListCs()
{
	printf("testing test_GetPlayerListCs...\n");
	tagPlayerInfo* pp=0;
	int prows;
	int rlt = GetPlayerListCs(0, &prows,&pp);
	if (rlt==1)
	{
		std::cout<<"record count:"<<prows<<std::endl;
		if (prows==0) return;
		for (int i=0;i<prows;i++)
		{
			std::cout<<"Rec:" << i+1 << "----------------------------------------------\n";
			std::cout<<"   ID="<<pp[i].id<<" PlayerID="<<pp[i].PlayerID<<" PlayerName="<<pp[i].PlayerName << " IP:" << pp[i].Address <<std::endl;
		}
		CoTaskMemFree(pp);
	}
}


void test_GongHuiListCs()
{
	
	printf("testing test_GongHuiListCs...\n");
	tagGongHuiInfo* pp=0;
	int prows;
	int rlt = GetGongHuiListCs(0, &prows,&pp);
	if (rlt==1)
	{
		std::cout<<"record count:"<<prows<<std::endl;
		if (prows==0) return;
		for (int i=0;i<prows;i++)
		{
			std::cout<<"Rec:" << i+1 << "----------------------------------------------\n";
			std::cout<<"   " << pp[i].ID<<","<<pp[i].ConID<<","<<pp[i].ConName<<","<<pp[i].Level<<","<<pp[i].UserCount<<","
				<<pp[i].PlayerID<<","<<pp[i].ChairmanName<<","<<pp[i].Country<<","<<pp[i].CreateTime<<","<< pp[i].Address << std::endl;
		}
		CoTaskMemFree(pp);
	}
}


void test_GetScore()
{
	printf("testing test_GetScore...\n");
	int money_(0);
	int rlt = GetScore(100000,money_);
	printf("GetMoney Result=%d\n",rlt);
	if (rlt > 0) printf("money=%d \n",money_);
}

void test_SubScore()
{
	printf("testing test_SubScore...\n");
	int money_(0);
	int rlt = SubScore(100000,5);
	printf("SubScore Result=%d\n",rlt);
}

void test_GetGmUserList()
{
	printf("testing test_GetGmUserList...\n");
	//int i1=0;
	//int i2=0;
	char * buf;
	int len;
	//for (int i=0;i<1000;i++)
	{
		buf=0;
		len=0;
		int rlt = GetGmUserList(buf,len);
		if (rlt==1)
		{
			//++i1;
			int rows = len / sizeof(tagGmUserInfo);
			tagGmUserInfo * gminfo=(tagGmUserInfo*)buf;
			printf("rows %d \n",rows);
			printf("accountid   username permission \n");
			for (int i=0;i<rows;i++)
				printf("%d %s %d \n",gminfo[i].account_id,gminfo[i].user_name,gminfo[i].permission);
			//if (buf) delete [] buf;
			DeleteBuf(buf);
		}
		else 
		{
			printf("GetGmUserList Failed: errorcoce:%d\n",rlt);
			//++i2;
		}
	}		
	//printf("%d,%d\n",i1,i2);

}

void test_GetUserExtInfo()
{
	printf("testing test_GetUserExtInfo...\n");

	char factname[20];
	char nameid[20];
	char born[20];
	char telcode[20];
	char sex[5];
	char address[200];
	char postcode[10];
	char accountname[16];
	int rlt = GetUserExtInfo(120753,factname,nameid,born,telcode,sex,address,postcode,accountname);
	printf("GetUserExtInfo rlt:%d\n",rlt);
	
	if (rlt==1) 
	{
		printf("FactName:%s\n",factname);
		printf("nameid:%s\n",nameid);
		printf("born:%s\n",born);
		printf("telcode:%s\n",telcode);
		printf("sex:%s\n",sex);
		printf("address:%s\n",address);
		printf("postcode:%s\n",postcode);
		printf("accountname:%s\n",accountname);
	}
}

void test_UpdateUserExtInfo()
{
	printf("testing test_UpdateUserExtInfo...\n");
	int rlt=UpdateUserExtInfo(120753,"赖长河","360111820316001","1982年03月16日","13424181570","男","深圳市福田区新飞扬","518026","");
	printf("UpdateUserExtInfo rlt:%d\n",rlt);
}

void test_UpdatePasswordExt()
{
	printf("testing test_UpdatePasswordExt...\n");
	int rlt=UpdatePasswordExt("laich","theGameIsGood","123456789","我的真实姓名","赖长河");
	printf("UpdatePasswordExt rlt:%d\n",rlt);
}

void test_DataTotal()
{
	printf("testing test_DataTotal...\n");

	int size;
	char * outArray;
	int rlt ;
	for(int type=1;type<7;type++)
	{
		rlt=DataTotal(type,1,30,"","1","2008-01-01","2009-12-31","", &size,&outArray);
		if (rlt!=1)
		{
			printf("调用 testDataTotal spType=%d,失败 errorcode=%d\n",type,rlt);
			continue;
		}
		printf("testDataTotal spType=%d, 记录数:%d\n",type,size);
		if (size==0) continue ;
	
		switch (type)
		{
			case 1:
			{
				tagAddMoneyTotalInfo*  manArray = (tagAddMoneyTotalInfo*)outArray;
				for( int i = 0; i < size; i++ )
				{
					printf( "RecNo:%d,%d,%d,%d\n", i,manArray[i].AccountID,manArray[i].CurrMoney,manArray[i].NewMoney);         
				}
				break;
			}
			case 2:
			{
				tagAddMoneyTotalInfoEx* manArray =(tagAddMoneyTotalInfoEx*)outArray;
				for( int i = 0; i < size; i++ )
				{
					printf( "RecNo:%d,%d,%d,%d,%d,%d\n ", i,manArray[i].AccountID,manArray[i].TotalMoney,
						manArray[i].CurrMonthMoney,manArray[i].CurrDayMoney,manArray[i].FromAccountID);
				}
				break;
			}
			case 3:
			{
				tagAddMoneyTotalInfoThird* manArray = (tagAddMoneyTotalInfoThird*)outArray;
				for( int i = 0; i < size; i++ )
				{
					printf( "RecNo:%d,%d,%d,%d,%d,%s,%d\n", i,manArray[i].id,manArray[i].AccountID,
						manArray[i].Money,manArray[i].Score,manArray[i].AddDate,manArray[i].AddType);
				}
				break;
			}
			case 4:
			{
				tagPlayerListTotal* manArray = (tagPlayerListTotal*)outArray;
				for( int i = 0; i < size; i++ )
				{
					printf( "RecNo:%d,%d,%s,%d,%d,%d,%s,%s\n", i,manArray[i].AccountID,manArray[i].RegDate,
						manArray[i].FromAccountID,manArray[i].Levels,manArray[i].TaskNum,manArray[i].LastLoginTime,\
						manArray[i].regip);

				}
				break;
			}
			case 5:
			{
				tagUserListTotal* manArray =(tagUserListTotal*)outArray;
				for( int i = 0; i < size; i++ )
				{
					printf( "RecNo:%d, %d, %d, %d, %d, %d\n", i,manArray[i].FromAccountID,manArray[i].RegNum,\
						manArray[i].TwoLogins,manArray[i].ActiveNum,manArray[i].onelogtimes);
				}
				break;
			}
		}
		CoTaskMemFree(outArray);
	}
}

void test_tagUserListTotal3()
{
	printf("testing test_tagUserListTotal3...\n");

	int size;
	char * outArray;
	int rlt ;

	rlt=DataTotal(6,1,20,"1=1","1","","","1", &size,&outArray);
	if (rlt!=1)
	{
		printf("调用 testDataTotal spType=%d,失败 errorcode=%d\n",6,rlt);
		return ;
	}
	printf("testDataTotal spType=%d, 记录数:%d\n",6,size);
	if (size==0)
		return ;

	tagUserListTotal3* manArray =(tagUserListTotal3*)outArray;
	for( int i = 0; i < size; i++ )
	{
		printf( "RecNo:%d, %d, %d, %d, %d, %d, %d, %d, %d\n", i,manArray[i].FromAccountID,manArray[i].RegNum,\
			manArray[i].TwoLogins,manArray[i].ActiveNum,manArray[i].onelogtimes, manArray[i].Level3Num,manArray[i].Level15Num, manArray[i].ValidNum);
	}
	CoTaskMemFree(outArray);
}

void test_getplayerfriendlist()
{
	printf("testing test_getplayerfriendlist...\n");

	int size;
	char * outArray;
	int rlt ;

	rlt=DataTotal(7,1,20,"1=1","1","","", "", &size,&outArray);
	if (rlt!=1)
	{
		printf("调用 testDataTotal spType=%d,失败 errorcode=%d\n",7,rlt);
		return ;
	}
	printf("testDataTotal spType=%d, 记录数:%d\n",7,size);
	if (size==0)
		return ;

	tagPlayerFriendList* manArray =(tagPlayerFriendList*)outArray;
	for( int i = 0; i < size; i++ )
	{
	printf( "RecNo:%d,%d,%s,%d,%d,%d,%s,%s,%s,%d,%d\n", i,manArray[i].AccountID,manArray[i].RegDate,
		manArray[i].FromAccountID,manArray[i].Levels,manArray[i].TaskNum,manArray[i].LastLoginTime,
		manArray[i].regip,manArray[i].AccountName,manArray[i].cpuid,manArray[i].GoDian);
	}
	CoTaskMemFree(outArray);
}

void test_GetUserByIDTotal()
{
	printf("testing test_GetUserByIDTotal...\n");

	tagGetUserByIDTotal info;
	int rlt = GetUserByIDTotal(1,"","",&info);
	printf("testGetUserByIDTotal Result=%d\n",rlt);
	if (rlt==1)
	{
		printf("%d,%d,%d,%d,%d,%d,%d\n",info.i1,info.i2,info.i3,info.i4,info.i5,info.i6,info.i7);
	}
}

void test_GetRecordCount()
{
	printf("testing test_GetRecordCount...\n");
			
	int rlt ,rcount;
	rlt = GetRecordCount("select count(distinct fromaccountid) from vgameuser where  1=1 and fromaccountid<20000",&rcount);
	printf("testGetRecordCount Result=%d\n",rlt);

	if (rlt==1)
	{
		printf("Recount=%d\n",rcount);
	}
}

void test_NoReconn()
{
	while (true)
	{
		test_StartWebClient(3000,0);
		test_GetMoney();
		test_StopWebClient();
		getchar();
	}
}

void test_Reconn()
{
	test_StartWebClient(3000,50);
	while (true)
	{
		test_GetMoney();
		getchar();
	}
	test_StopWebClient();
	getchar();
}

void test_loop()
{
	printf("test_loop...\n");
	test_StartWebClient(3000,50);

	//to do list...
	test_StopWebClient();
}

void test_GetLogInfo()
{
	int rows = 0;
	tagLogInfo * info = 0;

	GetLogInfo(1, 20, "", "", "", &rows, &info);
	CoTaskMemFree(info);
}

void test_GetLogOnline()
{
	int rows = 0;
	tagLogOnline * online = 0;

	GetLogOnline(1, 20, "", "", "", &rows, &online);
	CoTaskMemFree(online);
}

void test_GetLogPerf()
{
	int rows = 0;
	tagLogPerf * perf = 0;

	GetLogPerf(1, 20, "", "", "", &rows, &perf);
	CoTaskMemFree(perf);
}

void test_GetResultByProc()
{
	char * perf = 0;

	GetResultByProc("sp_GetUnPayMoneyByAccount", "10001", "", "", "", "", &perf);
	CoTaskMemFree(perf);
}

void test_GetLogOnlineEx()
{
	printf("testing test_GetLogOnlineEx...\n");

	int size;
	tagLogOnlineEx * outArray;
	int rlt ;

	rlt=GetLogOnlineEx(1,20,"",1,"","", &size,&outArray);
	if (rlt!=1)
	{
		printf("调用 GetLogOnlineEx 失败 errorcode=%d\n",rlt);
		return ;
	}
	printf("GetLogOnlineEx 记录数:%d\n",size);
	if (size==0) return ;

	tagLogOnlineEx* manArray =(tagLogOnlineEx*)outArray;
	for( int i = 0; i < size; i++ )
	{
		printf( "%s,%s,%d\n", manArray[i].address, manArray[i].datetime, manArray[i].avgnum);
	}
	CoTaskMemFree(outArray);
}

void test_GetGodianLog()
{
	printf("testing test_GetGodianLog...\n");

	int size;
	tagGodianLog * outArray;
	int rlt ;

	rlt=GetGodianLog(1,20,"",1,"","", &size,&outArray);
	if (rlt!=1)
	{
		printf("调用 GetGodianLog 失败 errorcode=%d\n",rlt);
		return ;
	}
	printf("GetGodianLog 记录数:%d\n",size);
	if (size==0) return ;

	tagGodianLog* manArray =(tagGodianLog*)outArray;
	for( int i = 0; i < size; i++ )
	{
		printf( "%d,%d,%d,%s,%s\n", manArray[i].id, manArray[i].playerid, manArray[i].godian,manArray[i].usetype,manArray[i].logdate);
	}
	CoTaskMemFree(outArray);
}


void main()
{
	//test_Reconn();
	//return;

	//test_Noreconn();
	//return;

	test_StartWebClient(50000,0);

	test_GetGodianLog();
	test_GetLogOnlineEx();

	//test_GongHuiListCs();
	//test_GetPlayerListCs();
	//test_RegUser();
	//test_tagUserListTotal3();
	//test_getplayerfriendlist();

	//test_GetResultByProc();
	//test_SubMoney();
//	test_GetLogOnline();
//	test_GetLogInfo();
//	test_GetLogPerf();

	//AddMoney("gameadmin",0,100,100,-1);
	//to do list...
	//test_GongHuiListCs();
	//test_GetPlayerListCs();
//	test_DataTotal();
	
	test_StopWebClient();
	getchar();
	return;
}

