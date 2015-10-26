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

    static std::vector<OSMWay> fetchWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<OSMWay> fetchClosedWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<OSMWay> fetchOpenWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<OSMWay> fetchModifiedWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<OSMWay> fetchModifiedClosedWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<OSMWay> fetchModifiedOpenWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<OSMWay> fetchDeletedWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<OSMWay> fetchDeletedClosedWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<OSMWay> fetchDeletedOpenWays(std::shared_ptr<AmigoCloud::Database> db);
    
    static std::vector<OSMWay> fetchWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<OSMWay> fetchClosedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<OSMWay> fetchOpenWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<OSMWay> fetchModifiedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<OSMWay> fetchModifiedClosedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<OSMWay> fetchModifiedOpenWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<OSMWay> fetchDeletedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<OSMWay> fetchDeletedClosedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<OSMWay> fetchDeletedOpenWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    
    
    
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
