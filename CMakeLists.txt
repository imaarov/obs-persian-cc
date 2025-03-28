cmake_minimum_required(VERSION 3.16)
project(persian-subtitle-plugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(libobs REQUIRED)
include_directories(/usr/include/obs)
include_directories(${LIBOBS_INCLUDE_DIRS})


# include whisper
include_directories("external/whisper.cpp/include")  # include to include/ for whisper.h
include_directories("external/whisper.cpp/ggml/include")  # include ggml.h

add_library(${PROJECT_NAME} MODULE
    src/plugin-main.cpp
    src/subtitle-source.cpp
)
add_subdirectory(external/whisper.cpp build_whisper)

set_target_properties(whisper PROPERTIES
    COMPILE_OPTIONS "-Wno-error" # prevent the whisper throw warnings
)

target_link_libraries(${PROJECT_NAME}
    ${LIBOBS_LIBRARIES}
    whisper
)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "obs-plugins/64bit")
install(DIRECTORY data/ DESTINATION "data/obs-plugins/${PROJECT_NAME}")

add_custom_target(package_source
  COMMAND ${CMAKE_COMMAND} -E tar czf ${CMAKE_BINARY_DIR}/persian-subtitle-plugin-source.tar.gz -C ${CMAKE_SOURCE_DIR} .
  COMMENT "Creating source tarball for ${PROJECT_NAME}"
)

add_custom_target(package
  DEPENDS package_source
  COMMENT "Packaging the Persian Subtitle Plugin"
)
