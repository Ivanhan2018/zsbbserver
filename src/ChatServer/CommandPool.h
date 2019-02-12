#ifndef COMMANDPOOL
#define COMMANDPOOL
#pragma once
#include "Global_Macro.h"
#include <map>
typedef int (*Function)(void *);

#define ADDRECVCMD( CommandID, CommandFunc ) {{AddRecvCmd( CommandID, (Function)CommandFunc );}}
class CCommandPool
{
public:
	CCommandPool(void);
	~CCommandPool(void);

	void InitRecvCmdPool(void);
	void UnInitRecvCmdPool(void);

	static std::map<PROTOCOL_CHAT, Function> m_RecvCommandPool;

private:
	void AddRecvCmd( PROTOCOL_CHAT CmdID, Function lpFunction );
};
#endif