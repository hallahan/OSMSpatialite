//
//  OSMDatabase.cpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#include "OSMDatabase.hpp"

#include <iostream>

namespace OSM
{
    
    OSMDatabase::OSMDatabase(const std::string& dbPath) :
    _dbPath(dbPath),
    _db(dbPath, false)
    {
        _initDB();
    }
    
    void OSMDatabase::_initDB()
    {
        _db.beginTransaction();
        
        // OSM XML Node (one per OSM XML file)
        _db.executeSQL("CREATE TABLE IF NOT EXISTS osm(version TEXT, generator TEXT);");
        
        // Nodes, Ways, Relations
        _db.executeSQL("CREATE TABLE IF NOT EXISTS nodes(id INTEGER PRIMARY KEY, action TEXT, lat REAL, lon REAL, version INTEGER, timestamp TEXT, changeset INTEGER, uid INTEGER, user TEXT, visible TEXT);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS ways(id INTEGER PRIMARY KEY, action TEXT, version INTEGER, timestamp TEXT, changeset INTEGER, uid INTEGER, user TEXT, visible TEXT);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS relations(id INTEGER PRIMARY KEY, action TEXT, version INTEGER, timestamp TEXT, changeset INTEGER, uid INTEGER, user TEXT, visible TEXT);");
        
        // Index their action so we can quickly query modified or deleted elements
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_nodes_action ON nodes (action);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_ways_action ON ways (action);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_relations_action ON relations (action);");
        
        // Tag tables for each OSM element type
        _db.executeSQL("CREATE TABLE IF NOT EXISTS nodes_tags(id INTEGER, k TEXT, v TEXT);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS ways_tags(id INTEGER, k TEXT, v TEXT);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS relations_tags(id INTEGER, k TEXT, v TEXT);");
        
        // Indices for tag tables
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_nodes_tags_id ON nodes_tags (id);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_ways_tags_id ON ways_tags (id);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_relations_tags_id ON relations_tags (id);");
        
        // Member tables
        _db.executeSQL("CREATE TABLE IF NOT EXISTS ways_nodes(way_id  INTEGER, node_id INTEGER, way_pos INTEGER);");
        _db.executeSQL("CREATE TABLE IF NOT EXISTS relations_members(relation_id  INTEGER, ref INTEGER, type TEXT, role TEXT);");
        
        // Indices for member tables
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_ways_nodes_way_id ON ways_nodes (way_id);");
        _db.executeSQL("CREATE INDEX IF NOT EXISTS idx_relations_members_relation_id ON relations_members (relation_id);");
        
        _db.commitTransaction();
    }
    
    void OSMDatabase::addOSM(const std::string &version, const std::string &generator)
    {
        std::string sql = "INSERT INTO osm VALUES ('" + version + "', '" + generator + "');";
        _db.executeSQL(sql.c_str());
    }
    
    
}
