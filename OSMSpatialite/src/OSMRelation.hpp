//
//  OSMRelation.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 theoutpost. All rights reserved.
//

#ifndef OSMRelation_hpp
#define OSMRelation_hpp

#include "OSMElement.hpp"

namespace OSM {
    
class OSMRelation : public OSMElement {
public:
    
    static std::vector<OSMRelation> fetchRelations(AmigoCloud::Database db);
    static std::vector<OSMRelation> fetchModifiedRelations(AmigoCloud::Database db);
    static std::vector<OSMRelation> fetchDeletedRelations(AmigoCloud::Database db);
    
private:

};
    
}

#endif /* OSMRelation_hpp */
