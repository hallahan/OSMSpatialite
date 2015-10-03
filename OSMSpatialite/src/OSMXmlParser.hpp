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

#include "OSMElement.hpp"
#include "OSMDatabase.hpp"

namespace OSM {
    
    
class OSMXmlParser {
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
    void _readTag();
    void _readNd();
    void _readMember();
    
    xmlTextReaderPtr _reader;
    std::string _filePath;
    OSMDatabase _osmdb;
    
    ElementType _parentElementType;
    std::string _parentElementId;
    
    // This is used internally for incrementing way_pos as we are reading nds in a way.
    // This should ONLY be referenced by _readNd.
    std::string _prevParentElementId;
    unsigned int _ndPosCount = 0; // only use in _readNd
};
    
}

#endif /* OSMXmlParser_hpp */
