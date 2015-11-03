//
//  OSMNode.cpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#include "OSMNode.hpp"

namespace OSM {
    
/**
 * STATIC MEMBER FUNCTIONS
 */

void OSMNode::_nodeQuery(std::shared_ptr<AmigoCloud::Database> db, std::vector<OSMNode>& nodes, const std::string& sql) {
    AmigoCloud::DatabaseResult result;
    db->executeSQL(sql.c_str(), result);
    if (result.isOK()) {
        const std::vector< std::vector<std::string> > &records = result.records;
        if (records.size() > 0) {
            for ( auto const &record : records) {
                
//                const std::string &id = record[0];
//                const std::string &action = record[1];
//                const std::string &version = record[2];
//                const std::string &timestamp = record[3];
//                const std::string &changeset = record[4];
//                const std::string &uid = record[5];
//                const std::string &user = record[6];
//                const std::string &visible = record[7];
//                const std::string &point = record[8];
//                
//                bool standaloneBool = (point != "NULL");
//                
//                OSMNode n(db, id, action, version, timestamp, changeset, uid,
//                         user, visible, standaloneBool, point );
//                nodes.push_back(n);
                
            }
        }
    }
}

std::vector<OSMNode> OSMNode::fetchNodesForWayId(std::shared_ptr<AmigoCloud::Database> db, const long wayId) {
	std::vector<OSMNode> nodes;
    
    const std::string sql = "SELECT id, action, lat, lon, version, timestamp, changeset, uid, user, visible, way_pos FROM nodes JOIN ways_nodes ON nodes.id = ways_nodes.node_id WHERE ways_nodes.way_id = " + std::to_string(wayId) + ";";
    
    AmigoCloud::DatabaseResult result;
    db->executeSQL(sql.c_str(), result);
    if (result.isOK()) {
        const std::vector< std::vector<std::string> > &records = result.records;
        if (records.size() > 0) {
            for ( auto const &record : records) {
                
                const std::string &id = record[0];
                const std::string &action = record[1];
                const std::string &latStr = record[2];
                const std::string &lonStr = record[3];
                const std::string &version = record[4];
                const std::string &timestamp = record[5];
                const std::string &changeset = record[6];
                const std::string &uid = record[7];
                const std::string &user = record[8];
                const std::string &visible = record[9];
                const std::string &wayPosStr = record[10];
                
                long lat;
                long lon;
                unsigned int wayPos;
                
                try {
                    lat = std::stol(latStr);
                } catch (...) {
                    continue;
                }
                
                try {
                    lon = std::stol(lonStr);
                } catch (...) {
                    continue;
                }
                
                try {
                    wayPos = std::stoi(wayPosStr);
                } catch (...) {
                    continue;
                }
                
                OSMNode n(db, id, action, lat, lon, version, timestamp, changeset, uid,
                          user, visible, false, "" );
                nodes.insert(nodes.begin() + wayPos, n);
            }
        }
    }

	return nodes;
}

std::vector<OSMNode> OSMNode::fetchStandaloneNodes(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMNode> nodes;

	return nodes;
}

std::vector<OSMNode> OSMNode::fetchModifiedStandaloneNodes(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMNode> nodes;

	return nodes;
}

std::vector<OSMNode> OSMNode::fetchDeletedStandaloneNodes(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMNode> nodes;

	return nodes;
}

OSMNode::OSMNode(std::shared_ptr<AmigoCloud::Database> db, const std::string& id, const std::string& action,
               long lat, long lon, const std::string& version, const std::string& timestamp,
               const std::string& changeset, const std::string& uid, const std::string& user,
               const std::string& visible, bool standalone, const std::string& geometry) :
OSMElement(db, id, action, version, timestamp, changeset, uid, user, visible, geometry),
_lat(lat),
_lon(lon),
_standalone(standalone) {
    
    // Get Tags.
    std::string sqlT = "SELECT k, v FROM nodes_tags WHERE id = " + id + ';';
    AmigoCloud::DatabaseResult resultT;
    db->executeSQL(sqlT.c_str(), resultT);
    if (resultT.isOK()) {
        const std::vector< std::vector<std::string> > &recordsT = resultT.records;
        if (recordsT.size() > 0) {
            for ( auto const &recordT : recordsT) {
                const std::string &k = recordT[0];
                const std::string &v = recordT[1];
                _tags[k] = v;
            }
        }
    }
    
}
    
}