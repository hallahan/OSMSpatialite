#include "catch.hpp"

// AmigoCloud Includes
#include <core/Database.h>
#include <core/RestClient.h>

#include <OSMSpatialite/Util.hpp>
#include <OSMSpatialite/XmlParser.hpp>
#include <OSMSpatialite/Way.hpp>

void queryAssert(std::shared_ptr<AmigoCloud::Database> db, const std::string& query, const std::string& equalsNum) {
    AmigoCloud::DatabaseResult r;
    db->executeSQL(query.c_str(), r);
    if (r.isOK()) {
        const std::vector< std::vector<std::string> > &records = r.records;
        if (records.size() > 0) {
            REQUIRE(records[0][0] == equalsNum);
        } else {
            FAIL(query + " returned no records");
        }
    } else {
        FAIL(query + " returned a bad result.");
    }
}

TEST_CASE("Building a database with baseline arcade creek OSM data.", "[build_db]") {
    std::string dbPath = OSM::Util::cwd() + "/test/tmp/arcade.db";
    std::shared_ptr<AmigoCloud::Database> db = std::make_shared<AmigoCloud::Database>(dbPath, true);
    OSM::XmlParser parser(db);
    parser.xmlFile(OSM::Util::cwd() + "/test/data/arcade.osm").parse();
    
    SECTION("db has the correct number of nodes, ways, relations") {
        queryAssert(db, "SELECT COUNT(id) FROM nodes;", "213");
        queryAssert(db, "SELECT COUNT(id) FROM ways;", "27");
        queryAssert(db, "SELECT COUNT(id) FROM relations;", "0");
        queryAssert(db, "SELECT COUNT(way_id) FROM ways_nodes;", "246");
    }
    
    
    SECTION("ways table has the correct number of lines and polygons") {
        queryAssert(db, "SELECT COUNT(line) FROM ways WHERE line IS NOT NULL", "9");
        queryAssert(db, "SELECT COUNT(polygon) FROM ways WHERE polygon IS NOT NULL", "18");
    }
}



/*

std::shared_ptr<AmigoCloud::Database> db2 = std::make_shared<AmigoCloud::Database>("/temp/arcade.db", true);
OSM::XmlParser parser2(db2);
parser2.xmlFile("/temp/arcade.osm").parse();

//    OSM::Way::fetchWays(db2, "-121.369328,38.649437,-121.369215,38.649527");
//
//    std::vector<OSM::Node> nodes = OSM::Node::fetchNodesForWayId(db2, 349945505);
//
//    std::shared_ptr<AmigoCloud::Database> db3 = std::make_shared<AmigoCloud::Database>("/temp/fbi.db", true);
//    OSM::XmlParser parser3(db3);
//    parser3.xmlFile("/temp/fbi.osm").parse();
//    std::vector<OSM::Node> nodes3 = OSM::Node::fetchStandaloneNodes(db3, "-121.381330,38.634573,-121.350946,38.651735");
//
//    AmigoCloud::RestClient restClient("OSMSpatialite/0.0.1");
//    AmigoCloud::RestClient::response response = restClient.get("http://www.openstreetmap.org/api/0.6/map?bbox=-121.0359907102686,39.24774208062821,-121.00122928619385,39.27266344858914");
//
//    OSM::EditingAPI api(db3);
//
//    api.get("-121.369317,38.649377,-121.369202,38.649533");

return 0;
*/
