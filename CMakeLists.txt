cmake_minimum_required(VERSION 3.10)
project(capsimage)

set(PROJECT_TITLE CAPSImage)
set(PROJECT_TITLE_WIN32 CAPSImg)

set(MAJOR_VERSION 5)
set(MINOR_VERSION 1)

option(BUILD_SHARED_LIBS "Build a dynamic library instead of static library" ON)

if (APPLE)
  option(BUILD_FRAMEWORK "Build a macOS framework instead of a shared library" ON)
  if (BUILD_FRAMEWORK AND NOT BUILD_SHARED_LIBS)
    set(BUILD_FRAMEWORK OFF)
  endif()
else()
  set(BUILD_FRAMEWORK OFF)
endif()

option(ADD_INCLUDE_SUFFIX "Add major version suffix to include directory" OFF)
if (ADD_INCLUDE_SUFFIX)
  set(INCLUDE_SUFFIX ${MAJOR_VERSION})
  message(STATUS "Using '${INCLUDE_SUFFIX}' suffix on include directory")
endif()

include(CheckStructHasMember)
CHECK_STRUCT_HAS_MEMBER("struct dirent" d_type dirent.h HAVE_STRUCT_DIRENT_D_TYPE LANGUAGE CXX)

configure_file(src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(src/LibIPF/CapsLibVersion.h.in ${CMAKE_CURRENT_BINARY_DIR}/CapsLibVersion.h)

set(SOURCES
  src/Core/BaseFile.cpp
  src/Core/BitBuffer.cpp
  src/Core/CRC.cpp
  src/Core/DiskFile.cpp
  src/Core/MemoryFile.cpp
  src/CAPSImg/CAPSImg.cpp
  src/CAPSImg/CapsAPI.cpp
  src/CAPSImg/CapsFDCEmulator.cpp
  src/CAPSImg/CapsFile.cpp
  src/CAPSImg/CapsFormatMFM.cpp
  src/CAPSImg/CapsImage.cpp
  src/CAPSImg/CapsImageStd.cpp
  src/CAPSImg/CapsLoader.cpp
  src/CAPSImg/DiskImage.cpp
  src/CAPSImg/DiskImageFactory.cpp
  src/CAPSImg/dllmain.cpp
  src/CAPSImg/stdafx.cpp
  src/CAPSImg/StreamCueImage.cpp
  src/CAPSImg/StreamImage.cpp
  src/Codec/CTRawCodec.cpp
  src/Codec/CTRawCodecDecompressor.cpp
  src/Codec/DiskEncoding.cpp
)

set(PRIVATE_HEADERS
  src/Core/BaseFile.h
  src/Core/BitBuffer.h
  src/Core/CRC.h
  src/Core/DiskFile.h
  src/Core/MemoryFile.h
  src/CAPSImg/CapsCore.h
  src/CAPSImg/CapsFDCEmulator.h
  src/CAPSImg/CapsFile.h
  src/CAPSImg/CapsFormatMFM.h
  src/CAPSImg/CapsImage.h
  src/CAPSImg/CapsImageStd.h
  src/CAPSImg/CapsLoader.h
  src/CAPSImg/DiskImage.h
  src/CAPSImg/DiskImageFactory.h
  src/CAPSImg/stdafx.h
  src/CAPSImg/StreamCueImage.h
  src/CAPSImg/StreamImage.h
  src/CAPSImg/targetver.h
  src/Codec/CapsDefinitions.h
  src/Codec/CTRawCodec.h
  src/Codec/DiskEncoding.h
  src/Device/C2Comm.h
)

set(API_SOURCE_HEADERS
  src/LibIPF/CapsAPI.h
  src/LibIPF/CapsFDC.h
  src/LibIPF/CapsForm.h
  src/LibIPF/CapsLib.h
  src/LibIPF/CapsLibAll.h
  src/LibIPF/ComLib.h
  src/Core/CommonTypes.h
)

set(API_HEADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/include/caps${INCLUDE_SUFFIX})
file(MAKE_DIRECTORY ${API_HEADER_DIR})

set(API_HEADERS)
foreach(api_header IN LISTS API_SOURCE_HEADERS)
  get_filename_component(api_header_name ${api_header} NAME)
  configure_file(${api_header} ${API_HEADER_DIR}/${api_header_name} COPYONLY)
  list(APPEND API_HEADERS ${API_HEADER_DIR}/${api_header_name})
endforeach()
configure_file(${CMAKE_CURRENT_BINARY_DIR}/CapsLibVersion.h ${API_HEADER_DIR}/CapsLibVersion.h COPYONLY)
list(APPEND API_HEADERS ${API_HEADER_DIR}/CapsLibVersion.h)

set(CAPSIMAGE_LIBRARY_TYPE STATIC)
if (BUILD_SHARED_LIBS)
  set(CAPSIMAGE_LIBRARY_TYPE SHARED)
endif()

add_library(${PROJECT_NAME} ${CAPSIMAGE_LIBRARY_TYPE} ${SOURCES} ${PRIVATE_HEADERS} ${API_HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION ${MAJOR_VERSION}.${MINOR_VERSION}
  SOVERSION ${MAJOR_VERSION}
  CXX_STANDARD 11)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    src/CAPSImg
    src/Codec
    src/Device
    src/Core
    src/LibIPF
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/Core>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/LibIPF>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if (APPLE AND BUILD_FRAMEWORK)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_TITLE}
    FRAMEWORK TRUE
    FRAMEWORK_VERSION C
    MACOSX_FRAMEWORK_IDENTIFIER com.kryoflux)
  set_source_files_properties(${API_HEADERS} PROPERTIES
    MACOSX_PACKAGE_LOCATION Headers/caps${INCLUDE_SUFFIX})
endif()

if (WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_TITLE_WIN32})
  target_include_directories(${PROJECT_NAME} PRIVATE src/Compatibility)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_CONFIG_H=1)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime)

if (NOT APPLE OR NOT BUILD_FRAMEWORK)
  install(FILES ${API_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/caps${INCLUDE_SUFFIX})
endif()
