
file(GLOB_RECURSE SOURCES "./*.c")

sdk_add_include_directories(.)
add_library(components ${SOURCES})
