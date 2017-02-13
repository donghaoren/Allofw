@mkdir win32_x64

rem Build results
copy liballofw\dependencies\skia\skia\out\Release\skia.dll win32_x64
copy liballofw\build\lib\Release\allofw.dll win32_x64
copy liballofw\build\lib\Release\allofw-graphics.dll win32_x64
copy liballofw\build\lib\Release\glfw3.dll win32_x64
copy liballofw\build\lib\Release\lodepng.dll win32_x64
copy liballofw\build\lib\Release\yaml-cpp.dll win32_x64
copy allofw.node\build\Release\allofw.node win32_x64
copy allofw-openvr.node\build\Release\allofw_openvr.node win32_x64

rem Dependencies
copy liballofw\dependencies\windows-binaries\ffmpeg-3.2.2\bin\avcodec-57.dll win32_x64
copy liballofw\dependencies\windows-binaries\ffmpeg-3.2.2\bin\avformat-57.dll win32_x64
copy liballofw\dependencies\windows-binaries\ffmpeg-3.2.2\bin\avutil-55.dll win32_x64
copy liballofw\dependencies\windows-binaries\ffmpeg-3.2.2\bin\swresample-2.dll win32_x64
copy liballofw\dependencies\windows-binaries\ffmpeg-3.2.2\bin\swscale-4.dll win32_x64
copy liballofw\dependencies\windows-binaries\glew-2.0.0\bin\Release\x64\glew32.dll win32_x64

copy allofw-openvr.node\openvr\bin\win64\openvr_api.dll win32_x64