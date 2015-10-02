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


#include "Thread.h"
#include "Logger.h"

//#include "ExceptionHandler.h"

namespace AmigoCloud {
    static std::string TAG("Thread");
    
    AsyncTaskExecutor::AsyncTaskExecutor(const std::string &name, int maxQueueSize) :
    _name(name),
    _isRunning(false),
    _isBusy(false),
    _maxQueueSize(maxQueueSize),
    _currentTask(NULL)
    {
    }
    
    void AsyncTaskExecutor::start()
    {
        if(!_isRunning)
        {
            _isRunning = true;
            _thread.reset(new std::thread([] (AsyncTaskExecutor *executor)
                                          {
                                              //                                              COFFEE_TRY_NATIVE( executor->run(); );
                                              executor->run();
                                          }, this));
        }
    }
    
    void AsyncTaskExecutor::stop()
    {
        if(_isRunning)
        {
            _isRunning = false;
            clearQueue();
            _signal.notify_one();
            if(_thread)
            {
                _thread->join();
                _thread.reset(nullptr);
            }
        }
    }
    
    AsyncTaskExecutor::~AsyncTaskExecutor()
    {
        stop();
    }
    
    void AsyncTaskExecutor::execute(Runnable *task)
    {
        if(_maxQueueSize != 0 && _queue.size() > _maxQueueSize)
        {
            clearQueue();
        }
        
        _mutex.lock();
        _queue.push_back( task );
        _mutex.unlock();
        _signal.notify_one();
    }
    
    void AsyncTaskExecutor::execute(std::function<void(void)> l)
    {
        execute(new LambdaRunnable(l));
    }
    
    int AsyncTaskExecutor::getQueueSize()
    {
        int size=(int)_isBusy;
        _mutex.lock();
        size += _queue.size();
        _mutex.unlock();
        return size;
    }
    
    bool AsyncTaskExecutor::isInQueue(Runnable *task)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        
        if(_currentTask != NULL && *_currentTask == *task)
        {
            return true;
        }
        
        std::deque<Runnable*>::iterator it;
        for(it = _queue.begin(); it != _queue.end(); it++)
        {
            if(*task == *(*it))
            {
                return true;
            }
        }
        
        return false;
    }
    
    void AsyncTaskExecutor::run()
    {
        while(_isRunning)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if(!_queue.empty())
            {
                Runnable *task = _queue.front();
                _queue.pop_front();
                if(task!=NULL)
                {
                    _isBusy = true;
                    _currentTask = task;
                    lock.unlock();
                    task->run();
                    lock.lock();
                    _currentTask = NULL;
                    if(task->toDelete())
                        delete task;
                }
            }
            else if(_isRunning)
            {
                _isBusy = false;
                _signal.wait(lock);
            }
        }
        return;
    }
    
    void AsyncTaskExecutor::clearQueue()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        for(int i = 0; i < _queue.size(); i++)
        {
            Runnable *task = _queue[i];
            if(task && task->toDelete())
            {
                delete task;
            }
        }
        _queue.clear();
    }
    
    AsyncTaskExecutorPool::AsyncTaskExecutorPool(const std::string &name, int maxSize) :
    _name(name),
    _maxSize(maxSize)
    {
    }
    
    AsyncTaskExecutorPool::~AsyncTaskExecutorPool()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        for(int i = 0; i < _executors.size(); i++)
        {
            delete _executors[i];
        }
        _executors.clear();
    }
    
    AsyncTaskExecutor *AsyncTaskExecutorPool::getAvailable()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if(_executors.empty())
        {
            AsyncTaskExecutor *t = new AsyncTaskExecutor(_name, _maxSize);
            t->start();
            return t;
        }
        else
        {
            AsyncTaskExecutor *t = _executors.front();
            _executors.pop_front();
            t->start();
            return t;
        }
    }
    
    void AsyncTaskExecutorPool::putBack(AsyncTaskExecutor* executor)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _executors.push_back(executor);
    }
    
    TaskExecutor::TaskExecutor() {}
    TaskExecutor::~TaskExecutor() {}
    
    void TaskExecutor::execute(Runnable *task)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push_back( task );
    }
    
    void TaskExecutor::execute(std::function<void(void)> l)
    {
        execute(new LambdaRunnable(l));
    }
    
    void TaskExecutor::executeAll()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        while(!_queue.empty())
        {
            Runnable *task = _queue.front();
            _queue.pop_front();
            if(task!=NULL)
            {
                task->run();
                if(task->toDelete())
                    delete task;
            }
        }
    }
    
    void TaskExecutor::clearAll()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        for(int i = 0; i < _queue.size(); i++)
        {
            Runnable *task = _queue[i];
            if(task && task->toDelete())
                delete task;
        }
        _queue.clear();
    }
    
} // namespace
