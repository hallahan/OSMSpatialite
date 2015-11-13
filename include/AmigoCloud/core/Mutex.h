/*
*
* AmigoMobile
* 
* Copyright (c) 2011-2015 AmigoCloud Inc., All rights reserved.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
* 
* You should have received a copy of the GNU General Public
* License along with this library.
* 
*/


#ifndef __MUTEX_H
#define __MUTEX_H 1

#include <core/Logger.h>
#include <mutex>
#include <string>
#include <memory.h>
#include <unistd.h>

namespace AmigoCloud
{

//#define DEADLOCK_DEBUG 30

#define MUTEX_TAG "Mutex"
#define SCOPED_PRINT_TAG "ScopedPrint"

class Mutex
{
public:

	Mutex(const std::string &name) :
		_name(name.c_str())
	{
	}

    Mutex(const Mutex &m)
    {
        _name = m._name;
    }

    Mutex &operator=(const Mutex &m)
    {
        _name = m._name;
        return *this;
    }

    Mutex(Mutex&& m) :
        _name(std::move(m._name)) {}

    Mutex& operator=(Mutex&& m)
    {
        if (this != &m)
        {
            _name = std::move(m._name);
        }
        return *this;
    }

	virtual ~Mutex() {}

	void lock()
	{
    #ifdef DEADLOCK_DEBUG
        int i = 0;
        for(; i < DEADLOCK_DEBUG; i++)
        {
            if(!_mutex.try_lock())
            {
                AMIGO_LOG_E(MUTEX_TAG, "::lock() %s try lock failed i=%d\n", _name.c_str(), i);
                usleep(1*1000*1000);
            }
            else
            {
                break;
            }
        }
        if(i == DEADLOCK_DEBUG)
        {
            AMIGO_LOG_E(MUTEX_TAG, "::lock() %s mutex timed out\n", _name.c_str());
        }
    #else
        _mutex.lock();
    #endif
	}

	void unlock()
	{
		_mutex.unlock();
	}

	const std::string &getName() {return _name;}

private:
	std::recursive_mutex _mutex;
	std::string _name;

};

class ScopedLock
{
public:
	ScopedLock(Mutex *m)
	{
		_m = m;
		_m->lock();
		locked = true;
	}

	virtual ~ScopedLock()
	{
		unlock();
	}

	void unlock()
	{
	    if(locked)
	    {
	        _m->unlock();
	        locked = false;
	    }
	}

    void lock()
    {
        if(!locked)
        {
            _m->lock();
            locked = true;
        }
    }

private:
	Mutex *_m;
	bool locked;
};

class ScopedPrint
{
public:
	ScopedPrint(const std::string &text) {
		_text = text;
	}

	virtual ~ScopedPrint() {
	}

private:
	std::string _text;
};
}
#endif //__MUTEX_H
