//
//  OSMElement.cpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#include "OSMElement.hpp"

namespace OSM {
    
OSMElement::OSMElement(	const std::string& id, const std::string& action, 
						const std::string& version, const std::string& changeset,
           				const std::string& uid, const std::string& user, 
           				const std::string& visible, const std::string& closed) :
_id(std::stol(id)),
_action(action),
_version(std::stol(version)),
_changeset(std::stol(changeset)),
_uid(std::stol(uid)),
_user(user),
_visible(visible),
_closed(closed == "1") {

}
    
    

}