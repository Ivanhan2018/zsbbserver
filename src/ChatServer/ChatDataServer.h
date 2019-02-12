#pragma once

#include "ProChatSystem.h"
#include "DataTss.h"
#include "ChatSystemLogic.h"
class CChatDataServer
{
public:
	CChatDataServer(void);
	~CChatDataServer(void);
	void GetData(char* block , CChatSystemLogic& Logic);
};
extern CChatDataServer g_GetChatData;