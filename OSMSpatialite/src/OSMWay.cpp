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

std::vector<OSMWay> OSMWay::fetchWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

    std::string sql = "SELECT * FROM ways WHERE Intersects(polygon, BuildMbr(" + bbox + ")) AND polygon IS NOT NULL UNION ALL SELECT * FROM ways WHERE Intersects(line, BuildMbr(" + bbox + ")) AND line IS NOT NULL;";

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
                
                // Get Tags.
                std::shared_ptr<std::map<std::string,std::string>> tags = std::make_shared<std::map<std::string,std::string>>();
                std::string sqlT = "SELECT k, v FROM ways_tags WHERE id = " + id + ';';
                AmigoCloud::DatabaseResult resultT;
                db->executeSQL(sqlT.c_str(), resultT);
                if (resultT.isOK()) {
                    const std::vector< std::vector<std::string> > &recordsT = resultT.records;
                    if (recordsT.size() > 0) {
                        for ( auto const &recordT : recordsT) {
                            const std::string &k = recordT[0];
                            const std::string &v = recordT[1];
                            (*tags)[k] = v;
                        }
                    }
                }
                
            }
        }
    }
    
	return ways;
}

std::vector<OSMWay> OSMWay::fetchClosedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchOpenWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchModifiedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchModifiedClosedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchModifiedOpenWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchDeletedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchDeletedClosedWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchDeletedOpenWays(std::shared_ptr<AmigoCloud::Database> db, const std::string& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}


//    OSMWay::OSMWay() {
//        
//    }
}
