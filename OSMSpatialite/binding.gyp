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
      "include_dirs": ["<!(node -e \"require('nan')\")", "/usr/include/libxml2"]
    }
  ]
}
