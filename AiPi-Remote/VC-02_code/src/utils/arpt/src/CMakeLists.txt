project(arpt LANGUAGES C)

add_library(arpt STATIC uni_arpt.c)

target_include_directories(arpt PUBLIC
    "../inc"
    "../../log/inc"
    "../../../hal/inc")