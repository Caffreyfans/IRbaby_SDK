file (GLOB srcs "*.c")
     
add_library(IRext STATIC ${srcs})

target_include_directories(IRext PUBLIC "include")
