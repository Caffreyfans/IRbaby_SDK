set( srcs ${CMAKE_CURRENT_LIST_DIR}/cJSON/cJSON.c) 

set( includes ${CMAKE_CURRENT_LIST_DIR}/cJSON)

add_library(cJSON  STATIC ${srcs})

target_include_directories(cJSON PUBLIC ${includes})
