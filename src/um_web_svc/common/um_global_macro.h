#ifndef __um_global_macro_H__
#define __um_global_macro_H__

//WebServer��WebClient֮���ͨ������
enum command
{
	//����˻��Ƿ����
	CM_CHECK_USER		=	0x006000,
	//�˻�ע��
	CM_REGUSER			=	0x006010,
	//�˻�WEB��½
	CM_USER_LOING		=	0x006020,
	//�����˻�����
	CM_MODIFY_PASSWORD	=	0x006030,
	//GO���ֵ
	CM_ADDMONEY			=	0x006040,
	//��ȡս��
	CM_GETWARNEWS		=	0x006050,
	//�����˻�Email��Ϣ
	CM_UPDATEEMAIL		=	0x006060,
	//��ȡ��
	CM_GETANSWER		=	0x006070,
	//��ֵ
	CM_SUBMONEY			=	0x006080,
	//��ѯGO����
	CM_GETMONEY			=	0x006090,

	//�ظ��Ի�ȡ��
	CM_REP_GETANSWER	=	0x008810,
	//�ظ���ѯGO����
	CM_REP_GETMONEY		=	0x008820,

	//��������
	CM_PLAYERLIST		=	0x009901,
	//��������
	CM_GONGHUILIST		=	0x009902,
	//��ѯ����
	CM_GETSCORE			=	0x009903,
	//�۳�����
	CM_SUBSCORE			=	0x009904,
	//�ظ���ѯ����
	CM_REP_GETSCORE		=	0x009905,
	//�޸����루����֤��
	CM_MODIFY_PASSWORD_NO=	0x009906,
	//��ѯEmail
	CM_GETEMAIL			=	0x009907,
	//GM�˻��б�
	CM_GMUSERLIST		=   0x009908,
	//�����˻�������Ϣ
	CM_UPDATEUSEREXTINFO=	0x009909,
	//��ѯ���˻�������Ϣ
	CM_GETUSEREXTINFO	=	0x009910,
	//��������,�����ַ�ʽ
	CM_MODIFY_PASSWORD_EXT=	0x009911,
	//��ȡ��������
	CM_GETPLAYERTASK	=	0x009912,
	//�ظ�����������dataserver��
	CM_BACKWRITETASK	=	0x009913,
	
	//��Ӧ�洢����sp_pgetCZlist
	CM_ADDMONEYTOTAL	=	0x009914,
	//��Ӧ�洢����sp_pgetCZ2list
	CM_ADDMONEYTOTALEX	=	0x009915,
	//��Ӧ�洢����sp_pgetCZdetaillist
	CM_ADDMONEYTOTALTHIRD=	0x009916,
	//��Ӧ�洢����sp_pgetplayerlist
	CM_PLAYERLISTTOTAL	=	0x009917,
	//��Ӧ�洢����sp_pgetuserlist
	CM_USERLISTTOTAL	=	0x009918,
	//��Ӧ�洢����sp_pgetuserbypid
	CM_USERBYPIDTOTAL	=	0x009919,
	//ExecSQL,clientֱ�Ӵ�SQL���
	CM_GETRECORDCOUNT	=	0x009920,

	CM_GETLOGINFO = 0x009921,
	CM_RPSLOGINFO = 0x009922,
	CM_GETLOGONLINE = 0x009923,
	CM_RPSLOGONLINE = 0x009924,
	CM_GETLOGPERF = 0x009925,
	CM_RPSLOGPERF = 0x009926,

	//��Ӧ�洢����sp_pgetuserlist3
	CM_USERLISTTOTAL3	=	0x009927,
	CM_GETRESULTBYPROC = 0x009928,
	CM_RSPGETRESULTBYPROC = 0x009929,
	CM_PLAYERFRIENDLIST = 0x00992A,

	CM_GETLOGONLINEEX = 0x00992B,
	CM_RPSLOGONLINEEX = 0x00992C,
	CM_GETGODIANLOG = 0x00992D,
	CM_RPSGODIANLOG = 0x00992E,

	CM_GETHEROLIST = 0x00992F,
	CM_RPSGETHEROLIST = 0x009930
};




#endif