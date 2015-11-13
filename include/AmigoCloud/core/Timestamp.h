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


#ifndef TIMESTAMP_CORE_DEF
#define TIMESTAMP_CORE_DEF 1


#include <sstream>
#include <string>
#include <chrono>

namespace AmigoCloud {
    
    typedef unsigned long long Timestamp_t;
    
    class Timestamp
    {
    public:
        Timestamp()
        {
            this->operator()();
        }
        
        Timestamp(const Timestamp &timestamp)
        {
            t = timestamp.t;
        }
        
        void operator() ()
        {
            t = std::chrono::high_resolution_clock::now();
        }
        
        double deltaMs(Timestamp &ts)
        {
            return std::chrono::duration_cast<std::chrono::microseconds>(ts.t - t).count() / 1000.0;
        }
        
        static double deltaNowMs(Timestamp &t)
        {
            Timestamp end;
            return t.deltaMs(end);
        }
        
        static Timestamp_t unixTimeMs()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                         std::chrono::system_clock::now().time_since_epoch()).count();
        }
        
        static Timestamp_t unixTime()
        {
            return std::chrono::duration_cast<std::chrono::seconds>(
                                                                    std::chrono::system_clock::now().time_since_epoch()).count();
        }
        
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> t;
    };
    
}

#endif
