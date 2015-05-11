# liballofw: Framework for Immserive Multi-display Applications.
# Author: Donghao Ren, 2015

  CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )
  PROJECT ( liballofw )

#----------------------------------------------------------------------------------------------------------------------
# Configuration Parameters

  SET ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" )

  # Enable C++11 support (mandatory)
  SET ( CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}" )

  SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )

#----------------------------------------------------------------------------------------------------------------------
# Main Definitions

  # Put built libraries and binaries to bin and lib directories.
  SET ( CMAKE_BUILD_TYPE "Release" )
  SET ( EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" )
  SET ( LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" )

  FIND_PACKAGE ( PkgConfig )

  IF ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    # For linux, report error when shared libraries contains undefined references.
    SET ( CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined" )
    ADD_DEFINITIONS ( -DPLATFORM_LINUX )
  ENDIF ( )

  IF ( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    ADD_DEFINITIONS ( -DPLATFORM_MACOSX )
  ENDIF ( )

#----------------------------------------------------------------------------------------------------------------------
# Dependencies

  PKG_SEARCH_MODULE ( YAMLCPP REQUIRED yaml-cpp )
  PKG_SEARCH_MODULE ( GLFW REQUIRED glfw3 )

  # Boost C++ Libraries
  FIND_PACKAGE ( Boost REQUIRED COMPONENTS thread system )
  FIND_PACKAGE ( OpenGL REQUIRED )
  FIND_PACKAGE ( FFMPEG REQUIRED )
  FIND_PACKAGE ( GLEW REQUIRED )
  FIND_PACKAGE ( FreeImage REQUIRED )
  FIND_PACKAGE ( GLUT REQUIRED )
  FIND_PACKAGE ( Skia REQUIRED )
  FIND_PACKAGE ( Armadillo REQUIRED )

  INCLUDE_DIRECTORIES (
    ${Boost_INCLUDE_DIRS}
    ${Skia_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${YAMLCPP_INCLUDE_DIRS}
    ${ARMADILLO_INCLUDE_DIR}
  )

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

    src/graphics_skia.cpp
    src/graphics_ffmpeg.cpp

    src/logger.cpp

    src/config.cpp
    src/opengl.cpp
    src/opengl_utils.cpp
    src/omnistereo.cpp
    src/omniapp.cpp
    src/warpblend.cpp

    src/allofw.cpp
  )

  TARGET_LINK_LIBRARIES ( allofw
    LINK_PRIVATE
      ${Skia_LIBRARIES}
      ${FFMPEG_LIBRARIES}
      ${OPENGL_LIBRARIES}
      ${GLEW_LIBRARIES}
      ${GLFW_LIBRARIES}
      ${FREEIMAGE_LIBRARIES}
      ${YAMLCPP_LIBRARIES}
      ${ARMADILLO_LIBRARIES}
  )

  ADD_EXECUTABLE ( test_glfw
    src/test/test_glfw.cpp
  )

  TARGET_LINK_LIBRARIES ( test_glfw
    allofw
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
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
