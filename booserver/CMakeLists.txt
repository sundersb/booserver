cmake_minimum_required (VERSION 3.0)
project (Booclient)

set(PROJECT_VER_MAJOR 1)
set(PROJECT_VER_MINOR 0)
set(PROJECT_VER_PATCH "1.0")

set(PROJECT_VER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")

#option (BOOTHECA_BUILD_LIBZ "Build libz library" ${WIN32})

# C++ 14 required
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED on)

set_property(GLOBAL PROPERTY COMPILE_DEFINITIONS_DEBUG "DEBUG;_DEBUG")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set (BOOSERVER_CONFIG_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(MAKE_DIRECTORY ${BOOSERVER_CONFIG_DIRECTORY})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/")  
find_package(MySQL REQUIRED)
find_package(Freetype REQUIRED)
find_package(PkgConfig REQUIRED)

#find_package(FindGStreamer)

set (BOOSERVER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/booserver.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/canvas.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/connection.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/date.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/doctor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ftype.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/image_builder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/multicast_streamer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/options.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/profile.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/provider.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/text_bitmap.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/timetable_screen_builder.cpp)
  
# Main binary
add_executable (booserver WIN32 ${BOOSERVER_SOURCES})

target_compile_definitions(booserver PRIVATE WINDOWS _WINDOWS UNICODE _UNICODE)
#target_include_directories (booserver PRIVATE ${CMAKE_SOURCE_DIR}/include)

pkg_check_modules(GST REQUIRED gstreamer-1.0
                               gstreamer-rtsp-server-1.0
                               gstreamer-video-1.0
                               gstreamer-app-1.0)

target_compile_definitions(booserver PUBLIC "$<$<CONFIG:Debug>:DEBUG>")
target_compile_options(booserver PRIVATE ${GST_CFLAGS})

# Find relevant libs
#if (WIN32)
#    if (MSVC)
#        set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
#    else (MSVC)
#        set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll" ".dll.a" ".a")
#        target_compile_options(booclient PRIVATE -Wno-deprecated-declarations)
#    endif (MSVC)
#else(WIN32)
#    set(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
#endif(WIN32)
#
#find_library(MYSQL_LIBRARY
#    NAMES mysqlclient
#    HINTS "lib"
#)

target_include_directories (booserver PRIVATE
  ${GST_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${MYSQL_INCLUDE_DIR})

target_link_libraries (booserver PRIVATE
  ${GST_LIBRARIES}
  ${FREETYPE_LIBRARY}
  ${MYSQL_LIBRARY})

message ("Gstreamer libs: ${GST_LIBRARIES}")
message ("Install prefix: ${CMAKE_INSTALL_PREFIX}")

if (WIN32)
  set (BOOSERVER_INSTALL_BINARY_DIR ".")
  set (BOOSERVER_INSTALL_CONFIG_DIR ".")
  install(FILES
    ${MYSQL_LIBRARY}
    DESTINATION ${BOOSERVER_INSTALL_BINARY_DIR}
    COMPONENT binaries)
else()
  set (BOOSERVER_INSTALL_BINARY_DIR bin)
  set (BOOSERVER_INSTALL_CONFIG_DIR share/booserver)
endif()

install (TARGETS booserver DESTINATION ${BOOSERVER_INSTALL_BINARY_DIR}
  COMPONENT binaries
  PERMISSIONS WORLD_EXECUTE GROUP_EXECUTE OWNER_READ OWNER_WRITE OWNER_EXECUTE)

install (FILES
  booserver.conf
  color_orig.conf
  color_blue.conf
  color_brown.conf
  color_green.conf
  DESTINATION ${BOOSERVER_INSTALL_CONFIG_DIR}
  COMPONENT data_files
  PERMISSIONS WORLD_READ OWNER_WRITE OWNER_READ GROUP_READ)
