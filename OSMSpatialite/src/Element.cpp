//
//  Element.cpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#include "Element.hpp"


namespace OSM {
    
Element::Element(std::shared_ptr<AmigoCloud::Database> db, const std::string& id,
                       const std::string& action, const std::string& version, const std::string& timestamp,
                       const std::string& changeset, const std::string& uid, const std::string& user,
                       const std::string& visible, const std::string& geometry) :
_db(db),
_action(action),
_timestamp(timestamp),
_user(user),
_visible(visible),
_geometry( AmigoCloud::GeometryHandler::parseWKBBin(geometry) ) {
    
    try {
       _id = std::stol(id);
    } catch (...) {
        _id = 0;
    }
    
    if (action == "NULL") {
        _action = "";
    }
    
    try {
        _version = std::stol(version);
    } catch (...) {
        _version = 0;
    }
    
    if (timestamp == "NULL") {
        _timestamp = "";
    }
    
    try {
        _changeset = std::stol(changeset);
    } catch (...) {
        _changeset = 0;
    }
    
    try {
        _uid = std::stol(uid);
    } catch (...) {
        _uid = 0;
    }
    
    if (user == "NULL") {
        _user = "";
    }
    
}
    
    
    

}