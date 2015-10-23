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
    
    /**
     * These static member functions fetch specific ways from a supplied SQLite database.
     * If you want all of these Ways from the database, pass a nullptr to bbox. If you
     * want only ways within a given bounding box, provide one.
     */
    
    static std::vector<OSMWay> fetchWays(AmigoCloud::Database& db);
    static std::vector<OSMWay> fetchClosedWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox);
    static std::vector<OSMWay> fetchOpenWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox);
    static std::vector<OSMWay> fetchModifiedWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox);
    static std::vector<OSMWay> fetchModifiedClosedWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox);
    static std::vector<OSMWay> fetchModifiedOpenWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox);
    static std::vector<OSMWay> fetchDeletedWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox);
    static std::vector<OSMWay> fetchDeletedClosedWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox);
    static std::vector<OSMWay> fetchDeletedOpenWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox);
    
    bool isClosed() { return _closed; };
    
private:
    
    bool _closed = false;
    std::vector<OSMNode> _linkedNodes;
    
    
};
    
}

#endif /* OSMWay_hpp */
