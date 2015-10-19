#include <iostream>
#include <sqlite3.h>
#include <spatialite/gaiageo.h>
#include <spatialite.h>

#include "OSMXmlParser.hpp"


int main(int argc, const char * argv[]) {
    
    std::string xmlStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><osm version=\"0.6\" generator=\"Overpass API\"><meta osm_base=\"2015-10-19T15:48:02Z\"/><node id=\"368174593\" lat=\"38.6393485\" lon=\"-121.3799524\" version=\"1\" timestamp=\"2009-04-01T07:58:57Z\" changeset=\"17182\" uid=\"28145\" user=\"amillar\"><tag k=\"addr:state\" v=\"CA\"/><tag k=\"ele\" v=\"22\"/><tag k=\"gnis:county_name\" v=\"Sacramento\"/><tag k=\"gnis:feature_id\" v=\"1817495\"/><tag k=\"gnis:import_uuid\" v=\"57871b70-0100-4405-bb30-88b2e001a944\"/><tag k=\"gnis:reviewed\" v=\"no\"/><tag k=\"name\" v=\"Discovery Museum Science and Space Center\"/><tag k=\"source\" v=\"USGS Geonames\"/><tag k=\"tourism\" v=\"museum\"/></node></osm>";
    
    AmigoCloud::Database db("/temp/xmlstr.db", true);
    OSM::OSMXmlParser parser(db);
    parser.xmlString(xmlStr).parse();
    
    AmigoCloud::Database db2("/temp/xmlfile.db", true);
    OSM::OSMXmlParser parser2(db2);
    parser2.xmlFile("/temp/include_standalone.osm").parse();

    return 0;
}
