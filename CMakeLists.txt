cmake_minimum_required(VERSION 3.3)
project(capsimage)

include (GNUInstallDirs)
include (FindPkgConfig)

set(PROJECT_TITLE CAPSImage)
set(PROJECT_TITLE_WIN32 CAPSImg)

set(MAJOR_VERSION 5)
set(MINOR_VERSION 1)
set(PROJECT_VERSION ${MAJOR_VERSION}.${MINOR_VERSION})
set(PROJECT_DESCRIPTION "CAPSImage IPF/SPS decoder library written in C++")

option(BUILD_SHARED_LIBS "Build a dynamic library instead of static library" ON)

if (APPLE)
  option(BUILD_FRAMEWORK "Build a macOS framework instead of a shared library" ON)
  set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
  set(BUILD_SHARED_LIBS ON)
endif()

option(ADD_INCLUDE_SUFFIX "Add major version suffix to include directory" OFF)
if (ADD_INCLUDE_SUFFIX)
  set(INCLUDE_SUFFIX ${MAJOR_VERSION})
  message(STATUS "Using '${INCLUDE_SUFFIX}' suffix on include directory")
endif()

include(CheckStructHasMember)
CHECK_STRUCT_HAS_MEMBER("struct dirent" d_type dirent.h HAVE_STRUCT_DIRENT_D_TYPE LANGUAGE CXX)

file(GLOB SOURCES src/Core/*.cpp src/CAPSImg/*.cpp src/Codec/*.cpp)
file(GLOB HEADERS src/Core/*.h src/CAPSImg/*.h src/Codec/*.h src/Device/*.h)
file(GLOB API_HEADERS src/LibIPF/*.h;src/Core/CommonTypes.h)
set(API_HEADERS ${API_HEADERS} ${CMAKE_BINARY_DIR}/CapsLibVersion.h)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${API_HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION ${MAJOR_VERSION}.${MINOR_VERSION}
  SOVERSION ${MAJOR_VERSION}
  CXX_STANDARD 11)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_BINARY_DIR}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/caps${INCLUDE_SUFFIX}>
  src/LibIPF
  src/CAPSImg
  src/Core
  src/Codec
  src/Device)

if (APPLE AND BUILD_FRAMEWORK)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_TITLE}
    FRAMEWORK TRUE
    FRAMEWORK_VERSION C
    MACOSX_FRAMEWORK_IDENTIFIER com.kryoflux)
  set_source_files_properties(${API_HEADERS} PROPERTIES
    MACOSX_PACKAGE_LOCATION Headers/caps${INCLUDE_SUFFIX})
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9")
endif()

if (WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_TITLE_WIN32})
  target_include_directories(${PROJECT_NAME} PRIVATE src/Compatibility)
endif()

configure_file(src/config.h.in ${CMAKE_BINARY_DIR}/config.h)
configure_file(src/LibIPF/CapsLibVersion.h.in ${CMAKE_BINARY_DIR}/CapsLibVersion.h)
target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_CONFIG_H=1)

set(install_prefix "${CMAKE_INSTALL_PREFIX}")
set(install_libdir "${CMAKE_INSTALL_LIBDIR}")
set(install_include_suffix "${INCLUDE_SUFFIX}")

configure_file(lib${PROJECT_NAME}.pc.in ${PROJECT_BINARY_DIR}/lib${PROJECT_NAME}.pc @ONLY)

set(ENV{PKG_CONFIG_PATH} "${PROJECT_BINARY_DIR}:$ENV{PKG_CONFIG_PATH}")

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Config
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  FRAMEWORK DESTINATION "/Library/Frameworks"
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/caps${INCLUDE_SUFFIX})

install(FILES lib${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/)


if (NOT APPLE OR NOT BUILD_FRAMEWORK)
  install(FILES ${API_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/caps${INCLUDE_SUFFIX})
endif()
