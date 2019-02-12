#pragma once
#include "globalmacrodef.h"
#pragma  warning(push)
#pragma  warning(disable:4244)
#include <boost/thread/shared_mutex.hpp>
#pragma  warning(pop)
template <typename T> class MutexGuard
{
public:
	MutexGuard(T& mutex):
	 mutex_(mutex)
	 {
		mutex_.lock();
	}
	~MutexGuard(void){
		mutex_.unlock();
	}
private:
	T& mutex_;
private:
	NO_COPYABLE_AND_ASSIGN(MutexGuard);
};
template <typename T> class ShareMutexGuard
{
public:
	ShareMutexGuard(T& mutex):
	  mutex_(mutex)
	{
		mutex_.lock_shared();
	}
	~ShareMutexGuard(void){
		mutex_.unlock_shared();
	}
private:
	T& mutex_;
private:
	NO_COPYABLE_AND_ASSIGN(ShareMutexGuard);
};  