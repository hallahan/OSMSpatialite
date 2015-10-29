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


#ifndef __AMIGO_WKBPARSER_H
#define __AMIGO_WKBPARSER_H

#include <geos/export.h>

#include <geos/geom/GeometryFactory.h>

#include <iosfwd>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <ostream>
#include <istream>

namespace AmigoCloud {

class IStream
{
public:
	IStream() : _doSwap(false), _is(NULL) {}
	
	void setStream(std::istream *is)
	{
		_is = is;
	}

	void doSwap() {_doSwap = true;}

	int readInt();
	unsigned char readByte();
	double readDouble();

private:
	bool _doSwap;
	std::istream *_is;
};

class WKBParser 
{
public:
	enum
	{
		WKB_BIG_ENDIAN = 0, 
		WKB_LITTLE_ENDIAN = 1
	};

	enum 
	{
		Point_Type = 1,
		LineString_Type = 2,
		Polygon_Type = 3,
		MultiPoint_Type = 4,
		MultiLineString_Type = 5,
		MultiPolygon_Type = 6,
		GeometryCollection_Type = 7,
	};

	WKBParser();

	geos::geom::Geometry* read(std::istream &is);
	geos::geom::Geometry *readHEX(std::istream &is);
	static std::ostream &writeHEX(std::istream &is, std::ostream &os);
 
private:
    WKBParser(const WKBParser& other) : _factory(other._factory){}
    WKBParser& operator=(const WKBParser& rhs) { return *this;}

	const geos::geom::GeometryFactory &_factory;
	std::vector<double> _ordinates;
	unsigned int _dimension;

	geos::geom::Geometry *_readGeometry();
	geos::geom::Point *_readPoint();
	geos::geom::MultiPoint *_readMultiPoint();
	geos::geom::LineString *_readLineString();
	geos::geom::MultiLineString *_readMultiLineString();
	geos::geom::LinearRing *_readLinearRing();
	geos::geom::Polygon *_readPolygon();
	geos::geom::MultiPolygon *_readMultiPolygon();
	geos::geom::GeometryCollection *_readGeometryCollection();
	geos::geom::CoordinateSequence *_readCoordinateSequence(int size);
	void _readCoordinate();
	void _clean(std::vector<geos::geom::Geometry *> *g);

 	IStream _is;

};


}

#endif

