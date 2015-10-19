//
//  OSMWay.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef OSMWay_hpp
#define OSMWay_hpp

#include <vector>
#include "OSMElement.hpp"
#include "OSMNode.hpp"

namespace OSM {
    
class OSMWay : public OSMElement {
public:
    
    bool isClosed() { return _closed; };
    
private:
    
    bool _closed = false;
    std::vector<OSMNode> _linkedNodes;
    
    
};
    
}

#endif /* OSMWay_hpp */
