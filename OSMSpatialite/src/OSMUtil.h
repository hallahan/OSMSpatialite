//
//  OSMUtil.h
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/2/15.
//  Copyright (c) 2015 theoutpost. All rights reserved.
//

#ifndef __OSMSpatialite__OSMUtil__
#define __OSMSpatialite__OSMUtil__

#include <string>
#include <stdlib.h>

namespace OSM
{
namespace Util
{
    
    
inline bool isLong(const std::string& s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
    
    char *p;
    strtol(s.c_str(), &p, 10);
    
    return (*p == 0);
}

inline bool isDouble(const std::string& s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
    
    char *p;
    strtod(s.c_str(), &p);
    
    return (*p == 0);
}
    
    
}
}

#endif /* defined(__OSMSpatialite__OSMUtil__) */
