#include "catch.hpp"

// AmigoCloud Includes
#include <core/Database.h>
#include <core/RestClient.h>

// GEOS
#include <geos/geom/Geometry.h>

// OSMSpatialite
#include <OSMSpatialite/Util.hpp>
#include <OSMSpatialite/XmlParser.hpp>
#include <OSMSpatialite/Way.hpp>


TEST_CASE("Fetching some ways.", "[way]") {
    std::string dbPath = OSM::Util::cwd() + "/test/tmp/arcade.db";
    std::shared_ptr<AmigoCloud::Database> db = std::make_shared<AmigoCloud::Database>(dbPath, true);
    OSM::XmlParser parser(db);
    parser.xmlFile(OSM::Util::cwd() + "/test/data/arcade.osm").parse();
    
    SECTION("Check barn way in bbox.") {
        INFO("In the bbox for the arcade data set, there is one way in the bbox, and it is the barn in my backyard.");
        std::vector<OSM::Way> ways = OSM::Way::fetchWays(db, "-121.369328,38.649437,-121.369215,38.649527");
        REQUIRE(ways.size() == 1);
        OSM::Way way = ways[0];
        REQUIRE(way.isClosed() == true);
        REQUIRE(way.getId() == 349945505);
        REQUIRE(way.getVersion() == 1);
        REQUIRE(way.getChangeset() == 31575864);
        REQUIRE(way.getUser() == "TheOutpost");
        
        SECTION("Check geometry of barn way.") {
            std::shared_ptr<geos::geom::Geometry> geom = way.getGeometry();
            std::size_t numPoints = geom->getNumPoints();
            REQUIRE(numPoints == 5);
        }

    }
    
    SECTION("Check num of closed ways in bbox.") {
        INFO("In the bbox for the arcade data set, there is one way in the bbox, and it is the barn in my backyard.");
        std::vector<OSM::Way> ways = OSM::Way::fetchClosedWays(db, "-121.369328,38.649437,-121.369215,38.649527");
        REQUIRE(ways.size() == 1);
        OSM::Way way = ways[0];
        REQUIRE(way.isClosed() == true);
        REQUIRE(way.getId() == 349945505);
    }
}