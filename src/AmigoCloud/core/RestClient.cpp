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


#include <core/RestClient.h>

#include <cstring>
#include <string>
#include <iostream>
#include <sys/time.h>
#include <core/Mutex.h>
#include <core/SystemConfig.h>

#include <core/Logger.h>
#include <stack>

namespace AmigoCloud
{
    static std::string TAG("RestClient");
    static int TIMEOUT_MS = 30000;
    static int MAX_HANDLE_POOL_SIZE = 20;
    static std::stack<CURL*> curlPool;
    static Mutex _mutex("RestClient");

//    fCache *RestClient::_getNetworkCache()
//    {
//    	static fCache *cache = new fCache(1000, SystemConfig::getFilesDir(), "networkcache.db");
//    	return cache;
//    }

    static unsigned long long timeStamp()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    static bool isSSLVerificationError(CURLcode code)
    {
        return code == 51 || code == 60 || code == 77 || code == 82 || code == 83;
    }
    
    RestClient::RestClient(const char* user_agent, bool useCache):
    _userAgent(user_agent),
    _auth_header(""),
    _useCache(useCache),
    _useCacheForWrite(useCache)
    {
        ScopedLock sl(&_mutex);
        
        curl_global_init(CURL_GLOBAL_ALL);

        _pCurl=NULL;
        if(curlPool.empty())
        {
            _pCurl = curl_easy_init();
        }
        else
        {
            _pCurl = curlPool.top();
            curlPool.pop();
        }
    }
    
    RestClient::~RestClient()
    {
        ScopedLock sl(&_mutex);
        if(_pCurl)
        {
            if(curlPool.size() < MAX_HANDLE_POOL_SIZE)
            {
                curlPool.push(_pCurl);
            }
            else
            {
                curl_easy_cleanup(_pCurl);
            }
        }
    }

//    void RestClient::clearCache()
//    {
//        ScopedLock sl(&_mutex);
//        _getNetworkCache()->clear();
//    }

//    RestClient::response RestClient::_checkCache(const std::string &url)
//    {
//        RestClient::response ret;
//        auto body = _getNetworkCache()->get(url);
//        if(body)
//            ret.body = *body.get();
//        else
//            ret.body = "";
//
//        if(ret.body.empty())
//            ret.code = -1;
//        else
//            ret.code = 200;
//        
//        return ret;
//    }

//    void RestClient::_addToCache(const std::string &url, const std::string &data)
//    {
//        ScopedLock sl(&_mutex);
//        auto dataPtr = std::make_shared<std::string>(data);
//        _getNetworkCache()->put(url, std::move(dataPtr), true);
//    }

    void RestClient::_setupProxy()
    {
#if 0
        // This proxy setup is for debug only
        curl_easy_setopt(_pCurl, CURLOPT_PROXY, "192.168.0.2:8888");
#endif
    }

    void RestClient::_prepareCurl(response &ret, const std::string& url)
    {
        _setupProxy();

        progress_obj.lastTimestamp = timeStamp();
        progress_obj.lastDownProgress = 0;
        progress_obj.lastUpProgress = 0;

        curl_easy_setopt(_pCurl, CURLOPT_USERAGENT, _userAgent.c_str());
        curl_easy_setopt(_pCurl, CURLOPT_ERRORBUFFER, errorBuffer);
        curl_easy_setopt(_pCurl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(_pCurl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(_pCurl, CURLOPT_TRANSFER_ENCODING, 1L);
        curl_easy_setopt(_pCurl, CURLOPT_WRITEFUNCTION, RestClient::write_callback);
        curl_easy_setopt(_pCurl, CURLOPT_WRITEDATA, &ret);
        curl_easy_setopt(_pCurl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(_pCurl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(_pCurl, CURLOPT_XFERINFOFUNCTION, RestClient::progress_callback);
        curl_easy_setopt(_pCurl, CURLOPT_XFERINFODATA, &progress_obj);
        curl_easy_setopt(_pCurl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4 );
        
        if (SystemConfig::getIgnoreSSLCertificate())
        {
            curl_easy_setopt(_pCurl, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(_pCurl, CURLOPT_SSL_VERIFYHOST, 2);
        }
        else
        {
            curl_easy_setopt(_pCurl, CURLOPT_SSL_VERIFYPEER, 1);
            curl_easy_setopt(_pCurl, CURLOPT_SSL_VERIFYHOST, 2);
            curl_easy_setopt(_pCurl, CURLOPT_CAPATH, SystemConfig::getCacheDir().c_str());
            curl_easy_setopt(_pCurl, CURLOPT_CAINFO, (SystemConfig::getCacheDir() + "/ca-bundle.crt").c_str());
        }
    }
    

    RestClient::response RestClient::get(const std::string& url, int timeout)
    {
        RestClient::response ret;
        
        if(url.empty())
            return ret;
        
//        if(_useCache || !SystemConfig::isOnLine()) {
//            ScopedLock sl(&_mutex);
//            ret = _checkCache(url);
//            if(!ret.isBad())
//            {
//                return ret;
//            }
//        }

        if(!SystemConfig::isOnLine())
        {
             return ret;
        }
        
        curl_easy_reset(_pCurl);
        
        _prepareCurl(ret, url);

        if(_auth_header.size() > 0)
        {
            curl_slist* header = NULL;
            header = curl_slist_append(header, _auth_header.c_str());
            curl_easy_setopt(_pCurl, CURLOPT_HTTPHEADER, header);
        }

        CURLcode res = curl_easy_perform(_pCurl);
        
        if (res != 0)
        {
            ret.body = errorBuffer;
            ret.code = -1;

            return ret;
        }

        long http_code = 0;
        curl_easy_getinfo(_pCurl, CURLINFO_RESPONSE_CODE, &http_code);
        ret.code = static_cast<int>(http_code);
        
        if(ret.code / 100 == 2 && _useCacheForWrite)
        {
            //_addToCache(url, ret.body);
        }
        return ret;
    }
    
    RestClient::response RestClient::post(const std::string& url,
                                          const std::string& ctype,
                                          const std::string& data,
                                          int timeout)
    {
        
        RestClient::response ret;
        
//        if(_useCache || !SystemConfig::isOnLine()) {
//            ScopedLock sl(&_mutex);
//            ret = _checkCache(url);
//            if(!ret.isBad())
//            {
//                return ret;
//            }
//        }
        
        if(!SystemConfig::isOnLine())
        {
            return ret;
        }
        
    	curl_easy_reset(_pCurl);
        
        std::string ctype_header = "Content-Type: " + ctype;
        
        /** initialize upload object */
        
        RestClient::upload_object up_obj;
        up_obj.data = data.c_str();
        up_obj.length = data.size();

        _prepareCurl(ret, url);
        curl_easy_setopt(_pCurl, CURLOPT_POST, 1L);
        curl_easy_setopt(_pCurl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(_pCurl, CURLOPT_POSTFIELDSIZE, data.size());
        
        /** set content-type header */
        curl_slist* header = NULL;
        header = curl_slist_append(header, ctype_header.c_str());

        if(_auth_header.size() > 0)
            header = curl_slist_append(header, _auth_header.c_str());

        curl_easy_setopt(_pCurl, CURLOPT_HTTPHEADER, header);

        
        CURLcode res = curl_easy_perform(_pCurl);
        if (res != 0)
        {
            ret.body = errorBuffer;
            if(isSSLVerificationError(res) || ret.body.compare(0, 3, "SSL") == 0)
                ret.code = -2;
            else
                ret.code = -1;
            AMIGO_LOG_E(TAG, "::post() curlCode:%d, code:%d, '%s' url:'%s'\n", res, ret.code, ret.body.c_str(), url.c_str());
            return ret;
        }
        
        long http_code = 0;
        curl_easy_getinfo(_pCurl, CURLINFO_RESPONSE_CODE, &http_code);
        ret.code = static_cast<int>(http_code);

        if(ret.code / 100 == 2 && _useCacheForWrite)
        {
            //_addToCache(url, ret.body);
        }
        return ret;
    }
    
    
    RestClient::response RestClient::post(const std::string& url,
                                      const std::string& ctype,
                                      const std::string& contentRange,
                                      std::vector<upload_object>& data,
                                      int timeout)
    {
        RestClient::response ret;
        
//        if(_useCache || !SystemConfig::isOnLine()) {
//            ScopedLock sl(&_mutex);
//            ret = _checkCache(url);
//            if(!ret.isBad())
//            {
//                return ret;
//            }
//        }
        
        if(!SystemConfig::isOnLine())
        {
            return ret;
        }
        
    	curl_easy_reset(_pCurl);
        
        std::string ctype_header = "Content-Type: " + ctype;
        
        /** initialize upload object */
        struct curl_httppost *formpost=NULL;
        struct curl_httppost *lastptr=NULL;
        struct curl_slist *headerlist=NULL;
        static const char buf[] = "Expect:";
        
        std::vector<upload_object>::iterator it;
        for(it=data.begin(); it!=data.end(); it++)
        {
            RestClient::upload_object &up_obj = *it;
            if(up_obj.isBuffer)
            {
                curl_formadd( &formpost, &lastptr,
                    CURLFORM_COPYNAME, up_obj.name.c_str(),
                    CURLFORM_BUFFER, up_obj.bufferName.c_str(),
                    CURLFORM_BUFFERPTR, up_obj.data,
                    CURLFORM_BUFFERLENGTH, up_obj.length,
                    CURLFORM_END);
            } else
            {
                curl_formadd(&formpost, &lastptr, 
                    CURLFORM_COPYNAME, up_obj.name.c_str(),
                    CURLFORM_COPYCONTENTS, up_obj.data,
                    CURLFORM_CONTENTSLENGTH, up_obj.length,
                    CURLFORM_END);
            }
        }

        headerlist = curl_slist_append(headerlist, buf);
       if(_auth_header.size() > 0)
            headerlist = curl_slist_append(headerlist, _auth_header.c_str());
       if(contentRange.size() > 0)
            headerlist = curl_slist_append(headerlist, contentRange.c_str());

        _prepareCurl(ret, url);
        curl_easy_setopt(_pCurl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(_pCurl, CURLOPT_HTTPPOST, formpost);
    
        CURLcode res = curl_easy_perform(_pCurl);
        if (res != 0)
        {
            ret.body = errorBuffer;
            ret.code = -1;
            AMIGO_LOG_E(TAG, "::post() code:%d, '%s' url:'%s'\n", ret.code, ret.body.c_str(), url.c_str());
            return ret;
        }
        
        long http_code = 0;
        curl_easy_getinfo(_pCurl, CURLINFO_RESPONSE_CODE, &http_code);
        ret.code = static_cast<int>(http_code);

        if(ret.code / 100 == 2 && _useCacheForWrite)
        {
            //_addToCache(url, ret.body);
        }
        
        /* then cleanup the formpost chain */
        curl_formfree(formpost);
        /* free slist */
        curl_slist_free_all(headerlist);
        
        return ret;
    }
    
    RestClient::response RestClient::put(const std::string& url,
                                         const std::string& ctype,
                                         const std::string& data,
                                         int timeout)
    {
        RestClient::response ret;
        if(!SystemConfig::isOnLine())
        {
            return ret;
        }
        
    	curl_easy_reset(_pCurl);
        
        /** build content-type header string */
        std::string ctype_header = "Content-Type: " + ctype;
        
        /** initialize upload object */
        RestClient::upload_object up_obj;
        up_obj.data = data.c_str();
        up_obj.length = data.size();

        _prepareCurl(ret, url);
        curl_easy_setopt(_pCurl, CURLOPT_PUT, 1L);
        curl_easy_setopt(_pCurl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(_pCurl, CURLOPT_READFUNCTION, RestClient::read_callback);
        curl_easy_setopt(_pCurl, CURLOPT_READDATA, &up_obj);
        curl_easy_setopt(_pCurl, CURLOPT_INFILESIZE,
                         static_cast<long>(up_obj.length));
        
        /** set content-type header */
        curl_slist* header = NULL;
        header = curl_slist_append(header, ctype_header.c_str());

        if(_auth_header.size() > 0)
            header = curl_slist_append(header, _auth_header.c_str());

        curl_easy_setopt(_pCurl, CURLOPT_HTTPHEADER, header);
        
        CURLcode res = curl_easy_perform(_pCurl);
        if (res != 0)
        {
            ret.body = errorBuffer;
            ret.code = -1;
            AMIGO_LOG_E(TAG, "::put() code:%d, '%s'\n", ret.code, ret.body.c_str());
            return ret;
        }
        
        long http_code = 0;
        curl_easy_getinfo(_pCurl, CURLINFO_RESPONSE_CODE, &http_code);
        ret.code = static_cast<int>(http_code);

        return ret;
    }
    
    RestClient::response RestClient::del(const std::string& url, int timeout)
    {
        RestClient::response ret;

        if(!SystemConfig::isOnLine())
        {
            return ret;
        }
        
        curl_easy_reset(_pCurl);
        
        _prepareCurl(ret, url);

        const char* http_delete = "DELETE";
        curl_easy_setopt(_pCurl, CURLOPT_CUSTOMREQUEST, http_delete);
        
        if(_auth_header.size() > 0)
        {
            curl_slist* header = NULL;
            header = curl_slist_append(header, _auth_header.c_str());
            curl_easy_setopt(_pCurl, CURLOPT_HTTPHEADER, header);
        }

        CURLcode res = curl_easy_perform(_pCurl);
        if (res != 0)
        {
            ret.body = errorBuffer;
            ret.code = -1;
            AMIGO_LOG_E(TAG, "::del() code:%d, '%s'\n", ret.code, ret.body.c_str());
            return ret;
        }
        long http_code = 0;
        curl_easy_getinfo(_pCurl, CURLINFO_RESPONSE_CODE, &http_code);
        ret.code = static_cast<int>(http_code);
        
        return ret;
    }
    
    
    size_t RestClient::write_callback(void *data, size_t size, size_t nmemb,
                                      void *userdata)
    {
        RestClient::response* r;
        r = reinterpret_cast<RestClient::response*>(userdata);
        r->body.append(reinterpret_cast<char*>(data), size*nmemb);
        
        return (size * nmemb);
    }
    
    size_t RestClient::read_callback(void *data, size_t size, size_t nmemb,
                                     void *userdata)
    {
        RestClient::upload_object* u;
        u = reinterpret_cast<RestClient::upload_object*>(userdata);
        
        /** set correct sizes */
        size_t curl_size = size * nmemb;
        size_t copy_size = (u->length < curl_size) ? u->length : curl_size;
        
        /** copy data to buffer */
        memcpy(data, u->data, copy_size);
        
        /** decrement length and increment data pointer */
        u->length -= copy_size;
        u->data += copy_size;
        
        /** return copied size */
        return copy_size;
    }

    int RestClient::progress_callback(void *p, curl_off_t downTotal, curl_off_t down, curl_off_t upTotal, curl_off_t up)
    {
        progress_object *myp = (progress_object *)p;
        if((down > 0 && myp->lastDownProgress != down) ||
           (up   > 0 && myp->lastUpProgress   != up))
        {
            myp->lastDownProgress = down;
            myp->lastUpProgress = up;
            myp->lastTimestamp = timeStamp();
            return 0;
        }

        if(downTotal==0 && down==0 && upTotal==0 && up==0)
        {
            // AMIGO_LOG_W(TAG,  "::progress_callback() empty block\n");
        }
        
        if((timeStamp() - myp->lastTimestamp) / 1000 > TIMEOUT_MS)
        {
            // No progress was made: timeout the connection
            return 1;
        }
        return 0;
    }
    
}
