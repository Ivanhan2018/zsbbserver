#include "CommonFunction.h"
#pragma warning(disable:4706)
/**************************************************************************
*说明：去掉字符串的空格和TAB
*输入参数：1、buffer：字符串
		   2、mode:0：去除前后空格，1：去除左空格；2：去除右空格
*返回值：去除空格后的字符串
**************************************************************************/
char* Trim(char *buffer,int mode)
{
	char *pLeftPos = buffer;
	char *pRightPos;
	size_t Len = 0;
	if(buffer ==0)
		return 0;

	Len = strlen(buffer);
	pRightPos = buffer + Len -1;
	//去掉左空格
	if(mode ==0 || mode ==1)
	{
		while(*(pLeftPos) == 0x20 || *(pLeftPos) == 0x09)
		{
			pLeftPos++;
		}
	}

	//去掉又空格
	if(mode ==0 || mode ==2)
	{
		while(*(pRightPos) ==0x20  || *(pRightPos) == 0x09)
		{
			*pRightPos = 0;
			pRightPos--;
		}
	}
	if(pLeftPos != buffer)
	{
		strcpy(buffer,pLeftPos);
	}
	return buffer;
}
/**************************************************************************
*说明：去掉配置项中的注释,注释符号为：#或//
*输入参数：1、buffer：字符串
*返回值：去除注释后的字符串
**************************************************************************/
char* DelRemark(char *buffer)
{
	if(buffer ==0)
		return 0;

	char *pPos = buffer;//找到注释
	while(*pPos)
	{
		if((*pPos == '#') || (strncmp(pPos,"//",2) ==0))
		{
			*pPos = 0;
			break;
		}
		pPos++;
	}
	return buffer;
}

/**************************************************************************
*说明：从配置项中获取配置的数据，即=右边的字符串
*输入参数：1、buffer：字符串
*返回值：数据
**************************************************************************/
char *GetValue(char *buffer)
{
	char *pPos = buffer;
	while(*pPos)
	{
		if(*pPos == '=')
		{
			pPos++;
			break;
		}
		pPos++;
	}
	return pPos;
}

/**************************************************************************
*说明：从配置项中获取配置的数据，即=右边的字符串
*输入参数：1、buffer：字符串
*返回值：数据
**************************************************************************/
int ReadConfig(const char *szFileName,const char *szSection,const char *szKey,char *buffer,int len)
{
	FILE *fp;
	bool bSection = false;
	bool bFinded = false;
	char tmpBuf[1024] = {0};
	char tmpSection[128] = {0};
	size_t tmpLen = 0;

	if((fp = fopen(szFileName,"r")) == NULL)
	{
		return -1;
	}
	sprintf(tmpSection,"[%s]",szSection);
	while(!feof(fp))
	{
		if(fgets(tmpBuf,1024,fp)== NULL)
			break;
		//去掉最后的换行符
		tmpLen = strlen(tmpBuf)-1;

		if(tmpBuf[tmpLen] == 0x0A)
		{
			tmpBuf[strlen(tmpBuf)-1] = 0;
		}
		Trim(tmpBuf);
		DelRemark(tmpBuf);
		if(strlen(tmpBuf) ==0)
		{
			continue;
		}
		if(bSection && tmpBuf[0] == '[')
		{//未找到配置项
			break;
		}
		if(!bSection)
		{//匹配Section
			if(tmpBuf[0] != '[')
			{
				continue;
			}
			if(strcmp(tmpSection,tmpBuf) ==0)
			{
				bSection = true;
			}
			else
			{
				continue;
			}
		}
		else
		{//匹配Key
			if(strncmp(tmpBuf,szKey,strlen(szKey)) ==0)
			{
				memset(buffer,0,len);
				strncpy(buffer,GetValue(tmpBuf),len);
				Trim(buffer);
				bFinded = true;
				break;
			}
		}
	}
	fclose(fp);
	return (bFinded)?0:1;
}

/**************************************************************************
*说明：从源字符串根据分隔符获取第几个字符串
*输入参数：1、src_buf：源字符
           2、dest_buf：输出字符串
		   3、index：序号，从0开始
		   4、cSeparam：分割字符
*返回值：0:成功；其他：失败
**************************************************************************/
int GetValue(const char *src_buf,char *dest_buf,int index,char cSeparam)
{
	int count = 0;
	char *pStar = (char*)src_buf;
	char *pStrEnd = (char*)src_buf + strlen(src_buf);
	char *pEnd = 0;
	for(count = 0;count <index;count++)
	{
		pStar = strchr(pStar,cSeparam);
		if(pStar ==0/* || pStar >=pStrEnd*/)
		{
			pStar = 0;
			break;
		}
		pStar++;
	}	

	if(pStar ==0)
		return 1;

	pEnd = strchr(pStar,cSeparam);
	if(pEnd == 0)
		pEnd = pStrEnd;

	if(pEnd <= pStar)
		return 2;
	strncpy(dest_buf,pStar,pEnd - pStar);
	return 0;
}
char *StrReplace(const char *srcstr,char *deststr,const char *substr,const char *newstr)
{
	char *pStar =(char*) srcstr;
	char *pTmp = NULL;
	//char *pEnd = NULL;
	char *pDeststr = deststr;
	size_t iLen_substr = strlen(substr);
	size_t iLen_newstr = strlen(newstr);
	size_t iLenTmp = 0 ;

	while((pTmp = strstr(pStar,substr)))
	{
		//拷贝子串前的部分
		iLenTmp = pTmp - pStar;
		strncpy(pDeststr,pStar,iLenTmp);
		pDeststr += iLenTmp;
		pStar = pTmp + iLen_substr;
		//拷贝新串
		strcpy(pDeststr,newstr);
		pDeststr += iLen_newstr;
	}
	/*
	if(pStar == srcstr)
	{
		return NULL;
	}
	else */
	if(pStar)
	{
		strcpy(pDeststr,pStar);
	}

	return deststr;
}
