{
  "targets": [
    {
      "target_name": "node_phasespace",
      "include_dirs": [
        "include",
        "<!@(pkg-config liballofw --cflags-only-I | sed s/-I//g)",
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [
      ],
      "cflags_cc": [
        "-std=c++11"
      ],
      'conditions': [
        [ 'OS=="mac"', {
          'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11']
          },
        } ],
      ],
      "sources": [
        "src/node_phasespace.cpp",
        "src/phasespace.cpp"
      ]
    },
  ]
}
