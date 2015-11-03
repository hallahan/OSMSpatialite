This is a cherry-picked subset of classes found in [AmigoClient](https://github.com/amigocloud/amigoclient). Some functions and includes are commented out to reduce adding extra dependencies into the OSMSpatialite project. The API (functions) interacting with these AmigoClient classes remains unaltered. All of our usage with AmigoClient should work unchanged with the main library when used upstream.

The `#include` paths have been changed. We are using relative paths to files, and this will have to be altered when merged upstream into AmigoClient.
