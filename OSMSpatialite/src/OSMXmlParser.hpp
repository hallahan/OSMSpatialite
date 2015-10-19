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
#include "OSMDatabaseBuilder.hpp"

namespace OSM {
    
    
class OSMXmlParser {
public:
    explicit OSMXmlParser(const AmigoCloud::Database& db);
    
    OSMXmlParser& xmlFile(const std::string& filePath);
    OSMXmlParser& xmlString(const std::string& xmlString);
    
    OSMXmlParser& parse();
    
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
    OSMDatabaseBuilder _dbBuilder;
    
    ElementType _parentElementType;
    std::string _parentElementId;
    
    // This is used internally for incrementing way_pos as we are reading nds in a way.
    // This should ONLY be referenced by _readNd.
    std::string _prevParentElementId;
    unsigned int _ndPosCount = 0; // only use in _readNd()
    
    // This should only be referenced by _readTag()
    std::string _prevTagParentElementId;
    
    // This should only be referenced by _readMember()
    std::string _prevMemberParentElementId;
};
    
}

#endif /* OSMXmlParser_hpp */
