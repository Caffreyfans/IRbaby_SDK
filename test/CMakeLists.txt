add_executable(http_test http_test.c)

target_link_libraries(http_test 
                      coreHTTP
                      IRbaby)
add_executable(download_test download_test.c)
target_link_libraries(download_test
		      coreHTTP
		      IRbaby)

	      
