# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME "xiaozhi-esp32")
project(${PROJECT_NAME})
set(CMAKE_TOOLCHAIN_FILE ${IDF_PATH}/tools/cmake/toolchain-esp32.cmake)

# 引入组件
set(EXTRA_COMPONENT_DIRS components/audio_utils)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)


# 设置库路径
set(LIBS_PATH "${CMAKE_SOURCE_DIR}/components/audio_utils/lib")

# 添加库
add_library(opus STATIC IMPORTED)
set_target_properties(opus PROPERTIES IMPORTED_LOCATION "${LIBS_PATH}/opus/libopus.a")

add_library(lame STATIC IMPORTED)
set_target_properties(lame PROPERTIES IMPORTED_LOCATION "${LIBS_PATH}/lame/liblame.a")

add_library(libsndfile STATIC IMPORTED)
set_target_properties(libsndfile PROPERTIES IMPORTED_LOCATION "${LIBS_PATH}/libsndfile/libsndfile.a")

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE opus lame libsndfile)
