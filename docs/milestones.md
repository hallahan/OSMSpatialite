## Short Term Milestones

- [x] Parse OSM XML into SQLite
- [x] Unit Tests
- [x] Create WKB Geometries for Nodes and Ways from the topology. RTree Index as applicable.
- [ ] Replicate OSM data model API as found in [OpenMapKit](https://github.com/AmericanRedCross/OpenMapKit/tree/master/MapboxAndroidSDK/src/main/java/com/spatialdev/osm/model).
- [ ] Design workflow for fetching, editing, and submitting edits with OSM API
- [x] Sort out getting Spatialite working properly.

## Medium Term Milestones

- [ ] Cross Platform Build Process

## Long Term Milestones

- [x] Integrate into [AmigoClient](https://github.com/amigocloud/amigoclient/)
	- [ ] Create OSM editing UX
- [ ] Hook up Mapnik (Local Proof of Concept)
	- [ ] Have Mapnik create vector tiles on iOS / Android
	- [ ] Consume vector tiles form MapboxGL Native
