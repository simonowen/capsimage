cmake_minimum_required(VERSION 3.0)

if (APPLE)
  option(BUILD_FRAMEWORK "Build a Mac OS X framework instead of a shared library" OFF)
  set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
endif ()

include(CheckStructHasMember)
CHECK_STRUCT_HAS_MEMBER("struct dirent" d_type dirent.h HAVE_STRUCT_DIRENT_D_TYPE LANGUAGE CXX)

file(GLOB SOURCES src/Core/*.cpp src/CAPSImg/*.cpp src/Codec/*.cpp)
file(GLOB HEADERS src/Core/*.h src/CAPSImg/*.h src/Codec/*.h src/Device/*.h)
file(GLOB API_HEADERS src/LibIPF/*.h;src/Core/CommonTypes.h)
add_library(capsimage SHARED ${SOURCES} ${HEADERS} ${API_HEADERS})

set_target_properties(capsimage PROPERTIES
  VERSION 5.1
  SOVERSION 5
  CXX_STANDARD 11)

target_include_directories(capsimage PRIVATE
  ${CMAKE_BINARY_DIR}
  src/LibIPF
  src/CAPSImg
  src/Core
  src/Codec
  src/Device)

if (APPLE AND BUILD_FRAMEWORK)
  set_target_properties(capsimage PROPERTIES
    OUTPUT_NAME CAPSimage
    FRAMEWORK TRUE
    FRAMEWORK_VERSION C
    MACOSX_FRAMEWORK_IDENTIFIER com.kryoflux)
  set_source_files_properties(${API_HEADERS} PROPERTIES
    MACOSX_PACKAGE_LOCATION Headers/caps)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9")
endif ()

if (WIN32)
  target_include_directories(capsimage PRIVATE src/Compatibility)
endif()

configure_file(src/config.h.in ${CMAKE_BINARY_DIR}/config.h)
target_compile_definitions(capsimage PRIVATE HAVE_CONFIG_H=1)

install(TARGETS capsimage
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  FRAMEWORK DESTINATION "/Library/Frameworks"
  PUBLIC_HEADER DESTINATION include/caps)

if (NOT APPLE OR NOT BUILD_FRAMEWORK)
  install(FILES ${API_HEADERS} DESTINATION include/caps)
endif ()
