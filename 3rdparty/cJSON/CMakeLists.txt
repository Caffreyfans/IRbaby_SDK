file (GLOB srcs "cJSON/*.c")
add_library(cJSON  STATIC ${srcs})
target_include_directories(cJSON PUBLIC "cJSON")
