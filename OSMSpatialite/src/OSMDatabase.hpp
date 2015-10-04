//
//  OSMDatabase.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef OSMDatabase_hpp
#define OSMDatabase_hpp

#include "AmigoCloud/Database.h"
#include "OSMElement.hpp"

namespace OSM {
    
class OSMDatabase {
public:
    explicit OSMDatabase(const std::string& dbPath);
    
    // OSM Element found at beginning of XML Document
    void addOSM(const std::string& version, const std::string& generator);
    
    void addNode(const std::string& idStr, const std::string& latStr, const std::string& lonStr, const std::string& versionStr,
                 const std::string& timestampStr, const std::string& changesetStr, const std::string& uidStr,
                 const std::string& userStr, const std::string& actionStr, const std::string& visibleStr);
    
    void addWay(const std::string& idStr, const std::string& versionStr,
                 const std::string& timestampStr, const std::string& changesetStr, const std::string& uidStr,
                 const std::string& userStr, const std::string& actionStr, const std::string& visibleStr);
    
    void addRelation(const std::string& idStr, const std::string& versionStr,
                const std::string& timestampStr, const std::string& changesetStr, const std::string& uidStr,
                const std::string& userStr, const std::string& actionStr, const std::string& visibleStr);
    
    void addTag(const ElementType parentElementType, const std::string& idStr,
                const std::string& kStr, const std::string& vStr);
    
    void addNd(const std::string& idStr, const std::string& refStr, unsigned int pos);
    
    void addMember(const std::string& relationIdStr, const std::string& refStr,
                   const std::string& typeStr, const std::string& roleStr);
    
private:
    // Initializes DB to have OSM Schema if needed.
    void _initDB();
    
    const std::string _dbPath;
    AmigoCloud::Database _db;
};
}

#endif /* OSMDatabase_hpp */
