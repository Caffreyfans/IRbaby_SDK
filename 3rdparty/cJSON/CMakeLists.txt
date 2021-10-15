file (GLOB srcs "cJSON/*.c")

add_library(cjson  STATIC ${srcs})

target_include_directories(cjson PUBLIC "cJSON")
