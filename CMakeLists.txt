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

target_link_libraries(${PROJECT_NAME}
    ${LIBOBS_LIBRARIES}
    "external/whisper.cpp/build/src/libwhisper.so"  # shared library whisper
)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "obs-plugins/64bit")
install(DIRECTORY data/ DESTINATION "data/obs-plugins/${PROJECT_NAME}")