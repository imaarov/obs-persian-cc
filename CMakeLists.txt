cmake_minimum_required(VERSION 3.16...3.30)
project(persian-subtitle-plugin VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(libobs REQUIRED)

add_library(${PROJECT_NAME} MODULE src/plugin-main.cpp src/subtitle-source.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${LIBOBS_INCLUDE_DIRS})
target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
    "external/whisper.cpp/include" # whisper
    "external/whisper.cpp/ggml/include" # ggml.h
)
target_link_libraries(${PROJECT_NAME} PRIVATE OBS::libobs)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBOBS_LIBRARIES} whisper)

add_subdirectory(external/whisper.cpp build_whisper)
set_target_properties(whisper PROPERTIES COMPILE_OPTIONS "-Wno-error")

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "obs-plugins/64bit")
install(DIRECTORY data/ DESTINATION "data/obs-plugins/${PROJECT_NAME}")

add_custom_target(
  package_source
  COMMAND ${CMAKE_COMMAND} -E tar czf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-source.tar.gz -- ${CMAKE_SOURCE_DIR}
  COMMENT "Creating source tarball for ${PROJECT_NAME}"
)

add_custom_target(package DEPENDS package_source COMMENT "Packaging the Persian Subtitle Plugin")

if(WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
endif()
