//
//  OSMRest.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 11/3/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef OSMRest_hpp
#define OSMRest_hpp

#include "AmigoCloud/RestClient.h"

namespace OSM {
    
class OSMRest {
public:
    explicit OSMRest(const std::string& baseUrl="http://www.openstreetmap.org/api/0.6/");
    
protected:
    const std::string _baseUrl;
};
    
}

#endif /* OSMRest_hpp */
