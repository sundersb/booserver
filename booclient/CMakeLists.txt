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

set (BOOCLIENT_CONFIG_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(MAKE_DIRECTORY ${BOOCLIENT_CONFIG_DIRECTORY})

# Add Windows resources file
if (WIN32)
  set (BOOCLIENT_RESOURCES ${CMAKE_SOURCE_DIR}/win_resources.rc)
else (WIN32)
  set (BOOCLIENT_RESOURCES ${CMAKE_SOURCE_DIR}/buttons.cpp)
endif (WIN32)

# Select wxWidgets debug or release configuration
if (CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  set (wxWidgets_CONFIGURATION mswud)
else ()
  set (wxWidgets_CONFIGURATION mswu)
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/")  
find_package(MySQL)

# wxWidgets libraries. Order matters! These libs are not required: png jpeg tiff zlib regexp expat
find_package (wxWidgets COMPONENTS media net qa xml adv html core base REQUIRED)

set (BOOCLIENT_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/booclient.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/chart.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/connection.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/date.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/dlg_login.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/doctor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/gui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/profile.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/repository.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rule.cpp)
  
# Main binary
add_executable (booclient WIN32
  ${BOOCLIENT_RESOURCES};
  ${BOOCLIENT_SOURCES})

include (${wxWidgets_USE_FILE})

# Compile Bootheca UNICODE
target_compile_definitions(booclient PRIVATE WINDOWS _WINDOWS UNICODE _UNICODE)
#target_include_directories (booclient PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_compile_definitions(booclient PUBLIC "$<$<CONFIG:Debug>:DEBUG>")
target_compile_options(booclient PRIVATE -Wno-deprecated-declarations)

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

target_include_directories (booclient PRIVATE ${MYSQL_INCLUDE_DIR})

target_link_libraries (booclient PRIVATE
  ${wxWidgets_LIBRARIES}
  ${MYSQL_LIBRARY})

message ("Install prefix: ${CMAKE_INSTALL_PREFIX}")

if (WIN32)
  set (BOOCLIENT_INSTALL_BINARY_DIR ".")
  set (BOOCLIENT_INSTALL_CONFIG_DIR ".")
  install(FILES
    ${MYSQL_LIBRARY}
    DESTINATION ${BOOCLIENT_INSTALL_BINARY_DIR}
    COMPONENT binaries)
else()
  set (BOOCLIENT_INSTALL_BINARY_DIR bin)
  set (BOOCLIENT_INSTALL_CONFIG_DIR share/booclient)
  install (FILES bootheca/resource/booclient.desktop
    DESTINATION share/applications COMPONENT data_files
    PERMISSIONS WORLD_READ OWNER_WRITE OWNER_READ GROUP_READ)
  install (FILES icon.png
    DESTINATION share/pixmaps COMPONENT data_files
    PERMISSIONS WORLD_READ OWNER_WRITE OWNER_READ GROUP_READ)
endif()

install (TARGETS booclient DESTINATION ${BOOCLIENT_INSTALL_BINARY_DIR}
  COMPONENT binaries
  PERMISSIONS WORLD_EXECUTE GROUP_EXECUTE OWNER_READ OWNER_WRITE OWNER_EXECUTE)

install (FILES
  booclient.xml
  DESTINATION ${BOOCLIENT_INSTALL_CONFIG_DIR}
  COMPONENT data_files
  PERMISSIONS WORLD_READ OWNER_WRITE OWNER_READ GROUP_READ)
