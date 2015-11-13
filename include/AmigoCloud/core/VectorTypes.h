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


#ifndef __VECTORTYPES_H
#define __VECTORTYPES_H 1

#include <Chaskii/Math/vec3.h>
#include <Chaskii/Math/vec4.h>
#include <vector>
#include <list>

namespace AmigoCloud {

typedef struct vectorAttributes_t
{
	bool enable;
	float lineWidth;
	iiMath::vec4f color;
} vectorAttributes_t;


typedef struct FloatList
{
	bool     visible;
    iiMath::vec3 offset;
	float    *p;
	unsigned size;
}FloatList;

typedef std::list<iiMath::vec3f> PointListF;
typedef std::list<PointListF> PointListListF;
typedef std::list<iiMath::vec3> PointList;
typedef std::list<iiMath::vec3>* PointListPtr;
typedef std::list<PointList> PointListList;
typedef std::list<PointList>* PointListListPtr;

typedef std::vector<iiMath::vec3f> PointVectorF;
typedef std::vector<PointVectorF> PointVectorVectorF;
typedef std::vector<iiMath::vec3> PointVector;
typedef std::vector<iiMath::vec3>* PointVectorPtr;
typedef std::vector<PointVector> PointVectorVector;
typedef std::vector<PointVector>* PointVectorVectorPtr;

typedef std::vector<float> FloatVector;

typedef std::vector<FloatList> FloatListVector;
typedef std::vector<vectorAttributes_t> vAttributesVector;
typedef std::vector<vectorAttributes_t>* vAttributesVectorPtr;
}

#endif // __VECTORTYPES_H
