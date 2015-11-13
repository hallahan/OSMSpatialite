//
//  EditingAPI.cpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 11/4/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#include <OSMSpatialite/EditingAPI.hpp>

namespace OSM {

EditingAPI::EditingAPI(const std::shared_ptr<AmigoCloud::Database> db, const std::string& baseUrl) :
RestAPI(db, baseUrl) {
    
}

void EditingAPI::get(const std::string &bbox) {
    AmigoCloud::RestClient::response rsp = _restClient.get(_baseUrl + "map?bbox=" + bbox);
    if (rsp.isBad()) {
        // TODO: Do something about this...
        return;
    }
    std::string& osmXml = rsp.body;
    OSM::XmlParser parser(_db);
    parser.xmlString(osmXml).parse();
}
    
}