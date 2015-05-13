# liballofw: Framework for Immserive Multi-display Applications.
# Author: Donghao Ren, 2015

  CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )
  PROJECT ( liballofw )

#----------------------------------------------------------------------------------------------------------------------
# User Configuration Parameters

  SET ( WITH_SKIA   FALSE CACHE BOOL "Build with Skia.")


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
    SET ( CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined -static-libgcc -static-libstdc++" )
    ADD_DEFINITIONS ( -DPLATFORM_LINUX )
  ENDIF ( )

  IF ( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    ADD_DEFINITIONS ( -DPLATFORM_MACOSX )
  ENDIF ( )

#----------------------------------------------------------------------------------------------------------------------
# Dependencies

  PKG_SEARCH_MODULE ( YAMLCPP REQUIRED yaml-cpp )
  PKG_SEARCH_MODULE ( GLFW REQUIRED glfw3 )
  PKG_SEARCH_MODULE ( ZeroMQ REQUIRED libzmq )

  # Boost C++ Libraries
  FIND_PACKAGE ( Boost REQUIRED COMPONENTS thread system )
  FIND_PACKAGE ( OpenGL REQUIRED )
  FIND_PACKAGE ( FFMPEG REQUIRED )
  FIND_PACKAGE ( GLEW REQUIRED )
  FIND_PACKAGE ( FreeImage REQUIRED )
  FIND_PACKAGE ( GLUT REQUIRED )
  FIND_PACKAGE ( Armadillo REQUIRED )

  INCLUDE_DIRECTORIES (
    ${Boost_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${YAMLCPP_INCLUDE_DIRS}
    ${ARMADILLO_INCLUDE_DIR}
  )

  IF ( ${WITH_SKIA} )
    FIND_PACKAGE ( Skia REQUIRED )
    INCLUDE_DIRECTORIES (
      ${Skia_INCLUDE_DIRS}
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
      ${FFMPEG_LIBRARIES}
      ${OPENGL_LIBRARIES}
      ${GLEW_LIBRARIES}
      ${GLFW_LIBRARIES}
      ${FREEIMAGE_LIBRARIES}
      ${YAMLCPP_LIBRARIES}
      ${ARMADILLO_LIBRARIES}
      pthread
  )

  ADD_LIBRARY ( allofw-omniapp SHARED
    src/omniapp.cpp
    src/omniapp_broadcasting.cpp
    src/omniapp_broadcasting_socket.cpp
  )

  TARGET_LINK_LIBRARIES ( allofw-omniapp
    LINK_PRIVATE
      allofw
      ${ZeroMQ_LIBRARIES}
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
        ${FFMPEG_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${FREEIMAGE_LIBRARIES}
        ${YAMLCPP_LIBRARIES}
        ${ARMADILLO_LIBRARIES}
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
        allofw-omniapp
        ${OPENGL_LIBRARIES}
      )
      SET_TARGET_PROPERTIES ( ${sample_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/samples"
      )
  ENDFUNCTION(ADD_SAMPLE_EXECUTABLE)

  ADD_SAMPLE_EXECUTABLE ( cubes cubes.cpp )
  ADD_SAMPLE_EXECUTABLE ( cubes_bcast cubes_bcast.cpp )

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
      allofw-omniapp
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )
