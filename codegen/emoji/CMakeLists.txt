add_executable(codegen_emoji)
init_target(codegen_emoji)

get_filename_component(src_loc ../.. REALPATH)

set(codegen_emoji_sources
PRIVATE
    codegen/emoji/data.cpp
    codegen/emoji/data.h
    codegen/emoji/generator.cpp
    codegen/emoji/generator.h
    codegen/emoji/main.cpp
    codegen/emoji/options.cpp
    codegen/emoji/options.h
    codegen/emoji/replaces.cpp
    codegen/emoji/replaces.h
)
nice_target_sources(codegen_emoji ${src_loc} "${codegen_emoji_sources}")

target_include_directories(codegen_emoji
PUBLIC
    ${src_loc}
)

target_link_libraries(codegen_emoji
PUBLIC
    codegen_common
    external_qt
    common
)
