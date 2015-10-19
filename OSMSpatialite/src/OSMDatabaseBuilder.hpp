//
//  OSMDatabaseBuilder.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef OSMDatabaseBuilder_hpp
#define OSMDatabaseBuilder_hpp

#include <array>
#include "AmigoCloud/Database.h"
#include "OSMElement.hpp"
#include "OSMNode.hpp"
#include "OSMWay.hpp"
#include "OSMRelation.hpp"

namespace OSM {
    
class OSMDatabaseBuilder {
public:
    explicit OSMDatabaseBuilder(const AmigoCloud::Database& db);
    explicit OSMDatabaseBuilder(const std::string& dbPath);
    
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
                const std::string& kStr, const std::string& vStr, bool reset);
    
    void addNd(const std::string& idStr, const std::string& refStr, unsigned int pos);
    
    void addMember(const std::string& relationIdStr, const std::string& refStr,
                   const std::string& typeStr, const std::string& roleStr, bool reset);
    
    // To be called after update / insert of OSM elements.
    // Creates indices and WKB geometries.
    void postProcess();
    
    
private:
    // Initializes DB to have OSM Schema if needed.
    void _initDB();
    void _addIndices();
    void _buildGeometries();
    void _buildStandaloneNodeGeometries();
    void _buildNodeGeometry(const std::string& nodeIdStr, const std::string& nodeLatStr, const std::string& nodeLonStr);
    void _buildWayGeometries();
    void _checkNodeCountForWay(const std::string& wayId);
    void _createAndInsertWayGeometry(const std::string& wayId, const std::vector< std::vector<std::string> >& latLons);
    
    AmigoCloud::Database _db;
};
}

#endif /* OSMDatabaseBuilder_hpp */
