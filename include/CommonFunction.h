#ifndef COMMONFUNCTION_H__
#define COMMONFUNCTION_H__
#include <stdio.h>
#include <string.h>
/**********************��ȡ�����ļ��ĺ���*******************************/
char* Trim(char *buffer,int mode =0);
char* DelRemark(char *buffer);
char *GetValue(char *buffer);
int ReadConfig(const char *szFileName,const char *szSection,const char *szKey,char *buffer,int len);

int GetValue(const char *src_buf,char *dest_buf,int index,char cSeparam);
char *StrReplace(const char *srcstr,char *deststr,const char *substr,const char *newstr);
#endif
