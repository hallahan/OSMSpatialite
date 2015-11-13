//
//  Way.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef Way_hpp
#define Way_hpp

#include <vector>
#include <OSMSpatialite/Element.hpp>
#include <OSMSpatialite/Node.hpp>

namespace OSM {
    
class Way : public Element {
public:

    static std::vector<Way> fetchWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<Way> fetchClosedWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<Way> fetchOpenWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<Way> fetchModifiedWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<Way> fetchModifiedClosedWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<Way> fetchModifiedOpenWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<Way> fetchDeletedWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<Way> fetchDeletedClosedWays(std::shared_ptr<AmigoCloud::Database> db);
    static std::vector<Way> fetchDeletedOpenWays(std::shared_ptr<AmigoCloud::Database> db);
    
    static std::vector<Way> fetchWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<Way> fetchClosedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<Way> fetchOpenWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<Way> fetchModifiedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<Way> fetchModifiedClosedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<Way> fetchModifiedOpenWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<Way> fetchDeletedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<Way> fetchDeletedClosedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    static std::vector<Way> fetchDeletedOpenWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox);
    
    
    bool isClosed() { return _closed; };
    
private:
    
    static void _wayQuery(std::shared_ptr<AmigoCloud::Database> db, std::vector<Way>& ways, const std::string& sql);
    
    Way(std::shared_ptr<AmigoCloud::Database> db, const std::string& id,
           const std::string& action, const std::string& version, const std::string& timestamp,
           const std::string& changeset, const std::string& uid, const std::string& user,
           const std::string& visible, bool closed, const std::string& geometry);
    
    bool _closed;
    std::vector<Node> _linkedNodes;
    
    
};
    
}

#endif /* Way_hpp */
