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


#include <core/GeometryHandler.h>
#include <common/WKBParser.h>
#include <sstream>
#include <core/Logger.h>
// #include <core/Globe.h>
// #include <core/JSONUtils.h>
#include <algorithm>

namespace AmigoCloud
{
    static std::string TAG("GeometryHandler");

	static geos::geom::GeometryFactory *sGeomFactory=NULL;

	geos::geom::GeometryFactory *GeometryHandler::getGeometryFactory()
	{
	    if(sGeomFactory==NULL)
        {
            geos::geom::PrecisionModel *pm = new geos::geom::PrecisionModel();
            sGeomFactory = new geos::geom::GeometryFactory(pm, 4326);
        }
        return sGeomFactory;
	}

    bool GeometryHandler::validateParsingWKBBin(const std::string &wkbStr)
    {
        if(wkbStr.empty() || wkbStr=="NULL" || wkbStr=="null")
            return false;

        bool ret = true;
        geos::geom::Geometry *geometry=NULL;
        std::istringstream is(wkbStr, std::ios_base::binary|std::ios_base::in|std::ios_base::out);
        WKBParser reader;
        try
        {
             geometry = reader.read(is);
        } catch (geos::io::ParseException e)
        {
            AMIGO_LOG_W(TAG, "::validateParseWKBBin() geos::io::ParseException: '%s' size=%d\n", e.what(), wkbStr.size());
            ret = false;
        } catch (geos::util::GEOSException e)
        {
            AMIGO_LOG_W(TAG, "::validateParseWKBBin() geos::util::GEOSException: '%s' size=%d\n", e.what(), wkbStr.size());
        }
        
        if(geometry)
            delete geometry;

        return ret;
    }

    geos::geom::Geometry *GeometryHandler::parseWKBBin(const std::string &wkbStr)
    {
        if(wkbStr.empty() || wkbStr=="NULL" || wkbStr=="null")
            return NULL;

        std::istringstream is(wkbStr, std::ios_base::binary|std::ios_base::in|std::ios_base::out);
        WKBParser reader;
        try
        {
            geos::geom::Geometry *geometry = reader.read(is);
            return geometry;
        } catch (geos::util::GEOSException e)
        {
            AMIGO_LOG_W(TAG, "::parseWKBBin() geos::util::GEOSException: '%s' size=%d\n", e.what(), wkbStr.size());
        }

        return NULL;
    }

    geos::geom::Geometry *GeometryHandler::parseWKBHex(const std::string &wkbStr)
    {
        if(wkbStr.empty() || wkbStr=="NULL" || wkbStr=="null")
            return NULL;

        std::string wkbStr2 = wkbStr;

        if(wkbStr2[0] == '\'')
        {
            // Remove "'"
            wkbStr2.erase(0, 1);
            wkbStr2.erase(wkbStr2.size()-1, 1);
        }
        
        std::size_t pos = wkbStr2.find(";");
        if(pos != std::string::npos)
        {
            wkbStr2.erase(0, pos+1);
        }

        std::istringstream is(wkbStr2);
        WKBParser reader;
        try
        {
	        geos::geom::Geometry *geometry = reader.readHEX(is);
	        return geometry;
        } catch (geos::util::GEOSException e)
        {
            AMIGO_LOG_W(TAG, "::parseWKBHex() geos::util::GEOSException: '%s', '%s'\n", e.what(), wkbStr2.c_str());
        } catch(...)
        {
            AMIGO_LOG_W(TAG, "::parseWKBHex() UknownException\n");
        }

        return NULL;
    }

    geos::geom::Geometry *GeometryHandler::parseWKT(const std::string &wktStr, bool filter)
    {
        if(wktStr.empty() || wktStr=="NULL" || wktStr=="null")
            return NULL;

        std::string wktStr2 = wktStr;
        
        if(filter)
        {
            // Remove "'"
            wktStr2.erase(0, 1);
            wktStr2.erase(wktStr2.size()-1, 1);
        }
        
        geos::io::WKTReader reader;
		try
		{
        	geos::geom::Geometry *geometry = reader.read(wktStr2);
	        return geometry;
        } catch (geos::util::GEOSException e)
		{
			AMIGO_LOG_W(TAG, "::parseWKT() geos::util::GEOSException: '%s', wkt:'%s'\n", e.what(), wktStr.c_str());
		}

        return NULL;
    }
    
    std::string GeometryHandler::toWKB(const geos::geom::Geometry *geom, int dimension)
    {
        if(geom==NULL)
            return "";
        try
        {
            geos::io::WKBWriter writer( dimension );
            writer.setIncludeSRID(4326);
            std::ostringstream os;
            writer.writeHEX(*geom, os);
            std::string wkb= os.str();
            return wkb;
        }
        catch(geos::util::GEOSException e)
        {
            AMIGO_LOG_W(TAG, "::toWKB() geos::util::GEOSException: '%s'\n", e.what());
        }
        return "";
    }
    
    bool GeometryHandler::getBound(geos::geom::Geometry* geom, iiMath::bound &bound)
    {
        bound.reset();
        geos::geom::Geometry *b = geom->getEnvelope();
        if(b==NULL)
            return false;
        
        geos::geom::CoordinateSequence *coords = b->getCoordinates();
        if(coords==NULL)
            return false;

        for(int i=0; i<coords->size(); i++)
        {
            geos::geom::Coordinate c;
            coords->getAt(i, c);
            bound.expandBy(iiMath::vec3(c.x, c.y, 0));
        }
        delete coords;
        return true;
    }

    std::string GeometryHandler::filterEWKB(const std::string &ewkb, bool filter)
    {
        std::string wkbStr2 = ewkb;
        
        if(filter || wkbStr2[0] == '\'')
        {
            // Remove "'"
            wkbStr2.erase(0, 1);
            wkbStr2.erase(wkbStr2.size()-1, 1);
        }
        return wkbStr2;        
    }

    std::string GeometryHandler::getWKBStatement(const std::string &wkb, const std::string &geometryType, bool filter)
    {
        std::string geom = "GeomFromEWKB('" + GeometryHandler::filterEWKB(wkb, filter) + "')";
        if(geometryType.compare(0, 5, "MULTI")==0)
        {
            return "CastToMulti(" + geom + ")";
        }
        else
        {
            return geom;
        }
    }

	geos::geom::Geometry* GeometryHandler::createGeometry(const std::string &wktType)
	{
		geos::geom::Geometry *g=NULL;
		if(wktType.compare( 0, 5, "POINT")==0)
		{
			g = GeometryHandler::getGeometryFactory()->createPoint();
		} else if(wktType.compare(0, 10, "LINESTRING")==0)
		{
			g = GeometryHandler::getGeometryFactory()->createLineString();
		} else if(wktType.compare(0, 7, "POLYGON")==0)
		{
			g = GeometryHandler::getGeometryFactory()->createPolygon();
		} else if(wktType.compare(0, 10, "MULTIPOINT")==0)
		{
			g = GeometryHandler::getGeometryFactory()->createMultiPoint();
		} else if(wktType.compare(0, 15, "MULTILINESTRING")==0)
		{
			g = GeometryHandler::getGeometryFactory()->createMultiLineString();
		} else if(wktType.compare(0, 12, "MULTIPOLYGON")==0)
		{
			g = GeometryHandler::getGeometryFactory()->createMultiPolygon();
		}
		AMIGO_LOG_W(TAG, "::createGeometry() type=%s, g=%p\n", wktType.c_str(), g);
		return g;
	}

    geos::geom::Geometry* GeometryHandler::createMultiPointGeometry(PointList &coords)
    {
       if(coords.size() < 2)
            return NULL;
        try
        {
            geos::geom::CoordinateArraySequence cs;
            PointList::iterator it;
            for(it=coords.begin(); it!=coords.end(); it++)
            {
                iiMath::vec3 &p = *it;
                cs.add(geos::geom::Coordinate(p.y(), p.x(), p.z()));
            }
            return getGeometryFactory()->createMultiPoint(cs);
        }
        catch(geos::util::GEOSException e)
        {
            AMIGO_LOG_W(TAG, "::createMultiPointGeometry() geos::util::GEOSException: '%s'\n", e.what());
        }
        return NULL;        
    }

	geos::geom::Geometry* GeometryHandler::createLineStringGeometry(PointList &coords)
	{
       if(coords.size() < 2)
            return NULL;
	    try
        {
            geos::geom::CoordinateArraySequence cs;
            PointList::iterator it;
            for(it=coords.begin(); it!=coords.end(); it++)
            {
                iiMath::vec3 &p = *it;
                cs.add(geos::geom::Coordinate(p.y(), p.x(), p.z()));
            }
            return getGeometryFactory()->createLineString(cs);
        }
        catch(geos::util::GEOSException e)
        {
            AMIGO_LOG_W(TAG, "::createLineStringGeometry() geos::util::GEOSException: '%s'\n", e.what());
        }
        return NULL;
    }

    geos::geom::Geometry* GeometryHandler::createPolygonGeometry(PointList &coords)
    {
        if(coords.size() < 4)
            return NULL;
        try
        {
            geos::geom::CoordinateArraySequence cs;
            PointList::iterator it;
            for(it=coords.begin(); it!=coords.end(); it++)
            {
			    iiMath::vec3 &p = *it;
                cs.add(geos::geom::Coordinate(p.y(), p.x(), p.z()));
            }
            return getGeometryFactory()->createPolygon(getGeometryFactory()->createLinearRing(cs), NULL);
        }
        catch(geos::util::GEOSException e)
        {
            AMIGO_LOG_W(TAG, "::createPolygonGeometry() geos::util::GEOSException: '%s'\n", e.what());
        }
        return NULL;
    }
    
	geos::geom::Geometry* GeometryHandler::createMultiLineStringGeometry(std::vector<geos::geom::CoordinateSequence*> &css)
	{
        geos::geom::Geometry* multiLine = NULL;
        try 
        {
    	    std::vector<geos::geom::Geometry*> lines;
            std::vector<geos::geom::CoordinateSequence*>::iterator it1;
            for(it1=css.begin(); it1!=css.end(); it1++)
            {
                geos::geom::CoordinateSequence *cs = *it1;
                geos::geom::Geometry* line = GeometryHandler::getGeometryFactory()->createLineString(cs);
                lines.push_back(line);
            }
            multiLine = GeometryHandler::getGeometryFactory()->createMultiLineString(lines);
            std::vector<geos::geom::Geometry*>::iterator it;
            for(it = lines.begin(); it != lines.end(); it++)
            {
                delete *it;
            }
        } catch (geos::util::GEOSException e)
        {
            AMIGO_LOG_W(TAG, "::createMultiLineStringGeometry() geos::util::GEOSException: '%s'\n", e.what());
        }

        return multiLine;
    }
    
    geos::geom::Geometry* GeometryHandler::createMultiLineStringGeometry(PointListList &coords)
    {
        geos::geom::Geometry* multiLine = NULL;
        try 
        {
            std::vector<geos::geom::Geometry*> lines;
            PointListList::iterator it;
            for(it=coords.begin(); it!=coords.end(); it++)
            {
                geos::geom::Geometry* line = createLineStringGeometry(*it);
                if(line)
                    lines.push_back(line);
            }
            multiLine = GeometryHandler::getGeometryFactory()->createMultiLineString(lines);
            std::vector<geos::geom::Geometry*>::iterator itg;
            for(itg = lines.begin(); itg != lines.end(); itg++)
            {
                delete *itg;
            }
        } catch (geos::util::GEOSException e)
        {
            AMIGO_LOG_W(TAG, "::createMultiLineStringGeometry() geos::util::GEOSException: '%s'\n", e.what());
        }
        return multiLine;
    }

    geos::geom::Geometry* GeometryHandler::bufferGeometry(const geos::geom::Geometry *geom, double width)
    {
        try
        {
            return geos::operation::buffer::BufferOp::bufferOp(geom, width,
                    geos::operation::buffer::BufferParameters::CAP_ROUND, 
                    geos::operation::buffer::BufferParameters::JOIN_ROUND);

        } catch (geos::util::GEOSException e)
        {
            AMIGO_LOG_W(TAG, "::bufferGeometry() geos::util::GEOSException: '%s'\n", e.what());
        }    
        return NULL;
    }

//    void GeometryHandler::getCoordinates(geos::geom::CoordinateSequence *cs, PointList &pList)
//    {
//        int elevLevel = 0;
//        getCoordinates(cs, pList, elevLevel);
//    }

//     void GeometryHandler::getCoordinates(geos::geom::CoordinateSequence *cs, PointList &pList, int &elevLevel)
//     {
//         if(cs->getSize()==0)
//             return;
//
//         for(unsigned i=0; i< cs->getSize(); i++ )
//         {
//             geos::geom::Coordinate c;
//             cs->getAt(i, c);
//             double alt = Globe::instance()->getGlobeSceneHandler()->getElevationAt( c.y, c.x, elevLevel);
//             pList.push_back( iiMath::vec3(c.y, c.x, alt) );
//         }
//     }

//    bool GeometryHandler::getCoordinates(const geos::geom::Geometry *geometry, geos::geom::Geometry *clipArea, PointListList &coords)
//    {
//        int elevLevel = 0;
//        if(clipArea)
//            return getCoordinates(geometry, clipArea, coords, elevLevel);
//        else
//            return getCoordinates(geometry, coords, elevLevel);
//    }

//    bool GeometryHandler::getCoordinates(const geos::geom::Geometry *geometry, geos::geom::Geometry *clipArea, PointListList &coords, int &elevLevel)
//    {
//        if(geometry == NULL)
//        {
//            return false;
//        }
//
//        if(clipArea==NULL)
//            return getCoordinates(geometry, coords, elevLevel);
//
//        bool toDeleteGeom=false;
//        const geos::geom::Geometry *geomClipped=NULL;
//        try
//        {
//            if(clipArea!=NULL)
//            {
//                geomClipped = geometry->intersection( clipArea );
//                toDeleteGeom = true;
//            } else
//            {
//                geomClipped = geometry;
//            }
//        
//            for(size_t i = 0; i < geomClipped->getNumGeometries(); i++)
//            {
//                const geos::geom::Geometry *geomN = geomClipped->getGeometryN(i);
//                // PointListList coordVec;
//                if(geomN->getGeometryTypeId() == geos::geom::GEOS_POLYGON)
//                {
//                    const geos::geom::Polygon *poly = dynamic_cast<const geos::geom::Polygon*>(geomN);
//                    if(poly)
//                    {
//                        geos::geom::CoordinateSequence *cs = poly->getExteriorRing()->getCoordinates();
//                        PointList pList;
//                        getCoordinates(cs, pList, elevLevel);
//                        if(!pList.empty())
//                        {
//                            coords.push_back(pList);
//                            for(size_t n = 0; n < poly->getNumInteriorRing(); n++)
//                            {
//                                geos::geom::CoordinateSequence *rcs = poly->getInteriorRingN(n)->getCoordinates();
//                                PointList ringList;
//                                getCoordinates(rcs, ringList, elevLevel);
//                                if(!ringList.empty())
//                                    coords.push_back(ringList);
//                                delete rcs;
//                            }
//                        }
//                        delete cs;
//                    }
//                }
//                else
//                {
//                    geos::geom::CoordinateSequence *cs = geomN->getCoordinates();
//                    PointList pList;
//                    getCoordinates(cs, pList, elevLevel);
//                    if(!pList.empty())
//                        coords.push_back(pList);
//                    delete cs;
//                }
//            }
//
//        } catch (geos::util::GEOSException e)
//        {
//            AMIGO_LOG_W(TAG, "::getCoordinates() geos::util::GEOSException: '%s'\n", e.what());
//            geomClipped = geometry;
//        }
//
//        if(toDeleteGeom && geomClipped)
//            delete geomClipped;
//
//        return true;
//    }

//    bool GeometryHandler::getCoordinates(const geos::geom::Geometry *geometry, PointListList &coords, int &elevLevel)
//    {
//        if(geometry == NULL)
//        {
//            return false;
//        }
//        try
//        {
//            for(size_t i = 0; i < geometry->getNumGeometries(); i++)
//            {
//                const geos::geom::Geometry *geomN = geometry->getGeometryN(i);
//                // PointListList coordVec;
//                if(geomN->getGeometryTypeId() == geos::geom::GEOS_POLYGON)
//                {
//                    const geos::geom::Polygon *poly = dynamic_cast<const geos::geom::Polygon*>(geomN);
//                    if(poly)
//                    {
//                        geos::geom::CoordinateSequence *cs = poly->getExteriorRing()->getCoordinates();
//                        PointList pList;
//                        getCoordinates(cs, pList, elevLevel);
//                        if(!pList.empty())
//                        {
//                            coords.push_back(pList);
//                            // AMIGO_LOG_I(TAG, "::getCoordinates() polygon with %d holes\n", poly->getNumInteriorRing());
//                            for(size_t n = 0; n < poly->getNumInteriorRing(); n++)
//                            {
//                                geos::geom::CoordinateSequence *rcs = poly->getInteriorRingN(n)->getCoordinates();
//                                PointList ringList;
//                                getCoordinates(rcs, ringList, elevLevel);
//                                if(!ringList.empty())
//                                    coords.push_back(ringList);
//                                delete rcs;
//                            }
//                        }
//                        delete cs;
//                    }
//                }
//                else
//                {
//                    geos::geom::CoordinateSequence *cs = geomN->getCoordinates();
//                    PointList pList;
//                    getCoordinates(cs, pList, elevLevel);
//                    if(!pList.empty())
//                        coords.push_back(pList);
//                    delete cs;
//                }
//            }
//        } catch (geos::util::GEOSException e)
//        {
//            AMIGO_LOG_W(TAG, "::getCoordinates() geos::util::GEOSException: '%s'\n", e.what());
//        }
//
//        return true;
//    }

//    bool GeometryHandler::getCoordinates(std::vector<geos::geom::Geometry*> &geometries, geos::geom::Geometry *clipArea, PointListList &coords, int &elevLevel)
//    {
//        std::vector<geos::geom::Geometry*>::iterator it;
//        for(it = geometries.begin(); it != geometries.end(); it++)
//        {
//            const geos::geom::Geometry *g = *it;
//            const geos::geom::Geometry *geomClipped=NULL;
//            bool toDeleteGeom = false;
//            try
//            {
//                if(clipArea!=NULL)
//                {
//                    geomClipped = g->intersection( clipArea );
//                    toDeleteGeom = true;
//                } else
//                {
//                    geomClipped = g;
//                }
//
//
//                for(size_t i = 0; i < geomClipped->getNumGeometries(); i++)
//                {
//                    const geos::geom::Geometry *geomN = geomClipped->getGeometryN(i);
//                    // PointListList coordVec;
//                    if(geomN->getGeometryTypeId() == geos::geom::GEOS_POLYGON)
//                    {
//                        const geos::geom::Polygon *poly = dynamic_cast<const geos::geom::Polygon*>(geomN);
//                        if(poly)
//                        {
//                            geos::geom::CoordinateSequence *cs = poly->getExteriorRing()->getCoordinates();
//                            PointList pList;
//                            getCoordinates(cs, pList, elevLevel);
//                            if(!pList.empty())
//                            {
//                                coords.push_back(pList);
//                                // AMIGO_LOG_I(TAG, "::getCoordinates() polygon with %d holes\n", poly->getNumInteriorRing());
//                                for(size_t n = 0; n < poly->getNumInteriorRing(); n++)
//                                {
//                                    geos::geom::CoordinateSequence *rcs = poly->getInteriorRingN(n)->getCoordinates();
//                                    PointList ringList;
//                                    getCoordinates(rcs, ringList, elevLevel);
//                                    if(!ringList.empty())
//                                        coords.push_back(ringList);
//                                    delete rcs;
//                                }
//                            }
//                            delete cs;
//                        }
//                    }
//                    else
//                    {
//                        geos::geom::CoordinateSequence *cs = geomN->getCoordinates();
//                        PointList pList;
//                        getCoordinates(cs, pList, elevLevel);
//                        if(!pList.empty())
//                            coords.push_back(pList);
//                        delete cs;
//                    }
//                }
//
//            } catch (geos::util::GEOSException e)
//            {
//                AMIGO_LOG_W(TAG, "::getCoordinates() geos::util::GEOSException: '%s'\n", e.what());
//            }
//
//            if(toDeleteGeom && geomClipped)
//                delete geomClipped;
//        }
//        return true;         
//    }

//    bool GeometryHandler::getCoordinates(std::vector<geos::geom::Geometry*> &geometries, PointListList &coords, int &elevLevel)
//    {
//        try
//        {
//            std::vector<geos::geom::Geometry*>::iterator it;
//            for(it = geometries.begin(); it != geometries.end(); it++)
//            {
//                const geos::geom::Geometry *g = *it;
//                for(size_t i = 0; i < g->getNumGeometries(); i++)
//                {
//                    const geos::geom::Geometry *geomN = g->getGeometryN(i);
//                    // PointListList coordVec;
//                    if(geomN->getGeometryTypeId() == geos::geom::GEOS_POLYGON)
//                    {
//                        const geos::geom::Polygon *poly = dynamic_cast<const geos::geom::Polygon*>(geomN);
//                        if(poly)
//                        {
//                            geos::geom::CoordinateSequence *cs = poly->getExteriorRing()->getCoordinates();
//                            PointList pList;
//                            getCoordinates(cs, pList, elevLevel);
//                            if(!pList.empty())
//                            {
//                                coords.push_back(pList);
//                                // AMIGO_LOG_I(TAG, "::getCoordinates() polygon with %d holes\n", poly->getNumInteriorRing());
//                                for(size_t n = 0; n < poly->getNumInteriorRing(); n++)
//                                {
//                                    geos::geom::CoordinateSequence *rcs = poly->getInteriorRingN(n)->getCoordinates();
//                                    PointList ringList;
//                                    getCoordinates(rcs, ringList, elevLevel);
//                                    if(!ringList.empty())
//                                        coords.push_back(ringList);
//                                    delete rcs;
//                                }
//                            }
//                            delete cs;
//                        }
//                    }
//                    else
//                    {
//                        geos::geom::CoordinateSequence *cs = geomN->getCoordinates();
//                        PointList pList;
//                        getCoordinates(cs, pList, elevLevel);
//                        if(!pList.empty())
//                            coords.push_back(pList);
//                        delete cs;
//                    }
//                }
//            }
//        } catch (geos::util::GEOSException e)
//        {
//            AMIGO_LOG_W(TAG, "::getCoordinates() geos::util::GEOSException: '%s'\n", e.what());
//        }        
//        return true;        
//    }

#define _FIRST_POINT 0x01
#define _MID_POINT   0x02
#define _LAST_POINT  0x04

    static iiMath::vec3 _getMiter(const iiMath::vec3 &p0, const iiMath::vec3 &p1, const iiMath::vec3 &p2, double width, double &length)
    {
        iiMath::vec3 v0 = (p1-p0);
        v0.normalize();
        iiMath::vec3 v1 = (p2-p1);
        v1.normalize();
        iiMath::vec3 miter = iiMath::vec3(-(v0.y()+v1.y()), v0.x()+v1.x(), 0);
        miter.normalize();
        
        length = width;
        return miter;
    }

    static void _generateSegment(const iiMath::vec3 &_p0, const iiMath::vec3 &_p1, const iiMath::vec3 &_p2, double width, int mode, PointList &poly)
    {
        iiMath::vec3 p0=_p0;
        p0[2]=0;
        iiMath::vec3 p1=_p1;
        p1[2]=0;
        iiMath::vec3 p2=_p2;
        p2[2]=0;

        if(mode == _FIRST_POINT) 
        {
            iiMath::vec3 v = (p2-p1);
            double ang = atan2(v.y(), v.x())+M_PI/2;
            iiMath::vec3 p(cos(ang)*width, sin(ang)*width, 0);
            
            poly.push_back(_p1 + p);
        } else if(mode == _LAST_POINT)
        {
            iiMath::vec3 v = (p1-p0);
            double ang = atan2(v.y(), v.x())+M_PI/2;
            iiMath::vec3 p(cos(ang)*width, sin(ang)*width, 0);
            poly.push_back(_p2 + p);
        } else
        {
            if((iiMath::vec3)p1 == (iiMath::vec3)p2 || (iiMath::vec3)p0 == (iiMath::vec3)p1)
                return;

            double length;
            iiMath::vec3 miter = _getMiter(p0, p1, p2, width, length);
            iiMath::vec3 pp = miter * length;
            poly.push_back(_p1 + pp);
        }
    }

//    void GeometryHandler::convertLinesToPolygons(std::vector<geos::geom::Geometry*> &lines, PointListList &coords, PointListList &outline, geos::geom::Geometry *clipArea, double width, double outlineWidth)
//    {
//        std::vector<geos::geom::Geometry*> bufferedLines;
//        std::vector<geos::geom::Geometry*> bufferedOutLines;
//        std::vector<geos::geom::Geometry*>::iterator it;
//        try
//        {
//            for(it=lines.begin(); it!=lines.end(); it++)
//            {
//                geos::geom::Geometry* poly =  GeometryHandler::bufferGeometry(*it, width);
//                if(poly)
//                {
//                    if(clipArea)
//                    {
//                        geos::geom::Geometry *geomClipped = poly->intersection( clipArea );
//                        bufferedLines.push_back(geomClipped);
//                        delete poly;
//                    } else
//                    {
//                        bufferedLines.push_back(poly);                    
//                    }
//                }
//
//                if(outlineWidth > 0)
//                {
//                    geos::geom::Geometry* polyOutline =  GeometryHandler::bufferGeometry(*it, outlineWidth);
//                    if(polyOutline)
//                    {
//                        if(clipArea)
//                        {
//                            geos::geom::Geometry *geomClipped = polyOutline->intersection( clipArea );
//                            bufferedOutLines.push_back(geomClipped);
//                            delete polyOutline;
//                        } else
//                        {
//                            bufferedOutLines.push_back(polyOutline);                        
//                        }
//                    }
//                }
//            }
//            int elevLevel=0;
//            getCoordinates(bufferedLines, coords, elevLevel);
//
//            if(outlineWidth > 0)
//            {
//                getCoordinates(bufferedOutLines, outline, elevLevel);
//            }
//
//            std::vector<geos::geom::Geometry*>::iterator itl;
//            for(itl = bufferedLines.begin(); itl != bufferedLines.end(); itl++)
//            {
//                delete *itl;
//            }
//
//            if( !bufferedOutLines.empty() )
//            {
//                for(itl = bufferedOutLines.begin(); itl != bufferedOutLines.end(); itl++)
//                {
//                    delete *itl;
//                }
//            }
//        } catch (geos::util::GEOSException e)
//        {
//            AMIGO_LOG_W(TAG, "::convertLinesToPolygons() geos::util::GEOSException: '%s'\n", e.what());
//        }                   
//    }

//    void GeometryHandler::convertLinesToPolygons(std::vector<geos::geom::Geometry*> &lines, PointListList &coords, geos::geom::Geometry *clipArea, double width)
//    {
//        PointListList outline;
//        convertLinesToPolygons(lines, coords, outline, clipArea, width, 0);
//    }

//     std::string GeometryHandler::getGeometryType(const std::string &json)
//     {
//         Document document;
//         if(!document.Parse<0>(json.c_str()).HasParseError())
//         {
//             const Value& columns = document["columns"];
//             if(columns.IsArray())
//             {
//                 for (rapidjson::SizeType i = 0; i < columns.Size(); i++)
//                 {
//                     const Value& c = columns[i];
//                     std::string t = getJSONString(c, "geometry_type");
//                     if(!t.empty())
//                         return t;
//                 }
//             }
//         }
//         return "";
//     }

     static bool IsSpace(char c)
     {
         switch(c)
         {
         case ' ':
             return true;
         default:
             return false;
         }
     }

//     int GeometryHandler::getGeometryDimension(const std::string &json)
//     {
//         std::string geometry_type = getGeometryType(json);
//         return getGeometryDimensionByType(geometry_type);
//     }

    int GeometryHandler::getGeometryDimensionByType(std::string geometry_type)
    {
        if(!geometry_type.empty())
        {
            // Remove spaces
            geometry_type.erase(std::remove_if(geometry_type.begin(), geometry_type.end(), &IsSpace), geometry_type.end());

            if(geometry_type == "POINT" ||
               geometry_type == "LINESTRING" ||
               geometry_type == "POLYGON" ||
               geometry_type == "MULTIPOINT" ||
               geometry_type == "MULTILINESTRING" || 
               geometry_type == "MULTIPOLYGON" || 
               geometry_type == "GEOMETRYCOLLECTION" || 
               geometry_type == "GEOMETRY")
                return 2;

            if(geometry_type == "POINTZ" ||
               geometry_type == "LINESTRINGZ" ||
               geometry_type == "POLYGONZ" ||
               geometry_type == "MULTIPOINTZ" ||
               geometry_type == "MULTILINESTRINGZ" || 
               geometry_type == "MULTIPOLYGONZ" || 
               geometry_type == "GEOMETRYCOLLECTIONZ" || 
               geometry_type == "GEOMETRYZ" ||

               geometry_type == "POINTM" ||
               geometry_type == "LINESTRINGM" ||
               geometry_type == "POLYGONM" ||
               geometry_type == "MULTIPOINTM" ||
               geometry_type == "MULTILINESTRINGM" || 
               geometry_type == "MULTIPOLYGONM" || 
               geometry_type == "GEOMETRYCOLLECTIONM" || 
               geometry_type == "GEOMETRYM")
                return 3;

            if(geometry_type == "POINTZM" ||
               geometry_type == "LINESTRINGZM" ||
               geometry_type == "POLYGONZM" ||
               geometry_type == "MULTIPOINTZM" ||
               geometry_type == "MULTILINESTRINGZM" || 
               geometry_type == "MULTIPOLYGONZM" || 
               geometry_type == "GEOMETRYCOLLECTIONZM" || 
               geometry_type == "GEOMETRYZM")
                return 4;
        }
        return 0;
    }

    geos::geom::GeometryTypeId GeometryHandler::getGeometryTypeByWKTType(std::string geometry_type)
    {
        if(!geometry_type.empty())
        {
            // Remove spaces
            geometry_type.erase(std::remove_if(geometry_type.begin(), geometry_type.end(), &IsSpace), geometry_type.end());            

            if( geometry_type == "POINT" ||
                geometry_type == "POINTZ" ||
                geometry_type == "POINTM" ||
                geometry_type == "POINTZM")
                return geos::geom::GEOS_POINT;

            if( geometry_type == "LINESTRING" ||
                geometry_type == "LINESTRINGZ" ||
                geometry_type == "LINESTRINGM" ||
                geometry_type == "LINESTRINGZM")
                return geos::geom::GEOS_LINESTRING;

            if( geometry_type == "POLYGON" ||
                geometry_type == "POLYGONZ" ||
                geometry_type == "POLYGONM" ||
                geometry_type == "POLYGONZM")
                return geos::geom::GEOS_POLYGON;

            if( geometry_type == "MULTIPOINT" ||
                geometry_type == "MULTIPOINTZ" ||
                geometry_type == "MULTIPOINTM" ||
                geometry_type == "MULTIPOINTZM")
                return geos::geom::GEOS_MULTIPOINT;

            if( geometry_type == "MULTILINESTRING" || 
                geometry_type == "MULTILINESTRINGZ" || 
                geometry_type == "MULTILINESTRINGM" || 
                geometry_type == "MULTILINESTRINGZM")
                return geos::geom::GEOS_MULTILINESTRING;

            if( geometry_type == "MULTIPOLYGON" ||
                geometry_type == "MULTIPOLYGONZ" ||
                geometry_type == "MULTIPOLYGONM" ||
                geometry_type == "MULTIPOLYGONZM")
                return geos::geom::GEOS_MULTIPOLYGON;

            if( geometry_type == "GEOMETRYCOLLECTION" ||
                geometry_type == "GEOMETRYCOLLECTIONZ" ||
                geometry_type == "GEOMETRYCOLLECTIONM" ||
                geometry_type == "GEOMETRYCOLLECTIONZM")
                return geos::geom::GEOS_GEOMETRYCOLLECTION;

        }
        return geos::geom::GEOS_GEOMETRYCOLLECTION;
    }

    geos::geom::Geometry* GeometryHandler::getGeometry(geos::geom::GeometryTypeId type, int dimension, PointListList &points, bool &ret)
    {
        if(points.size()==0)
        {
            AMIGO_LOG_W(TAG, "::getGeometry() PointListList is empty\n");
            return NULL;
        }

        std::vector<geos::geom::CoordinateSequence*> coordSequences;
        PointListList::iterator it1;
        for(it1 = points.begin(); it1!=points.end(); it1++)
        {
            PointList &pl = *it1;
            if(!pl.empty())
            {
                geos::geom::CoordinateSequence *cl = new geos::geom::CoordinateArraySequence();
                PointList::iterator it;
                for(it = pl.begin(); it != pl.end(); it++)
                {
                    iiMath::vec3 &p = *it;
                    AMIGO_LOG_V(TAG, "::getGeometryWKB() add coordinate %f, %f\n", p.x(), p.y());
                    cl->add(geos::geom::Coordinate(p.y(), p.x(), p.z()));
                }
                coordSequences.push_back(cl);
            }
        }

        ret = true;    
        geos::geom::Geometry *geom=NULL;
        try {
            switch (type) {
                case geos::geom::GEOS_POINT:
                    if(coordSequences.size() > 0 && coordSequences[0]->size() == 1)
                    {
                        geom = GeometryHandler::getGeometryFactory()->createPoint(coordSequences[0]);
                    } else
                    {
                        AMIGO_LOG_W(TAG, "::getGeometryWKB() POINT could only have one coordinate\n");
                        ret = false;
                    }
                    break;
                    
                case geos::geom::GEOS_LINEARRING:
                    if(coordSequences.size() > 0)
                        geom = GeometryHandler::getGeometryFactory()->createLinearRing(coordSequences[0]);
                    break;
                    
                case geos::geom::GEOS_POLYGON:
                {
                    if(points.begin()->size() > 2 && coordSequences.size() > 0)
                    {
                        coordSequences[0]->add(geos::geom::Coordinate(points.begin()->begin()->y(), points.begin()->begin()->x(), points.begin()->begin()->z() ));
                        geos::geom::LinearRing *lr = GeometryHandler::getGeometryFactory()->createLinearRing(coordSequences[0]);
                        geom = GeometryHandler::getGeometryFactory()->createPolygon(lr, NULL);
                    }
                }
                    break;
                    
                case geos::geom::GEOS_MULTIPOINT:
                {
                    std::vector<geos::geom::Geometry*> vp;
                    for(int i = 0; i < coordSequences.size(); i++)
                    {
                        for(int j = 0; j < coordSequences[i]->getSize(); j++)
                        {
                            geos::geom::Geometry *point = GeometryHandler::getGeometryFactory()->createPoint(coordSequences[i]->getAt(j));
                            vp.push_back(point);
                        }
                        delete coordSequences[i];
                    }
                    if(!vp.empty())
                        geom = GeometryHandler::getGeometryFactory()->createMultiPoint(vp);
                }
                    break;
                    
                case geos::geom::GEOS_LINESTRING:
                    if(coordSequences.size() > 0)
                        geom = GeometryHandler::getGeometryFactory()->createLineString(coordSequences[0]);
                    break;
                    
                case geos::geom::GEOS_MULTILINESTRING:
                    if(coordSequences.size() > 0)
                        geom = GeometryHandler::createMultiLineStringGeometry( coordSequences );
                    break;
                    
                case geos::geom::GEOS_MULTIPOLYGON:
                {
                    std::vector<geos::geom::Geometry*> vp;
                    PointListList::iterator it;
                    int i = 0;
                    for(it = points.begin(); it!=points.end(); it++)
                    {
                        PointList &pl = *it;
                        if(pl.size() > 2 && coordSequences.size() > i)
                        {
                            coordSequences[i]->add(geos::geom::Coordinate(pl.begin()->y(), pl.begin()->x(), pl.begin()->z()));
                            geos::geom::LinearRing *lr = GeometryHandler::getGeometryFactory()->createLinearRing( coordSequences[i] );
                            geos::geom::Geometry *poly = GeometryHandler::getGeometryFactory()->createPolygon(lr, NULL);
                            vp.push_back(poly);
                        }
                        i++;
                    }
                    geom = GeometryHandler::getGeometryFactory()->createMultiPolygon(vp);
                }
                    break;
                    
                case geos::geom::GEOS_GEOMETRYCOLLECTION:
                    break;
                    
                default:
                    break;
            }
        } catch (geos::util::GEOSException e)
        {
            AMIGO_LOG_W(TAG, "::getGeometryWKB() '%s'\n", e.what());
            ret = false;
        }

        return geom;              
    }

    std::string GeometryHandler::getGeometryWKB(geos::geom::GeometryTypeId type, int dimension, PointListList &points)
    {
        bool ret;
        geos::geom::Geometry* geom = getGeometry(type, dimension, points, ret);
        if(geom)
        {
            if(ret)
            {
                std::string wkbOut = GeometryHandler::toWKB(geom, dimension);
                return wkbOut;
            }
            else
            {
                AMIGO_LOG_W(TAG, "::getGeometryWKB() Geometry is not valid.\n");
                ret = false;
            }
            delete geom;
        }
        return "";
    }

}



