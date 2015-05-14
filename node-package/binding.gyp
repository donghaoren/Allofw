{
  "targets": [
    {
      "target_name": "allofw",
      "include_dirs": [
        "native/include",
         "<!(node -e \"require('nan')\")"
      ],
      "libraries": [
        "-L../native/lib", "-lallofw", "-lallofw-graphics"
      ],
      "cflags!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags_cc": [
        "-std=c++11"
      ],
      'conditions': [
        [ 'OS=="mac"', {
          'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11'],
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'GCC_ENABLE_CPP_RTTI': 'YES'
          },
        } ],
      ],
      "sources": [
        "src/allofw.cpp",
        "src/node_graphics.cpp",
        "src/node_sharedmemory.cpp",
        "src/node_opengl.cpp",
        "src/node_omnistereo.cpp",
        "src/gl3binding/glbind.cpp"
      ]
    }
  ]
}
