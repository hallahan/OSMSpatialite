//
//  Util.cpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/2/15.
//  Copyright (c) 2015. All rights reserved.
//

#include <OSMSpatialite/Util.hpp>

#include <unistd.h>
#include <sys/stat.h>

namespace OSM {
namespace Util {

std::string cwd() {
    char path[1024];
    getcwd(path, sizeof(path));
    return path;
}

std::string createTestTmpDir() {
	std::string path = cwd() + "/test/tmp";
	if(mkdir(path.c_str(), S_IRWXU) != 0) {
		rmdir(path.c_str());
		mkdir(path.c_str(), S_IRWXU);
	}
	return path;
}

}
}
