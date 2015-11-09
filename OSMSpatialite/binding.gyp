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
              '../lib/libcharset.1.dylib',
              '../lib/libgeos-3.4.2.dylib',
              '../lib/libgeos_c.1.dylib',
              '../lib/libiconv.2.dylib',
              '../lib/libproj.9.dylib',
              '../lib/libspatialite.7.dylib',
              '../lib/libsqlite3.0.dylib',
              '../lib/libxml2.2.dylib',
              '../lib/libz.1.dylib'
          ]
      }
    }
  ]
}
