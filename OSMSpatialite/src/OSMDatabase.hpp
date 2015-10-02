//
//  OSMDatabase.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef OSMDatabase_hpp
#define OSMDatabase_hpp

#include "AmigoCloud/Database.h"

namespace OSM
{
    
    class OSMDatabase
    {
    public:
        explicit OSMDatabase(const std::string& dbPath);
        
        void addOSM(const std::string& version, const std::string& generator);
        
    private:
        // Initializes DB to have OSM Schema if needed.
        void _initDB();
        
        const std::string _dbPath;
        AmigoCloud::Database _db;
    };
}

#endif /* OSMDatabase_hpp */
