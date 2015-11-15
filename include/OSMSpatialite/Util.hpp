//
//  Util.h
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/2/15.
//  Copyright (c) 2015 theoutpost. All rights reserved.
//

#ifndef __OSMSpatialite__Util__
#define __OSMSpatialite__Util__

#include <string>
#include <stdlib.h>

namespace OSM {
namespace Util {

/**
 *	The current working directory of the process.
 */
std::string cwd();

/**
 *  Creates a temporary directory.
 *  Deletes the current directory if exits.
 */
std::string createTestTmpDir();

    
inline bool isLong(const std::string& s) {
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
    
    char *p;
    strtol(s.c_str(), &p, 10);
    
    return (*p == 0);
}

inline bool isDouble(const std::string& s) {
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
    
    char *p;
    strtod(s.c_str(), &p);
    
    return (*p == 0);
}
    
}
}

#endif /* defined(__OSMSpatialite__Util__) */
