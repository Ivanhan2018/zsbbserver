#include "DataTss.h"
#include <iostream>
using namespace std;

CDataTss g_DataTss ;
CDataTss::CDataTss(void)
{
}

CDataTss::~CDataTss(void)
{
}
void CDataTss::PutData(int info)
{
	//cout<<"Put a data "<< info <<endl;
	m_data = info;
}
int CDataTss::GetData()
{
	return m_data ;
}