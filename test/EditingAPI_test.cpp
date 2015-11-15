#include "catch.hpp"

// AmigoCloud Includes
#include <core/Database.h>
#include <core/RestClient.h>

// GEOS
#include <geos/geom/Geometry.h>

// OSMSpatialite
#include <OSMSpatialite/Util.hpp>
#include <OSMSpatialite/XmlParser.hpp>
#include <OSMSpatialite/Node.hpp>
#include <OSMSpatialite/Way.hpp>
#include <OSMSpatialite/EditingAPI.hpp>

TEST_CASE("Get OSM data from Editing API for American River College", "[EditingAPI][online_only]") {
    INFO("We are doing a GET request from the OSM Editing API 0.6 with a bbox around American River College.");
    
    std::string dbPath = OSM::Util::cwd() + "/test/tmp/american_river.db";
    std::shared_ptr<AmigoCloud::Database> db = std::make_shared<AmigoCloud::Database>(dbPath, true);
    
    OSM::EditingAPI api(db);
    api.get("-121.351140,38.644059,-121.343029,38.654097");
    
    SECTION("Nodes and ways can be fetched from db.") {
        std::vector<OSM::Way> ways = OSM::Way::fetchWays(db, "-121.351140,38.644059,-121.343029,38.654097");
        REQUIRE(ways.size() > 0);
        std::vector<OSM::Node> nodes = OSM::Node::fetchStandaloneNodes(db, "-121.351140,38.644059,-121.343029,38.654097");
        REQUIRE(nodes.size() > 0);
    }
}