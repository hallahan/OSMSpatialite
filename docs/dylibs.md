# Building Dynamic Libraries in Xcode

To run OSMSpatialite in XCode, we have included the required dependencies as `.dylib`. To make these work, we have to use `install_name_tool` so that the dylibs points to the right spot internally, both it's own path as well as the paths to the dependencies of that dylib.

To see where the dylib and its dependencies point:
```
otool -L <lib_name.dylib>
otool -L libgeos-3.4.2.dylib
```

To change the location of the dylib itself:
```
install_name_tool -id <new_path> <lib_name.dylib>
install_name_tool -id /Users/njh/code/OSMSpatialite/OSMSpatialite/lib/libspatialite.7.dylib libspatialite.7.dylib
```

To change the location of the dependencies in the dylib:
```
install_name_tool -change <old_path> <new_path> <lib_name.dylib>
install_name_tool -change /usr/local/Cellar/geos/3.4.2/lib/libgeos-3.4.2.dylib /Users/njh/code/OSMSpatialite/OSMSpatialite/lib/libgeos-3.4.2.dylib libgeos_c.1.dylib
```

You can also trick Xcode to just look in the right directory and override what is found in the dylib paths. Set your `DYLD_LIBRARY_PATH` environment variable to the path of the OSMSpatialite libs directory.

![](https://cloud.githubusercontent.com/assets/556367/11103411/1b1ed4ec-8877-11e5-884b-caa27c0c640e.png)
```
export DYLD_LIBRARY_PATH="/Users/njh/code/OSMSpatialite/OSMSpatialite/lib"
```