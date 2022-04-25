cmake_minimum_required(VERSION 3.12)
project(unebi CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(tests)