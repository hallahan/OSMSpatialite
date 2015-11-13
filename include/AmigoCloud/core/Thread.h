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


#ifndef __ACTHREAD_H
#define __ACTHREAD_H 1

#include <atomic>
#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <string>
#include <deque>

namespace AmigoCloud {
    
    class Runnable
    {
    public:
        virtual ~Runnable() {}
        virtual void run() {}
        virtual bool toDelete() {return true;}
        virtual bool operator == (const Runnable& r) const {return false;}
    };
    
    class LambdaRunnable : public Runnable
    {
    public:
        LambdaRunnable(std::function<void(void)> l)
        {
            _l = l;
        }
        
        virtual void run()
        {
            _l();
        }
        
    private:
        std::function<void(void)> _l;
    };
    
    class AsyncTaskExecutor
    {
    public:
        AsyncTaskExecutor(const std::string &name, int maxQueueSize=100);
        virtual ~AsyncTaskExecutor();
        virtual bool toDelete() {return false;}
        
        void start();
        void execute(Runnable *task);
        void execute(std::function<void(void)> l);
        void clearQueue();
        void stop();
        
        bool isInQueue(Runnable *task);
        
        bool isBusy() {return _isBusy;}
        int getQueueSize();
        
    protected:
        void run();
        
    private:
        std::unique_ptr<std::thread> _thread;
        std::atomic_bool _isRunning;
        bool _isBusy;
        std::deque<Runnable*> _queue;
        std::mutex _mutex;
        std::condition_variable _signal;
        std::string _name;
        int _maxQueueSize;
        Runnable *_currentTask;
    };
    
    struct AsyncTaskExecutorObject
    {
        AsyncTaskExecutor *executor;
        bool available;
    };
    
    class AsyncTaskExecutorPool
    {
    public:
        AsyncTaskExecutorPool(const std::string &name, int maxQueueSize);
        virtual ~AsyncTaskExecutorPool();
        
        AsyncTaskExecutor *getAvailable();
        void putBack(AsyncTaskExecutor* executor);
        
    private:
        std::deque<AsyncTaskExecutor*> _executors;
        std::string _name;
        int _maxSize;
        std::mutex _mutex;
    };
    
    class TaskExecutor
    {
    public:
        TaskExecutor();
        virtual ~TaskExecutor();
        
        void execute(Runnable *task);
        void execute(std::function<void(void)> l);
        void executeAll();
        void clearAll();
        
    private:
        std::deque<Runnable*> _queue;
        std::mutex _mutex;
    };
    
}

#endif// __ACTHREAD_H