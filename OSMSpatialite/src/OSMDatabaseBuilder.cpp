//
//  OSMDatabase.cpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#include "OSMDatabaseBuilder.hpp"

#include <iostream>
#include "Util.h"

namespace OSM
{
    
    OSMDatabaseBuilder::OSMDatabaseBuilder(const std::string& dbPath) :
    _dbPath(dbPath),
    _db(dbPath, true) { // NH FIXME: Crash when enable spatial?
        _initDB();
    }
    
    void OSMDatabaseBuilder::_initDB() {
        _db.beginTransaction();
        
        // OSM XML Node (one per OSM XML file)
        _db.executeSQL("CREATE TABLE IF NOT EXISTS osm(version TEXT, generator TEXT);");
        
        // Nodes, Ways, Relations
        _db.executeSQL("CREATE TABLE IF NOT EXISTS nodes(id INTEGER PRIMARY KEY, action TEXT, lat REAL, lon REAL, version INTEGER, timestamp TEXT, changeset INTEGER, uid INTEGER, user TEXT, visible TEXT);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS ways(id INTEGER PRIMARY KEY, action TEXT, version INTEGER, timestamp TEXT, changeset INTEGER, uid INTEGER, user TEXT, visible TEXT, closed INTEGER);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS relations(id INTEGER PRIMARY KEY, action TEXT, version INTEGER, timestamp TEXT, changeset INTEGER, uid INTEGER, user TEXT, visible TEXT);");
        
        // Add geometry columns for Nodes and Ways.
        _db.executeSQL("SELECT AddGeometryColumn('nodes', 'point', 4326, 'POINT', 'XY')");
        _db.executeSQL("SELECT AddGeometryColumn('ways', 'line', 4326, 'LINESTRING', 'XY')");
        _db.executeSQL("SELECT AddGeometryColumn('ways', 'polygon', 4326, 'POLYGON', 'XY')");
        
        // Tag tables for each OSM element type
        _db.executeSQL("CREATE TABLE IF NOT EXISTS nodes_tags(id INTEGER, k TEXT, v TEXT);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS ways_tags(id INTEGER, k TEXT, v TEXT);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS relations_tags(id INTEGER, k TEXT, v TEXT);");
        
        // Member tables
        _db.executeSQL("CREATE TABLE IF NOT EXISTS ways_nodes(way_id  INTEGER, node_id INTEGER, way_pos INTEGER);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS relations_members(relation_id  INTEGER, ref INTEGER, type TEXT, role TEXT);");
        
        _db.commitTransaction();
    }
    
    void OSMDatabaseBuilder::addOSM(const std::string& version, const std::string& generator) {
        std::string sql = "INSERT INTO osm VALUES ('" + version + "', '" + generator + "');";
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabaseBuilder::addNode(const std::string& idStr, const std::string& latStr, const std::string& lonStr, const std::string& versionStr,
                              const std::string& timestampStr, const std::string& changesetStr, const std::string& uidStr,
                              const std::string& userStr, const std::string& actionStr, const std::string& visibleStr) {
        // If we are unable to get a valid value, we want to simply insert a NULL.
        std::string lat = "NULL";
        std::string lon = "NULL";
        std::string version = "NULL";
        std::string timestamp = "NULL";
        std::string changeset = "NULL";
        std::string uid = "NULL";
        std::string user = "NULL";
        std::string action = "NULL";
        std::string visible = "NULL";
        
        
        if (!Util::isLong(idStr)) {
            // need to have an id
            return;
        }
        if (Util::isDouble(latStr)) {
            lat = latStr;
        }
        if (Util::isDouble(lonStr)) {
            lon = lonStr;
        }
        if (Util::isLong(versionStr)) {
            version = versionStr;
        }
        if (!timestampStr.empty()) {
            // needs surrounding '' for string in insert statement
            timestamp = "'" + timestampStr + "'";
        }
        if (Util::isLong(changesetStr)) {
            changeset = changesetStr;
        }
        if (Util::isLong(uidStr)) {
            uid = uidStr;
        }
        if (!userStr.empty()) {
            user = "'" + userStr + "'";
        }
        if (!actionStr.empty()) {
            action = "'" + actionStr + "'";
        }
        if (!visibleStr.empty()) {
            visible = "'" + visibleStr + "'";
        }
        
        std::string sql = "INSERT OR REPLACE INTO nodes VALUES (" +
                            idStr + ',' + action + ',' + lat + ',' +
                            lon + ',' + version + ',' + timestamp + ',' +
                            changeset + ',' + uid + ',' + user + ',' + visible + ",NULL);";
        
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabaseBuilder::addWay(const std::string& idStr, const std::string& versionStr,
           const std::string& timestampStr, const std::string& changesetStr, const std::string& uidStr,
           const std::string& userStr, const std::string& actionStr, const std::string& visibleStr) {
        // If we are unable to get a valid value, we want to simply insert a NULL.
        std::string version = "NULL";
        std::string timestamp = "NULL";
        std::string changeset = "NULL";
        std::string uid = "NULL";
        std::string user = "NULL";
        std::string action = "NULL";
        std::string visible = "NULL";
        
        if (!Util::isLong(idStr)) {
            // need to have an id
            return;
        }
        if (Util::isLong(versionStr)) {
            version = versionStr;
        }
        if (!timestampStr.empty()) {
            // needs surrounding '' for string in insert statement
            timestamp = "'" + timestampStr + "'";
        }
        if (Util::isLong(changesetStr)) {
            changeset = changesetStr;
        }
        if (Util::isLong(uidStr)) {
            uid = uidStr;
        }
        if (!userStr.empty()) {
            user = "'" + userStr + "'";
        }
        if (!actionStr.empty()) {
            action = "'" + actionStr + "'";
        }
        if (!visibleStr.empty()) {
            visible = "'" + visibleStr + "'";
        }
        
        std::string sql = "INSERT OR REPLACE INTO ways VALUES (" +
                            idStr + ',' + action + ',' + version + ',' + timestamp + ',' +
                            changeset + ',' + uid + ',' + user + ',' + visible + ",NULL,NULL,NULL);";
        
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabaseBuilder::addRelation(const std::string& idStr, const std::string& versionStr,
                             const std::string& timestampStr, const std::string& changesetStr, const std::string& uidStr,
                             const std::string& userStr, const std::string& actionStr, const std::string& visibleStr) {
        // If we are unable to get a valid value, we want to simply insert a NULL.
        std::string version = "NULL";
        std::string timestamp = "NULL";
        std::string changeset = "NULL";
        std::string uid = "NULL";
        std::string user = "NULL";
        std::string action = "NULL";
        std::string visible = "NULL";
        
        
        if (!Util::isLong(idStr)) {
            // need to have an id
            return;
        }
        if (Util::isLong(versionStr)) {
            version = versionStr;
        }
        if (!timestampStr.empty()) {
            // needs surrounding '' for string in insert statement
            timestamp = "'" + timestampStr + "'";
        }
        if (Util::isLong(changesetStr)) {
            changeset = changesetStr;
        }
        if (Util::isLong(uidStr)) {
            uid = uidStr;
        }
        if (!userStr.empty()) {
            user = "'" + userStr + "'";
        }
        if (!actionStr.empty()) {
            action = "'" + actionStr + "'";
        }
        if (!visibleStr.empty()) {
            visible = "'" + visibleStr + "'";
        }
        
        std::string sql = "INSERT OR REPLACE INTO relations VALUES (" +
                            idStr + ',' + action + ',' + version + ',' + timestamp + ',' +
                            changeset + ',' + uid + ',' + user + ',' + visible + ");";
        
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabaseBuilder::addTag(const ElementType parentElementType, const std::string& idStr,
                             const std::string& kStr, const std::string& vStr, bool reset) {
        std::string k = "NULL";
        std::string v = "NULL";
        std::string sql;
        
        if (!Util::isLong(idStr)) {
            return;
        }
        if (!kStr.empty()) {
            k = "'" + kStr + "'";
        }
        if (!vStr.empty()) {
            v = "'" + vStr + "'";
        }
        
        std::string table;
        if (parentElementType == ElementType::NODE) {
            table = "nodes_tags";
        } else if (parentElementType == ElementType::WAY) {
            table = "ways_tags";
        } else {
            table = "relations_tags";
        }
        
        // We want to blast out the previous tags for the given element when doing an update.
        if (reset) {
            sql = "DELETE FROM " + table + " WHERE id = " + idStr + ';';
            _db.executeSQL(sql.c_str());
        }
        
        sql = "INSERT INTO " + table + " VALUES (" +
               idStr + ',' + k + ',' + v + ");";
        
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabaseBuilder::addNd(const std::string& idStr, const std::string& refStr, unsigned int pos) {
        std::string wayId = "NULL";
        std::string nodeId = "NULL";
        std::string sql;
        
        if (!Util::isLong(idStr)) {
            // need to have an id
            return;
        }
        if (!Util::isLong(refStr)) {
            // need to have an id
            return;
        }
        
        // We want to blast out the previous node_ways for the given way when doing an update.
        if (pos == 0) {
            sql = "DELETE FROM ways_nodes WHERE way_id = " + idStr + ';';
            _db.executeSQL(sql.c_str());
        }
        
        sql = "INSERT INTO ways_nodes VALUES (" +
               idStr + ',' + refStr + ',' + std::to_string(pos) + ");";
        
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabaseBuilder::addMember(const std::string& relationIdStr, const std::string& refStr,
                                const std::string& typeStr, const std::string& roleStr, bool reset) {
        std::string type = "NULL";
        std::string role = "NULL";
        std::string sql;
        
        if (!Util::isLong(relationIdStr)) {
            // need to have an id
            return;
        }
        if (!Util::isLong(refStr)) {
            // need to have an id
            return;
        }
        if (!typeStr.empty()) {
            type = "'" + typeStr + "'";
        }
        if (!roleStr.empty()) {
            role = "'" + roleStr + "'";
        }
        
        if (reset) {
            sql = "DELETE FROM relations_members WHERE relation_id = " + relationIdStr + ';';
            _db.executeSQL(sql.c_str());
        }
        
        sql = "INSERT INTO relations_members VALUES (" +
               relationIdStr + ',' + refStr + ',' + type + ',' + role + ");";
        
        _db.executeSQL(sql.c_str());
    }

    void OSMDatabaseBuilder::postProcess() {
        _addIndices();
        _buildGeometries();
    }
    
    void OSMDatabaseBuilder::_addIndices() {
        // Index OSM element's action so we can quickly query modified or deleted elements
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_nodes_action ON nodes (action);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_ways_action ON ways (action);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_relations_action ON relations (action);");
        
        // Index on ways closed. Closed ways are polygons. Open ways are lines.
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_ways_closed ON ways (closed);");
        
        // Indices for tag tables
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_nodes_tags_id ON nodes_tags (id);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_ways_tags_id ON ways_tags (id);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_relations_tags_id ON relations_tags (id);");
        // Indices on k and v will be useful for typeahead.
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_nodes_tags_k ON nodes_tags (k);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_ways_tags_k ON ways_tags (k);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_relations_tags_k ON relations_tags (k);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_nodes_tags_v ON nodes_tags (v);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_ways_tags_v ON ways_tags (v);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_relations_tags_v ON relations_tags (v);");
        
        // Indices for member tables
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_ways_nodes_way_id ON ways_nodes (way_id);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_relations_members_relation_id ON relations_members (relation_id);");
    }
    
    void OSMDatabaseBuilder::_buildGeometries() {
        _buildStandaloneNodeGeometries();
        _buildWayGeometries();
    }
    
    void OSMDatabaseBuilder::_buildStandaloneNodeGeometries() {
        // Fetch Standalone Nodes (Nodes not in a Way).
        // This gets the ids of the nodes not in a way.
        std::string sql = "SELECT nodes.id, nodes.lat, nodes.lon FROM nodes LEFT OUTER JOIN ways_nodes ON nodes.id = ways_nodes.node_id WHERE ways_nodes.node_id IS NULL;";
        
        AmigoCloud::DatabaseResult result;
        _db.executeSQL(sql.c_str(), result);
        if (result.isOK()) {
            const std::vector< std::vector<std::string> > &records = result.records;
            if (records.size() > 0) {
                for ( auto const &record : records) {
                    _buildNodeGeometry(record[0], record[1], record[2]);
                }
            }
        }
        
    }
    
    void OSMDatabaseBuilder::_buildNodeGeometry(const std::string& nodeIdStr, const std::string& nodeLatStr, const std::string& nodeLonStr) {
        std::string sql = "UPDATE nodes SET point = GeomFromText('POINT(" + nodeLonStr + " " + nodeLatStr + ")', 4326) WHERE nodes.id = " + nodeIdStr + ";";
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabaseBuilder::_buildWayGeometries() {
        std::string sql = "SELECT id FROM ways;";
        
        AmigoCloud::DatabaseResult result;
        _db.executeSQL(sql.c_str(), result);
        if (result.isOK()) {
            const std::vector< std::vector<std::string> > &records = result.records;
            if (records.size() > 0) {
                
                // For each way, we need to check to see if we have the same number off
                // corresponding nodes to nds. If that count is not the same, we know
                // that we don't have all of the nodes needed to construct a way geom.
                for (auto const &record : records) {
                    _checkNodeCountForWay(record[0]); // wayId
                }
                
            }
        }
    }
    
    void OSMDatabaseBuilder::_checkNodeCountForWay(const std::string& wayId) {
        std::string sql = "SELECT COUNT(node_id) FROM ways_nodes WHERE way_id = " + wayId + ";";
        
        AmigoCloud::DatabaseResult result;
        _db.executeSQL(sql.c_str(), result);
        if (result.isOK()) {
            const std::vector< std::vector<std::string> > &records = result.records;
            if (records.size() > 0) {
                
                const std::string& countStr = records[0][0];
                long count = std::stol(countStr);
                
                sql = "SELECT lat, lon  FROM ways_nodes JOIN nodes ON ways_nodes.node_id = nodes.id WHERE way_id = " + wayId + " ORDER BY way_pos;";
                
                AmigoCloud::DatabaseResult result;
                _db.executeSQL(sql.c_str(), result);
                if (result.isOK()) {
                    const std::vector< std::vector<std::string> > &records = result.records;
                    
                    // Checking to see that the join has the same count as the number
                    // of nodes in ways_nodes for the given way.
                    if (count > 1 && records.size() == count) {

                        _createAndInsertWayGeometry(wayId, records);
                        
                    }
                }
            }
        }
    }
    
    
    std::string createWKTPolygon(const std::vector< std::vector<std::string> >& latLons) {
        std::string wkt = "POLYGON((" + latLons[0][1] + " " + latLons[0][0];
        std::size_t size = latLons.size();
        for (long i = 1; i < size; ++i) {
            const std::vector<std::string>& latLon = latLons[i];
            wkt += ", " + latLon[1] + " " + latLon[0];
        }
        return wkt += "))";
    }
    
    std::string createWKTLineString(const std::vector< std::vector<std::string> >& latLons) {
        std::string wkt = "LINESTRING(" + latLons[0][1] + " " + latLons[0][0];
        std::size_t size = latLons.size();
        for (long i = 1; i < size; ++i) {
            const std::vector<std::string>& latLon = latLons[i];
            wkt += ", " + latLon[1] + " " + latLon[0];
        }
        return wkt += ")";
    }
    
    void OSMDatabaseBuilder::_createAndInsertWayGeometry(const std::string& wayId,
                                                  const std::vector< std::vector<std::string> >& latLons) {
        const std::string& firstLat = latLons[0][0];
        const std::string& firstLon = latLons[0][1];
        const std::string& lastLat = latLons[latLons.size()-1][0];
        const std::string& lastLon = latLons[latLons.size()-1][1];
        
        // if it is a polygon
        if (firstLat == lastLat && firstLon == lastLon) {
            std::string wktPolygon = createWKTPolygon(latLons);
            std::string sql = "UPDATE ways SET polygon = GeomFromText('" + wktPolygon + "', 4326), closed = 1 WHERE id = " + wayId + ";";
            _db.executeSQL(sql.c_str());
        }
        // if it is a polyline (linestring)
        else {
            std::string wktLineString = createWKTLineString(latLons);
            std::string sql = "UPDATE ways SET line = GeomFromText('" + wktLineString + "', 4326), closed = 0 WHERE id = " + wayId + ";";
            _db.executeSQL(sql.c_str());
        }
    }
    
    
}
