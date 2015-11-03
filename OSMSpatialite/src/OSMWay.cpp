//
//  OSMWay.cpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#include "OSMWay.hpp"

namespace OSM {

/**
 * STATIC MEMBER FUNCTIONS
 */

void OSMWay::_wayQuery(std::shared_ptr<AmigoCloud::Database> db, std::vector<OSMWay>& ways, const std::string& sql) {
    AmigoCloud::DatabaseResult result;
    db->executeSQL(sql.c_str(), result);
    if (result.isOK()) {
        const std::vector< std::vector<std::string> > &records = result.records;
        if (records.size() > 0) {
            for ( auto const &record : records) {
                
                const std::string &id = record[0];
                const std::string &action = record[1];
                const std::string &version = record[2];
                const std::string &timestamp = record[3];
                const std::string &changeset = record[4];
                const std::string &uid = record[5];
                const std::string &user = record[6];
                const std::string &visible = record[7];
                const std::string &closed = record[8];
                const std::string &line = record[9];
                const std::string &polygon = record[10];
                
                bool closedBool = (closed == "1");
                
                OSMWay w(db, id, action, version, timestamp, changeset, uid,
                         user, visible, closedBool, (closedBool ? polygon : line) );
                ways.push_back(w);
                
            }
        }
    }
}

std::vector<OSMWay> OSMWay::fetchWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

    const std::string sql = "SELECT id, action, version, timestamp, changeset, uid, user, visible, closed, AsBinary(line), AsBinary(polygon) FROM ways WHERE Intersects(polygon, BuildMbr(" + bbox + ")) AND closed = 1 UNION ALL SELECT id, action, version, timestamp, changeset, uid, user, visible, closed, AsBinary(line), AsBinary(polygon) FROM ways WHERE Intersects(line, BuildMbr(" + bbox + ")) AND closed = 0;";

    _wayQuery(db, ways, sql);
    
	return ways;
}

std::vector<OSMWay> OSMWay::fetchClosedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

    const std::string sql = "SELECT id, action, version, timestamp, changeset, uid, user, visible, closed, AsBinary(line), AsBinary(polygon) FROM ways WHERE Intersects(polygon, BuildMbr(" + bbox + ")) AND closed = 1;";
    
    _wayQuery(db, ways, sql);
    
	return ways;
}

std::vector<OSMWay> OSMWay::fetchOpenWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

    const std::string sql = "SELECT id, action, version, timestamp, changeset, uid, user, visible, closed, AsBinary(line), AsBinary(polygon) FROM ways WHERE Intersects(line, BuildMbr(" + bbox + ")) AND closed = 0;";
    
    _wayQuery(db, ways, sql);
    
	return ways;
}

std::vector<OSMWay> OSMWay::fetchModifiedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

    const std::string sql = "SELECT id, action, version, timestamp, changeset, uid, user, visible, closed, AsBinary(line), AsBinary(polygon) FROM ways WHERE Intersects(polygon, BuildMbr(" + bbox + ")) AND closed = 1 AND action = 'modify' UNION ALL SELECT id, action, version, timestamp, changeset, uid, user, visible, closed, AsBinary(line), AsBinary(polygon) FROM ways WHERE Intersects(line, BuildMbr(" + bbox + ")) AND closed = 0 AND action = 'modify';";
    
    _wayQuery(db, ways, sql);
    
	return ways;
}

std::vector<OSMWay> OSMWay::fetchModifiedClosedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

    const std::string sql = "SELECT id, action, version, timestamp, changeset, uid, user, visible, closed, AsBinary(line), AsBinary(polygon) FROM ways WHERE Intersects(polygon, BuildMbr(" + bbox + ")) AND closed = 1 AND action = 'modify';";
    
    _wayQuery(db, ways, sql);
    
	return ways;
}

std::vector<OSMWay> OSMWay::fetchModifiedOpenWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;
    
    const std::string sql = "SELECT id, action, version, timestamp, changeset, uid, user, visible, closed, AsBinary(line), AsBinary(polygon) FROM ways WHERE Intersects(polygon, BuildMbr(" + bbox + ")) AND closed = 0 AND action = 'modify';";
    
    _wayQuery(db, ways, sql);

	return ways;
}

std::vector<OSMWay> OSMWay::fetchDeletedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

    const std::string sql = "SELECT id, action, version, timestamp, changeset, uid, user, visible, closed, AsBinary(line), AsBinary(polygon) FROM ways WHERE Intersects(polygon, BuildMbr(" + bbox + ")) AND closed = 1 AND action = 'delete' UNION ALL SELECT id, action, version, timestamp, changeset, uid, user, visible, closed, AsBinary(line), AsBinary(polygon) FROM ways WHERE Intersects(line, BuildMbr(" + bbox + ")) AND closed = 0 AND action = 'delete';";
    
    _wayQuery(db, ways, sql);
    
	return ways;
}

std::vector<OSMWay> OSMWay::fetchDeletedClosedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;
    
    const std::string sql = "SELECT id, action, version, timestamp, changeset, uid, user, visible, closed, AsBinary(line), AsBinary(polygon) FROM ways WHERE Intersects(polygon, BuildMbr(" + bbox + ")) AND closed = 1 AND action = 'delete';";
    
    _wayQuery(db, ways, sql);

	return ways;
}

std::vector<OSMWay> OSMWay::fetchDeletedOpenWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;
    
    const std::string sql = "SELECT id, action, version, timestamp, changeset, uid, user, visible, closed, AsBinary(line), AsBinary(polygon) FROM ways WHERE Intersects(polygon, BuildMbr(" + bbox + ")) AND closed = 0 AND action = 'delete';";
    
    _wayQuery(db, ways, sql);

	return ways;
}
    
    
OSMWay::OSMWay(std::shared_ptr<AmigoCloud::Database> db, const std::string& id,
               const std::string& action, const std::string& version, const std::string& timestamp,
               const std::string& changeset, const std::string& uid, const std::string& user,
               const std::string& visible, bool closed, const std::string& geometry) :
OSMElement(db, id, action, version, timestamp, changeset, uid, user, visible, geometry),
_closed(closed) {
    
    // Get Tags.
    std::string sqlT = "SELECT k, v FROM ways_tags WHERE id = " + id + ';';
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
