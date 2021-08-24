set( srcs cJSON/cJSON.c) 

set( includes "cJSON")

add_library(cJSON  STATIC ${srcs})

target_include_directories(cJSON PUBLIC ${includes})
