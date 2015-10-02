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


#ifndef AMIGO_ERROR_H
#define AMIGO_ERROR_H

#include <string>

namespace AmigoCloud
{
    enum AMIGO_ERRORCODE
    {
        AMIGO_OK = 0,
        AMIGO_ERROR,
        AMIGO_E_SERVER_CONNECT,
        AMIGO_E_BAD_CREDENTIALS,
        AMIGO_DISK_WRITE_ERROR
    };
    
    struct AmigoStatus
    {
        AmigoStatus():
        errorCode(AMIGO_OK), debugError(""){}
        
        AmigoStatus(AMIGO_ERRORCODE code, const char* debugErrorString = ""):
        errorCode(code), debugError(debugErrorString){}
        
        AMIGO_ERRORCODE errorCode;
        std::string debugError;
        
        inline bool isOK() { return errorCode == AMIGO_OK; }
        inline bool isBad() { return !isOK(); }
        
        const char* getUserMessage()
        {
            // if this list becomes too long, switch to a hash lookup
            
            switch (errorCode)
            {
                case AMIGO_OK:                   return "OK";
                case AMIGO_ERROR:                return std::string("Error: " + debugError).c_str();
                case AMIGO_E_SERVER_CONNECT:     return "Could not connect with servers";
                case AMIGO_E_BAD_CREDENTIALS:    return "The username/password was invalid";
                case AMIGO_DISK_WRITE_ERROR:     return "Failed to write to internal disk. Is your disk or SD card full?";
            }
        }
        
    };
    
}

#endif
