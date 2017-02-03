# Skia as graphics backend.
# Together with its dependencies.
SET ( Skia_SOURCE_DIR ${CMAKE_SOURCE_DIR}/dependencies/skia/skia )

SET ( Skia_INCLUDE_DIRS
  "${Skia_SOURCE_DIR}/include/config"
  "${Skia_SOURCE_DIR}/include/core"
  "${Skia_SOURCE_DIR}/include/images"
  "${Skia_SOURCE_DIR}/include/effects"
  "${Skia_SOURCE_DIR}/include/gpu"
)

SET ( Skia_LIBRARY_DIRS "${Skia_SOURCE_DIR}/out/Release" )

# With skia branch: chrome/m50
SET ( Skia_LIBRARIES
  "${Skia_LIBRARY_DIRS}/libskia.so"
)

# IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#     FIND_LIBRARY(ApplicationServices_LIBRARY ApplicationServices )
#     LIST ( APPEND Skia_LIBRARIES
#         "${Skia_LIBRARY_DIRS}/libicuuc.a"
#         "${Skia_LIBRARY_DIRS}/libsfntly.a"
#         "${Skia_LIBRARY_DIRS}/libetc1.a"
#         "${Skia_LIBRARY_DIRS}/libzlib.a"
#         "${Skia_LIBRARY_DIRS}/libgiflib.a"
#         "${Skia_LIBRARY_DIRS}/libdng_sdk.a"
#         "${Skia_LIBRARY_DIRS}/libpiex.a"
#         "${Skia_LIBRARY_DIRS}/libraw_codec.a"
#         "${Skia_LIBRARY_DIRS}/libSkKTX.a"
#         "${Skia_LIBRARY_DIRS}/libpng_static.a"
#         "${Skia_LIBRARY_DIRS}/libjpeg-turbo.a"
#         "${Skia_LIBRARY_DIRS}/libwebp_enc.a"
#         "${Skia_LIBRARY_DIRS}/libwebp_dec.a"
#         "${Skia_LIBRARY_DIRS}/libwebp_utils.a"
#         "${Skia_LIBRARY_DIRS}/libwebp_dsp.a"
#         "${Skia_LIBRARY_DIRS}/libwebp_dsp_enc.a"
#         "${Skia_LIBRARY_DIRS}/libwebp_demux.a"
#         ${ApplicationServices_LIBRARY}
#     )
#   ENDIF ( )
#
#   IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#     PKG_CHECK_MODULES ( FreeType freetype2 )
#     PKG_CHECK_MODULES ( FontConfig fontconfig )
#     FIND_PACKAGE ( OpenGL )
#     FIND_PACKAGE ( Threads )
#     LIST ( APPEND Skia_LIBRARIES
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libicuuc.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libsfntly.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libetc1.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libzlib.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libgiflib.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libSkKTX.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libraw_codec.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libdng_sdk.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libpiex.a"
#         "${Skia_LIBRARY_DIRS}/libpng_static.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libjpeg-turbo.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libwebp_enc.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libwebp_dec.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libwebp_utils.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libwebp_dsp.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libwebp_dsp_enc.a"
#         "${Skia_LIBRARY_DIRS}/obj/gyp/libwebp_demux.a"
#         -lrt
#         ${FreeType_LIBRARIES}
#         ${FontConfig_LIBRARIES}
#         ${OPENGL_LIBRARIES}
#         ${CMAKE_THREAD_LIBS_INIT}
#     )
#     # In Linux, to avoid circular dependency problem, add start-group and end-group for Skia static files.
#     SET ( Skia_LIBRARIES -Wl,--start-group ${Skia_LIBRARIES} -Wl,--end-group )
#   ENDIF ( )
#