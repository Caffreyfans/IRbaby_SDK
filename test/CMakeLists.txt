add_executable(irext_api_test irext_api_test.c)

target_link_libraries(irext_api_test 
                      cJSON
                      coreHTTP
                      IRbaby)

add_executable(download_test download_test.c)
target_link_libraries(download_test
              cJSON
		      coreHTTP
		      IRbaby
              )

	      
