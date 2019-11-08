add_executable(codegen_lang)
init_target(codegen_lang "(codegen)")

get_filename_component(src_loc ../.. REALPATH)

set(codegen_lang_sources
PRIVATE
    codegen/lang/generator.cpp
    codegen/lang/generator.h
    codegen/lang/main.cpp
    codegen/lang/options.cpp
    codegen/lang/options.h
    codegen/lang/parsed_file.cpp
    codegen/lang/parsed_file.h
    codegen/lang/processor.cpp
    codegen/lang/processor.h
)
nice_target_sources(codegen_lang ${src_loc} "${codegen_lang_sources}")

target_include_directories(codegen_lang
PUBLIC
    ${src_loc}
)

target_link_libraries(codegen_lang
PUBLIC
    codegen_common
    external_qt
    common
)
