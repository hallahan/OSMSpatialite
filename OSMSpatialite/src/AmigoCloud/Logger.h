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



#ifndef amigocloud_Logger_h
#define amigocloud_Logger_h

#include <string>
#include <map>

// #define NO_LOGGER 1

namespace AmigoCloud
{
    namespace Logger
    {
        class Control
        {
        public:
            enum
            {
                VERBOSE,
                INFO,
                WARNING,
                ERROR,
                QUIET
            };
            
            static void setDebugLevel(const std::string &tag, int level);
            static int getDebugLevel(const std::string &tag);
            
        private:
            static std::map<std::string, int> _debugLevels;
        };
        
        typedef void (*DebugLogFormatV)(const std::string &tag, const char *format,...);
        DebugLogFormatV debugLogSetupV(const char *file,int line);
        
        typedef void (*DebugLogFormatI)(const std::string &tag, const char *format,...);
        DebugLogFormatI debugLogSetupI(const char *file,int line);
        
        typedef void (*DebugLogFormatW)(const std::string &tag, const char *format,...);
        DebugLogFormatW debugLogSetupW(const char *file,int line);
        
        typedef void (*DebugLogFormatE)(const std::string &tag, const char *format,...);
        DebugLogFormatE debugLogSetupE(const char *file,int line);
        
        
#ifndef NO_LOGGER
        
#define AMIGO_LOG_V (AmigoCloud::Logger::debugLogSetupV(__FILE__,(int)__LINE__))
#define AMIGO_LOG_I (AmigoCloud::Logger::debugLogSetupI(__FILE__,(int)__LINE__))
#define AMIGO_LOG_W (AmigoCloud::Logger::debugLogSetupW(__FILE__,(int)__LINE__))
#define AMIGO_LOG_E (AmigoCloud::Logger::debugLogSetupE(__FILE__,(int)__LINE__))
        
#else
        
#define AMIGO_LOG_V
#define AMIGO_LOG_I
#define AMIGO_LOG_W
#define AMIGO_LOG_E
        
#endif
        
    }
}

#endif
