//
//  OSMDatabase.cpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#include "OSMDatabase.hpp"

#include <iostream>
#include "Util.h"

namespace OSM
{
    
    OSMDatabase::OSMDatabase(const std::string& dbPath) :
    _dbPath(dbPath),
    _db(dbPath, false) {
        _initDB();
    }
    
    void OSMDatabase::_initDB() {
        _db.beginTransaction();
        
        // OSM XML Node (one per OSM XML file)
        _db.executeSQL("CREATE TABLE IF NOT EXISTS osm(version TEXT, generator TEXT);");
        
        // Nodes, Ways, Relations
        _db.executeSQL("CREATE TABLE IF NOT EXISTS nodes(id INTEGER PRIMARY KEY, action TEXT, lat REAL, lon REAL, version INTEGER, timestamp TEXT, changeset INTEGER, uid INTEGER, user TEXT, visible TEXT, wkb_geometry GEOMETRY);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS ways(id INTEGER PRIMARY KEY, action TEXT, version INTEGER, timestamp TEXT, changeset INTEGER, uid INTEGER, user TEXT, visible TEXT, wkb_geometry GEOMETRY);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS relations(id INTEGER PRIMARY KEY, action TEXT, version INTEGER, timestamp TEXT, changeset INTEGER, uid INTEGER, user TEXT, visible TEXT);");
        
        // Tag tables for each OSM element type
        _db.executeSQL("CREATE TABLE IF NOT EXISTS nodes_tags(id INTEGER, k TEXT, v TEXT);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS ways_tags(id INTEGER, k TEXT, v TEXT);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS relations_tags(id INTEGER, k TEXT, v TEXT);");
        
        // Member tables
        _db.executeSQL("CREATE TABLE IF NOT EXISTS ways_nodes(way_id  INTEGER, node_id INTEGER, way_pos INTEGER);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS relations_members(relation_id  INTEGER, ref INTEGER, type TEXT, role TEXT);");
        
        _db.commitTransaction();
    }
    
    void OSMDatabase::addOSM(const std::string& version, const std::string& generator) {
        std::string sql = "INSERT INTO osm VALUES ('" + version + "', '" + generator + "');";
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabase::addNode(const std::string& idStr, const std::string& latStr, const std::string& lonStr, const std::string& versionStr,
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
        std::string wkb_geometry = "NULL";
        
        
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
        
        std::string sql = "INSERT INTO nodes VALUES (" +
                            idStr + ',' + action + ',' + lat + ',' +
                            lon + ',' + version + ',' + timestamp + ',' +
                            changeset + ',' + uid + ',' + user + ',' + visible + ',' + wkb_geometry + ");";
        
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabase::addWay(const std::string& idStr, const std::string& versionStr,
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
        std::string wkb_geometry = "NULL";
        
        
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
        
        std::string sql = "INSERT INTO ways VALUES (" +
                            idStr + ',' + action + ',' + version + ',' + timestamp + ',' +
                            changeset + ',' + uid + ',' + user + ',' + visible + ',' + wkb_geometry + ");";
        
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabase::addRelation(const std::string& idStr, const std::string& versionStr,
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
        
        std::string sql = "INSERT INTO relations VALUES (" +
                            idStr + ',' + action + ',' + version + ',' + timestamp + ',' +
                            changeset + ',' + uid + ',' + user + ',' + visible + ");";
        
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabase::addTag(const ElementType parentElementType, const std::string& idStr, const std::string& kStr, const std::string& vStr) {
        std::string k = "NULL";
        std::string v = "NULL";
        
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
        
        std::string sql = "INSERT INTO " + table + " VALUES (" +
                            idStr + ',' + k + ',' + v + ");";
        
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabase::addNd(const std::string& idStr, const std::string& refStr, unsigned int pos) {
        std::string wayId = "NULL";
        std::string nodeId = "NULL";
        
        if (!Util::isLong(idStr)) {
            // need to have an id
            return;
        }
        if (!Util::isLong(refStr)) {
            // need to have an id
            return;
        }
        
        std::string sql = "INSERT INTO ways_nodes VALUES (" +
                            idStr + ',' + refStr + ',' + std::to_string(pos) + ");";
        
        _db.executeSQL(sql.c_str());
    }
    
    void OSMDatabase::addMember(const std::string& relationIdStr, const std::string& refStr,
                                const std::string& typeStr, const std::string& roleStr) {
        std::string type = "NULL";
        std::string role = "NULL";
        
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
        
        std::string sql = "INSERT INTO relations_members VALUES (" +
                            relationIdStr + ',' + refStr + ',' + type + ',' + role + ");";
        
        _db.executeSQL(sql.c_str());
    }

    void OSMDatabase::postProcess() {
        _addIndices();
    }
    
    void OSMDatabase::_addIndices() {
        // Index OSM element's action so we can quickly query modified or deleted elements
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_nodes_action ON nodes (action);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_ways_action ON ways (action);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_relations_action ON relations (action);");
        
        // Indices for tag tables
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_nodes_tags_id ON nodes_tags (id);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_ways_tags_id ON ways_tags (id);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_relations_tags_id ON relations_tags (id);");
        // Indexes on k and v will be useful for typeahead.
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
 
}
