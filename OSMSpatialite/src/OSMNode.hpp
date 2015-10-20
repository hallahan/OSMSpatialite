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
    
    static std::vector<OSMNode> fetchNodes(AmigoCloud::Database db);
    static std::vector<OSMNode> fetchStandaloneNodes(AmigoCloud::Database db);
    static std::vector<OSMNode> fetchEditedNodes(AmigoCloud::Database db);
    static std::vector<OSMNode> fetchEditedStandaloneNodes(AmigoCloud::Database db);
    static std::vector<OSMNode> fetchDeletedNodes(AmigoCloud::Database db);
    static std::vector<OSMNode> fetchDeletedStandaloneNodes(AmigoCloud::Database db);

private:
    double _lat;
    double _lon;
    
};

}

#endif /* OSMNode_hpp */
