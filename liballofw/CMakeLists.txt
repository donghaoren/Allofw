# liballofw: Framework for Immserive Multi-display Applications.
# Author: Donghao Ren, 2015

  CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )
  PROJECT ( liballofw )

#----------------------------------------------------------------------------------------------------------------------
# User Configuration Parameters

  SET ( WITH_SKIA   TRUE CACHE BOOL "Build with Skia.")

#----------------------------------------------------------------------------------------------------------------------
# Configuration Parameters

  SET ( CMAKE_BUILD_TYPE Release )

  SET ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" )

  # Enable C++11 support (mandatory)
  SET ( CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}" )

  # Show all warnings.
  SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )

#----------------------------------------------------------------------------------------------------------------------
# Main Definitions

  # Put built libraries and binaries to bin and lib directories.
  SET ( EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" )
  SET ( LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" )

  # Use libraries in the source directory if possible.
  SET ( ENV{PKG_CONFIG_PATH} "${CMAKE_SOURCE_DIR}/dependencies/local/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}" )
  FIND_PACKAGE ( PkgConfig )

  # OS specific configurations.
  IF ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    # For linux, report error when shared libraries contains undefined references.
    # Link c/c++ libraries statically.
    SET ( CMAKE_EXE_LINKER_FLAGS "-Wl,--no-undefined -static-libgcc -static-libstdc++" )
    SET ( CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined -static-libgcc -static-libstdc++" )
    ADD_DEFINITIONS ( -DPLATFORM_LINUX )
  ENDIF ( )

  IF ( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    ADD_DEFINITIONS ( -DPLATFORM_MACOSX )
  ENDIF ( )

#----------------------------------------------------------------------------------------------------------------------
# Dependencies

  INCLUDE_DIRECTORIES (
    BEFORE SYSTEM
      ${Boost_INCLUDE_DIRS}
      ${OPENGL_INCLUDE_DIRS}
      dependeicies/yaml-cpp/include
      dependeicies/glfw/include
      dependencies/lodepng
  )

  ADD_LIBRARY ( lodepng
    dependencies/lodepng/lodepng.cpp
  )

  SET ( GLFW_BUILD_EXAMPLES OFF )
  SET ( GLFW_BUILD_TESTS OFF )
  SET ( GLFW_BUILD_DOCS  OFF )
  SET ( GLFW_INSTALL OFF )
  SET ( GLFW_DOCUMENT_INTERNALS OFF )
  ADD_SUBDIRECTORY ( dependencies/glfw )

  # Boost C++ Libraries (we use headers only).
  FIND_PACKAGE ( Boost REQUIRED )
  FIND_PACKAGE ( OpenGL REQUIRED )

  FILE ( GLOB YAML_CPP_SOURCES "dependencies/yaml-cpp/src/[a-zA-Z]*.cpp")
  ADD_LIBRARY ( yaml-cpp
    ${YAML_CPP_SOURCES}
  )

  IF ( ${WITH_SKIA} )
    FIND_PACKAGE ( Skia REQUIRED )
    PKG_CHECK_MODULES ( FFMPEG REQUIRED libavcodec libavutil libavformat libswscale )
    INCLUDE_DIRECTORIES (
      ${Skia_INCLUDE_DIRS}
      ${FFMPEG_INCLUDE_DIR}
    )
  ENDIF ( )

#----------------------------------------------------------------------------------------------------------------------
# Included Dependencies

#----------------------------------------------------------------------------------------------------------------------
# Global Include Directories

  INCLUDE_DIRECTORIES ( include )

#----------------------------------------------------------------------------------------------------------------------
# Skia Graphics Backend Library

  ADD_LIBRARY ( allofw SHARED
    src/stream.cpp
    src/math.cpp
    src/config.cpp
    src/logger.cpp
    src/opengl.cpp
    src/opengl_utils.cpp
    src/omnistereo.cpp
    src/warpblend.cpp
    src/allofw.cpp
  )

  TARGET_LINK_LIBRARIES ( allofw
    LINK_PRIVATE
      ${OPENGL_LIBRARIES}
      ${GLEW_LIBRARIES}
      ${FREEIMAGE_LIBRARIES}
      ${GLFW_LIBRARIES}
      lodepng
      yaml-cpp
      pthread
  )

  IF ( ${WITH_SKIA} )

    ADD_LIBRARY ( allofw-graphics SHARED
      src/graphics_skia.cpp
      src/graphics_ffmpeg.cpp
    )

    TARGET_LINK_LIBRARIES ( allofw-graphics
      LINK_PRIVATE
        allofw
        ${Skia_LIBRARIES}
        ${FFMPEG_LDFLAGS}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW_LDFLAGS}
        ${FREEIMAGE_LIBRARIES}
        ${YAMLCPP_LDFLAGS}
    )

    INSTALL (
      TARGETS
        allofw-graphics
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib
    )
  ENDIF ( )

#----------------------------------------------------------------------------------------------------------------------
# Samples

  FUNCTION(ADD_SAMPLE_EXECUTABLE sample_name sample_sources)
      ADD_EXECUTABLE ( ${sample_name} samples/${sample_sources} )
      TARGET_LINK_LIBRARIES ( ${sample_name}
        allofw
        ${OPENGL_LIBRARIES}
      )
      SET_TARGET_PROPERTIES ( ${sample_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/samples"
      )
  ENDFUNCTION(ADD_SAMPLE_EXECUTABLE)

#----------------------------------------------------------------------------------------------------------------------
# PKG-Config File

  CONFIGURE_FILE ( liballofw.pc.in
    ${LIBRARY_OUTPUT_PATH}/pkgconfig/liballofw.pc
    @ONLY
  )

#----------------------------------------------------------------------------------------------------------------------
# Install

  INSTALL (
    DIRECTORY
      include/allofw
    DESTINATION include
  )

  # Targets.
  INSTALL (
    TARGETS
      allofw
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )

  INSTALL (
    FILES
      ${LIBRARY_OUTPUT_PATH}/pkgconfig/liballofw.pc
    DESTINATION
      lib/pkgconfig
  )
