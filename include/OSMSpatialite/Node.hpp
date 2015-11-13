//
//  Node.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <vector>
#include <OSMSpatialite/Element.hpp>

namespace OSM {
    
class Node : public Element {
public:
    
    static std::vector<Node> fetchNodesForWayId(std::shared_ptr<AmigoCloud::Database> db, const long wayId);
    
    static std::vector<Node> fetchStandaloneNodes(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<Node> fetchModifiedStandaloneNodes(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<Node> fetchDeletedStandaloneNodes(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);

private:
    
    static void _nodeQuery(std::shared_ptr<AmigoCloud::Database> db, std::vector<Node>& nodes, const std::string& sql);
    
    Node(std::shared_ptr<AmigoCloud::Database> db, const std::string& id, const std::string& action,
            long lat, long lon, const std::string& version, const std::string& timestamp,
            const std::string& changeset, const std::string& uid, const std::string& user,
            const std::string& visible, bool standalone, const std::string& geometry);
    
    double _lat;
    double _lon;
    
    bool _standalone;
    
};

}

#endif /* Node_hpp */
