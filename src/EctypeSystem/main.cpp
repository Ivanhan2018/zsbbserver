// FightSystem.cpp : 定义控制台应用程序的入口点。
//

//#include "FightRole/allocator.hpp"
//static allocator __alloc;
//void * operator new(std::size_t size){
//	return __alloc.malloc(size);
//} 
//void operator delete(void * ptr){
//	__alloc.free(ptr);
//}
//void * operator new[](std::size_t size){
//	return __alloc.malloc(size);
//}
//void operator delete[]( void * ptr ){
//	__alloc.free(ptr);
//}


//void * operator new(std::size_t size)
//{
//	_ASSERT(size > 0 && size < 2*1024*1024);
//
//	char * ptr = (char *)__alloc.malloc(size + 12);
//
//	strcpy(ptr , "beg");
//	*(int *)(ptr + 4) = size;
//	strcpy(ptr + size + 8, "end");
//	return ptr + 8;
//} 
//
//void operator delete(void * ptr)
//{
//	_ASSERT(ptr);
//
//	char * pstr = ((char *)ptr) - 8;
//	int size = *(int *)(pstr + 4);
//
//
//	if (strcmp(pstr, "beg"))
//	{
//		_ASSERT(false);
//	}
//	if (strcmp(pstr + size + 8, "end"))
//	{
//		_ASSERT(false);
//	}
//	__alloc.free(pstr);
//}
//
//void * operator new[](std::size_t size)
//{
//	_ASSERT(size > 0 && size < 2*1024*1024);
//
//	char * ptr = (char *)__alloc.malloc(size + 12);
//
//	strcpy(ptr , "beg");
//	*(int *)(ptr + 4) = size;
//	strcpy(ptr + size + 8, "end");
//	return ptr + 8;
//}
//
//void operator delete[]( void * ptr )
//{
//	_ASSERT(ptr);
//
//	char * pstr = ((char *)ptr) - 8;
//	int size = *(int *)(pstr + 4);
//
//
//	if (strcmp(pstr, "beg"))
//	{
//		_ASSERT(false);
//	}
//	if (strcmp(pstr + size + 8, "end"))
//	{
//		_ASSERT(false);
//	}
//	__alloc.free(pstr);
//}
#include <iostream>
#include <tchar.h>

#include "../EctypeSystem/NetProcesser.h"
#include "exception_handler.hpp"
#include "IniFile.h"
#include "socketsvc.hpp"
#include <Windows.h>
#include "PlayerArmy.h"
bool g_bIsExit = false;
BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
	switch (ctrl_type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		g_bIsExit = true;
		return TRUE;
	default:
		return FALSE;
	}
}

#include "boost/shared_ptr.hpp"

void * operator new(std::size_t size)
{
	return ::malloc(((size >> 7) + !(!(size&127))) << 7);
}

void * operator new[](std::size_t size)
{
	return ::malloc(((size >> 7) + !(!(size&127))) << 7);
}

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{ 
 	exception_handler::init_exception_handler(1256);
	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
	g_IniFile.ReadFile("serverconfig.ini"); 
	NetProcesser::instance()->setProcessID(1256);
	NetProcesser::instance()->initialize(); 
	std::cout<< "PLEASE CTRL + C FOR EXIT!" << std::endl;
	while(g_bIsExit == false){
		Sleep(100);
	}
	fini();
	NetProcesser::instance()->release();
	return 0;
}

//void * operator new(std::size_t size)
//{
//	_ASSERT(size > 0 && size < 2*1024*1024);
//
//	char * ptr = (char *)::malloc(size + 4 + strlen("TEST MEMORY") + 1);
//
//	*(int *)ptr = size;
//	strcpy(ptr + size + 4, "TEST MEMORY");
//
//	return ptr + 4;
//} 
//
//void operator delete(void * ptr)
//{
//	_ASSERT(ptr);
//
//	char * pstr = ((char *)ptr) - 4;
//	int size = *(int *)pstr;
//
//	if (strcmp(pstr + size + 4, "TEST MEMORY"))
//	{
//		_ASSERT(false);
//	}
//
//	::free(pstr);
//}
//
//void * operator new[](std::size_t size)
//{
//	_ASSERT(size > 0 && size < 2*1024*1024);
//
//	char * ptr = (char *)::malloc(size + 4 + strlen("TEST MEMORY") + 1);
//
//	*(int *)ptr = size;
//	strcpy(ptr + size + 4, "TEST MEMORY");
//
//	return ptr + 4;
//}
//
//void operator delete[]( void * ptr )
//{
//	_ASSERT(ptr);
//
//	char * pstr = ((char *)ptr) - 4;
//	int size = *(int *)pstr;
//
//	if (strcmp(pstr + size + 4, "TEST MEMORY"))
//	{
//		_ASSERT(false);
//	}
//
//	::free(pstr);
//}