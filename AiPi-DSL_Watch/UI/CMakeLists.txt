
file(GLOB_RECURSE SOURCES "custom/*.c" "generated/guider_fonts/*.c" "generated/images/*.c" "generated/*.c")

add_library(ui ${SOURCES})
