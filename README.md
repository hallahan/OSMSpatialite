# OSMSpatialite

OSMSpatialite is a C++ data model bridging OSM data and Spatialite. The goal of this project is to parse OSM XML, put it in a Spatialite database with a schema that makes sense, and write it back out as OSM XML to POST updates to the OSM API. This library is intended to be integrated into Android / iOS mapping applications as well as a NodeJS REST API.

This project is being developed for the needs of [OpenMapKit](https://github.com/americanredcross/openmapkit).

## Setup

This project is intended to work on 1) Android, 2) iOS, 3) Intel Edison (Yocto Linux), 4) Raspberry Pi (Ubuntu), and 5) Mac OS X. Currently, it runs on OSX in XCode and also as a NodeJS module via `node-gyp`.

### NodeJS

In the main OSMSpatialite directory:

```
sudo npm install -g node-gyp
npm install
node-gyp rebuild
```

Then, execute the node module:

```
node index.js
```

### XCode

You can run the unit tests in Xcode. Open `OSMSpatialite.xcodeproj` in Xcode. Next to the Play button in the upper right, you must edit your scheme. Click on `Edit Scheme`. Then, in that dialog, you need to set your `DYLD_LIBRARY_PATH` environment variable to the path of the OSMSpatialite libs directory.

![](https://cloud.githubusercontent.com/assets/556367/11103411/1b1ed4ec-8877-11e5-884b-caa27c0c640e.png)

Now, all you have to do is press _play_ in Xcode. I have gotten it to run without hitches on Xcode 6.4 and 7.0.

---

To see where things are going, please refer to the [project milestones](docs/milestones.md) document.