cmake_minimum_required (VERSION 3.0)
project (Booclient)

set(PROJECT_VER_MAJOR 1)
set(PROJECT_VER_MINOR 0)
set(PROJECT_VER_PATCH "1.0")

set(PROJECT_VER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")

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

include(FindPkgConfig)
if (NOT PKG_CONFIG_FOUND)
  message(FATAL_ERROR "pkg-config not found")
endif()

find_package(MySQL REQUIRED)
find_package(Freetype REQUIRED)
find_package(PkgConfig REQUIRED)

set (BOOSERVER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/booserver.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/canvas.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/connection.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/date.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/doctor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ftype.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/image_builder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/streamer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/http_streamer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rtsp_streamer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/options.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/png_files.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/profile.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/provider.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/text_bitmap.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/timetable_screen_builder.cpp)
  
# Main binary
add_executable (booserver ${BOOSERVER_SOURCES})

if (WIN32)
  target_compile_definitions(booserver PRIVATE
    WINDOWS
    _WINDOWS
    UNICODE
    _UNICODE)
endif()

#target_include_directories (booserver PRIVATE ${CMAKE_SOURCE_DIR}/include)

pkg_check_modules(LIBPNG libpng16 REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0
                               gstreamer-rtsp-server-1.0
                               gstreamer-video-1.0
                               gstreamer-app-1.0)

target_compile_definitions(booserver PUBLIC "$<$<CONFIG:Debug>:DEBUG>")
target_compile_options(booserver PRIVATE ${GST_CFLAGS})

target_include_directories (booserver PRIVATE
  ${GST_INCLUDE_DIRS}
  ${LIBPNG_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${MYSQL_INCLUDE_DIR})

target_link_libraries (booserver PRIVATE
  ${GST_LIBRARIES}
  ${LIBPNG_LIBRARIES}
  ${FREETYPE_LIBRARY}
  ${MYSQL_LIBRARY})

#message ("Install prefix: ${CMAKE_INSTALL_PREFIX}")
message ("Gstreamer libs: ${GST_LIBRARIES}")
message ("PNG libs: ${LIBPNG_LIBRARIES}")

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
  color_grey.conf
  color_slate.conf
  color_pale_blue.conf
  DESTINATION ${BOOSERVER_INSTALL_CONFIG_DIR}
  COMPONENT data_files
  PERMISSIONS WORLD_READ OWNER_WRITE OWNER_READ GROUP_READ)
