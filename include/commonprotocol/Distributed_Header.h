/***************************************************************************
*   Copyright (C) 2007 by wangshenwang                                    *
*   wangshenwang1@gmail.com                                               *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, CHN.             *
***************************************************************************/

#ifndef __INCLUDE_DISTRIBUTED_HEADER_H__
#define __INCLUDE_DISTRIBUTED_HEADER_H__

#pragma pack(push,1)

#define HTTP_TUNNEL "GET /T"

/***************************************************************************
// ����Э�������Э��ͷ����Ҫ���ڷֲ�ʽ����
***************************************************************************/
struct DISTRIBUTED_HEADER
{
	inline void serialize(int, int, int, int, int);
	char http_tunnel[7];		// �̶����ݣ���GET /T��
	int length;					// Э�������
	int agent;					// ������
	int from;					// ������, �磺��ID�����ID
	int to;						// ����ȥ, �磺��ID�����ID
	int command;				// Э������
	unsigned char compress;		// �Ƿ�ѹ����1Ϊѹ����0Ϊûѹ��
	int src_len;				// Э�������
};

#pragma pack(pop)


inline void DISTRIBUTED_HEADER::serialize(int length, int agent, int from, int to, int command)
{
	this->http_tunnel[0] = 'G';
	this->http_tunnel[1] = 'E';
	this->http_tunnel[2] = 'T';
	this->http_tunnel[3] = ' ';
	this->http_tunnel[4] = '/';
	this->http_tunnel[5] = 'T';
	this->http_tunnel[6] = '\0';
	this->length = length;
	this->agent = agent;
	this->from = from;
	this->to = to;
	this->command = command;
	this->compress = 0;
	this->src_len = 0;
}


#endif // __INCLUDE_DISTRIBUTED_HEADER_H__
