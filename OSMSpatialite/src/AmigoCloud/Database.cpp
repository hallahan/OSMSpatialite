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


#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
//#include <core/JSONUtils.h>
#include "Database.h"
#include "Logger.h"
//#include "SpatialiteInit.h"
#include <fcntl.h>

namespace AmigoCloud
{
    static std::string TAG("Database");
    
    /////////////////////
    //
    // Database
    //
    static bool isFileExist(const char *path)
    {
        int fd= open(path, O_RDONLY );
        if(fd != -1)
        {
            close(fd);
            return true;
        }
        return false;
    }
    
    Database::Database() :
    _filename(""),
    _transactionInProgress(false),
    _readsInProgress(0),
    _cache(NULL),
    _isSpatial(false)
    {
        _pDatabase = NULL;
    }
    
    //this constructor needs to be called by all derived classes first!
    Database::Database(const std::string &filename, bool isSpatial) :
    _filename(filename),
    _transactionInProgress(false),
    _readsInProgress(0),
    _cache(NULL),
    _isSpatial(isSpatial)
    {
        _pDatabase = NULL;
    }
    
    Database::~Database()
    {
        _close();
    }
    
    bool Database::isExists()
    {
        std::lock_guard<std::mutex> lock(_connectionMutex);
        return isFileExist(_filename.c_str());
    }
    
    bool Database::_open()
    {
        std::lock_guard<std::mutex> lock(_connectionMutex);
        
        if(_filename.empty())
            return false;
        
        connectionCount++;
        if(isOpen())
            return true;
        
        std::lock_guard<std::recursive_mutex> writeLock(_mutex);
        std::lock_guard<std::recursive_mutex> readLock(_readMutex);
        bool fileExists = false; //TODO: fix
        
        if(sqlite3_open(_filename.c_str(), &_pDatabase) == SQLITE_OK)
        {
            _executeSQLInternal("PRAGMA temp_store=MEMORY;");
            _executeSQLInternal("PRAGMA journal_mode=WAL;");
            _executeSQLInternal("PRAGMA page_size=4096;");
            _executeSQLInternal("PRAGMA read_uncommited=ON;");
            if(_isSpatial)
            {
                _cache = spatialite_alloc_connection ();
                spatialite_init_ex(_pDatabase, _cache, 0);
                if (!fileExists)
                {
                    _executeSQLInternal("SELECT InitSpatialMetaData('NONE')");
                    _executeSQLInternal("SELECT InsertEpsgSrid(4326)");
                }
            }
            sqlite3_soft_heap_limit64(1024*1024);
            _executeSQLInternal("CREATE TABLE IF NOT EXISTS amigo_config(name TEXT PRIMARY KEY, value BLOB);");
            return true;
        }
        return false;
    }
    
    void Database::_close()
    {
        std::lock_guard<std::mutex> lock(_connectionMutex);
        if(connectionCount-- <= 0)
        {
            std::lock_guard<std::recursive_mutex> writeLock(_mutex);
            std::lock_guard<std::recursive_mutex> readLock(_readMutex);
            if (!_transactionInProgress && isOpen())
            {
                sqlite3_close(_pDatabase);
                _pDatabase=NULL;
                
                if(_cache)
                    spatialite_cleanup_ex (_cache);
            }
            clearMemory();
        }
    }
    
    bool Database::beginTransaction()
    {
        _mutex.lock();
        _open();
        if(!_transactionInProgress)
        {
            if(_executeSQLInternal("BEGIN;"))
            {
                _transactionInProgress = true;
                return true;
            }
        }
        _mutex.unlock();
        return false;
    }
    
    void Database::commitTransaction()
    {
        if(_transactionInProgress)
        {
            _executeSQLInternal("COMMIT;");
            _transactionInProgress = false;
            _close();
            _mutex.unlock();
        }
    }
    
    void Database::rollbackTransaction()
    {
        if(_transactionInProgress)
        {
            _executeSQLInternal("ROLLBACK;");
            _transactionInProgress = false;
            _close();
            _mutex.unlock();
        }
    }
    
    // NH TODO
    //    sqlite3_stmt* Database::prepareStatement(const char* sql)
    //    {
    //        if (sql == NULL)
    //            return NULL;
    //
    //        sqlite3_stmt *statement = NULL;
    //
    //        int errorCode = sqlite3_prepare_v2(_pDatabase, sql, -1, &statement, 0);
    //
    //        if (errorCode != SQLITE_OK) {
    //            return NULL;
    //        }
    //
    //        return statement;
    //    }
    //
    //    bool Database::executeStatement(sqlite3_stmt* statement)
    //    {
    //        return false;
    //    }
    
    bool Database::executeSQL(const char* sql, DatabaseResult &outResults)
    {
        if(sql==NULL)
            return false;
        
        std::unique_lock<std::recursive_mutex> writeLock(_mutex, std::defer_lock);
        std::unique_lock<std::recursive_mutex> readLock(_readMutex, std::defer_lock);
        _open();
        bool read = _checkReads(writeLock, readLock, sql);
        
        sqlite3_stmt *statement = NULL;
        
        outResults.errorCode = sqlite3_prepare_v2(_pDatabase, sql, -1, &statement, 0);
        
        bool once=true;
        
        if(outResults.errorCode == SQLITE_OK)
        {
            int cols = sqlite3_column_count(statement);
            int result = 0;
            while(true)
            {
                result = sqlite3_step(statement);
                
                if(result == SQLITE_ROW)
                {
                    std::vector<std::string> values;
                    for(int col = 0; col < cols; ++col)
                    {
                        if(once)
                        {
                            const char *name = sqlite3_column_name(statement, col);
                            outResults.columnNames.push_back(name);
                        }
                        switch(sqlite3_column_type(statement, col))
                        {
                            case SQLITE_BLOB:
                            {
                                char *blob = (char*) sqlite3_column_blob(statement, col);
                                int size = sqlite3_column_bytes(statement, col);
                                if(size > 0)
                                {
                                    std::string blobStr;
                                    blobStr.reserve(size);
                                    blobStr.insert(0, blob, size);
                                    values.push_back(std::move(blobStr));
                                }
                            }
                                break;
                                
                            case SQLITE_INTEGER:
                            {
                                std::stringstream val;
                                val << sqlite3_column_int64(statement, col);
                                values.push_back(val.str());
                            }
                                break;
                                
                            case SQLITE_FLOAT:
                            {
                                std::stringstream val;
                                val << std::setprecision(16) << sqlite3_column_double(statement, col);
                                values.push_back(val.str());
                            }
                                break;
                                
                            case SQLITE_TEXT:
                            {
                                values.push_back((char*)sqlite3_column_text(statement, col));
                            }
                                break;
                                
                            case SQLITE_NULL:
                            {
                                values.push_back("NULL");
                            }
                                break;
                                
                            default:
                                values.push_back("");
                                break;
                        }
                        
                    }
                    once=false;
                    
                    outResults.records.push_back(std::move(values));
                }
                else
                {
                    if(result != SQLITE_DONE)
                    {
                        AMIGO_LOG_E(TAG, "::executeSQL() 'sqlite3_step encountered error' for '%s'\n", sql);
                    }
                    break;
                }
            }
        }
        
        sqlite3_finalize(statement);
        
        if(read)
        {
            _readMutex.lock();
            _readsInProgress--;
            _readMutex.unlock();
            _signal.notify_one();
        }
        
        std::string error = sqlite3_errmsg(_pDatabase);
        _close();
        if(outResults.errorCode != SQLITE_OK && error != "not an error")
        {
            AMIGO_LOG_W(TAG, "::executeSQL() error = %d '%s' for '%s'\n", outResults.errorCode, error.c_str(), sql);
            return false;
        }
        
        return true;
    }
    
    bool Database::executeSQL(const char* sql, const char* blob, size_t blobSize)
    {
        if(sql==NULL)
            return false;
        
        std::unique_lock<std::recursive_mutex> writeLock(_mutex, std::defer_lock);
        std::unique_lock<std::recursive_mutex> readLock(_readMutex, std::defer_lock);
        _open();
        bool read = _checkReads(writeLock, readLock, sql);
        
        sqlite3_stmt *statement = NULL;
        int rc = sqlite3_prepare_v2(_pDatabase, sql, -1, &statement, 0);
        if(rc == SQLITE_OK)
        {
            if(statement)
            {
                // For Blob collumn bind 1
                sqlite3_bind_blob(statement, 1, blob, blobSize, SQLITE_TRANSIENT);
                sqlite3_step(statement);
            }
        }
        sqlite3_finalize(statement);
        
        if(read)
        {
            _readMutex.lock();
            _readsInProgress--;
            _readMutex.unlock();
            _signal.notify_one();
        }
        
        std::string error = sqlite3_errmsg(_pDatabase);
        _close();
        if(rc != SQLITE_OK && error != "not an error")
        {
            AMIGO_LOG_E(TAG, "::executeSQL(%s) error = %d '%s' for '%s'\n", _filename.c_str(), rc, error.c_str(), sql);
            return false;
        }
        
        return true;
    }
    
    bool Database::executeSQL(const char* sql, std::vector<std::string> &blobs)
    {
        if(sql==NULL)
            return false;
        
        // AMIGO_LOG_I(TAG, "::executeSQL() '%s'\n", sql);
        std::unique_lock<std::recursive_mutex> writeLock(_mutex, std::defer_lock);
        std::unique_lock<std::recursive_mutex> readLock(_readMutex, std::defer_lock);
        _open();
        bool read = _checkReads(writeLock, readLock, sql);
        
        sqlite3_stmt *statement = NULL;
        int rc = sqlite3_prepare_v2(_pDatabase, sql, -1, &statement, 0);
        if(rc == SQLITE_OK)
        {
            if(statement)
            {
                int blob_index = 1;
                for(std::string blob:blobs)
                {
                    // For Blob collumn bind blob_index
                    sqlite3_bind_blob(statement, blob_index, blob.c_str(), blob.size(), SQLITE_TRANSIENT);
                    blob_index++;
                }
                sqlite3_step(statement);
            }
        }
        sqlite3_finalize(statement);
        
        if(read)
        {
            _readMutex.lock();
            _readsInProgress--;
            _readMutex.unlock();
            _signal.notify_one();
        }
        
        std::string error = sqlite3_errmsg(_pDatabase);
        _close();
        if(rc != SQLITE_OK && error != "not an error")
        {
            AMIGO_LOG_E(TAG, "::executeSQL(%s) error = %d '%s' for '%s'\n", _filename.c_str(), rc, error.c_str(), sql);
            return false;
        }
        
        return true;
    }
    
    bool Database::executeSQL(const char* sql)
    {
        if(sql==NULL)
            return false;
        
        std::unique_lock<std::recursive_mutex> writeLock(_mutex, std::defer_lock);
        std::unique_lock<std::recursive_mutex> readLock(_readMutex, std::defer_lock);
        _open();
        bool ret = _executeSQLInternal(sql);
        _close();
        return ret;
    }
    
    bool Database::_executeSQLInternal(const char* sql)
    {
        std::unique_lock<std::recursive_mutex> writeLock(_mutex, std::defer_lock);
        std::unique_lock<std::recursive_mutex> readLock(_readMutex, std::defer_lock);
        bool read = _checkReads(writeLock, readLock, sql);
        
        char *zErrMsg = 0;
        int rc = sqlite3_exec(_pDatabase, sql, NULL, NULL, &zErrMsg);
        
        if(read)
        {
            _readMutex.lock();
            _readsInProgress--;
            _readMutex.unlock();
            _signal.notify_one();
        }
        
        if(rc!=SQLITE_OK)
        {
            std::string error = sqlite3_errmsg(_pDatabase);
            if(error != "not an error")
            {
                AMIGO_LOG_E(TAG, "::executeSQL(%s) error = %d '%s' for '%s'\n", _filename.c_str(), rc, error.c_str(), sql);
            }
            
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    }
    
    void Database::removeConfig(const std::string &property_name)
    {
        if(property_name.size()==0)
            return;
        
        std::string sql = "DELETE FROM amigo_config WHERE name='"+property_name+"';";
        executeSQL(sql.c_str());
    }
    
    bool Database::setConfig(const std::string &property_name, const std::string &property_value)
    {
        if(property_name.size()==0)
            return false;
        
        std::string sql;
        sql = "INSERT OR REPLACE INTO amigo_config(name,value) VALUES('" + property_name + "',?);";
        return executeSQL(sql.c_str(), (char*)property_value.c_str(), property_value.size());
    }
    
    const std::string Database::getConfig(const std::string &property_name)
    {
        if(property_name.size()==0)
            return "";
        
        std::string sql = "SELECT value FROM amigo_config WHERE name='" + property_name + "';";
        
        DatabaseResult result;
        if(!executeSQL(sql.c_str(), result) || result.records.size() == 0)
            return "";
        
        if(result.records.size()>=1 && result.records[0].size()>=1)
        {
            return result.records[0][0];
        } else
        {
            return "";
        }
    }
    
    bool Database::tableExists(const char *tableName)
    {
        std::string sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + std::string(tableName) + "';";
        DatabaseResult result;
        if (!executeSQL(sql.c_str(),result))
        {
            return false;
        }
        
        if (result.records.size() > 0 && result.records[0][0] == tableName)
        {
            return true;
        }
        return false;
    }
    
    void Database::clearMemory()
    {
        sqlite3_release_memory(1024*1024*128); // 128MB
    }
    
    bool Database::_checkReads(std::unique_lock<std::recursive_mutex> &writeLock, std::unique_lock<std::recursive_mutex> &readLock, const char* sql)
    {
        std::string sqlStr(sql);
        if(sqlStr.find("SELECT") == 0)
        {
            if(readLock.try_lock())
            {
                _readsInProgress++;
                readLock.unlock();
            }
            else
            {
                readLock.lock();
                _readsInProgress++;
                readLock.unlock();
            }
            return true;
        }
        else
        {
            writeLock.lock();
            if(sqlStr.find("DROP") == 0 || sqlStr.find("CREATE") == 0)
            {
                readLock.lock();
                while(!_signal.wait_for(readLock, std::chrono::milliseconds(250), [&](){return _readsInProgress == 0;}));
            }
            return false;
        }
    }
}

