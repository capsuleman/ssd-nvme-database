set(BINARY ${CMAKE_PROJECT_NAME}_bench)

file(GLOB_RECURSE BENCH_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${BENCH_SOURCES})

add_executable(${BINARY} ${BENCH_SOURCES})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib benchmark::benchmark)