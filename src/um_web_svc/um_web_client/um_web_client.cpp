#include "um_write_thread.h"
#include "um_read_thread.h"
#include "um_web_client.h"
#include "aes_plus.h"
#include <objbase.h>


aes eak;

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//启动服务(增加断开重连功能，重连间隔单位毫秒.<=0表示不重连)
int UMWEBCLIENT StartWebClient(const char * ip,short port,int timeout,int reconntime)
{
	if (!ip) return -4;
	WSADATA wsaData;
	WSAStartup( 0x0202, &wsaData );

	if (!um_public_client::Instance().login(ip,port,timeout,reconntime))
	{
		return 0;  
	}
	um_public_client::Instance().SetActive(true);

	um_write_thread::Instance().startTask(1);
	um_read_thread::OnInstance();
	um_read_thread::Instance()->start();

	unsigned char key[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	if (c_name(set_key)(key, 16, both, &eak) != aes_good)
	{
		StopWebClient();
		return 0;
	}
	return 1;
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//结束服务
int UMWEBCLIENT StopWebClient()
{
	if (!um_public_client::Instance().GetActive()) return 0;

	um_public_client::Instance().LoginOut();
	um_public_client::Instance().SetActive(false);

	um_write_thread::Instance().stopTask();
	um_read_thread::Instance()->getThreadControl().join();
	um_read_thread::DownInstance();

	WSACleanup();
	//um_public_client::Instance().SetActive(false);
	return 1;
}

//查询套接字状态--0断开 1连接
int UMWEBCLIENT NetState()
{
	if (um_public_client::Instance().getsocket()==INVALID_SOCKET)
		return 0;
	else
		return 1;
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//检查帐户是否可以注册
int UMWEBCLIENT CheckUserName(const char * UserName)
{
	if (!UserName) return -4;
	if ( (strchr(UserName,';') !=NULL) || strlen(UserName)>username_size-1 ) return -4;
	if (um_public_client::Instance().getsocket()==INVALID_SOCKET) return -1;

	tagCheckUser * checkuser=new tagCheckUser;
	checkuser->serialize(um_public_client::Instance().getindex(),UserName);
	

	HDN hdn = new tagDataNode;
	hdn->sbuf=(char*)checkuser;
	hdn->event=(long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn)) return -3;

		int rlt;
		if (hdn->rlt)
			rlt = ((tagRepResult*)(hdn->rbuf))->bsucceed;
		else
			rlt = -1;
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;		
	}
	else 
	{
		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return -2;
	}
}


//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//注册账户
int UMWEBCLIENT RegUser(const char* username,const char* pwd,const char * ip,const char* em,
						const int fromAccountID,const char * fromurl, const char * qq, const char * memo, int accounttype)
{
	if (!qq || !memo) return -4;
	if (!username || !pwd || !ip || !em || !fromurl) return -4;
	if (strchr(username,';') !=NULL) return -4;
	if ( strlen(username)>username_size-1 || strlen(pwd) > password_size-1 || strlen(ip)>fromip_size-1 || \
		 strlen(em)>email_size-1 || strlen(fromurl)>fromurl_size-1) 
	{
		return -4;
	}

	if (um_public_client::Instance().getsocket()==INVALID_SOCKET) return -1;

	unsigned int sip=inet_addr(ip);
	if (sip == INADDR_NONE)
	{
		return -3;
	}

	tagRegUser* reguser = new tagRegUser;
	reguser->serialize(um_public_client::Instance().getindex(),username,pwd,sip,em,fromAccountID,fromurl, qq, memo, accounttype);
	if (c_name(encrypt)( (unsigned char *)reguser->userinfo.password, (unsigned char *)reguser->userinfo.password , &eak) != aes_good)
	{
		return -3;
	}

	HDN hdn = new tagDataNode;
	hdn->sbuf=(char*)reguser;
	hdn->event=(long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;
	
	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn)) return -3;

		int rlt;
		if (hdn->rlt)
			rlt = ((tagRepResult*)(hdn->rbuf))->bsucceed;
		else
			rlt = -1;

		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;		
	}
	else 
	{
		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//帐户登陆
int UMWEBCLIENT UserLogin(const char * UserName,const char * password,const char * ip)
{
	if (!UserName || !password || !ip) return -4;
	if ( strlen(UserName)>username_size-1 || strlen(password)>password_size-1 ) return -4;

	if (um_public_client::Instance().getsocket()==INVALID_SOCKET) return -1;
	
	unsigned int addr = inet_addr(ip);
	if (addr==INADDR_NONE)
	{	
		return -3;
	}

	tagUserLogin* userlogin = new tagUserLogin;
	userlogin->serialize(um_public_client::Instance().getindex(),UserName,password,addr);
	if (c_name(encrypt)( (unsigned char *)userlogin->password, (unsigned char *)userlogin->password, &eak) != aes_good)
	{
		return -3;
	}

	HDN hdn = new tagDataNode;
	hdn->sbuf=(char*)userlogin;
	hdn->event=(long)CreateEvent(NULL,false,false,NULL);
	
	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;

		int rlt;
		if (hdn->rlt)
			rlt = ((tagRepResult*)(hdn->rbuf))->bsucceed;
		else
			rlt = -1;

		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return rlt;		
	}
	else 
	{
		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//更改帐户密码
int UMWEBCLIENT UpdatePassword(const char * UserName,const char * OldPassword,const char * NewPassword)
{
	if (!UserName || !OldPassword || !NewPassword) return -4;
	if ( strlen(UserName)>username_size-1 || strlen(OldPassword)>password_size-1 || strlen(NewPassword)>password_size-1 ) return -4;
	if (um_public_client::Instance().getsocket()==INVALID_SOCKET) return -1;

	tagModifyPassword* mpwd = new tagModifyPassword;
	mpwd->serialize(um_public_client::Instance().getindex(),UserName,OldPassword,NewPassword);
	
	if (c_name(encrypt)( (unsigned char *)mpwd->old_password, (unsigned char *)mpwd->old_password, &eak) != aes_good)
	{
		return -3;
	}
	if (c_name(encrypt)( (unsigned char *)mpwd->new_password, (unsigned char *)mpwd->new_password, &eak) != aes_good)
	{
		return -3;
	}

	HDN hdn = new tagDataNode;
	hdn->sbuf=(char*)mpwd;
	hdn->event=(long)CreateEvent(NULL,false,false,NULL);
	
	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
	
		int rlt;
		if (hdn->rlt)
			rlt = ((tagRepResult*)(hdn->rbuf))->bsucceed;
		else
			rlt = -1;

		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return rlt;		
	}
	else 
	{
		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return -2;
	}
}


//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//更改用户密码--带question及answer
int UMWEBCLIENT UpdatePasswordExt(const char * UserName,const char * OldPassword,const char * NewPassword,const char * ques,\
								  const char * ans)
{
	if (!UserName || !OldPassword || !NewPassword ||!ques ||!ans) return -4;
	if (strlen(UserName)>username_size-1 || strlen(OldPassword)>password_size-1 || strlen(NewPassword)>password_size-1 || \
		strlen(ques)>question_size-1 || strlen(ans)>answer_size-1) return -4;
	
	if (um_public_client::Instance().getsocket()==INVALID_SOCKET) return -1;

	tagModifyPasswordExt* mpwd = new tagModifyPasswordExt;
	mpwd->serialize(um_public_client::Instance().getindex(),UserName,OldPassword,NewPassword,ques,ans);
	
	if (c_name(encrypt)( (unsigned char *)mpwd->old_password, (unsigned char *)mpwd->old_password, &eak) != aes_good)
	{
		return -3;
	}
	if (c_name(encrypt)( (unsigned char *)mpwd->new_password, (unsigned char *)mpwd->new_password, &eak) != aes_good)
	{
		return -3;
	}

	HDN hdn = new tagDataNode;
	hdn->sbuf=(char*)mpwd;
	hdn->event=(long)CreateEvent(NULL,false,false,NULL);
	
	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
	
		int rlt;
		if (hdn->rlt)
			rlt = ((tagRepResult*)(hdn->rbuf))->bsucceed;
		else
			rlt = -1;

		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return rlt;		
	}
	else 
	{
		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//更改用户密码(不验证)
int UMWEBCLIENT UpdatePasswordNoCheck(const char * UserName,const char * NewPassword)
{
	if (!UserName || !NewPassword) return -4;
	if ( strlen(UserName)>username_size-1 || strlen(NewPassword)>password_size-1 ) return -4;
	if (um_public_client::Instance().getsocket()==INVALID_SOCKET) return -1;

	tagModifyPasswordNoCheck* mpwd = new tagModifyPasswordNoCheck;
	mpwd->serialize(um_public_client::Instance().getindex(),UserName,NewPassword);

	if (c_name(encrypt)( (unsigned char *)mpwd->new_password, (unsigned char *)mpwd->new_password, &eak) != aes_good)
	{
		return -3;
	}

	HDN hdn = new tagDataNode;
	hdn->sbuf=(char*)mpwd;
	hdn->event=(long)CreateEvent(NULL,false,false,NULL);
	
	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
	
		int rlt;
		if (hdn->rlt)
			rlt = ((tagRepResult*)(hdn->rbuf))->bsucceed;
		else
			rlt = -1;

		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return rlt;		
	}
	else 
	{
		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//帐户充值	
//int UMWEBCLIENT AddMoney(const char * UserName,int money,int score)
int UMWEBCLIENT AddMoney(const char * UserName,float money,int godian,int score,int addtype)
{
	if (!UserName) return -4;
	if (strlen(UserName)>username_size-1) return -4;
	if (um_public_client::Instance().getsocket()==INVALID_SOCKET) return -1;
	
	tagAddMoney* addmoney = new tagAddMoney;
	addmoney->serialize(um_public_client::Instance().getindex(),UserName,money,godian, score, addtype);

	HDN hdn = new tagDataNode;
	hdn->sbuf=(char*)addmoney;
	hdn->event=(long)CreateEvent(NULL,false,false,NULL);
	
	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;
	

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;

		int rlt;
		if (hdn->rlt)
			rlt = ((tagRepResult*)(hdn->rbuf))->bsucceed;
		else
			rlt = -1;

		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return rlt;		
	}
	else 
	{
		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//扣钱
int UMWEBCLIENT SubGoDian(const int AccountID,const int GoDian, const char * desc)
{
	if (!desc) return -4;
	if (um_public_client::Instance().getsocket()==INVALID_SOCKET) return -1;
	tagSubMoney * submoney = new tagSubMoney;
	submoney->serialize(um_public_client::Instance().getindex(),AccountID,GoDian, desc);

	HDN hdn = new tagDataNode;
	hdn->sbuf=(char*)submoney;
	hdn->event=(long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;

		int rlt;
		if (hdn->rlt)
			rlt = ((tagRepResult*)(hdn->rbuf))->bsucceed;
		else
			rlt = -1;

		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return rlt;		
	}
	else 
	{
		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//查询GO点
int UMWEBCLIENT GetGoDian(const int AccountID,int& GoDian)
{
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;
	tagGetMoney * getmoney = new tagGetMoney;
	getmoney->serialize(um_public_client::Instance().getindex(),AccountID);

	HDN hdn = new tagDataNode;
	hdn->sbuf=(char*)getmoney;
	hdn->event=(long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;

		int rlt;
		if (hdn->rlt)
		{
			rlt = ((tagRepGetMoney*)(hdn->rbuf))->bsucceed;
			if (rlt == 1)   GoDian = ((tagRepGetMoney*)(hdn->rbuf))->GoDian;
		}
		else 
		{
			rlt = -1;
		}
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else 
	{
		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//扣除积分
int UMWEBCLIENT SubScore(const int AccountID,const int score)
{

	if (um_public_client::Instance().getsocket()==INVALID_SOCKET) return -1;
	tagSubScore * subscore= new tagSubScore;
	subscore->serialize(um_public_client::Instance().getindex(),AccountID,score);

	HDN hdn = new tagDataNode;
	hdn->sbuf=(char*)subscore;
	hdn->event=(long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;

		int rlt;
		if (hdn->rlt)
			rlt = ((tagRepResult*)(hdn->rbuf))->bsucceed;
		else
			rlt = -1;

		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return rlt;		
	}
	else 
	{
		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//获取积分
int UMWEBCLIENT GetScore(const int AccountID,int& score)
{
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;
	tagGetScore * getscore = new tagGetScore;
	getscore->serialize(um_public_client::Instance().getindex(),AccountID);

	HDN hdn = new tagDataNode;
	hdn->sbuf=(char*)getscore;
	hdn->event=(long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;

		int rlt;
		if (hdn->rlt)
		{
			rlt = ((tagRepGetScore*)(hdn->rbuf))->bsucceed;
			if (rlt == 1)  score = ((tagRepGetScore*)(hdn->rbuf))->score;
		}
		else 
		{
			rlt = -1;
		}
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else 
	{
		um_public_client::Instance().DeleteData( ((header*)hdn->sbuf)->index);
		return -2;
	}
}


//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//获取用户答案
int UMWEBCLIENT GetAnswer(const char * UserName,char * Answer)
{
	if (!UserName || !Answer) return -4;
	if (strlen(UserName)>username_size-1) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagGetAnswer * answer = new tagGetAnswer;
	answer->serialize(um_public_client::Instance().getindex(),UserName);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)answer;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;

		int rlt;
		if (hdn->rlt)
		{
			rlt = ((tagRepGetAnswer*)(hdn->rbuf))->bsucceed;
			if (rlt == 1) strcpy(Answer,((tagRepGetAnswer*)(hdn->rbuf))->answer);
		}
		else 
		{
			rlt = -1;
		}
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else 
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;		
	}
}


//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//获取用户邮件
int UMWEBCLIENT GetEmail(const char * UserName,char * Email)
{
	if (!UserName || !Email) return -4;
	if (strlen(UserName)>username_size-1) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagGetEmail * email = new tagGetEmail;
	email->serialize(um_public_client::Instance().getindex(),UserName);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)email;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;

		int rlt;
		if (hdn->rlt)
		{
			rlt = ((tagRepGetEmail*)(hdn->rbuf))->bsucceed;
			if (rlt == 1) strcpy(Email,((tagRepGetEmail*)(hdn->rbuf))->email);
		}
		else 
		{
			rlt = -1;
		}
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else 
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;		
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//更改用户Email
int UMWEBCLIENT UpdateEmail(const char * UserName,const char * OldEmail,const char * NewEmail)
{
	if (!UserName || !OldEmail || !NewEmail) return -4;
	if ( strlen(UserName)>username_size-1 || strlen(OldEmail)>email_size-1 || strlen(NewEmail)>email_size-1 ) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;
	tagUpdateEmail * email = new tagUpdateEmail;
	email->serialize(um_public_client::Instance().getindex(),UserName,OldEmail,NewEmail);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)email;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
		int rlt ;
		if (hdn->rlt)
			rlt = ((tagRepResult*)(hdn->rbuf))->bsucceed;
		else
			rlt = -1;
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//个人排行 
int UMWEBCLIENT GetPlayerListCs(const int maxrows,int* rows, tagPlayerInfo** ppPlayerInfo)
{
	//if (maxrows<=0) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagGetPlayerList* player = new tagGetPlayerList;
	player->serialize(um_public_client::Instance().getindex(),maxrows);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)player;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
		
		int rlt ;
		if (hdn->rlt)  //返回成功
		{
			tagRepPlayerList * repplayer= (tagRepPlayerList*)(hdn->rbuf);
			rlt = repplayer->bSucceed;
			*rows = repplayer->bufflen / sizeof(tagPlayerInfo);
			if (*rows>0)
			{
				tagPlayerInfo * psource=(tagPlayerInfo *)(hdn->rbuf+sizeof(tagRepPlayerList));
				*ppPlayerInfo = (tagPlayerInfo*)CoTaskMemAlloc(repplayer->bufflen);
				memcpy(*ppPlayerInfo,psource,repplayer->bufflen);
			}
		}		
		else
			rlt = -1;

		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//获工会排行For C#
int UMWEBCLIENT GetGongHuiListCs(const int maxrows, int* rows, tagGongHuiInfo** ppGongHuiInfo)
{
	//if (maxrows<=0) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagGetGongHuiList* gonghui = new tagGetGongHuiList;
	gonghui->serialize(um_public_client::Instance().getindex(),maxrows);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)gonghui;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
		
		int rlt ;
		if (hdn->rlt)  //返回成功
		{
			tagRepGongHuiList * repgonghui= (tagRepGongHuiList *)(hdn->rbuf);
			rlt = repgonghui->bSucceed;
			*rows = repgonghui->bufflen / sizeof(tagGongHuiInfo);
			if (*rows>0)
			{
				tagGongHuiInfo * psource=(tagGongHuiInfo *)(hdn->rbuf+sizeof(tagRepGongHuiList));
				*ppGongHuiInfo = (tagGongHuiInfo*)CoTaskMemAlloc(repgonghui->bufflen);
				memcpy(*ppGongHuiInfo,psource,repgonghui->bufflen);
			} 
		}		
		else
			rlt = -1;

		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//获取GM账户列表
int UMWEBCLIENT GetGmUserList(char*& buf,int& len)
{
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	header * gmlist=new header;
	gmlist->serialize(sizeof(header),um_public_client::Instance().getindex(),CM_GMUSERLIST);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)gmlist;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
		int rlt ;
		if (hdn->rlt)  //返回成功
		{
			tagGmUserList * repgmlist=(tagGmUserList *)(hdn->rbuf);
			rlt = repgmlist->bSucceed;
			len=repgmlist->buflen;
			if ((rlt==1) && (len>0))
			{
				buf=new char[len];
				memcpy(buf,hdn->rbuf+sizeof(tagGmUserList),repgmlist->buflen);
			}
		}		
		else
			rlt = -1;

		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}

//清除内存
void UMWEBCLIENT DeleteBuf(char *& buf)
{
	if (buf)
	{
		delete [] buf;
		buf=0;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//获取用户扩展信息
int UMWEBCLIENT GetUserExtInfo(const int AccountID,char * FactName,char * NameID,char * Born,char * TelCode,char * Sex,\
							   char * Address,char * PostCode,char * FromAccountName)
{
	if (!FactName || !NameID || !Born || !TelCode || !Sex || !Address || !PostCode || !FromAccountName) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagReqGetUserExtInfo * userextinfo= new tagReqGetUserExtInfo;
	userextinfo->serialize(um_public_client::Instance().getindex(),AccountID);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)userextinfo;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;

		int rlt;
		if (hdn->rlt)
		{
			tagRpsGetUserExtInfo* p=(tagRpsGetUserExtInfo*)(hdn->rbuf);
			rlt =p->bSucceed;
			if (rlt == 1)  //成功且有信息
			{
				strcpy(FactName,p->UserExtInfo.FactName);
				strcpy(NameID,p->UserExtInfo.NameID);
				strcpy(Born,p->UserExtInfo.Born);
				strcpy(TelCode,p->UserExtInfo.TelCode);
				strcpy(Sex,p->UserExtInfo.Sex);
				strcpy(Address,p->UserExtInfo.Address);
				strcpy(PostCode,p->UserExtInfo.PostCode);
				strcpy(FromAccountName,p->UserExtInfo.FromAccountName);
			}
			else if (rlt == 9)  //成功但无信息
			{
				rlt =1;
				FactName[0]=0;
				NameID[0]=0;
				Born[0]=0;
				TelCode[0]=0;
				Sex[0]=0;
				Address[0]=0;
				PostCode[0]=0;
				FromAccountName[0]=0;
			}
		}
		else 
		{
			rlt = -1;
		}
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else 
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;		
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//更改用户扩展信息
int UMWEBCLIENT UpdateUserExtInfo(const int AccountID,const char * FactName,const char * NameID,const char * Born,\
								  const char * TelCode,const char * Sex,const char * Address,const char * PostCode,const char * FromAccountName)
{
	if (!FactName || !NameID || !Born || !TelCode || !Sex || !Address || !PostCode || !FromAccountName) return -4;
	if (strlen(FactName)>=factname_size || strlen(NameID)>=nameid_size || strlen(Born)>=born_size || strlen(TelCode)>=telcode_size ||\
		strlen(Sex)>=sex_size || strlen(Address)>=address_size || strlen(PostCode)>postcode_size || strlen(FromAccountName)>=username_size) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagReqUpdateUserExtInfo * userextinfo = new tagReqUpdateUserExtInfo;
	userextinfo->UserExtInfo.serialize(AccountID,FactName,NameID,Born,TelCode,Sex,Address,PostCode,FromAccountName);
	userextinfo->serialize(um_public_client::Instance().getindex(),NULL);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)userextinfo;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;

		int rlt;
		if (hdn->rlt)
		{
			tagRpsUpdateUserExtInfo* p=(tagRpsUpdateUserExtInfo*)(hdn->rbuf);
			rlt =p->bSucceed;
		}
		else 
		{
			rlt = -1;
		}
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else 
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;		
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//充值统计
int UMWEBCLIENT DataTotal(int spType,int CurrPage,int PageSize,const char * StrWhere,const char * AccountType,const char * BeginDate,\
							  const char * EndDate,const char * SearchType, int* rows, char** ppinfo)
{
	if (!StrWhere || !AccountType || !BeginDate || !EndDate || !SearchType) return -4;
	if (strlen(StrWhere)>=strwhere_size || strlen(AccountType)>=accounttype_size || strlen(BeginDate)>=date_size || \
		strlen(EndDate)>=date_size || strlen(SearchType) > 10) return -4;
	if (CurrPage<=0 || PageSize<=0 || PageSize>maxpage_size) return -4;
	
	int cmd=-1;
	switch (spType)
	{
	case stAddMoney:
		cmd=CM_ADDMONEYTOTAL;
		break;
	case stAddMoneyEx:
		cmd=CM_ADDMONEYTOTALEX;
		break;
	case stAddMoneyThird:
		cmd=CM_ADDMONEYTOTALTHIRD;
		break;
	case stPlayerList:
		cmd=CM_PLAYERLISTTOTAL;
		break;
	case stUserList:
		cmd=CM_USERLISTTOTAL;
		break;
	case stUserList3:
		cmd=CM_USERLISTTOTAL3;
		break;
	case stPlayerFriendList:
		cmd=CM_PLAYERFRIENDLIST;
		break;
	default:
		return -4;
	}

	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagReqAddMoneyTotal * req = new tagReqAddMoneyTotal;
	req->serialize(um_public_client::Instance().getindex(),cmd,CurrPage,PageSize,StrWhere,AccountType,BeginDate,EndDate, SearchType);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)req;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;

		int rlt;
		if (hdn->rlt)  //返回成功
		{
			tagRpsAddMoneyTotal * p =(tagRpsAddMoneyTotal*)(hdn->rbuf);
			rlt = p->bSucceed;
			
			switch (spType)
			{
			case stAddMoney:
				*rows = p->BufLen / sizeof(tagAddMoneyTotalInfo);
				break;
			case stAddMoneyEx:
				*rows = p->BufLen / sizeof(tagAddMoneyTotalInfoEx);
				break;
			case stAddMoneyThird:
				*rows = p->BufLen / sizeof(tagAddMoneyTotalInfoThird);
				break;
			case stPlayerList:
				*rows = p->BufLen / sizeof(tagPlayerListTotal);
				break;
			case stUserList:
				*rows = p->BufLen / sizeof(tagUserListTotal);
				break;
			case stUserList3:
				*rows = p->BufLen / sizeof(tagUserListTotal3);
				break;
			case stPlayerFriendList:
				*rows = p->BufLen / sizeof(tagPlayerFriendList);
				break;
			}
			if (*rows>0)
			{
				char * psource=hdn->rbuf+sizeof(tagRpsAddMoneyTotal);
				*ppinfo = (char*)CoTaskMemAlloc(p->BufLen);
				memcpy(*ppinfo,psource,p->BufLen);
			}
			//switch (spType)
			//{
			//case stAddMoney:
			//	*rows = p->BufLen / sizeof(tagAddMoneyTotalInfo);
			//	if (*rows>0)
			//	{
			//		tagAddMoneyTotalInfo * psource=(tagAddMoneyTotalInfo *)(hdn->rbuf+sizeof(tagRpsAddMoneyTotal));
			//		*ppinfo = (char*)CoTaskMemAlloc(p->BufLen);
			//		memcpy(*ppinfo,psource,p->BufLen);
			//	}
			//	break;
			//case stAddMoneyEx:
			//	*rows = p->BufLen / sizeof(tagAddMoneyTotalInfoEx);
			//	if (*rows>0)
			//	{
			//		tagAddMoneyTotalInfoEx * psource=(tagAddMoneyTotalInfoEx *)(hdn->rbuf+sizeof(tagRpsAddMoneyTotal));
			//		*ppinfo = (char*)CoTaskMemAlloc(p->BufLen);
			//		memcpy(*ppinfo,psource,p->BufLen);
			//	}
			//	break;
			//case stAddMoneyThird:
			//	*rows = p->BufLen / sizeof(tagAddMoneyTotalInfoThird);
			//	if (*rows>0)
			//	{
			//		tagAddMoneyTotalInfoThird * psource=(tagAddMoneyTotalInfoThird *)(hdn->rbuf+sizeof(tagRpsAddMoneyTotal));
			//		*ppinfo = (char*)CoTaskMemAlloc(p->BufLen);
			//		memcpy(*ppinfo,psource,p->BufLen);
			//	}
			//	break;
			//case stPlayerList:
			//	*rows = p->BufLen / sizeof(tagPlayerListTotal);
			//	if (*rows>0)
			//	{
			//		tagPlayerListTotal * psource=(tagPlayerListTotal *)(hdn->rbuf+sizeof(tagRpsAddMoneyTotal));
			//		*ppinfo = (char*)CoTaskMemAlloc(p->BufLen);
			//		memcpy(*ppinfo,psource,p->BufLen);
			//	}
			//	break;
			//case stUserList:
			//	*rows = p->BufLen / sizeof(tagUserListTotal);
			//	if (*rows>0)
			//	{
			//		tagUserListTotal * psource=(tagUserListTotal *)(hdn->rbuf+sizeof(tagRpsAddMoneyTotal));
			//		*ppinfo = (char*)CoTaskMemAlloc(p->BufLen);
			//		memcpy(*ppinfo,psource,p->BufLen);
			//	}
			//	break;
			//}
		}		
		else
			rlt = -1;

		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//sp_pgetuserbypid only one row...
int UMWEBCLIENT GetUserByIDTotal(int FromAccountID,const char * BeginDate,const char * EndDate, tagGetUserByIDTotal* ppinfo)
{
	if (!BeginDate || !EndDate) return -4;
	if (strlen(BeginDate)>=date_size || strlen(EndDate)>=date_size) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagReqGetUserByIDTotal * req = new tagReqGetUserByIDTotal;
	req->serialize(um_public_client::Instance().getindex(),FromAccountID,BeginDate,EndDate);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)req;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
		int rlt;
		if(hdn->rlt)
		{
			tagRpsAddMoneyTotal * p =(tagRpsAddMoneyTotal*)(hdn->rbuf);
			rlt = p->bSucceed;
			if (rlt && p->BufLen)
				*ppinfo=*(tagGetUserByIDTotal*)(hdn->rbuf+sizeof(tagRpsAddMoneyTotal));
		}
		else rlt=-1;
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
int UMWEBCLIENT GetRecordCount(const char * sqlstr,int * rcount)
{
	if (!sqlstr || strlen(sqlstr)>=sqlstr_size) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagReqGetRecordCount * req = new tagReqGetRecordCount;
	req->serialize(um_public_client::Instance().getindex(),sqlstr);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)req;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
		int rlt;
		if(hdn->rlt)
		{
			tagRpsGetRecordCount * p =(tagRpsGetRecordCount*)(hdn->rbuf);
			rlt = p->bSucceed;
			if (rlt) *rcount=p->RecCount;
		}
		else rlt=-1;
		
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}

//函数返回值统一定义：
//>0成功 0失败 -1通常是套接字断开,-2超时,-3内部异常错误,-4 参数错语
//sp_pgetuserbypid only one row...
int UMWEBCLIENT GetLogInfo(int PageIndex,int PageSize , const char * strWhere, const char * BeginDate, const char * EndDate, int * rows,tagLogInfo ** info)
{
	if (!PageIndex || !PageSize || !strWhere || !BeginDate || !EndDate) return -4;
	if (strlen(BeginDate)>=50 || strlen(EndDate)>=50) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagReqGetLogInfo * req = new tagReqGetLogInfo;
	req->serialize(um_public_client::Instance().getindex(),PageIndex,PageSize,strWhere, BeginDate, EndDate);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)req;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
		int rlt;
		if(hdn->rlt)
		{
			tagRpsGetLogInfo * p =(tagRpsGetLogInfo*)(hdn->rbuf);
			rlt = p->bSucceed;
			if (rlt && p->BufLen)
			{
				*info = (tagLogInfo*)CoTaskMemAlloc(p->BufLen);
				memcpy(*info, hdn->rbuf+sizeof(tagRpsGetLogInfo), p->BufLen);
				*rows = p->BufLen / sizeof(tagLogInfo);
			}
		}
		else rlt=-1;
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}

int UMWEBCLIENT GetLogOnline(int PageIndex,int PageSize , const char * strWhere, const char * BeginDate, const char * EndDate, int * rows,tagLogOnline ** online)
{
	if (!PageIndex || !PageSize || !strWhere || !BeginDate || !EndDate) return -4;
	if (strlen(BeginDate)>=50 || strlen(EndDate)>=50) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagReqGetLogOnline * req = new tagReqGetLogOnline;
	req->serialize(um_public_client::Instance().getindex(),PageIndex,PageSize,strWhere, BeginDate, EndDate);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)req;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
		int rlt;
		if(hdn->rlt)
		{
			tagRpsGetLogOnline * p =(tagRpsGetLogOnline*)(hdn->rbuf);
			rlt = p->bSucceed;
			if (rlt && p->BufLen)
			{
				*online = (tagLogOnline*)CoTaskMemAlloc(p->BufLen);
				memcpy(*online, hdn->rbuf+sizeof(tagRpsGetLogOnline), p->BufLen);
				*rows = p->BufLen / sizeof(tagLogOnline);
			}
		}
		else rlt=-1;
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}

int UMWEBCLIENT GetLogOnlineEx(int PageIndex,int PageSize , const char * strWhere, int accounttype, const char * BeginDate, const char * EndDate, int * rows,tagLogOnlineEx ** online)
{
	if (!PageIndex || !PageSize || !strWhere || !BeginDate || !EndDate) return -4;
	if (strlen(BeginDate)>=50 || strlen(EndDate)>=50) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagReqGetLogOnlineEx * req = new tagReqGetLogOnlineEx;
	req->serialize(um_public_client::Instance().getindex(),PageIndex,PageSize,strWhere, accounttype, BeginDate, EndDate);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)req;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
		int rlt;
		if(hdn->rlt)
		{
			tagRpsGetLogOnlineEx * p =(tagRpsGetLogOnlineEx*)(hdn->rbuf);
			rlt = p->bSucceed;
			if (rlt && p->BufLen)
			{
				*online = (tagLogOnlineEx*)CoTaskMemAlloc(p->BufLen);
				memcpy(*online, hdn->rbuf+sizeof(tagRpsGetLogOnlineEx), p->BufLen);
				*rows = p->BufLen / sizeof(tagLogOnlineEx);
			}
		}
		else rlt=-1;
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}

int UMWEBCLIENT GetGodianLog(int PageIndex,int PageSize , const char * strWhere, int accounttype, const char * BeginDate, const char * EndDate, int * rows, tagGodianLog ** online)
{
	if (!PageIndex || !PageSize || !strWhere || !BeginDate || !EndDate) return -4;
	if (strlen(BeginDate)>=50 || strlen(EndDate)>=50) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagReqGetGodianLog * req = new tagReqGetGodianLog;
	req->serialize(um_public_client::Instance().getindex(),PageIndex,PageSize,strWhere, accounttype, BeginDate, EndDate);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)req;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
		int rlt;
		if(hdn->rlt)
		{
			tagRpsGetGodianLog * p =(tagRpsGetGodianLog*)(hdn->rbuf);
			rlt = p->bSucceed;
			if (rlt && p->BufLen)
			{
				*online = (tagGodianLog*)CoTaskMemAlloc(p->BufLen);
				memcpy(*online, hdn->rbuf+sizeof(tagRpsGetGodianLog), p->BufLen);
				*rows = p->BufLen / sizeof(tagGodianLog);
			}
		}
		else rlt=-1;
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}



int UMWEBCLIENT GetLogPerf(int PageIndex,int PageSize , const char * strWhere, const char * BeginDate, const char * EndDate, int * rows, tagLogPerf ** perf)
{
	if (!PageIndex || !PageSize || !strWhere || !BeginDate || !EndDate) return -4;
	if (strlen(BeginDate)>=50 || strlen(EndDate)>=50) return -4;
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagReqGetLogPerf * req = new tagReqGetLogPerf;
	req->serialize(um_public_client::Instance().getindex(),PageIndex,PageSize,strWhere, BeginDate, EndDate);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)req;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
		int rlt;
		if(hdn->rlt)
		{
			tagRpsGetLogPerf * p =(tagRpsGetLogPerf*)(hdn->rbuf);
			rlt = p->bSucceed;
			if (rlt && p->BufLen)
			{
				*perf = (tagLogPerf*)CoTaskMemAlloc(p->BufLen);
				memcpy(*perf, hdn->rbuf+sizeof(tagRpsGetLogPerf), p->BufLen);
				*rows = p->BufLen / sizeof(tagLogPerf);
			}
		}
		else rlt=-1;
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}


int UMWEBCLIENT GetResultByProc(const char * procname,const char * p1,const char * p2,const char * p3,const char * p4,const char * p5, char ** output)
{
	if (!procname || !p1 || !p2 || !p3 || !p4 || !p5) return -4;
	if (strlen(procname)>=50 || strlen(p1)>=50 || strlen(p2)>=50 || 
		strlen(p3)>=50 || strlen(p4)>=50 || strlen(p5)>=100 )
		return -4;
	
	if (um_public_client::Instance().getsocket() == INVALID_SOCKET) return -1;

	tagReqGetResultByProc * req = new tagReqGetResultByProc;
	req->serialize(um_public_client::Instance().getindex(),procname, p1, p2, p3, p4, p5);

	HDN hdn = new tagDataNode;
	hdn->sbuf = (char *)req;
	hdn->event = (long)CreateEvent(NULL,false,false,NULL);

	if (hdn->event == NULL) return -3;
	if (!um_public_client::Instance().InsertData(hdn)) return -3;
	if (!um_write_thread::Instance().putQueue(hdn->sbuf)) return -3;

	if (WaitForSingleObject((HANDLE)(hdn->event),um_public_client::Instance().gettimeout()) == WAIT_OBJECT_0)
	{
		if (!um_public_client::Instance().FindData(((header*)hdn->sbuf)->index,hdn) ) return -3;
		int rlt;
		if(hdn->rlt)
		{
			tagRpsGetResultByProc * p =(tagRpsGetResultByProc*)(hdn->rbuf);
			rlt = p->bsucceed;
			if (rlt)
			{
				*output = (char *)CoTaskMemAlloc(50);
				memcpy(*output, p->result, 50);
			}
		}
		else 
			rlt=-1;
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return rlt;
	}
	else
	{
		um_public_client::Instance().DeleteData(((header*)hdn->sbuf)->index);
		return -2;
	}
}
