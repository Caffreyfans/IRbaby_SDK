file(GLOB ALL_SRCS "*.c" "port/*.c")
set(includes
    "../include"
    "../include/port")

add_library(IRbaby ${ALL_SRCS} )
target_include_directories(IRbaby PUBLIC ${includes})
target_link_libraries(IRbaby PUBLIC cjson coreHTTP IRext)
