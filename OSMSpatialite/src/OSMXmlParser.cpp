//
//  OSMXmlParser.cpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 theoutpost. All rights reserved.
//

#include "OSMXmlParser.hpp"

#include <iostream>
#include <stdio.h>

namespace OSM
{
    
    // Note: I am using namespaces with const ints instead of enum,
    //    because this allows me to avoid having to cast C return
    //    values to types. I get the comparable utility of enums,
    //    but I can still print out int values and look at them directly.
    
    // Return codes from XML Reader API
    namespace XmlStatus { const int ERROR=-1; const int FALSE=0; const int TRUE=1; };
    
    // Return codes for node type (xmlTextReaderNodeType)
    // http://www.gnu.org/software/dotgnu/pnetlib-doc/System/Xml/XmlNodeType.html
    namespace XmlNodeType {
        const int NONE=0; const int ELEMENT=1; const int ATTRIBUTE=2; const int TEXT=3;
        const int CDATA=4; const int ENTITY_REFERENCE=5; const int ENTITY=6;
        const int PROCESSING_INSTRUCTION=7; const int COMMENT=8; const int DOCUMENT=9;
        const int DOCUMENT_TYPE=10; const int DOCUMENT_FRAGMENT=11; const int NOTATION=12;
        const int WHITESPACE=13; const int SIGNIFICANT_WHITESPACE=14;
        const int END_ELEMENT=15; const int END_ENTITY=16; const int XML_DECLARATION=17;
    };
    
    
    OSMXmlParser::OSMXmlParser(const std::string& dbPath, const std::string& filePath) :
    _osmdb(dbPath),
    _filePath(filePath)
    {
        _reader = xmlReaderForFile(_filePath.c_str(), NULL, 0);
    }
    
    
    void OSMXmlParser::parse()
    {
        if (_reader != NULL) {
            int ret = xmlTextReaderRead(_reader);
            while (ret == 1) {
                _processXmlNode();
                ret = xmlTextReaderRead(_reader);
            }
        }
    }
    
    
    void OSMXmlParser::_processXmlNode()
    {
        const int nodeType       = xmlTextReaderNodeType(_reader);
        const int isEmptyElement = xmlTextReaderIsEmptyElement(_reader);
        const int hasTextValue   = xmlTextReaderHasValue(_reader);
        
        std::string value;
        const xmlChar *val = xmlTextReaderConstValue(_reader);
        if (val != NULL) {
            value = (char*) val;
        }
        
        std::string name;
        const xmlChar *n = xmlTextReaderConstName(_reader);
        if (n != NULL) {
            name = (char*) n;
        }
        
        
        std::cout   << "name: " << name << '\n'
        << "value: " << value << '\n'
        << "nodeType: " << nodeType << '\n'
        << "isEmptyElement: " << isEmptyElement << '\n'
        << "hasTextValue: " << hasTextValue << "\n\n";
        
        if (nodeType == XmlNodeType::ELEMENT) {
            if (name == "osm") {
                _readOsm();
            } else if (name == "note") {
                _readNote();
            } else if (name == "meta") {
                _readMeta();
            } else if (name == "node") {
                _readNode();
            } else if (name == "way") {
                _readWay();
            } else if (name == "relation") {
                _readRelation();
            }
            
        }
    }
    
    void OSMXmlParser::_readOsm() {
        std::string version;
        std::string generator;
        if (xmlTextReaderHasAttributes(_reader) == XmlStatus::TRUE) {
            while (xmlTextReaderMoveToNextAttribute(_reader)) {
                std::string k = (char*) xmlTextReaderConstName(_reader);
                std::string v = (char*) xmlTextReaderConstValue(_reader);
                if (k == "version") {
                    version = v;
                } else if (k == "generator") {
                    generator = v;
                }
            }
        }
        _osmdb.addOSM(version, generator);
    }
    
    void OSMXmlParser::_readNote() {
        
    }
    
    void OSMXmlParser::_readMeta() {
        
    }
    
    void OSMXmlParser::_readNode() {
        // Read attributes
        if (xmlTextReaderHasAttributes(_reader) == XmlStatus::TRUE) {
            while (xmlTextReaderMoveToNextAttribute(_reader)) {
                
            }
        }
        
        // Read tags
    }
    
    void OSMXmlParser::_readWay() {
        
    }
    
    void OSMXmlParser::_readRelation() {
        
    }
    
}