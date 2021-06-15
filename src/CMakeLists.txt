file(GLOB ALL_SRCS "*.c" "port/*.c")
set(includes
    "../include"
    "../include/port")

add_library(irbaby ${ALL_SRCS} )
target_include_directories(irbaby PUBLIC ${includes})
target_link_libraries(irbaby PUBLIC coreHTTP IRext)