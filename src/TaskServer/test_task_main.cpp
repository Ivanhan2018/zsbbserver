#include "Global_Macro.h"
#include "..\ClientLib\ClientLogin.h"
#include "test_task_task.h"
#include "test_task_logic.h"
#include "IceUtil.h"


#include <iostream>
using namespace std;

long gPackCount(0);

void __stdcall logic_exception(int code, int agent, int from, int to)
{
#ifdef _DEBUG
	printf("logic exception: \nrequest from %d to %d\angent %d\nmessage:%s\n", 
		from, to, agent, errorcode::error_message(code));
#endif
}

int __stdcall data_receiver(const char * buffer, unsigned int length, bool error)
{
	return 0;
	if (error)
   {
		printf("data_receiver have error \n");	  
		return -1;
   }
	
   DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer;
   switch (h->command)
   {
	//玩家任务列表
   case TC_REP_GET_TASK_LIST:
		{
			return RspGetTaskList(buffer,length);
		}
	//玩家任务完成个数  
   case TC_REP_TASK_COUNT:
	   {
			return RspGetTaskCount(buffer,length);		
	   }
	//任务状态更改结果
	case TC_REP_SET_TASK_STATE:
		{
			return RspSetTaskState((REP_SET_TASK_STATE*)buffer);
		}
	//发布任务
	case TC_ISSUE_USER_TASK:
		{
			return RspIssueUserTask((ISSUE_USER_TASK*)buffer);
		}
	//任务完成
	case TC_REP_TASKSTATE_COMPLETE:
		{
			return RspTaskStateComplete((REP_TASKSTATE_COMPLETE*)buffer);
		}
	default:
		{
			return -1;
			break;
		}
   }
}

/********************begin_测试逻辑部分******************************************************************************************/
void test_init(void)
{
	int code = ClientLogin::Instance().Login("192.168.1.18", 8888, "201188", "1",4);   //"101189"

	if (code == errorcode::client_login_succeed)
		printf("登录成功\n");
	else
		printf(errorcode::error_message(code+'\n'));

	if (ClientLogin::Instance().Initialized())
	{
		printf("  MyID: %d\n  AgentID: %d\n  AgentAddress: %s\n  AccountType:%d\n AgentPort: %d\n  Key: %s\n", 
				ClientLogin::Instance().PlayerID(), 
				ClientLogin::Instance().AgentID(), 
				ClientLogin::Instance().AgentAddress(), 
				ClientLogin::Instance().AccountType(),
				ClientLogin::Instance().AgentPort(), 
				ClientLogin::Instance().Key() );

		gTaskTask   = new CTaskTask();
		gTaskTask->init(ClientLogin::Instance().AgentAddress(), ClientLogin::Instance().AgentPort(),data_receiver, logic_exception);
	}
}

void test_finit(void)
{
	gTaskTask->finit();
	delete gTaskTask;
	ClientLogin::Instance().Logout();
}

//获取任务完成个数
void test_get_task_count(unsigned long playerid,CTaskTask* tasktask=NULL)
{
	GET_TASK_COUNT taskcount_;
	taskcount_.serialize(playerid);
	if (tasktask)
		tasktask->SendGetTaskCount(taskcount_);
	else
		gTaskTask->SendGetTaskCount(taskcount_);
}

//获取玩家列表
void test_get_task_list(unsigned long playerid,CTaskTask* tasktask=NULL)
{
	GET_TASK_LIST TaskList_;
	TaskList_.serialize(playerid);
	if (tasktask)
		tasktask->SendGetTaskList(TaskList_);
	else
		gTaskTask->SendGetTaskList(TaskList_);

}

//检测任务是否完成
void test_check_task_ok(unsigned long playerid,CTaskTask* tasktask=NULL)
{
	REQ_CHECK_TASK_OK CheckTaskOK_;
	CheckTaskOK_.serialize(playerid);
	if (tasktask)
		tasktask->SendChekTaskOK(CheckTaskOK_);
	else
		gTaskTask->SendChekTaskOK(CheckTaskOK_);  
}

//接受任务
void test_accept_task(unsigned long playerid,unsigned int taskid,CTaskTask* tasktask=NULL)
{
	SET_TASK_STATE SetTaskState_;
	SetTaskState_.serialize(playerid,taskid,tsYj);
	if (tasktask)
		tasktask->SendSetTaskState(SetTaskState_);
	else
		gTaskTask->SendSetTaskState(SetTaskState_);
}

//提交任务
void test_tj_task(unsigned long playerid,unsigned int taskid,CTaskTask* tasktask=NULL)
{
	SET_TASK_STATE SetTaskState_;
	SetTaskState_.serialize(playerid,taskid,tsTj);
	if (tasktask)
		tasktask->SendSetTaskState(SetTaskState_);
	else
		gTaskTask->SendSetTaskState(SetTaskState_);
}

//删除除任务
void test_delete_task(unsigned long playerid,unsigned int taskid,CTaskTask* tasktask=NULL)
{
	SET_TASK_STATE SetTaskState_;
	SetTaskState_.serialize(playerid,taskid,tsSc);
	if (tasktask) 
		tasktask->SendSetTaskState(SetTaskState_);
	else
		gTaskTask->SendSetTaskState(SetTaskState_);
}

void test_logic()
{
	int key_;

	printf("\n输入0，直接退出！输入1，获取任务列表\n");
	if (cin>>key_ && key_==1)
	{
		test_get_task_list(ClientLogin::Instance().PlayerID());
	}
	else return ;

	Sleep(1000);
	printf("\n输入0，直接退出！输入1,接受任务\n");
	if (cin>>key_ && key_==1)
	{
		test_accept_task(ClientLogin::Instance().PlayerID(),1);
	}
	else return;

	Sleep(1000);
	printf("\n输入0，直接退出！输入1,检测任务是否完成\n");
	if (cin>>key_ && key_==1)
	{
		test_check_task_ok(ClientLogin::Instance().PlayerID());
	}
	else return;
	

	Sleep(1000);
	printf("\n输入0，直接退出！输入1,提交任务\n");
	if (cin>>key_ && key_==1)
	{
		test_tj_task(ClientLogin::Instance().PlayerID(),1);
	}
	else return;
	

	Sleep(1000);
	printf("\n输入0，直接退出！输入1,删除任务\n");
	if (cin>>key_ && key_==1)
	{
		test_delete_task(ClientLogin::Instance().PlayerID(),1);
	}
	else return;

	Sleep(1000);
	printf("\n输入0，直接退出！输入1，获取玩家任务完成数\n");
	if (cin>>key_ && key_==1)
	{
		test_get_task_count(ClientLogin::Instance().PlayerID());
	}
	else return ;
}
/******************************************************end_测试逻辑部分****************************************************/





/*************************begin_压力测试部分******************************************************************************/
bool stop_(true);

class  client_socket
{
private: 
	ClientLogin cl_;
	CTaskTask tasktask_;
public:
	client_socket(){}
	~client_socket(){}


	void init(const char* player_)
	{
		int code = cl_.Login("192.168.1.18", 8888,player_, "0",4);
		if (code != errorcode::client_login_succeed)
			printf(errorcode::error_message(code));

		if (cl_.Initialized())
		{
			//printf("  MyID: %d\n  AgentID: %d\n  AgentAddress: %s\n  AgentPort: %d\n  Key: %s\n", 
			//		cl_.PlayerID(), 
			//		cl_.AgentID(), 
			//		cl_.AgentAddress(), 
			//		cl_.AgentPort(), 
			//		cl_.Key() );
			tasktask_.init(cl_.AgentAddress(), cl_.AgentPort(),data_receiver, logic_exception);
		}
	}

	void finit()
	{
		tasktask_.finit();
		cl_.Logout();
	}

	unsigned long playerid()
	{
		return cl_.PlayerID();
	}

	CTaskTask & tasktask()
	{
		return tasktask_;
	}

	int send(const char * buf, int len)
	{  
		return tasktask_.SendData((void*)buf,len);
	}
};

class test_thread :public IceUtil::Thread
{
private:
	unsigned int taskid_;
public:
	test_thread():taskid_(0){}
	~test_thread(){}

	client_socket cs_;

	void init(const char * player)
	{
		cs_.init(player);
	}
	void finit()
	{
		cs_.finit();
	}
	
	//模拟客户端测试
	/* 一个用户完成任务 50ms */
	void run()
	{
		const static int delay=100;
		while (!stop_ && taskid_<10)
		{
			++taskid_;
			for(int i=0; i<100;i++)
			{
				test_get_task_list(cs_.playerid(),&cs_.tasktask());
				::InterlockedIncrement(&gPackCount);
				Sleep(delay);

				test_accept_task(cs_.playerid(),taskid_,&cs_.tasktask());
				::InterlockedIncrement(&gPackCount);
				Sleep(delay);

				test_check_task_ok(cs_.playerid(),&cs_.tasktask());
				::InterlockedIncrement(&gPackCount);
				Sleep(delay);

				test_tj_task(cs_.playerid(),taskid_,&cs_.tasktask());
				::InterlockedIncrement(&gPackCount);
				Sleep(delay);

				test_get_task_count(cs_.playerid(),&cs_.tasktask());
				::InterlockedIncrement(&gPackCount);
				Sleep(delay);

				//test_delete_task();
				//Sleep(delay);
			}
		}
	}

};


void test_yl()
{
	WSADATA wsaData;
	WSAStartup( 0x0202, &wsaData );

	const int maxthreads_=500;

	printf("请输入玩家账号起始号,基数是200000 .\n");
	int playerid_;
	std::cin>>playerid_;

	char playern_[7];

	IceUtil::Handle<test_thread> vthreads_[maxthreads_];
	unsigned int begin_ = GetTickCount();

	for (int i=0 ; i<maxthreads_; i++)
	{
		vthreads_[i] = new test_thread;

		itoa(playerid_,playern_,10);
		playerid_++;
		vthreads_[i]->init(playern_);
	}

	unsigned int end_ = GetTickCount();

	printf("耗时：%d \nplease enter key to start threadpool.\n",end_-begin_);
	getchar();

	stop_=false;
	begin_=GetTickCount();
	for (int i=0;i<maxthreads_;i++)	
		vthreads_[i]->start();


	unsigned long aa(0);
	unsigned long bb(0);
	int num=0;

	while(++num<300)
	{
		Sleep(1000);
		bb=gPackCount;
		printf("%d \n",bb-aa);
		aa=bb;
	}

	for (int i=0;i<maxthreads_;i++)
	{
		vthreads_[i]->getThreadControl().join();
		vthreads_[i]->finit();
	}
	end_=GetTickCount();
	printf("\n时长=%d ,please enter any ket to exit.\n",end_-begin_);


	getchar();
	
	WSACleanup();

}
/*end_压力测试部分*/

void main()
{
	/*测试逻辑*/
	//WSADATA wsaData;
	//WSAStartup( 0x0202, &wsaData );
	//test_init();
	//test_logic();
	//test_finit();
	//WSACleanup();

/**********************************************************************************/
	test_yl();


	getchar();
	return;
}