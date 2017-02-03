{
  "targets": [
    {
      "target_name": "allofw",
      "include_dirs": [
        "../liballofw/include",
        "../liballofw/dependencies/glew-2.0.0/include",
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [
        "glu32.lib", "opengl32.lib",
        "../../liballofw/build/lib/Release/allofw.lib",
        "../../liballofw/dependencies/glew-2.0.0/lib/Release/x64/glew32.lib",
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
        "src/node_opengl.cpp",
        "src/node_omnistereo.cpp",
        "src/gl3binding/glbind.cpp"
      ]
    }
  ]
}
