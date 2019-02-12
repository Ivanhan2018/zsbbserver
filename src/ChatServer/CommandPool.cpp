
#include "Commandpool.h"
#include "ChatSystemLogic.h"
std::map<PROTOCOL_CHAT, Function>  CCommandPool::m_RecvCommandPool;
CCommandPool::CCommandPool(void)
{
}

CCommandPool::~CCommandPool(void)
{
}
void CCommandPool::AddRecvCmd( PROTOCOL_CHAT CmdID, Function lpFunction )
{
	m_RecvCommandPool[CmdID] = lpFunction;
}
void CCommandPool::InitRecvCmdPool()
{
	//ADDRECVCMD(REQ_ENTER_FIGHT_SYSTEM , ReqEnterFightSystem);
	ADDRECVCMD(REQ_ENTER_CHAT_SYSTEM, ReqEnterChatSystem);
	ADDRECVCMD(REQ_ENTER_CHAT_ROOM , ReqEnterChatRoom);
	ADDRECVCMD(REQ_INVITE_OTHER_ENTER_CHAT_ROOM ,ReqInviteOtherEnterChatRoom);
	ADDRECVCMD(REQ_EXIT_CHAT_ROOM , ReqExitRoom);
	ADDRECVCMD(REQ_EXIT_CHAT_SYSTEM ,ReqExitChatSystem);
	ADDRECVCMD(REQ_SNED_CHAT_INFO , ProChatInfo);
	ADDRECVCMD(REQ_CHANGE_MAP_INFO,ReqChangeMap);
}
void CCommandPool::UnInitRecvCmdPool(void)
{
	if ( !m_RecvCommandPool.empty() )
	{
		m_RecvCommandPool.clear();
	}
}