//
//  XmlParser.cpp
//  OSMSpatialite
//
//  Created by Nicholas Hallahan on 10/1/15.
//  Copyright © 2015 theoutpost. All rights reserved.
//

#include <OSMSpatialite/XmlParser.hpp>

#include <iostream>
#include <stdio.h>

namespace OSM {
    
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

XmlParser::XmlParser(std::shared_ptr<AmigoCloud::Database> db) :
_dbBuilder(db) {
    
}

XmlParser& XmlParser::xmlFile(const std::string& filePath) {
    xmlInitParser();
    _reader = xmlReaderForFile(filePath.c_str(), NULL, 0);
    return *this;
}

XmlParser& XmlParser::xmlString(const std::string& xmlString) {
    xmlInitParser();
    const char* cstr = xmlString.c_str();
    _reader = xmlReaderForMemory(cstr, (int)strlen(cstr), NULL, NULL, 0);
    return *this;
}

XmlParser& XmlParser::parse() {
    if (_reader != NULL) {
        int ret = xmlTextReaderRead(_reader);
        while (ret == 1) {
            _processXmlNode();
            ret = xmlTextReaderRead(_reader);
        }
        _dbBuilder.postProcess();
        xmlCleanupParser();
    }
    return *this;
}

void XmlParser::_processXmlNode() {
    const int nodeType       = xmlTextReaderNodeType(_reader);
//        const int isEmptyElement = xmlTextReaderIsEmptyElement(_reader);
//        const int hasTextValue   = xmlTextReaderHasValue(_reader);
    
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
    
    
//        std::cout   << "name: " << name << '\n'
//        << "value: " << value << '\n'
//        << "nodeType: " << nodeType << '\n'
//        << "isEmptyElement: " << isEmptyElement << '\n'
//        << "hasTextValue: " << hasTextValue << "\n\n";
    
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
        } else if (name == "tag") {
            _readTag();
        } else if (name == "nd") {
            _readNd();
        } else if (name == "member") {
            _readMember();
        }
    }
}

void XmlParser::_readOsm() {
    std::string versionStr;
    std::string generatorStr;
    // Read attributes
    if (xmlTextReaderHasAttributes(_reader) == XmlStatus::TRUE) {
        while (xmlTextReaderMoveToNextAttribute(_reader)) {
            std::string k = (char*) xmlTextReaderConstName(_reader);
            std::string v = (char*) xmlTextReaderConstValue(_reader);
            if (k == "version") {
                versionStr = v;
            } else if (k == "generator") {
                generatorStr = v;
            }
        }
    }
    _dbBuilder.addOSM(versionStr, generatorStr);
}

void XmlParser::_readNote() {
    
}

void XmlParser::_readMeta() {
    
}

void XmlParser::_readNode() {
    std::string idStr;
    std::string latStr;
    std::string lonStr;
    std::string versionStr;
    std::string timestampStr;
    std::string changesetStr;
    std::string uidStr;
    std::string userStr;
    std::string actionStr;
    std::string visibleStr;
    // Read attributes
    if (xmlTextReaderHasAttributes(_reader) == XmlStatus::TRUE) {
        while (xmlTextReaderMoveToNextAttribute(_reader)) {
            std::string k = (char*) xmlTextReaderConstName(_reader);
            std::string v = (char*) xmlTextReaderConstValue(_reader);
            if (k == "id") {
                idStr = v;
            } else if (k == "lat") {
                latStr = v;
            } else if (k == "lon") {
                lonStr = v;
            } else if (k == "version") {
                versionStr = v;
            } else if (k == "timestamp") {
                timestampStr = v;
            } else if (k == "changeset") {
                changesetStr = v;
            } else if (k == "uid") {
                uidStr = v;
            } else if (k == "user") {
                userStr = v;
            } else if (k == "action") {
                actionStr = v;
            } else if (k == "visible") {
                visibleStr = v;
            }
        }
    }
    _parentElementType = ElementType::NODE;
    _parentElementId = idStr;
    _dbBuilder.addNode(idStr, latStr, lonStr, versionStr, timestampStr,
                   changesetStr, uidStr, userStr, actionStr, visibleStr);
}

void XmlParser::_readWay() {
    std::string idStr;
    std::string versionStr;
    std::string timestampStr;
    std::string changesetStr;
    std::string uidStr;
    std::string userStr;
    std::string actionStr;
    std::string visibleStr;
    // Read attributes
    if (xmlTextReaderHasAttributes(_reader) == XmlStatus::TRUE) {
        while (xmlTextReaderMoveToNextAttribute(_reader)) {
            std::string k = (char*) xmlTextReaderConstName(_reader);
            std::string v = (char*) xmlTextReaderConstValue(_reader);
            if (k == "id") {
                idStr = v;
            } else if (k == "version") {
                versionStr = v;
            } else if (k == "timestamp") {
                timestampStr = v;
            } else if (k == "changeset") {
                changesetStr = v;
            } else if (k == "uid") {
                uidStr = v;
            } else if (k == "user") {
                userStr = v;
            } else if (k == "action") {
                actionStr = v;
            } else if (k == "visible") {
                visibleStr = v;
            }
        }
    }
    _parentElementType = ElementType::WAY;
    _parentElementId = idStr;
    _dbBuilder.addWay(idStr, versionStr, timestampStr, changesetStr,
                  uidStr, userStr, actionStr, visibleStr);
}

void XmlParser::_readRelation() {
    std::string idStr;
    std::string versionStr;
    std::string timestampStr;
    std::string changesetStr;
    std::string uidStr;
    std::string userStr;
    std::string actionStr;
    std::string visibleStr;
    // Read attributes
    if (xmlTextReaderHasAttributes(_reader) == XmlStatus::TRUE) {
        while (xmlTextReaderMoveToNextAttribute(_reader)) {
            std::string k = (char*) xmlTextReaderConstName(_reader);
            std::string v = (char*) xmlTextReaderConstValue(_reader);
            if (k == "id") {
                idStr = v;
            } else if (k == "version") {
                versionStr = v;
            } else if (k == "timestamp") {
                timestampStr = v;
            } else if (k == "changeset") {
                changesetStr = v;
            } else if (k == "uid") {
                uidStr = v;
            } else if (k == "user") {
                userStr = v;
            } else if (k == "action") {
                actionStr = v;
            } else if (k == "visible") {
                visibleStr = v;
            }
        }
    }
    _parentElementType = ElementType::RELATION;
    _parentElementId = idStr;
    _dbBuilder.addRelation(idStr, versionStr, timestampStr, changesetStr,
                  uidStr, userStr, actionStr, visibleStr);
}

void XmlParser::_readTag() {
    std::string k;
    std::string v;
    // Read attributes
    if (xmlTextReaderHasAttributes(_reader) == XmlStatus::TRUE) {
        while (xmlTextReaderMoveToNextAttribute(_reader)) {
            std::string key = (char*) xmlTextReaderConstName(_reader);
            std::string val = (char*) xmlTextReaderConstValue(_reader);
            if (key == "k") {
                k = val;
            } else if (key == "v") {
                v = val;
            }
        }
    }
    
    // Only blast the previous tags for the element once when updating.
    if (_prevTagParentElementId == _parentElementId) {
        _dbBuilder.addTag(_parentElementType, _parentElementId, k, v, false);
    } else {
        _dbBuilder.addTag(_parentElementType, _parentElementId, k, v, true);
        _prevTagParentElementId = _parentElementId;
    }
}

void XmlParser::_readNd() {
    if (_prevParentElementId == _parentElementId) {
        ++_ndPosCount;
    } else {
        _prevParentElementId = _parentElementId;
        _ndPosCount = 0;
    }
    std::string ref;
    // Read attributes
    if (xmlTextReaderHasAttributes(_reader) == XmlStatus::TRUE) {
        while (xmlTextReaderMoveToNextAttribute(_reader)) {
            std::string k = (char*) xmlTextReaderConstName(_reader);
            std::string v = (char*) xmlTextReaderConstValue(_reader);
            if (k == "ref") {
                ref = v;
            }
        }
    }
    _dbBuilder.addNd(_parentElementId, ref, _ndPosCount);
}

void XmlParser::_readMember() {
    std::string type;
    std::string ref;
    std::string role;
    // Read attributes
    if (xmlTextReaderHasAttributes(_reader) == XmlStatus::TRUE) {
        while (xmlTextReaderMoveToNextAttribute(_reader)) {
            std::string k = (char*) xmlTextReaderConstName(_reader);
            std::string v = (char*) xmlTextReaderConstValue(_reader);
            if (k == "type") {
                type = v;
            } else if (k == "ref") {
                ref = v;
            } else if (k == "role") {
                role = v;
            }
        }
    }
    if (_prevMemberParentElementId == _parentElementId) {
        _dbBuilder.addMember(_parentElementId, ref, type, role, false);
    } else {
        _dbBuilder.addMember(_parentElementId, ref, type, role, true);
        _prevMemberParentElementId = _parentElementId;
    }
}
}
