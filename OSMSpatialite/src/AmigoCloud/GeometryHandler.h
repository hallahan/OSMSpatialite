/*
*
* AmigoMobile
* 
* Copyright (c) 2011-2015 AmigoCloud Inc., All rights reserved.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
* 
* You should have received a copy of the GNU General Public
* License along with this library.
* 
*/



#ifndef amigocloud_GeometryHandler_h
#define amigocloud_GeometryHandler_h

#include "Chaskii/Math/vec3.h"
#include "Chaskii/Math/bound.h"

#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/CoordinateFilter.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/Dimension.h>
#include <geos/geom/Envelope.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryCollection.h>
#include <geos/geom/GeometryComponentFilter.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/GeometryFilter.h>
#include <geos/geom/LineString.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPoint.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/PrecisionModel.h>
#include <geos/geom/LineSegment.h>
#include <geos/geom/IntersectionMatrix.h>
#include <geos/geom/Location.h>
#include <geos/io/WKBReader.h>
#include <geos/io/WKBWriter.h>
#include <geos/io/WKTReader.h>
#include <geos/io/WKTWriter.h>
#include <geos/io/ParseException.h>
#include <geos/util/IllegalArgumentException.h>
#include <geos/util/TopologyException.h>
#include <geos/operation/union/CascadedPolygonUnion.h>
#include <geos/operation/union/CascadedUnion.h>
#include <geos/operation/buffer/BufferOp.h>
#include <geos/operation/buffer/BufferParameters.h>
#include <geos/simplify/TopologyPreservingSimplifier.h>
#include <vector>
#include <string>
#include "VectorTypes.h"

namespace AmigoCloud
{

    class GeometryHandler
    {
    public:
        static geos::geom::GeometryFactory *getGeometryFactory();

        static bool validateParsingWKBBin(const std::string &wkbStr);
        static geos::geom::Geometry* parseWKBBin(const std::string &wkb);
        static geos::geom::Geometry* parseWKBHex(const std::string &wkb);
        static geos::geom::Geometry* parseWKT(const std::string &wkt, bool filter=true);
        static std::string toWKB(const geos::geom::Geometry *geom, int dimension);
        
        static bool getBound(geos::geom::Geometry* geom, iiMath::bound &b);
        static std::string filterEWKB(const std::string &ewkb, bool filter=true);
        static geos::geom::Geometry* createGeometry(const std::string &wktType);
        static geos::geom::Geometry* createMultiPointGeometry(PointList &coords);
        static geos::geom::Geometry* createLineStringGeometry(PointList &coords);
        static geos::geom::Geometry* createPolygonGeometry(PointList &coords);
        static geos::geom::Geometry* createMultiPolygonGeometry(PointList &coords);
        static geos::geom::Geometry* createMultiLineStringGeometry(std::vector<geos::geom::CoordinateSequence*> &css);
        static geos::geom::Geometry* createMultiLineStringGeometry(PointListList &coords);
        static geos::geom::Geometry* bufferGeometry(const geos::geom::Geometry *geom, double width);
//        static void getCoordinates(geos::geom::CoordinateSequence *cs, PointList &pList);
//        static void getCoordinates(geos::geom::CoordinateSequence *cs, PointList &pList, int &elevLevel);
//        static bool getCoordinates(const geos::geom::Geometry *geometry, geos::geom::Geometry *clipArea, PointListList &coords);
//        static bool getCoordinates(const geos::geom::Geometry *geometry, geos::geom::Geometry *clipArea, PointListList &coords, int &elevLevel);
//        static bool getCoordinates(const geos::geom::Geometry *geometry, PointListList &coords, int &elevLevel);
//        static bool getCoordinates(std::vector<geos::geom::Geometry*> &geometries, PointListList &coords, int &elevLevel);
//        static bool getCoordinates(std::vector<geos::geom::Geometry*> &geometries, geos::geom::Geometry *clipArea, PointListList &coords, int &elevLevel);
//        static void convertLinesToPolygons(std::vector<geos::geom::Geometry*> &lines, PointListList &coords, geos::geom::Geometry *clipArea, double width);
//        static void convertLinesToPolygons(std::vector<geos::geom::Geometry*> &lines, PointListList &coords, PointListList &outline, geos::geom::Geometry *clipArea, double width, double outlineWidth);
        static std::string getWKBStatement(const std::string &wkb, const std::string &geometryType, bool filter=true);
        // static std::string getGeometryType(const std::string &json);
        // static int getGeometryDimension(const std::string &json);
        static int getGeometryDimensionByType(std::string geometry_type);
        static geos::geom::GeometryTypeId getGeometryTypeByWKTType(std::string geometry_type);

        static geos::geom::Geometry* getGeometry(geos::geom::GeometryTypeId type, int dimension, PointListList &points, bool &ret);
        static std::string getGeometryWKB(geos::geom::GeometryTypeId type, int dimension, PointListList &points);
    private:
    };
    
}

#endif
