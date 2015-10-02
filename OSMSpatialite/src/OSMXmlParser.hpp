//
//  OSMXmlParser.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef OSMXmlParser_hpp
#define OSMXmlParser_hpp

#include <string>
#include <libxml/xmlreader.h>

#include "OSMDatabase.hpp"

namespace OSM
{
    
    
class OSMXmlParser
{
public:
    explicit OSMXmlParser(const std::string& dbPath, const std::string& filePath);
    
    void parse();
    
private:
    void _processXmlNode();
    void _readOsm();
    void _readNote();
    void _readMeta();
    void _readNode();
    void _readWay();
    void _readRelation();
    
    xmlTextReaderPtr _reader;
    std::string _filePath;
    
    OSMDatabase _osmdb;
    
};
    
}

#endif /* OSMXmlParser_hpp */
