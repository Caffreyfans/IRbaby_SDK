set( HTTP_SOURCE
     ${CMAKE_CURRENT_LIST_DIR}/coreHTTP/source/core_http_client.c
     ${CMAKE_CURRENT_LIST_DIR}/coreHTTP/source/dependency/3rdparty/http_parser/http_parser.c
     ${CMAKE_CURRENT_LIST_DIR}/port/http_client.c)

# HTTP library public include directories.
set( HTTP_INCLUDE_PUBLIC_DIRS
     ${CMAKE_CURRENT_LIST_DIR}/coreHTTP/source/include
     ${CMAKE_CURRENT_LIST_DIR}/coreHTTP/source/interface
     ${CMAKE_CURRENT_LIST_DIR}/port/include
     ${CMAKE_CURRENT_LIST_DIR}/coreHTTP/source/dependency/3rdparty/http_parser )

add_library(coreHTTP STATIC ${HTTP_SOURCE})

target_compile_options(coreHTTP PUBLIC -DHTTP_DO_NOT_USE_CUSTOM_CONFIG=1 -g)
target_include_directories(coreHTTP PUBLIC ${HTTP_INCLUDE_PUBLIC_DIRS})
