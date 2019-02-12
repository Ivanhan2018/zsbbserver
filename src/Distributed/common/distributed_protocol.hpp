#ifndef __DISTRIBUTED_PROTOCOL_HPP__
#define __DISTRIBUTED_PROTOCOL_HPP__

#ifndef NONASIO
#include "header.hpp"
#endif

#include "Global_Macro.h"
#include "Distributed_Header.h"

#pragma pack(push,1)

// protocol format : from_command_to

struct NODE_ADDRESS
{
	int id;
	int address;
	u_short port;
};

// server to manager
struct SERVER_REGISTER_MANAGER : DISTRIBUTED_HEADER
{
	int id;
	u_short port;
	inline void serialize(int id, u_short port);
};

// manager to server
struct MANAGER_REGISTER_RESP_SERVER : DISTRIBUTED_HEADER
{
	int code;
	int num;
	u_short game_id;
	u_short group_id;
	u_long log_game_address;
	u_short log_game_port;
	inline void serialize(int code, int num, u_short game_id, u_short group_id, u_long log_game_address, u_short log_game_port);
	inline NODE_ADDRESS & operator [](int index);
};

// agent to login
struct AGENT_REPORT_LOGIN : DISTRIBUTED_HEADER
{
	int id;
	char group_name[SIZE_64];
	char address[SIZE_128];
	short port;
	inline void serialize(int id, const char * group_name, const char * address, short port);
};

// server1 to manager to server2
struct SERVER_NOTIFY_MANAGER_NOTIFY_SERVER : DISTRIBUTED_HEADER
{
	inline void serialize(int from, int to, int length);
};

// manager to server1
struct SERVER_NOTIFY_RESP_SERVER : DISTRIBUTED_HEADER
{
	int code;
	inline void serialize(int code, int sid_from, int sid_to);
};

// server1 to server2
struct SERVER_HANDSHAKE_SERVER : DISTRIBUTED_HEADER
{
	int id;
	inline void serialize(int id);
};

// server2 to server1
struct SERVER_HANDSHAKE_RESP_SERVER : DISTRIBUTED_HEADER
{
	int code;
	inline void serialize(int code);
};

// agent to server
struct AGENT_CLIENT_LEAVE_SERVER : DISTRIBUTED_HEADER
{
	inline void serialize(int agent, int sid, int pid);
};

// agent to client
struct AGENT_CLIENT_RESPONSE : DISTRIBUTED_HEADER
{
	int code;
	inline void serialize(int code, int agent, int sid, int pid);
};

// agent to server
struct AGENT_SERVER_RESPONSE : DISTRIBUTED_HEADER
{
	int code;
	inline void serialize(int code, int agent, int sid, int pid);
};

// client to login
struct CLIENT_REGISTER_LOGIN : DISTRIBUTED_HEADER
{
	char accounts[SIZE_16];
	char loginpass[SIZE_16];
	char group_name[SIZE_32];
	long version;
	long computer_id_;
	inline void serialize(const char * accounts, const char * loginpass, const char * group_name, long version, long computer_id);
};

// client to login
struct CLIENT_REGISTER_LOGOUT : DISTRIBUTED_HEADER
{
	int playerid_;
	long computer_id_;
	inline void serialize(int playerid, int agentid, long computer_id);
};

// login to client
struct LOGIN_REGISTER_RESP_CLIENT : DISTRIBUTED_HEADER
{
	int code;
	int playerid;
	int accounttype;
	int agentid;
	u_short port;
	u_char encdeckey[MAX_KEY_SIZE];
	char address[SIZE_128];
	inline void serialize(int code);
	inline void serialize(int code, int playerid, int accounttype, int agentid, const char * address, u_short port, u_char * encdeckey);
};

// server to client all
struct SERVER_BROADCAST_CLIENT : DISTRIBUTED_HEADER
{
	inline void serialize(int from, int);
};

// log
struct LOG_REPORT_INFO : DISTRIBUTED_HEADER
{
	char proc_name[128];
	char datetime[25];
	char msg[300];
	inline void serialize(const char *, const char *, const char *);
};

// log
struct LOG_REPORT_PERFORMANCE : DISTRIBUTED_HEADER
{
	char proc_name[128];
	char datetime[25];
	int bandwidth;
	int broadcast;
	int cpu;
	int mem;
	int vmem;

	inline void serialize(const char *, int bandwidth, int broadcast, int cpu, int mem, int vmem, const char * datetime);
};

// log
struct LOG_REPORT_ONLINE : DISTRIBUTED_HEADER
{
	char proc_name[128];
	char datetime[25];
	int online;

	inline void serialize(const char *, const char * datetime, int online);
};

// log
struct LOG_REPORT_GROUP_ONLINE : DISTRIBUTED_HEADER
{
	char group[128];
	char datetime[25];
	int online;

	inline void serialize(const char * group, const char * datetime, int online);
};

// log_game
struct LOG_GAME_WRITER : DISTRIBUTED_HEADER
{
	inline void serialize(int len);
	inline char * buf();
	inline int buf_len();
};

// log_game
struct LOG_GAME_READER : DISTRIBUTED_HEADER
{
	int bdate;
	int edate;
	inline void serialize(int, int);
};

// log_game
struct LOG_GAME_READER_RSP : DISTRIBUTED_HEADER
{
	char full_path[256];
	inline void serialize(int result, int max_file_num, int file_len, const char * full_path);
	inline int buf_len();
	inline char * buffer();
};

//////////////////////////////////////////////////////////////////////////////////////////


#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


inline void SERVER_REGISTER_MANAGER::serialize(int id, u_short port)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_SERVER_REGISTER_MANAGER);

	this->id = id;
	this->port = port;
}

inline void MANAGER_REGISTER_RESP_SERVER::serialize(int code, int num, u_short game_id, u_short group_id, u_long log_game_address, u_short log_game_port)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(NODE_ADDRESS) * num, 0, 0, 0, CMD_MANAGER_REGISTER_RESP_SERVER);
	this->num = num;
	this->code = code;
	this->game_id = game_id;
	this->group_id = group_id;
	this->log_game_address = log_game_address;
	this->log_game_port = log_game_port;
}

inline NODE_ADDRESS & MANAGER_REGISTER_RESP_SERVER::operator [](int index)
{
	return ((NODE_ADDRESS *)((u_char *)this + sizeof(*this)))[index];
}

inline void AGENT_REPORT_LOGIN::serialize(int id, const char * group_name, const char * address, short port)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_AGENT_REPORT_LOGIN);
	this->id = id;
	strcpy(this->group_name, group_name);
	strcpy(this->address, address);
	this->port = port;
}

inline void SERVER_NOTIFY_MANAGER_NOTIFY_SERVER::serialize(int from, int to, int length)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + length, 0, from, to, CMD_SERVER_NOTIFY_MANAGER_NOTIFY_SERVER);
}

inline void SERVER_NOTIFY_RESP_SERVER::serialize(int code, int sid_from, int sid_to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, sid_from, sid_to, CMD_SERVER_NOTIFY_RESP_SERVER);
	this->code = code;
}

inline void SERVER_HANDSHAKE_SERVER::serialize(int id)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_SERVER_HANDSHAKE_SERVER);
	this->id = id;
}

inline void SERVER_HANDSHAKE_RESP_SERVER::serialize(int code)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_SERVER_HANDSHAKE_RESP_SERVER);
	this->code = code;
}

inline void AGENT_CLIENT_LEAVE_SERVER::serialize(int agent, int sid, int pid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agent, sid, pid, CMD_AGENT_CLIENT_LEAVE_SERVER);
}

inline void AGENT_CLIENT_RESPONSE::serialize(int code, int agent, int sid, int pid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agent, sid, pid, CMD_AGENT_CLIENT_RESPONSE);
	this->code = code;
}

inline void AGENT_SERVER_RESPONSE::serialize(int code, int agent, int sid, int pid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agent, sid, pid, CMD_AGENT_SERVER_RESPONSE);
	this->code = code;
}

inline void CLIENT_REGISTER_LOGIN::serialize(const char * accounts, const char * loginpass, const char * group_name, long version, long computer_id)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_CLIENT_REGISTER_LOGIN);
	strcpy(this->accounts, accounts);
	strcpy(this->loginpass, loginpass);
	strcpy(this->group_name, group_name);
	this->version = version;
	this->computer_id_ = computer_id;
}

inline void CLIENT_REGISTER_LOGOUT::serialize(int playerid, int agentid, long computer_id)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agentid, 0, 0, CMD_CLIENT_REGISTER_LOGOUT);
	this->playerid_ = playerid;
	this->computer_id_ = computer_id;
}

inline void LOGIN_REGISTER_RESP_CLIENT::serialize(int code)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_LOGIN_REGISTER_RESP_CLIENT);
	this->code = code;
	this->playerid = 0;
	this->accounttype = 0;
	this->agentid = 0;
	this->port = 0;
	memset(this->encdeckey, 0, MAX_KEY_SIZE);
	memset(this->address, 0, SIZE_128);
}

inline void LOGIN_REGISTER_RESP_CLIENT::serialize(int code, int playerid, int accounttype, int agentid, const char * address, 
												  u_short port, u_char * encdeckey)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_LOGIN_REGISTER_RESP_CLIENT);
	this->code = code;
	this->playerid = playerid;
	this->accounttype = accounttype;
	this->agentid = agentid;
	strcpy(this->address, address);
	this->port = port;
	memcpy(this->encdeckey, encdeckey, MAX_KEY_SIZE);
}

inline void SERVER_BROADCAST_CLIENT::serialize(int from, int length)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + length, 0, from, 0, CMD_SERVER_BROADCAST_CLIENT);
}

inline void LOG_REPORT_INFO::serialize(const char * proc_name, const char * datetime, const char * msg)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_LOG_REPORT_INFO);
	strcpy(this->proc_name, proc_name);
	strcpy(this->datetime, datetime);
	strcpy(this->msg, msg);
}

inline void LOG_REPORT_PERFORMANCE::serialize(const char * proc_name, int bandwidth, int broadcast, int cpu, int mem, int vmem, const char * datetime)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_LOG_REPORT_PERFORMANCE);
	this->bandwidth = bandwidth;
	this->broadcast = broadcast;
	this->cpu = cpu;
	this->mem = mem;
	this->vmem = vmem;
	strcpy(this->proc_name, proc_name);
	strcpy(this->datetime, datetime);
}

inline void LOG_REPORT_ONLINE::serialize(const char * proc_name, const char * datetime, int online)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_LOG_REPORT_ONLINE);
	this->online = online;
	strcpy(this->proc_name, proc_name);
	strcpy(this->datetime, datetime);
}

inline void LOG_REPORT_GROUP_ONLINE::serialize(const char * group, const char * datetime, int online)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_LOG_REPORT_GROUP_ONLINE);
	this->online = online;
	strcpy(this->group, group);
	strcpy(this->datetime, datetime);
}

inline void LOG_GAME_WRITER::serialize(int len)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + len, 0, 0, 0, CMD_LOG_GAME_WRITER);
}

inline char * LOG_GAME_WRITER::buf()
{
	return ((char *)this) + sizeof(*this);
}

inline int LOG_GAME_WRITER::buf_len()
{
	return this->length - sizeof(*this);
}

inline void LOG_GAME_READER::serialize(int bdate, int edate)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_LOG_GAME_READER);
	this->bdate = bdate;
	this->edate = edate;
}

inline void LOG_GAME_READER_RSP::serialize(int result, int max_file_num, int file_len, const char * full_path)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + file_len, result, max_file_num, 0, CMD_LOG_GAME_READER_RSP);
	strcpy(this->full_path, full_path);
}

inline int LOG_GAME_READER_RSP::buf_len()
{
	return this->length - sizeof(*this);
}

inline char * LOG_GAME_READER_RSP::buffer()
{
	return (char*)(((u_char *)this) + sizeof(*this));
}

#endif // __DISTRIBUTED_PROTOCOL_HPP__

