add_executable(http_test http_test.c)
add_definitions(-g)
include_directories(../include)
target_link_libraries(http_test 
                      coreHTTP
                      irbaby)