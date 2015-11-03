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


#ifndef INCLUDE_RESTCLIENT_H_
#define INCLUDE_RESTCLIENT_H_

#include <curl/curl.h>
#include <string>
#include <vector>
#include <cstdlib>
//#include <cache/fCache.h>

namespace AmigoCloud
{
    
    class RestClient
    {
    public:
        
        RestClient(const char* user_agent, bool useCache=false);
        virtual ~RestClient();
        
        /**
         * public data definitions
         */
        /** response struct for queries */
        typedef struct response
        {
            int code;
            std::string body;
            
            response()
            {
                code = -1;
            }
            
            /*
            * Code 202 is the async request ACCEPTED by the server
            */
            bool isBad() { return (code >= 400 || code < 0 || code == 202); }
            
        } response;
        /** struct used for uploading data */
        typedef struct
        {
            bool isBuffer;
            std::string name;
            std::string bufferName;
            const char* data;
            size_t length;
        } upload_object;

        typedef struct
        {
            unsigned long long lastTimestamp;
            curl_off_t lastDownProgress;
            curl_off_t lastUpProgress;
        } progress_object;
        
        /** public methods */
        // HTTP GET
        response get(const std::string& url, int timeout=-1);
        // HTTP POST
        response post(const std::string& url, const std::string& ctype,
                      const std::string& data, int timeout=-1);
        
        // HTTP POST multipart
        // Example of contentRange "Content-Range: bytes 21010-47021/47022"
        response post(const std::string& url, const std::string& ctype,
                                const std::string& contentRange,
                                std::vector<upload_object>& data, int timeout=-1);
                      
        // HTTP PUT
        response put(const std::string& url, const std::string& ctype,
                     const std::string& data, int timeout=-1);
        // HTTP DELETE
        response del(const std::string& url, int timeout=-1);

        void setAuthorizationHeader(const std::string &auth_header) {_auth_header = auth_header;}

        void enableCacheWrite(bool enable) { _useCacheForWrite = enable; }

        static void clearCache();
        
    private:
        // writedata callback function
        static size_t write_callback(void *ptr, size_t size, size_t nmemb,
                                     void *userdata);
        // read callback function
        static size_t read_callback(void *ptr, size_t size, size_t nmemb,
                                    void *userdata);

        // progress callback function
        static int progress_callback(void *p, curl_off_t downTotal, curl_off_t down, curl_off_t upTotal, curl_off_t up);
        
        void _setupProxy();
        
        response _checkCache(const std::string &url);
        void _addToCache(const std::string &url, const std::string &data);
        std::string _userAgent;
        
        void _prepareCurl(response &ret, const std::string& url);   
        char errorBuffer[ CURL_ERROR_SIZE ];
        progress_object progress_obj;

        std::string _auth_header;
        CURL *_pCurl;
        bool _useCache;
        bool _useCacheForWrite;
//        static fCache *_getNetworkCache();

    };
    
}

#endif  // INCLUDE_RESTCLIENT_H_
