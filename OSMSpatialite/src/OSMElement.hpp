//
//  OSMElement.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef OSMElement_hpp
#define OSMElement_hpp

#include <map>
#include <vector>

namespace OSM {

// Used to differentiate between the various types of OSM Elements.
enum class ElementType { NODE, WAY, RELATION, TAG, ND, MEMBER, BOUNDS, OSM };

// Forward declaring OSMRelation so we can reference it within OSMElement.
class OSMRelation;
    
class OSMElement {
public:
    
    // Getters
    long getId()                { return _id; }
    long getVersion()           { return _version; }
    long getChangeset()         { return _changeset; }
    long getUid()               { return _uid; }
//    std::string getTimestamp()  { return _timestamp; }
//    std::string getUser()       { return _user; }
    bool isModified()           { return _modified; }
    
    
protected:
    long _id;
    long _version;
    long _changeset;
    long _uid;
//    std::string _timestamp;
//    std::string _user;
    
    // Element has been modified since fetched from OSM API.
    bool _modified = false;
    
    // Element modified in current session.
    bool _modifiedInInstance = false;
    
//    geos::geometry _geom;
    
    std::map<std::string, std::string> _tags;
    std::vector<OSMRelation> _linkedRelations;

    
private:
    
};
    
}

#endif /* OSMElement_hpp */
