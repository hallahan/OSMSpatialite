//
//  Element.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef Element_hpp
#define Element_hpp

#include <map>
#include <vector>
#include <geos.h>
#include "AmigoCloud/Database.h"
#include "AmigoCloud/GeometryHandler.h"

namespace OSM {

// Used to differentiate between the various types of Elements.
enum class ElementType { NODE, WAY, RELATION, TAG, ND, MEMBER, BOUNDS, OSM };

// Forward declaring Relation so we can reference it within Element.
class Relation;
    
class Element {
public:
    
    // Getters
    long getId()                { return _id; }
    long getVersion()           { return _version; }
    long getChangeset()         { return _changeset; }
    long getUid()               { return _uid; }
    std::string getTimestamp()  { return _timestamp; }
//    std::string getUser()       { return _user; }
    bool isModified()           { return _modified; }
    
    
protected:
    
    Element(std::shared_ptr<AmigoCloud::Database> db, const std::string& id,
               const std::string& action, const std::string& version, const std::string& timestamp,
               const std::string& changeset, const std::string& uid, const std::string& user,
               const std::string& visible, const std::string& geometry);
    
    std::shared_ptr<AmigoCloud::Database> _db;
    
    long _id;
    long _version;
    long _changeset;
    long _uid;
    
    std::string _action;
    std::string _timestamp;
    std::string _user;
    std::string _visible;
    
    bool _closed = false;
    
    std::map<std::string,std::string> _tags;
    
    std::shared_ptr<geos::geom::Geometry> _geometry;
    
    // Element has been modified since fetched from OSM API.
    bool _modified = false;
    
    // Element modified in current session.
    bool _modifiedInInstance = false;
    
//    std::vector<Relation> _linkedRelations;

    
    
private:
    
    

};
    
}

#endif /* Element_hpp */
