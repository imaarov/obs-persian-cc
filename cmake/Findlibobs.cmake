# Findlibobs.cmake
find_path(LIBOBS_INCLUDE_DIR
  NAMES obs.h
  PATHS "D:/obs-studio/include" )
  
find_library(LIBOBS_LIBRARY
  NAMES obs
  PATHS "D:/obs-studio/lib" )

if(LIBOBS_INCLUDE_DIR AND LIBOBS_LIBRARY)
  set(LIBOBS_FOUND TRUE)
  set(LIBOBS_INCLUDE_DIRS ${LIBOBS_INCLUDE_DIR})
  set(LIBOBS_LIBRARIES ${LIBOBS_LIBRARY})
else()
  set(LIBOBS_FOUND FALSE)
endif()

if(NOT LIBOBS_FOUND)
  message(FATAL_ERROR "Could not find libobs")
endif()

# Provide a cache variable for libobs_DIR if needed
mark_as_advanced(LIBOBS_INCLUDE_DIR LIBOBS_LIBRARY)