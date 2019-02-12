// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_UTIL_HANDLE_H
#define ICE_UTIL_HANDLE_H

#include "Exception.h"
#include <algorithm>

//
// "Handle" or "smart pointer" class for classes derived from
// IceUtil::Shared, IceUtil::SimpleShared, or Ice::GCShared.
//
namespace IceUtil
{

template<typename T>
class HandleBase
{
public:

    typedef T element_type;
    
    T* get() const
    {
        return _ptr;
    }

    T* operator->() const
    {
        if(!_ptr)
        {
            //
            // We don't throw directly NullHandleException here to
            // keep the code size of this method to a minimun (the
            // assembly code for throwing an exception is much bigger
            // than just a function call). This maximises the chances
            // of inlining by compiler optimization.
            //
            throwNullHandleException(__FILE__, __LINE__);           
        }

        return _ptr;
    }

    T& operator*() const
    {
        if(!_ptr)
        {
            //
            // We don't throw directly NullHandleException here to
            // keep the code size of this method to a minimun (the
            // assembly code for throwing an exception is much bigger
            // than just a function call). This maximises the chances
            // of inlining by compiler optimization.
            //
            throwNullHandleException(__FILE__, __LINE__);           
        }

        return *_ptr;
    }

    operator bool() const
    {
        return _ptr ? true : false;
    }

    void swap(HandleBase& other)
    {
        std::swap(_ptr, other._ptr);
    }

    T* _ptr;

private:

    void throwNullHandleException(const char *, int) const;
};

template<typename T> inline void 
HandleBase<T>::throwNullHandleException(const char* file, int line) const
{
    throw NullHandleException(file, line);
}

template<typename T, typename U>
inline bool operator==(const HandleBase<T>& lhs, const HandleBase<U>& rhs)
{
    T* l = lhs.get();
    U* r = rhs.get();
    if(l && r)
    {
        return *l == *r;
    }
    else
    {
        return !l && !r;
    }   
}

template<typename T, typename U>
inline bool operator!=(const HandleBase<T>& lhs, const HandleBase<U>& rhs)
{
    T* l = lhs.get();
    U* r = rhs.get();
    if(l && r)
    {
        return *l != *r;
    }
    else
    {
        return l || r;
    }   
}

template<typename T, typename U>
inline bool operator<(const HandleBase<T>& lhs, const HandleBase<U>& rhs)
{
    T* l = lhs.get();
    U* r = rhs.get();
    if(l && r)
    {
        return *l < *r;
    }
    else
    {
        return !l && r;
    }
}

template<typename T>
class Handle : public HandleBase<T>
{
public:
    
    Handle(T* p = 0)
    {
        this->_ptr = p;

        if(this->_ptr)
        {
            this->_ptr->__incRef();
        }
    }
    
    template<typename Y>
    Handle(const Handle<Y>& r)
    {
        this->_ptr = r._ptr;

        if(this->_ptr)
        {
            this->_ptr->__incRef();
        }
    }

    Handle(const Handle& r)
    {
        this->_ptr = r._ptr;

        if(this->_ptr)
        {
            this->_ptr->__incRef();
        }
    }
    
    ~Handle()
    {
        if(this->_ptr)
        {
            this->_ptr->__decRef();
        }
    }
    
    Handle& operator=(T* p)
    {
        if(this->_ptr != p)
        {
            if(p)
            {
                p->__incRef();
            }

            T* ptr = this->_ptr;
            this->_ptr = p;

            if(ptr)
            {
                ptr->__decRef();
            }
        }
        return *this;
    }
        
    template<typename Y>
    Handle& operator=(const Handle<Y>& r)
    {
        if(this->_ptr != r._ptr)
        {
            if(r._ptr)
            {
                r._ptr->__incRef();
            }

            T* ptr = this->_ptr;
            this->_ptr = r._ptr;

            if(ptr)
            {
                ptr->__decRef();
            }
        }
        return *this;
    }

    Handle& operator=(const Handle& r)
    {
        if(this->_ptr != r._ptr)
        {
            if(r._ptr)
            {
                r._ptr->__incRef();
            }

            T* ptr = this->_ptr;
            this->_ptr = r._ptr;

            if(ptr)
            {
                ptr->__decRef();
            }
        }
        return *this;
    }
        
    template<class Y>
    static Handle dynamicCast(const HandleBase<Y>& r)
    {
#ifdef __BCPLUSPLUS__
        return Handle<T>(dynamic_cast<T*>(r._ptr));
#else
        return Handle(dynamic_cast<T*>(r._ptr));
#endif
    }

    template<class Y>
    static Handle dynamicCast(Y* p)
    {
#ifdef __BCPLUSPLUS__
        return Handle<T>(dynamic_cast<T*>(p));
#else
        return Handle(dynamic_cast<T*>(p));
#endif
    }
};

}

#endif
