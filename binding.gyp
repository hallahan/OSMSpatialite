{
  "targets": [
    {
      "target_name": "osmspatialite",
      "sources": [
        "src/nantry/addon.cc",
        "src/nantry/pi_est.cc",
        "src/nantry/sync.cc",
        "src/nantry/async.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")", "lib/include"],
      'link_settings': {
          'libraries': [
              '-lcurl',
              '../lib/osx/libfreexl.1.dylib',
              '../lib/osx/libgeos-3.4.2.dylib',
              '../lib/osx/libgeos_c.1.dylib',
              '../lib/osx/libproj.9.dylib',
              '../lib/osx/libspatialite.7.dylib',
              '../lib/osx/libsqlite3.0.dylib',
              '../lib/osx/libxml2.2.dylib'
          ]
      }
    }
  ]
}
