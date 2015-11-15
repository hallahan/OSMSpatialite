#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <OSMSpatialite/Util.hpp>

int main( int argc, char* const argv[] ) {
    // Setup tmp directory.
    std::string tmpDir = OSM::Util::createTestTmpDir();
    std::cout << "Created fresh tmp dir for tests:\n" << tmpDir << std::endl;
    
    int result = Catch::Session().run( argc, argv );
    
    // Do cleanup if desired...
    
    return result;
}