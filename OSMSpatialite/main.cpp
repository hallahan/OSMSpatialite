#include <iostream>
#include <sqlite3.h>
#include <spatialite/gaiageo.h>
#include <spatialite.h>

#include "OSMXmlParser.hpp"

void testSqlite() {
    sqlite3 *db;
    char *zErr;
    int rc;
    std::string sql;
    
    rc = sqlite3_open("/temp/test.db", &db);
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sql = "create table if not exists episodes(id int, name text)";
    rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &zErr);
    if(rc != SQLITE_OK) {
        if (zErr != NULL) {
            fprintf(stderr, "SQL error: %s\n", zErr);
            sqlite3_free(zErr);
        }
    }
    sql = "insert into episodes values (10, 'The Dinner Party')";
    rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &zErr);
    if(rc != SQLITE_OK) {
        if (zErr != NULL) {
            fprintf(stderr, "SQL error: %s\n", zErr);
            sqlite3_free(zErr);
        }
    }
    sql = "insert into episodes values (11, 'Pixel Perfect')";
    rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &zErr);
    sqlite3_close(db);
}

void testSpatialite() {
    spatialite_init (0);
    const char* version = spatialite_version();
    std::cout << "Spatialite version: " << version << std::endl << std::endl;
}

int main(int argc, const char * argv[]) {
    testSqlite();
//    testSpatialite();
    
    OSM::OSMXmlParser parser("/temp/arcade_test4.db", "/temp/include_standalone.osm");
    parser.parse();

    return 0;
}
