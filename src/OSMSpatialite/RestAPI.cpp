//
//  RestAPI.cpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 11/4/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#include <OSMSpatialite/RestAPI.hpp>

namespace OSM {
   
RestAPI::RestAPI(const std::shared_ptr<AmigoCloud::Database> db, const std::string& baseUrl) :
_db(db),
_baseUrl(baseUrl),
_restClient("OSMSpatialite/0.0.1"){
    
}
    
}
