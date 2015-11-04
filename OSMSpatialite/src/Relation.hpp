//
//  Relation.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 theoutpost. All rights reserved.
//

#ifndef Relation_hpp
#define Relation_hpp

#include "Element.hpp"

namespace OSM {
    
class Relation : public Element {
public:
    
    static std::vector<Relation> fetchRelations(AmigoCloud::Database db);
    static std::vector<Relation> fetchModifiedRelations(AmigoCloud::Database db);
    static std::vector<Relation> fetchDeletedRelations(AmigoCloud::Database db);
    
private:

};
    
}

#endif /* Relation_hpp */
