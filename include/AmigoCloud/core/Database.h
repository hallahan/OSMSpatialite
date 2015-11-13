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


#ifndef __DATABASE_H
#define __DATABASE_H 1

#include <sqlite3.h>
#include <spatialite.h>

#include <core/AmigoError.h>
#include <core/Thread.h>
#include <mutex>
#include <condition_variable>
#include <memory.h>

#include <string>
#include <array>
#include <vector>
#include <set>
#include <atomic>

namespace AmigoCloud
{
    
    /********************************
     
     Database Result Struct
     
     Purpose: Store the Result to a Database query (with error code)
     
     *********************************/
    struct DatabaseResult
    {
        
        DatabaseResult():errorCode(SQLITE_OK){}
        
        DatabaseResult(const DatabaseResult &r)
        {
            columnNames = r.columnNames;
            records = r.records;
            errorCode = r.errorCode;
        }
        
        void clear()
        {
            errorCode = 0;
            columnNames.clear();
            records.clear();
        }
        
        DatabaseResult& operator=(const DatabaseResult &r)
        {
            columnNames = r.columnNames;
            records = r.records;
            errorCode = r.errorCode;
            return *this;
        }
        
        bool isOK() { return errorCode == SQLITE_OK; }
        int errorCode;
        
        std::vector<std::string>                columnNames;
        std::vector< std::vector<std::string> > records;
    };
    
    /*********************************
     
     Database (Base) Class
     
     Purpose:
     - Abstracts the concept of a sqlite database.
     - Provides schema migration functions
     
     **********************************/
    
    class Database
    {
    public:
        Database();
        Database(const std::string &filename, bool isSpatial=false);
        Database(const Database &_db)
        {
            _pDatabase = _db._pDatabase;
            _cache = _db._cache;
            _isSpatial = _db._isSpatial;
            _transactionInProgress = _db._transactionInProgress;
            _filename = _db._filename;
        }
        virtual ~Database();
        
        void setDBFileName(const std::string &fname) {_filename = fname;}
        const std::string &getDBFileName() {return _filename;}
        
        bool isOpen() {return (_pDatabase!=NULL);}
        bool isExists();
        
        bool beginTransaction();
        void commitTransaction();
        void rollbackTransaction();
        
        // NH TODO
        //        sqlite3_stmt* prepareStatement(const char* sql);
        //        bool executeStatement(sqlite3_stmt* sql);
        
        bool executeSQL(const char* query, DatabaseResult & outResult);
        bool executeSQL(const char* sql);
        bool executeSQL(const char* sql, const char* blob, size_t blobSize);
        bool executeSQL(const char* sql, std::vector<std::string> &blobs);
        
        bool setConfig(const std::string &property_name, const std::string &property_value );
        void removeConfig(const std::string &property_name);
        const std::string getConfig(const std::string &property_name);
        
        
        bool tableExists(const char* tableName);
        
        static void clearMemory();
        
    protected:
        bool _open();
        void _close();
        bool _executeSQLInternal(const char* sql);
        
        std::recursive_mutex _mutex;
        std::recursive_mutex _readMutex;
        std::condition_variable_any _signal;
        
    private:
        sqlite3 *_pDatabase;
        std::mutex _connectionMutex;
        unsigned int connectionCount;
        void     *_cache;
        bool _isSpatial;
        bool _transactionInProgress;
        std::atomic_ushort _readsInProgress;
        std::string _filename;
        
        bool _checkReads(std::unique_lock<std::recursive_mutex> &writeLock, std::unique_lock<std::recursive_mutex> &readLock, const char* sql);
    };
    
};

#endif
