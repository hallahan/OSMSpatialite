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


#include <common/WKBParser.h>

#include <geos/geom/Coordinate.h>
#include <geos/geom/Point.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/MultiPoint.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/PrecisionModel.h>

#include <core/GeometryHandler.h>

#include <core/Logger.h>

#include <string>

namespace AmigoCloud {

    static std::string TAG("WKBParser");

#define WKB_BAD_GEOMETRY_TYPE "Bad geometry type encountered in"

template <typename T> T swap_endian(T u)
{
    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

int IStream::readInt()
{
	int out;
	_is->read(reinterpret_cast<char*>(&out), sizeof(int));
	if(_doSwap)
	{
		return swap_endian<int>(out);
	}
	return out;
}

unsigned char IStream::readByte()
{
	unsigned char out;
	_is->read(reinterpret_cast<char*>(&out), sizeof(unsigned char));
	return out;
}

double IStream::readDouble()
{
	double out;
	_is->read(reinterpret_cast<char*>(&out), sizeof(double));
	if(_doSwap)
	{
		return swap_endian<double>(out);
	}
	return out;
}

WKBParser::WKBParser() :
	_factory(*GeometryHandler::getGeometryFactory())
{}

std::ostream &WKBParser::writeHEX(std::istream &is, std::ostream &os)
{
	static const char hexHalfs[] = "0123456789ABCDEF";

    std::streampos pos = is.tellg(); 
	is.seekg(0, std::ios::beg); 

	char ch=0;
	while(is.read(&ch, 1))
	{
		const unsigned char c=ch;
		int low = (c & 0x0F);
		int high = (c >> 4);
		os << hexHalfs[high] << hexHalfs[low];
	}

	is.clear(); 
	is.seekg(pos);

	return os;
}

geos::geom::Geometry *WKBParser::_readGeometry()
{
	unsigned char byteOrdr = _is.readByte();
    if (byteOrdr == WKB_BIG_ENDIAN)
        _is.doSwap();

	int typeInt = _is.readInt();
	int geomType = typeInt & 0xff;

	bool zPresent = ((typeInt & 0x80000000) != 0);
	if (zPresent) _dimension = 3;
	else _dimension = 2; // TODO: handle M values

	int srid = 0;
	bool sridPresent = ((typeInt & 0x20000000) != 0);
	if (sridPresent) srid = _is.readInt(); 

	if ( _ordinates.size() < _dimension )
		_ordinates.resize(_dimension);

	geos::geom::Geometry *geom=NULL;

	switch (geomType) {
		case Point_Type :
			geom = _readPoint();
		break;
		
		case MultiPoint_Type :
			geom = _readMultiPoint();
		break;
		
		case LineString_Type :
			geom = _readLineString();
		break;
		
		case MultiLineString_Type :
			geom = _readMultiLineString();
		break;
		
		case Polygon_Type :
			geom = _readPolygon();
		break;
		
		case MultiPolygon_Type :
			geom = _readMultiPolygon();
		break;
		
		case GeometryCollection_Type :
			geom = _readGeometryCollection();
		break;
		
		default:
			AMIGO_LOG_E(TAG, "::_readGeometry() Unknown WKB type (%d)\n", geomType);
			return NULL;
	}
	if(geom) geom->setSRID(srid);
	return geom;
}

geos::geom::Point *WKBParser::_readPoint()
{
	_readCoordinate();
	if(_dimension==3)
	{
	  return _factory.createPoint(geos::geom::Coordinate(_ordinates[0], _ordinates[1], _ordinates[2]));
	} else
	{
	  return _factory.createPoint(geos::geom::Coordinate(_ordinates[0], _ordinates[1]));
	}
}

geos::geom::MultiPoint *WKBParser::_readMultiPoint()
{
	int count = _is.readInt();
	std::vector<geos::geom::Geometry *> *geoms = new std::vector<geos::geom::Geometry *>(count);

	try {
		for (int i=0; i<count; i++)
		{
			geos::geom::Geometry *g = _readGeometry();
			if (!dynamic_cast<geos::geom::Point *>(g))
			{
				AMIGO_LOG_E(TAG, "::_readMultiPoint() Invalid geometry type\n");
				_clean(geoms);
				return NULL;
			}
			(*geoms)[i] = g;
		}
	} catch (...) 
	{
		_clean(geoms);
		AMIGO_LOG_E(TAG, "::_readMultiPoint() Read failed.\n");
		return NULL;
	}
	return _factory.createMultiPoint(geoms);
}

geos::geom::LineString *WKBParser::_readLineString()
{
	int size = _is.readInt();

	geos::geom::CoordinateSequence *pts = _readCoordinateSequence(size);
	return _factory.createLineString(pts);
}

geos::geom::MultiLineString *WKBParser::_readMultiLineString()
{
	int count = _is.readInt();
	std::vector<geos::geom::Geometry *> *geoms = new std::vector<geos::geom::Geometry *>(count);

	try {
		for (int i=0; i<count; i++)
		{
			geos::geom::Geometry *g = _readGeometry();
			if (!dynamic_cast<geos::geom::LineString *>(g))
			{
				AMIGO_LOG_E(TAG, "::_readMultiLineString() Invalid geometry type\n");
				_clean(geoms);
				return NULL;
			}
			(*geoms)[i] = g;
		}
	} catch (...) 
	{
		_clean(geoms);
		AMIGO_LOG_E(TAG, "::_readMultiLineString() Read failed.\n");
		return NULL;
	}
	return _factory.createMultiLineString(geoms);
}

geos::geom::LinearRing *WKBParser::_readLinearRing()
{
	int size = _is.readInt();

	geos::geom::CoordinateSequence *pts = _readCoordinateSequence(size);
	return _factory.createLinearRing(pts);
}

geos::geom::Polygon *WKBParser::_readPolygon()
{
	int count = _is.readInt();

    geos::geom::LinearRing *rind = NULL;
	std::vector<geos::geom::Geometry *> *holes=NULL;
	if ( count > 0 )
	{
		try {
		    if( count > 0 )
		    {
                rind = _readLinearRing();
		    }
			holes = new std::vector<geos::geom::Geometry *>(count-1);
			for (int i=0; i<count-1; i++)
			{
			    geos::geom::Geometry *g = (geos::geom::Geometry *)_readLinearRing();
				(*holes)[i] = g;
            }
		} catch (...) 
		{
		    if(holes)
		    {
				_clean(holes);
			}
            if(rind)
            {
			    delete rind;
            }
			AMIGO_LOG_E(TAG, "::_readPolygon() Read failed.\n");
            return NULL;
		}
	}
	return _factory.createPolygon(rind, holes);
}

geos::geom::MultiPolygon *WKBParser::_readMultiPolygon()
{
	int count = _is.readInt();
	std::vector<geos::geom::Geometry *> *geoms = new std::vector<geos::geom::Geometry *>(count);

	try {
		for (int i=0; i<count; i++)
		{
			geos::geom::Geometry *g = _readGeometry();
			if (!dynamic_cast<geos::geom::Polygon *>(g))
			{
				AMIGO_LOG_E(TAG, "::_readMultiPolygon() Invalid geometry type\n");
				_clean(geoms);
				return NULL;
			}
			(*geoms)[i] = g;
		}
	} catch (...) 
	{
		AMIGO_LOG_E(TAG, "::_readMultiPolygon() Read failed.\n");
		_clean(geoms);
		return NULL;
	}
	return _factory.createMultiPolygon(geoms);
}

geos::geom::GeometryCollection *WKBParser::_readGeometryCollection()
{
	int count = _is.readInt();
	std::vector<geos::geom::Geometry *> *geoms = new std::vector<geos::geom::Geometry *>(count);

	try {
		for (int i=0; i<count; i++)
		{
			geos::geom::Geometry *g = _readGeometry();
			if(g)
			{
				(*geoms)[i] = g;
			} else
			{
				AMIGO_LOG_E(TAG, "::_readGeometryCollection() Read failed.\n");
				_clean(geoms);
				return NULL;				
			}
		}
	} catch (...) 
	{
		_clean(geoms);
		return NULL;
	}
	return _factory.createGeometryCollection(geoms);
}

geos::geom::CoordinateSequence *WKBParser::_readCoordinateSequence(int size)
{
	geos::geom::CoordinateSequence *cs = _factory.getCoordinateSequenceFactory()->create(size, _dimension);
	unsigned long dim = cs->getDimension();
	if ( dim > _dimension )
		dim = _dimension;
	for (int i=0; i<size; i++) {
		_readCoordinate();
		for (unsigned int j=0; j<dim; j++) {
			cs->setOrdinate(i, j, _ordinates[j]);
		}
	}
	return cs;
}

void WKBParser::_readCoordinate()
{
	const geos::geom::PrecisionModel &pm = *_factory.getPrecisionModel();
	for (unsigned int i=0; i<_dimension; ++i)
	{
		if ( i <= 1 ) _ordinates[i] = pm.makePrecise(_is.readDouble());
		else _ordinates[i] = _is.readDouble();
	}

}

void WKBParser::_clean(std::vector<geos::geom::Geometry *> *g)
{
	for (unsigned int i=0; i<g->size(); i++)
	{
		delete (*g)[i];
	}
	delete g;
}

geos::geom::Geometry *WKBParser::read(std::istream &is)
{
	_is.setStream(&is); 
	return _readGeometry();
}

geos::geom::Geometry *WKBParser::readHEX(std::istream &is)
{
	unsigned char h;
	unsigned char l;
	unsigned char r_h;
	unsigned char r_l, v;
	std::stringstream os(std::ios_base::binary|std::ios_base::in|std::ios_base::out);

	while(!is.eof())
	{
		is >> h;
		is >> l;

		switch (h)
		{
			case '0':
				r_h = 0;
			break;
			
			case '1':
				r_h = 1;
			break;
			
			case '2':
				r_h = 2;
			break;
			
			case '3':
				r_h = 3;
			break;
			
			case '4':
				r_h = 4;
			break;
			
			case '5':
				r_h = 5;
			break;
			
			case '6':
				r_h = 6;
			break;
			
			case '7':
				r_h = 7;
			break;
			
			case '8':
				r_h = 8;
			break;
			
			case '9':
				r_h = 9;
			break;
			
			case 'A':
            case 'a':
				r_h = 10;
			break;
			
			case 'B':
            case 'b':
				r_h = 11;
			break;
			
			case 'C':
			case 'c':
				r_h = 12;
			break;
			
			case 'D':
			case 'd':
				r_h = 13;
			break;
			
			case 'E':
			case 'e':
				r_h = 14;
			break;
			
			case 'F':
			case 'f':
				r_h = 15;
			break;

			default:
				return NULL;
		}

		switch (l)
		{
			case '0':
				r_l = 0;
			break;
			
			case '1':
				r_l = 1;
			break;
			
			case '2':
				r_l = 2;
			break;
			
			case '3':
				r_l = 3;
			break;
			
			case '4':
				r_l = 4;
			break;
			
			case '5':
				r_l = 5;
			break;
			
			case '6':
				r_l = 6;
			break;
			
			case '7':
				r_l = 7;
			break;
			
			case '8':
				r_l = 8;
			break;
			
			case '9':
				r_l = 9;
			break;
			
			case 'A':
            case 'a':
				r_l = 10;
			break;
			
			case 'B':
            case 'b':
				r_l = 11;
			break;
			
			case 'C':
			case 'c':
				r_l = 12;
			break;
			
			case 'D':
			case 'd':
				r_l = 13;
			break;
			
			case 'E':
			case 'e':
				r_l = 14;
			break;

			case 'F':
			case 'f':
				r_l = 15;
			break;

			default:
				return NULL;
		}
		v = (r_h<<4) + r_l;
		os << v;
	}

	return read(os);
}

}