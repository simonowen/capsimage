cmake_minimum_required(VERSION 2.8)

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

configure_file(src/config.h.in ${CMAKE_BINARY_DIR}/config.h)
target_compile_definitions(capsimage PRIVATE HAVE_CONFIG_H=1)

install(TARGETS capsimage
	LIBRARY DESTINATION lib
	FRAMEWORK DESTINATION "/Library/Frameworks"
	PUBLIC_HEADER DESTINATION include/caps)
install(FILES ${API_HEADERS} DESTINATION include/caps)
