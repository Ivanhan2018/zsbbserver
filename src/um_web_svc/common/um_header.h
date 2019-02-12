#ifndef __header_H__
#define __header_H__

enum {offset=7,defsize=11,headsize=19};

#pragma pack(push,1)

struct header 
{
	char http_tunnel[7];		// 固定内容：“GET /T”
	int len;					// 包长度	
	int index;					// 包序号
	int command;				// 命令字
	inline void serialize(int len,int index,int command);
};

#pragma pack(pop)

inline 
void header::serialize(int len,int index,int command)
{
	this->http_tunnel[0] = 'G';
	this->http_tunnel[1] = 'E';
	this->http_tunnel[2] = 'T';
	this->http_tunnel[3] = ' ';
	this->http_tunnel[4] = '/';
	this->http_tunnel[5] = 'T';
	this->http_tunnel[6] = '\0';
	this->len=len;
	this->index=index;
	this->command=command;
}

#endif