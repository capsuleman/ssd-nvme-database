cmake_minimum_required(VERSION 3.10)
project(ssd-nvme-database)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(googletest)
add_subdirectory(benchmark)
add_subdirectory(benchtest)