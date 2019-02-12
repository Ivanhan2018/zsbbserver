#ifndef __um_web_client_H__
#define __um_web_client_H__

#include "um_web.h"

#if defined (WIN32)
#	define UMWEBCLIENT __stdcall
#else
#	define UMWEBCLIENT
#endif

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

enum enSpType
{
	stAddMoney=1,		//sp_pgetCZlist
	stAddMoneyEx=2,		//sp_pgetCZ2list
	stAddMoneyThird=3,  //sp_pgetCZdetaillist
	stPlayerList=4,		//sp_pgetplayerlist
	stUserList=5,        //sp_pgetuserlist
	stUserList3=6,        //sp_pgetuserlist3
	stPlayerFriendList=7  //sp_pgetplayerfriendlist
};

//��������ֵͳһ���壺
//>0�ɹ� 0ʧ�� -1ͨ�����׽��ֶϿ�,-2��ʱ,-3�ڲ��쳣����,-4 ��������

//����WebClient����ֻ��һ��
//����˵����ipָ��WebServer�ĵ�ַ��port�˿�,timeout ��ʱ��λΪms
int UMWEBCLIENT StartWebClient(const char * ip,short port,int timeout,int reconntime); //reconntime<=0������ 

//ֹͣWebClient������StartWebClient�ɶ�ʹ��
int UMWEBCLIENT StopWebClient();

//��ȡ��WebServer��������״̬:1 ���� 0�Ͽ�
int UMWEBCLIENT NetState();     

//����˻������Ƿ��ע��
int UMWEBCLIENT CheckUserName(const char * UserName);

//ע���û�
int UMWEBCLIENT RegUser(const char* username,const char* pwd,const char * ip,const char* em,
						const int fromAccountID,const char * fromurl, const char * qq, const char * memo, int accounttype);

//�û���¼���ɹ������û�ID
int UMWEBCLIENT UserLogin(const char * UserName,const char * password,const char * ip);

//�����û�����
int UMWEBCLIENT UpdatePassword(const char * UserName,const char * OldPassword,const char * NewPassword);

//�����û�����(����֤)
int UMWEBCLIENT UpdatePasswordNoCheck(const char * UserName,const char * NewPassword);

//�����û�����--��question��answer
int UMWEBCLIENT UpdatePasswordExt(const char * UserName,const char * OldPassword,const char * NewPassword,const char * ques,\
								  const char * ans);

//��ֵ
//int UMWEBCLIENT AddMoney(const char * UserName,int money,int score);
int UMWEBCLIENT AddMoney(const char * UserName,float money,int godian,int score,int addtype);

//��ȡ�û���
int UMWEBCLIENT GetAnswer(const char * UserName,char * Answer);

//��ȡ�û��ʼ�
int UMWEBCLIENT GetEmail(const char * UserName,char * Email);

//�����û�Email��ַ
int UMWEBCLIENT UpdateEmail(const char * UserName,const char * OldEmail,const char * NewEmail);

//��Ǯ
int UMWEBCLIENT SubGoDian(const int AccountID,const int GoDian, const char * desc);

//��ѯGO��
int UMWEBCLIENT GetGoDian(const int AccountID,int& GoDian);

//��ȡ����
int UMWEBCLIENT GetScore(const int AccountID,int& score);

//�ۻ�����
int UMWEBCLIENT SubScore(const int AccountID,const int score);

//��ȡ������ 
int UMWEBCLIENT GetPlayerListCs(const int maxrows, int* rows, tagPlayerInfo** ppPlayerInfo);

//�񹤻�����
int UMWEBCLIENT GetGongHuiListCs(const int maxrows, int* rows, tagGongHuiInfo** ppGongHuiInfo);

//��ȡGM�˻��б�
int UMWEBCLIENT GetGmUserList(char*& buf,int& len);

//����ڴ�
void UMWEBCLIENT DeleteBuf(char *& buf);

//��ȡ�û���չ��Ϣ
int UMWEBCLIENT GetUserExtInfo(const int AccountID,char * FactName,char * NameID,char * Born,char * TelCode,char * Sex,\
							   char * Address,char * PostCode,char * FromAccountName);

//�����û���չ��Ϣ
int UMWEBCLIENT UpdateUserExtInfo(const int AccountID,const char * FactName,const char * NameID,const char * Born,\
								  const char * TelCode,const char * Sex,const char * Address,const char * PostCode,const char * FromAccountName);

//��ֵͳ��
int UMWEBCLIENT DataTotal(int spType,int CurrPage,int PageSize,const char * StrWhere,const char * AccountType,const char * BeginDate,\
							  const char * EndDate,const char * SearchType, int* rows, char** ppinfo);

//sp_pgetuserbypid
int UMWEBCLIENT GetUserByIDTotal(int FromAccountID,const char * BeginDate,const char * EndDate, tagGetUserByIDTotal* ppinfo);

int UMWEBCLIENT GetRecordCount(const char * sqlstr,int * rcount);

//��ȡ��־�д�����Ϣ
int UMWEBCLIENT GetLogInfo(int PageIndex,int PageSize , const char * strWhere, const char * BeginDate, const char * EndDate, int * rows, tagLogInfo ** info);

//��ȡ��־����������
int UMWEBCLIENT GetLogOnline(int PageIndex,int PageSize , const char * strWhere, const char * BeginDate, const char * EndDate, int * rows, tagLogOnline ** online);

int UMWEBCLIENT GetLogOnlineEx(int PageIndex,int PageSize , const char * strWhere, int accounttype, const char * BeginDate, const char * EndDate, int * rows, tagLogOnlineEx ** online);

int UMWEBCLIENT GetGodianLog(int PageIndex,int PageSize , const char * strWhere, int  accounttype, const char * BeginDate, const char * EndDate, int * rows, tagGodianLog ** online);

//��ȡ��־�г��������
int UMWEBCLIENT GetLogPerf(int PageIndex,int PageSize , const char * strWhere, const char * BeginDate, const char * EndDate, int * rows, tagLogPerf ** perf);

int UMWEBCLIENT GetResultByProc(const char * procname,const char * p1,const char * p2,const char * p3,const char * p4,const char * p5, char ** output);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif /*c_plusplus __cplusplus*/

#endif /* __UMWEBCLIENT__ */


