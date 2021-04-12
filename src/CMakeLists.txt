set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})

find_package(OpenMP REQUIRED)

target_link_libraries(${BINARY}_lib uring OpenMP::OpenMP_CXX)
target_link_libraries(${BINARY}_run uring OpenMP::OpenMP_CXX)