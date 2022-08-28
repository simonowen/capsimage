include (CheckLibraryExists)
set(CapsImage_FOUND FALSE)
set(CMAKE_REQUIRED_QUIET_SAVE ${CMAKE_REQUIRED_QUIET})
set(CMAKE_REQUIRED_QUIET ${Math_FIND_QUIETLY})

pkg_search_module(CAPSIMAGE REQUIRED libcapsimage)
add_library(SPS::CapsImage INTERFACE IMPORTED)

unset(_cmake_find_capsimage_output)
