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

    static std::vector<OSMWay> fetchWays(AmigoCloud::Database& db);
    static std::vector<OSMWay> fetchClosedWays(AmigoCloud::Database& db);
    static std::vector<OSMWay> fetchOpenWays(AmigoCloud::Database& db);
    static std::vector<OSMWay> fetchModifiedWays(AmigoCloud::Database& db);
    static std::vector<OSMWay> fetchModifiedClosedWays(AmigoCloud::Database& db);
    static std::vector<OSMWay> fetchModifiedOpenWays(AmigoCloud::Database& db);
    static std::vector<OSMWay> fetchDeletedWays(AmigoCloud::Database& db);
    static std::vector<OSMWay> fetchDeletedClosedWays(AmigoCloud::Database& db);
    static std::vector<OSMWay> fetchDeletedOpenWays(AmigoCloud::Database& db);
    
    static std::vector<OSMWay> fetchWays(AmigoCloud::Database& db, const std::string& bbox);
    static std::vector<OSMWay> fetchClosedWays(AmigoCloud::Database& db, const std::string& bbox);
    static std::vector<OSMWay> fetchOpenWays(AmigoCloud::Database& db, const std::string& bbox);
    static std::vector<OSMWay> fetchModifiedWays(AmigoCloud::Database& db, const std::string& bbox);
    static std::vector<OSMWay> fetchModifiedClosedWays(AmigoCloud::Database& db, const std::string& bbox);
    static std::vector<OSMWay> fetchModifiedOpenWays(AmigoCloud::Database& db, const std::string& bbox);
    static std::vector<OSMWay> fetchDeletedWays(AmigoCloud::Database& db, const std::string& bbox);
    static std::vector<OSMWay> fetchDeletedClosedWays(AmigoCloud::Database& db, const std::string& bbox);
    static std::vector<OSMWay> fetchDeletedOpenWays(AmigoCloud::Database& db, const std::string& bbox);
    
    
    
    bool isClosed() { return _closed; };
    
private:
    
//    OSMWay(const std::string& id, const std::string& action, const std::string& version, const std::string& changeset,
//           const std::string& uid, const std::string& user, const std::string& visible, const std::string& closed,
//           );
    
    bool _closed = false;
    std::vector<OSMNode> _linkedNodes;
    
    
};
    
}

#endif /* OSMWay_hpp */
