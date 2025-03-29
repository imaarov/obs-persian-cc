# Require LIBOBS_ROOT to be defined
if(NOT DEFINED LIBOBS_ROOT)
  message(FATAL_ERROR "Please define LIBOBS_ROOT to the root of the libobs installation (e.g., -DLIBOBS_ROOT=/usr or -DLIBOBS_ROOT=D:/obs-studio)")
endif()

# Look for the OBS header (adjust "obs.h" if needed)
find_path(LIBOBS_INCLUDE_DIR
  NAMES obs.h
  PATHS ${LIBOBS_ROOT}/include
)

# Look for the OBS library (adjust the name if necessary)
find_library(LIBOBS_LIBRARY
  NAMES obs
  PATHS ${LIBOBS_ROOT}/lib
)

if(LIBOBS_INCLUDE_DIR AND LIBOBS_LIBRARY)
  set(LIBOBS_FOUND TRUE)
  set(LIBOBS_INCLUDE_DIRS ${LIBOBS_INCLUDE_DIR})
  set(LIBOBS_LIBRARIES ${LIBOBS_LIBRARY})
else()
  set(LIBOBS_FOUND FALSE)
endif()

if(NOT LIBOBS_FOUND)
  message(FATAL_ERROR "Could not find libobs in ${LIBOBS_ROOT}. Please ensure libobs is installed and LIBOBS_ROOT is set correctly.")
endif()

mark_as_advanced(LIBOBS_INCLUDE_DIR LIBOBS_LIBRARY)
