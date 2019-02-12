#include "CommonFunction.h"
#pragma warning(disable:4706)
/**************************************************************************
*˵����ȥ���ַ����Ŀո��TAB
*���������1��buffer���ַ���
		   2��mode:0��ȥ��ǰ��ո�1��ȥ����ո�2��ȥ���ҿո�
*����ֵ��ȥ���ո����ַ���
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
	//ȥ����ո�
	if(mode ==0 || mode ==1)
	{
		while(*(pLeftPos) == 0x20 || *(pLeftPos) == 0x09)
		{
			pLeftPos++;
		}
	}

	//ȥ���ֿո�
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
*˵����ȥ���������е�ע��,ע�ͷ���Ϊ��#��//
*���������1��buffer���ַ���
*����ֵ��ȥ��ע�ͺ���ַ���
**************************************************************************/
char* DelRemark(char *buffer)
{
	if(buffer ==0)
		return 0;

	char *pPos = buffer;//�ҵ�ע��
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
*˵�������������л�ȡ���õ����ݣ���=�ұߵ��ַ���
*���������1��buffer���ַ���
*����ֵ������
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
*˵�������������л�ȡ���õ����ݣ���=�ұߵ��ַ���
*���������1��buffer���ַ���
*����ֵ������
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
		//ȥ�����Ļ��з�
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
		{//δ�ҵ�������
			break;
		}
		if(!bSection)
		{//ƥ��Section
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
		{//ƥ��Key
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
*˵������Դ�ַ������ݷָ�����ȡ�ڼ����ַ���
*���������1��src_buf��Դ�ַ�
           2��dest_buf������ַ���
		   3��index����ţ���0��ʼ
		   4��cSeparam���ָ��ַ�
*����ֵ��0:�ɹ���������ʧ��
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
		//�����Ӵ�ǰ�Ĳ���
		iLenTmp = pTmp - pStar;
		strncpy(pDeststr,pStar,iLenTmp);
		pDeststr += iLenTmp;
		pStar = pTmp + iLen_substr;
		//�����´�
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
