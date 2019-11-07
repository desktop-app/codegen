add_executable(codegen_emoji)
init_target(codegen_emoji)

get_filename_component(src_loc ../.. REALPATH)

target_include_directories(codegen_emoji
PUBLIC
    ${src_loc}
)

target_sources(codegen_emoji
PRIVATE
    ${src_loc}/codegen/emoji/data.cpp
    ${src_loc}/codegen/emoji/data.h
    ${src_loc}/codegen/emoji/generator.cpp
    ${src_loc}/codegen/emoji/generator.h
    ${src_loc}/codegen/emoji/main.cpp
    ${src_loc}/codegen/emoji/options.cpp
    ${src_loc}/codegen/emoji/options.h
    ${src_loc}/codegen/emoji/replaces.cpp
    ${src_loc}/codegen/emoji/replaces.h
)

target_link_libraries(codegen_emoji
PUBLIC
    codegen_common
    external_qt
    common
)
