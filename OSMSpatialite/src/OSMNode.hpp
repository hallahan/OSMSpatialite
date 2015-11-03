//
//  OSMNode.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef OSMNode_hpp
#define OSMNode_hpp

#include <stdio.h>
#include <vector>
#include "OSMElement.hpp"

namespace OSM {
    
class OSMNode : public OSMElement {
public:
    
    static std::vector<OSMNode> fetchNodesForWayId(std::shared_ptr<AmigoCloud::Database> db, const long wayId);
    
    static std::vector<OSMNode> fetchStandaloneNodes(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<OSMNode> fetchModifiedStandaloneNodes(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<OSMNode> fetchDeletedStandaloneNodes(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);

private:
    
    static void _nodeQuery(std::shared_ptr<AmigoCloud::Database> db, std::vector<OSMNode>& nodes, const std::string& sql);
    
    OSMNode(std::shared_ptr<AmigoCloud::Database> db, const std::string& id, const std::string& action,
            long lat, long lon, const std::string& version, const std::string& timestamp,
            const std::string& changeset, const std::string& uid, const std::string& user,
            const std::string& visible, bool standalone, const std::string& geometry);
    
    double _lat;
    double _lon;
    
    bool _standalone;
    
};

}

#endif /* OSMNode_hpp */
