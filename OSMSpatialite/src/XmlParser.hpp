//
//  XmlParser.hpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 Nicholas Hallahan. All rights reserved.
//

#ifndef XmlParser_hpp
#define XmlParser_hpp

#include <string>
#include <libxml/xmlreader.h>

#include "Element.hpp"
#include "DatabaseBuilder.hpp"

namespace OSM {
    
    
class XmlParser {
public:
    explicit XmlParser(std::shared_ptr<AmigoCloud::Database> db);
    
    XmlParser& xmlFile(const std::string& filePath);
    XmlParser& xmlString(const std::string& xmlString);
    
    XmlParser& parse();
    
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
    DatabaseBuilder _dbBuilder;
    
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

#endif /* XmlParser_hpp */
