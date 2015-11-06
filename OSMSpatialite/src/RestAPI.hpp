//
//  RestAPI.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 11/4/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef RestAPI_hpp
#define RestAPI_hpp

#include "AmigoCloud/Database.h"
#include "AmigoCloud/RestClient.h"
#include "XmlParser.hpp"

namespace OSM {

class RestAPI {
public:
    
    explicit RestAPI(const std::shared_ptr<AmigoCloud::Database> db, const std::string& baseUrl);
    
protected:
    
    const std::shared_ptr<AmigoCloud::Database> _db;
    const std::string _baseUrl;
    AmigoCloud::RestClient _restClient;
    
};

}

#endif /* RestAPI_hpp */
