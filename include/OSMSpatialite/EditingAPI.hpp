//
//  EditingAPI.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 11/4/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef EditingAPI_hpp
#define EditingAPI_hpp

#include <OSMSpatialite/RestAPI.hpp>

namespace OSM {
    
class EditingAPI : public RestAPI {
public:
    
    explicit EditingAPI(const std::shared_ptr<AmigoCloud::Database> db, const std::string& baseUrl = "http://www.openstreetmap.org/api/0.6/");
    
    void get(const std::string& bbox);
    
    
private:
    
};
    
}

#endif /* EditingAPI_hpp */
