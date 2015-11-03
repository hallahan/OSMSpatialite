# OSMSpatialite

OSMSpatialite is a C++ data model bridging OSM data and Spatialite. The goal of this project is to parse OSM XML, put it in a Spatialite database with a schema that makes sense, and write it back out as OSM XML to POST updates to the OSM API.

## Setup

This project is intended to work on 1) Android, 2) iOS, 3) Intel Edison (Yocto Linux), and 4) Mac OS X. I have not yet created a Makefile, and it is currently made to build only on Mac OS X.

I have included the applicable XCode project files and library dependencies, so you should be able to clone this repo, open it in XCode, and _press play_.

I have gotten it to run without hitches on XCode 6.4 and 7.0.

## Short Term Milestones

- [x] Parse OSM XML into SQLite
- [ ] Unit Tests
- [x] Create WKB Geometries for Nodes and Ways from the topology. RTree Index as applicable.
- [ ] Replicate OSM data model API as found in [OpenMapKit](https://github.com/AmericanRedCross/OpenMapKit/tree/master/MapboxAndroidSDK/src/main/java/com/spatialdev/osm/model).
- [ ] Design workflow for fetching, editing, and submitting edits with OSM API
- [x] Sort out getting Spatialite working properly.

## Medium Term Milestones

- [ ] Cross Platform Build Process

## Long Term Milestones

- [ ] Integrate into [AmigoClient](https://github.com/amigocloud/amigoclient/)
	- [ ] Create OSM editing UX
- [ ] Hook up Mapnik (Local Proof of Concept)
	- [ ] Have Mapnik create vector tiles on iOS / Android
	- [ ] Consume vector tiles form MapboxGL Native
