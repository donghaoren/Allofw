{
  "targets": [
    {
      "target_name": "allofw_openvr",
      "include_dirs": [
        "./openvr/headers",
        "../liballofw/include",
        "../liballofw/dependencies/windows-binaries/glew-2.0.0/include",
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [
        "glu32.lib", "opengl32.lib",
        "../openvr/lib/win64/openvr_api.lib",
        "../../liballofw/build2/lib/Release/allofw.lib",
        "../../liballofw/dependencies/windows-binaries/glew-2.0.0/lib/Release/x64/glew32.lib",
      ],
      "cflags!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags_cc": [
        "-std=c++11"
      ],
      "sources": [
        "src/allofw_openvr.cpp",
        "src/node_omnistereo.cpp",
        "src/openvr_omnistereo.cpp"
      ]
    }
  ]
}
