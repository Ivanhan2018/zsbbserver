// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_UTIL_TASK_H
#define ICE_UTIL_TASK_H

#include "Thread.h"
#include "Monitor.h"
#include "RecMutex.h"

namespace IceUtil
{
	template < typename T, size_t S = ICE_MAX_QUEUE_SIZE>
	class ICE_UTIL_API Task : public noncopyable
	{
		class Call : public IceUtil::Thread
		{
		public:
			Call(Task<T, S> * p) : _parent(p){}
			void run() {_parent->run();}
			Task<T, S> * _parent;
		};

	public:
		Task();
		virtual ~Task();

		virtual bool startTask(size_t n_threads = 1);
		virtual void stopTask();

		bool putQueue(const T & data);
		bool getQueue(T & data);
		bool getQueue(T & data, const Time & timeout);

		size_t size() const { return _nelts; }

		virtual void run() = 0;

	private:
		Handle<Call> * _thread_pool;
		size_t _n_threads;

		Monitor<RecMutex> _monitor;

		const size_t _bounds;
		bool _run;
		size_t _nelts;
		size_t _in;
		size_t _out;
		T _data[S];
		size_t _wait;
	};


} // End namespace IceUtil


template < typename T, size_t S> inline
IceUtil::Task<T, S>::Task() :
	_nelts(0),
	_in(0),
	_out(0),
	_bounds(S),
	_run(false),
	_thread_pool(0),
	_n_threads(0)
{
}

template < typename T, size_t S> inline
IceUtil::Task<T, S>::~Task()
{
	assert(!_run && !_thread_pool);
}

template < typename T, size_t S> inline bool
IceUtil::Task<T, S>::startTask(size_t n_threads)
{
	if (_run) return false;

	_run = true;
	_wait = _nelts = _in = _out = 0;

	_n_threads = n_threads;
	_thread_pool = new IceUtil::Handle<Call>[_n_threads];

	for (size_t i = 0; i < n_threads; i++)
	{
		_thread_pool[i] = new Call(this);
		_thread_pool[i]->start();
	}

	return true;
}

template < typename T, size_t S> inline void
IceUtil::Task<T, S>::stopTask()
{
	if (!_run) return ;

	{
		Monitor<RecMutex>::Lock lock(_monitor);
		_run = false;
		_monitor.notifyAll();
	}

	for (size_t i = 0; i < _n_threads; i++)
		_thread_pool[i]->getThreadControl().join();

	delete [] _thread_pool;
	_thread_pool = 0;
	_n_threads = 0;
	_wait = _nelts = _in = _out = 0;
}

template < typename T, size_t S> inline bool
IceUtil::Task<T, S>::putQueue(const T & data)
{
	if (!_run) return false;
	if (_nelts == _bounds)
		throw PutQueueException(__FILE__, __LINE__);

	Monitor<RecMutex>::Lock lock(_monitor);
	if (!_run) return false;

	if (_nelts == _bounds)
		throw PutQueueException(__FILE__, __LINE__);

	if (_wait) _monitor.notify();

	_data[_in] = data;
	_in = (_in + 1) % _bounds;
	_nelts++;
	return true;
}

template < typename T, size_t S> inline bool
IceUtil::Task<T, S>::getQueue(T & data)
{
	if (!_run) return false;
	Monitor<RecMutex>::Lock lock(_monitor);

	while(_nelts == 0)
	{
		++_wait;
		_monitor.wait();
		--_wait;
		if (!_run) return false;
	}

	data = _data[_out];
	_nelts--;
	_out = (_out + 1) % _bounds;

	return true;
}

template < typename T, size_t S> inline bool
IceUtil::Task<T, S>::getQueue(T & data, const Time & timeout)
{
	if (!_run) return false;
	Monitor<RecMutex>::Lock lock(_monitor);

	while(_nelts == 0)
	{
		++_wait;
		if (!_monitor.timedWait(timeout)) 
		{
			--_wait;
			return false;
		}
		--_wait;
		if (!_run) return false;
	}

	data = _data[_out];
	_nelts--;
	_out = (_out + 1) % _bounds;

	return true;
}

#endif
