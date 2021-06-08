add_executable(http_test http_test.c)

target_link_libraries(http_test 
                      coreHTTP
                      irbaby)
target_link_directories(http_test PUBLIC
                        ../include)

add_executable(download_test download_test.c)
target_link_libraries(download_test
                      coreHTTP
                      irbaby)
target_link_directories(download PUBLIC
                        ../include)