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

#include "OSMElement.hpp"

namespace OSM {
    
class OSMNode : public OSMElement {
public:
    
    
private:
    double _lat;
    double _lon;
    
};

}

#endif /* OSMNode_hpp */
