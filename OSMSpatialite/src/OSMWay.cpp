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

std::vector<OSMWay> OSMWay::fetchWays(AmigoCloud::Database& db) {
	std::vector<OSMWay> ways;

    std::string sql = "SELECT * FROM ways WHERE MbrIntersects(polygon, BuildMbr(-121.758728,38.529905,-121.492310,38.593797));";
//    std::string sql = "SELECT * FROM ways WHERE FilterMbrIntersects(-121.758728,38.529905,-121.492310,38.593797);";
    AmigoCloud::DatabaseResult result;
    db.executeSQL(sql.c_str(), result);
    if (result.isOK()) {
        const std::vector< std::vector<std::string> > &records = result.records;
        
        
    }
    
	return ways;
}

std::vector<OSMWay> OSMWay::fetchClosedWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchOpenWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchModifiedWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchModifiedClosedWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchModifiedOpenWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchDeletedWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchDeletedClosedWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

std::vector<OSMWay> OSMWay::fetchDeletedOpenWays(AmigoCloud::Database& db, const geos::geom::Envelope& bbox) {
	std::vector<OSMWay> ways;

	return ways;
}

    
}
