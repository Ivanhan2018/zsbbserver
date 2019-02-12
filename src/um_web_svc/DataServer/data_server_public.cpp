#include "data_server_public.h"
#include <crtdbg.h>
#include <sys/timeb.h>
#include "data_server_public.h"
#include "zlib.h"


//#define PRINTOUT

data_server_public::data_server_public()
:bconn_(false)
{
}

data_server_public::~data_server_public()
{
	//_ASSERT(!bconn_);
//	LOG_NO_RETURN(!bconn_,("data_server_public::~data_server_public"));
}

Int64 data_server_public::GetTimeNow()
{
#ifdef _WIN32
#  if defined(_MSC_VER)
    struct _timeb tb;
    _ftime(&tb);
#  elif defined(__BCPLUSPLUS__)
    struct timeb tb;
    ftime(&tb);
#  endif
    return ( static_cast<Int64>(tb.time) * ICE_INT64(1000000) + 
                tb.millitm * 1000 ) / 1000;
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    return ( tv.tv_sec * ICE_INT64(1000000) + tv.tv_usec ) / 1000;
#endif
}

std::string data_server_public::toDatetime(Int64 sec)
{
    time_t time = static_cast<long>(sec / 1000000);

    struct tm* t;
#ifdef _WIN32
    t = localtime(&time);
#else
    struct tm tr;
    localtime_r(&time, &tr);
    t = &tr;
#endif

    char buf[32];
    strftime(buf, sizeof(buf), "%x %H:%M:%S", t);

    std::ostringstream os;
    os << buf << ".";
    os.fill('0');
    os.width(3);
    os << static_cast<long>(sec % 1000000 / 1000);
    return os.str();
}


bool data_server_public::ConData(const char * dsn,const char * user,const char * password)
{
	//_ASSERT(!bconn_);
//	LOG_NO_RETURN(!bconn_,("data_server_public::ConData"));

	try
	{
		std::string constr;
		constr = "UID=";
		constr += user;
		constr += ";PWD=";
		constr += password;
		constr += ";DSN=";
		constr += dsn;
		db_.rlogon(constr.c_str());
	}
	catch(otl_exception& p)
	{ 
//#ifdef PRINTOUT
		std::cerr <<"********"<<"data_server_public::ConData"<<"********"<<std::endl;
		std::cerr <<"otl_exception::msg : \n";
		std::cerr <<p.msg <<std::endl;
//#endif
		return false;
	}
	return bconn_=true,true;
}

void data_server_public::DisConnData()
{
	if (bconn_) db_.logoff();
	bconn_=false;

}

bool data_server_public::zlib_compress(char *& desc, char * src)
{
	static int header_len = sizeof(tagRpsGetHeroList);

	tagRpsGetHeroList * hh = (tagRpsGetHeroList *)src;
	char * ptr = src + header_len;
	int ptr_len = hh->len - header_len;

	uLong desc_len = compressBound(ptr_len) + header_len;
	desc = new char[desc_len];
	::memcpy(desc, src, header_len);

	if (compress((Bytef *)(desc + header_len),
		&desc_len, (Bytef *)ptr, ptr_len) != Z_OK)
	{
		delete [] desc;
		desc = 0;
		return false;
	}

	hh = (tagRpsGetHeroList *)desc;
	hh->len = desc_len + header_len;
	hh->src_len = ptr_len + header_len;
	hh->compress = 1;

	return true;
}

//获取个人排行
void data_server_public::OnGetPlayerList(internal_socket * i_s,const char* buf)
{
	tagGetPlayerList * prep = (tagGetPlayerList *)buf;

	char * buf1 = 0;
	tagPlayerInfo * pinfo=0; 
	tagRepPlayerList* rlp=0;

	try
	{
		int idx=0;
		int rows=0;
		{
			otl_stream i(100,"select count(1) from PlayerCompare",db_);
			if (!i.eof()) i >> rows;
		}

		buf1 = new char[sizeof(tagRepPlayerList)+rows*sizeof(tagPlayerInfo)];
		pinfo=(tagPlayerInfo *)(buf1+sizeof(tagRepPlayerList)); 
		rlp=(tagRepPlayerList*)buf1;

		if (rows > 0)
		{
			otl_stream i(100,"select * from PlayerCompare",db_);

			while (!i.eof() && idx<rows)
			{
				i>>pinfo[idx].id>>pinfo[idx].PlayerID>>pinfo[idx].PlayerName>>pinfo[idx].Country>>pinfo[idx].OfficerRace>>pinfo[idx].Level>> \
				pinfo[idx].MilitaryRank>>pinfo[idx].Money>>pinfo[idx].ResourceCount>>pinfo[idx].CityCount>>pinfo[idx].GeneralCount>>pinfo[idx].Credit;
				++idx;
			}
		}

		rlp->serialize(prep->index,1,idx*sizeof(tagPlayerInfo));
		i_s->start_async_write((char*)rlp,rlp->len);
	}
	catch(otl_exception&)
	{
//		LOG_NO_RETURN(false,("data_server_public::OnGetPlayerList p.msg:%s \n",p.msg));
		//返回失败包...
		if (!buf1)
		{
			buf1 = new char[sizeof(tagRepPlayerList)];
			rlp=(tagRepPlayerList*)buf1;
		}

		rlp->serialize(prep->index,0,0);
		i_s->start_async_write((char*)rlp,rlp->len);
		return;
	}
}

//获取公会排行
void data_server_public::OnGetGongHuiList(internal_socket * i_s,const char* buf)
{
	tagGetGongHuiList * prep = (tagGetGongHuiList *)buf;

	char * buf1 = 0;
	tagGongHuiInfo * pinfo = 0;
	tagRepGongHuiList* rlp = 0;

	try
	{
		int idx=0;
		int rows=0;
		{
			otl_stream i(100,"select count(1) from ConsortiaCompare",db_);
			if (!i.eof()) i >> rows;
		}

		buf1 = new char[sizeof(tagRepGongHuiList)+rows*sizeof(tagGongHuiInfo)];
		pinfo=(tagGongHuiInfo *)(buf1+sizeof(tagRepGongHuiList));
		rlp=(tagRepGongHuiList*)buf1;

		if (rows > 0)
		{
			otl_stream i(100,"select * from ConsortiaCompare",db_);

			while (!i.eof() && idx<rows)
			{
				i>>pinfo[idx].ID>>pinfo[idx].ConID>>pinfo[idx].ConName>>pinfo[idx].Level>>pinfo[idx].UserCount \
					>>pinfo[idx].PlayerID>>pinfo[idx].ChairmanName>>pinfo[idx].Country>>pinfo[idx].CreateTime;
				++idx;
			}
		}
		
		rlp->serialize(prep->index,1,idx*sizeof(tagGongHuiInfo));
		i_s->start_async_write((char*)rlp,rlp->len);
	}
	catch(otl_exception&)
	{
//		LOG_NO_RETURN(false,("data_server_public::OnGetGongHuiList p.msg:%s \n",p.msg));
		//返回失败包...
		if (!buf1)
		{
			buf1 = new char[sizeof(tagRepGongHuiList)];
			rlp=(tagRepGongHuiList*)buf1;
		}

		rlp->serialize(prep->index,0,0);
		i_s->start_async_write((char*)rlp,rlp->len);
		return;
	}
}

//获取玩家任务变更信息
void data_server_public::OnGetPlayerTaskList(internal_socket * i_s,const char* buf)
{
	tagReqGetPlayerTask * prep = (tagReqGetPlayerTask *)buf;

	char * buf111 = 0;
	try
	{
		otl_stream i;
		i.set_commit(0);
		i.open(1, "{call PRO_PLAYERTASK}",db_);//otl_implicit_select;
		db_.commit();
		
		otl_stream i_(100,"select count(*) from NewPlayerTask",db_);
		int rows=0;
		if (!i_.eof())
		{
			i_>>rows;
		}
		i_.flush();
		i_.close();

		if (rows==0)
		{
			buf111 = new char[sizeof(tagRpsGetPlayerTask)];
			tagRpsGetPlayerTask  * frlp = (tagRpsGetPlayerTask *)buf111;
			frlp->serialize(prep->index,1,0);
			i_s->start_async_write((char *)frlp,frlp->len);
		}
		else
		{
			buf111 = new char[sizeof(tagRpsGetPlayerTask)+rows*sizeof(tagPlayerTaskInfo)];
			tagPlayerTaskInfo * info=(tagPlayerTaskInfo*)(buf111+sizeof(tagRpsGetPlayerTask));

			i_.open(100,"select * from NewPlayerTask order by Optype desc",db_);
			int idx=0;
			while (!i_.eof())
			{
				i_>>info[idx].PlayerID>>info[idx].TaskID>>info[idx].State>>info[idx].TaskTime>>info[idx].OpType;
				++idx;
			}
			i_.flush();
			i_.close();

			tagRpsGetPlayerTask* srlp=(tagRpsGetPlayerTask*)buf111;
			srlp->serialize(prep->index,1,idx*sizeof(tagPlayerTaskInfo));

			char * ptr = 0;
			if (!zlib_compress(ptr, buf111))
				throw;
			else
				delete [] buf111;

			srlp=(tagRpsGetPlayerTask*)ptr;

			i_s->start_async_write((char*)srlp,srlp->len);
		}
	}
	catch (otl_exception&)
	{
//		LOG_NO_RETURN(false,("data_server_public::OnGetPlayerTaskList p.msg:%s \n",p.msg));
		if (!buf111) buf111 = new char[sizeof(tagRpsGetPlayerTask)];

		tagRpsGetPlayerTask  * frlp = (tagRpsGetPlayerTask *)buf111;
		frlp->serialize(prep->index,0,0);
		i_s->start_async_write((char *)frlp,frlp->len);
	}
}

//获取英雄表
void data_server_public::OnGetHeroList(internal_socket * i_s,const char* buf)
{
	tagReqGetHeroList * prep = (tagReqGetHeroList *)buf;

	char * buf111 = 0;
	try
	{
		otl_stream i_(100,"select count(*) from playerofficertable",db_);
		int rows=0;
		if (!i_.eof())
		{
			i_>>rows;
		}
		i_.flush();
		i_.close();

		if (rows==0)
		{
			buf111 = new char[sizeof(tagRpsGetHeroList)];
			tagRpsGetHeroList  * frlp = (tagRpsGetHeroList *)buf111;
			frlp->serialize(prep->index,1,0);
			i_s->start_async_write((char *)frlp,frlp->len);
		}
		else
		{
			buf111 = new char[sizeof(tagRpsGetHeroList)+rows*sizeof(tagGetHeroList)];
			tagGetHeroList * info=(tagGetHeroList*)(buf111+sizeof(tagRpsGetHeroList));

			i_.open(100,"select * from playerofficertable with (nolock)",db_);
			int idx=0;
			while (!i_.eof() && idx<rows)
			{
				i_ >> info[idx].officerid_ >> info[idx].playerid_ >> info[idx].officername_ >> info[idx].officerrace_ 
					>> info[idx].level_ >> info[idx].militaryrank_ >> info[idx].jobid_ >> info[idx].faceid_ >> info[idx].exp_ 
					>> info[idx].credit_ >> info[idx].enemycount_ >> info[idx].generalcount_ >> info[idx].mapid_ >> info[idx].cityid_ 
					>> info[idx].armyid_ >> info[idx].state_ >> info[idx].stamina_ >> info[idx].power_ >> info[idx].celerity_ >> info[idx].wit_ 
					>> info[idx].techdot_ >> info[idx].attack_ >> info[idx].physicrecovery_ >> info[idx].magicrecovery_ 
					>> info[idx].actiondot_ >> info[idx].updatetime_ >> info[idx].gbhtime_ >> info[idx].levelstamina_ 
					>> info[idx].levelpower_ >> info[idx].levelcelerity_ >> info[idx].levelwit_ >> info[idx].equipstamina_ 
					>> info[idx].equippower_ >> info[idx].equipcelerity_ >> info[idx].equipwit_ >> info[idx].magicattack_ 
					>> info[idx].herohp_ >> info[idx].injurerate_ >> info[idx].capturerate_ >> info[idx].beinjurerate_ 
					>> info[idx].becapturerate_ >> info[idx].forcerate_
					>> info[idx].lsttotalattri_ >> info[idx].lstmainattri_ >> info[idx].lststamina_ >> info[idx].grouprate_;
				++idx;
			}
			i_.flush();
			i_.close();

			tagRpsGetHeroList* srlp=(tagRpsGetHeroList*)buf111;
			srlp->serialize(prep->index,1,idx*sizeof(tagGetHeroList));

			char * ptr = 0;
			if (!zlib_compress(ptr, buf111))
				throw;
			else
				delete [] buf111;

			srlp=(tagRpsGetHeroList*)ptr;

			i_s->start_async_write((char*)srlp,srlp->len);
		}
	}
	catch (otl_exception&)
	{
//		LOG_NO_RETURN(false,("data_server_public::OnGetHeroListList p.msg:%s \n",p.msg));
		if (!buf111) buf111 = new char[sizeof(tagRpsGetHeroList)];

		tagRpsGetHeroList  * frlp = (tagRpsGetHeroList *)buf111;
		frlp->serialize(prep->index,0,0);
		i_s->start_async_write((char *)frlp,frlp->len);
	}
}

//回写玩家任务信息
void data_server_public::OnBackWriteTask(internal_socket * i_s,const char* buf)
{
	otl_stream i;
	i.set_commit(0);
	i.open(1, "{call PRO_BackWriteTask}",db_);
	db_.commit();
	return ;
}






