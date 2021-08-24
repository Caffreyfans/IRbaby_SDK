set( srcs cJSON/cJSON.c) 

set( includes "cJSON")

add_library(cjson  STATIC ${srcs})

target_include_directories(cjson PUBLIC ${includes})
